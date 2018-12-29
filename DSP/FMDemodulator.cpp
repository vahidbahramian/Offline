#include "StdAfx.h"
#include "FMDemodulator.h"
#include "../BCGP_HF/SharedVariables.h"

namespace NS_DSP
{

	//filters coefficients
	FILTER_COEFS<double>	CFMDemodulator::m_stFilterCoefBuffer;
	FILTER_COEFS<double>	CFMDemodulator::m_stFilterCoefOutput;

	CFMDemodulator::CFMDemodulator(void):CDemodulatorBase()
	{
		m_fInputBufferI			=	NULL;
		m_fInputBufferQ			=	NULL;
		m_nInputBufferLength	=	0;
		nType					=	eModulationFM;
		m_iRecordBuffer			=	NULL;
		m_fDemodOutBuffer		=	NULL;
		m_nDemodOutBufferLength	=	0;
		m_fFirOutI=m_fFirOutQ	=	0;
		m_fOldFirOutI=m_fOldFirOutQ	=	0;

	}

	CFMDemodulator::~CFMDemodulator(void)
	{
		Release();
	}

	BOOL CFMDemodulator::IsLoadedFIRCoef() 
	{
		if (!m_stFilterCoefBuffer.IsEmpty() && 
			!m_stFilterCoefOutput.IsEmpty() ) return TRUE;
		return FALSE;
	}

	BOOL CFMDemodulator::Create(int m_nID)
	{
		BOOL bRet = CDemodulatorBase::Create(m_nID);
		if (!bRet) return FALSE;
		if (!IsLoadedFIRCoef()) return FALSE;
		//Demod input buffers
		m_fInputBufferI = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fInputBufferI) {
			Release();
			return FALSE;
		}
		m_fInputBufferQ = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fInputBufferQ) {
			Release();
			return FALSE;
		}
		//Demod output Buffer
		m_fDemodOutBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH*2];
		if (!m_fDemodOutBuffer) {
			Release();
			return FALSE;
		}
		//Record Buffer
		m_iRecordBuffer = new short[HF_MAX_DEMOD_BUFFER_LENGTH*2];
		if (!m_iRecordBuffer) {
			Release();
			return FALSE;
		}

		//CREATE FILTERS
		if (!m_InputFilterI.Create(m_stFilterCoefBuffer.m_fCoefficients,
			m_stFilterCoefBuffer.m_nSize))
		{
			Release(); return FALSE;
		}
		if (!m_InputFilterQ.Create(m_stFilterCoefBuffer.m_fCoefficients,
			m_stFilterCoefBuffer.m_nSize))
		{
			Release(); return FALSE;
		}
		if (!m_OutputFilter.Create(m_stFilterCoefOutput.m_fCoefficients,
			m_stFilterCoefOutput.m_nSize))
		{
			Release(); return FALSE;
		}
		if (!m_CicFilter.Create(2,HF_FM_CIC_DOWNSAMPLE-1,ceil(2*LOG2(HF_FM_CIC_DOWNSAMPLE))))
		{
			Release(); return FALSE;
		}

		Reset();
		return TRUE;
	}//func

	void CFMDemodulator::Reset()
	{
		CDemodulatorBase::Reset();
		m_nInputBufferLength	=	0;//empty buffer
		m_InputFilterI.Reset();
		m_InputFilterQ.Reset();
		m_CicFilter.Reset();
		m_OutputFilter.Reset();//a filter after CIC
		m_fFirOutI=m_fFirOutQ	=	0;
		m_fOldFirOutI=m_fOldFirOutQ	=	0;
	}//func

	void CFMDemodulator::Release()
	{
		//CDemodulatorBase::Release();
		SAFERELEASE_ARRAY(m_fInputBufferI);
		SAFERELEASE_ARRAY(m_fInputBufferQ);
		SAFERELEASE_ARRAY(m_fDemodOutBuffer);
		SAFERELEASE_ARRAY(m_iRecordBuffer);
	}

	void CFMDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		int i , tmpI,tmpQ;
		nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		m_nInputBufferLength=0;
		for (i=0;i<nSize;i++)
		{
			if (GetSampleID(m_pInBuffer[i]) != nID) 
				continue;//samples is not belong to this demodulator

			tmpI = m_pInBuffer[i] & 0x0000ffff;
			tmpQ = (m_pInBuffer[i] & 0x1fff0000)>>16;
			if (tmpI & 0x00008000)	tmpI = tmpI |0xffff0000;
			if (tmpQ & 0x00001000)	tmpQ = tmpQ |0xffffe000;
			tmpI = tmpI >> 3;//scale I samples
			m_fInputBufferI[m_nInputBufferLength] = double(tmpI);
			m_fInputBufferQ[m_nInputBufferLength] = double(tmpQ);
			m_nInputBufferLength++;
		}
	}//func

	void CFMDemodulator::CalculateMSB()
	{
		int i , nVal=0;
		for (i =0;i<m_nInputBufferLength;i++)
		{
			if(nCalcMSBCounter >= nCalcMSBNumberSamples) {
				//nMSBPosition = log2(nMSBValue);
				break;//stop calculation
			}
			if ((nVal=(int)abs(m_fInputBufferI[i])) > nMSBValue)
				nMSBValue = nVal;
			nCalcMSBCounter++;
		}//for
	}//func


	//is called on app start to load all demodulators filters coefficients
	BOOL CFMDemodulator::LoadFiltersCoefficients()
	{
		BOOL bRet ;
		bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/FM_Buffer_FIR.txt");
		if (!bRet ) return FALSE;
		bRet = m_stFilterCoefOutput.ReadCoefsFromFile("../FIR_Coef/FM_Output_FIR.txt");
		if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CFMDemodulator::UnLoadFiltersCoefficients()
	{
		m_stFilterCoefBuffer.Release();
		m_stFilterCoefOutput.Release();
	}//func

	BOOL CFMDemodulator::Demodulate()
	{
		int i;
		BOOL bValid;
		double atanOut ,cicOut;
		m_nDemodOutBufferLength	=	0;
		for (i=0;i<m_nInputBufferLength;i++)
		{
			m_fFirOutI	=	m_InputFilterI.Filter(m_fInputBufferI[i]);	
			m_fFirOutQ	=	m_InputFilterQ.Filter(m_fInputBufferQ[i]);	
			atanOut = 
				atan2((m_fOldFirOutI*m_fFirOutQ - m_fOldFirOutQ*m_fFirOutI),
					  (m_fOldFirOutQ*m_fFirOutQ + m_fOldFirOutI*m_fFirOutI));

			cicOut	= m_CicFilter.Filter(int(atanOut*10402.0),bValid);
			if(bValid)
			{
				m_fDemodOutBuffer[m_nDemodOutBufferLength]=m_OutputFilter.Filter(cicOut);
				m_nDemodOutBufferLength++;
			}

			m_fOldFirOutI	=	m_fFirOutI;
			m_fOldFirOutQ	=	m_fFirOutQ;
		}//for 

		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
			m_iRecordBuffer[i] = (short)m_iRecordBuffer[i] = (short)(m_fDemodOutBuffer[i]*CSharedVariables::m_fVolume);
		return TRUE;
	}//func

	BOOL CFMDemodulator::Record(BOOL bStart)
	{
		int nWrite=0;
		if (!bStart)
		{//stop recording 
			m_Record.Close();
		}else{//start recording
			if (!IsRecording())
			{//if Begins Recording ,Create File First
				CString strName = CreateRecordFileName();
				return m_Record.Open(strName,1,HF_AUDIO_RATE);
			}
			//Record Samples
			nWrite = m_Record.Write(m_iRecordBuffer,m_nDemodOutBufferLength);
			if (nWrite != m_nDemodOutBufferLength) return FALSE;
		}//if
		return TRUE;
	}//func

	BOOL CFMDemodulator::PlayOut(LPARAM lParam)
	{
		int i;
		if (!CSharedVariables::m_Audio.IsRunning()) return FALSE;
		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
		{
			if (CSharedVariables::m_nAudioBufferFullCounter == HF_AUDIO_BUFFER_LENGTH)
			{
				CSharedVariables::m_Audio.putData((UCHAR *)CSharedVariables::m_iAudioBuffer,
					CSharedVariables::m_nAudioBufferFullCounter *sizeof(short));
				CSharedVariables::m_nAudioBufferFullCounter	=	0;
			}
			CSharedVariables::m_iAudioBuffer[CSharedVariables::m_nAudioBufferFullCounter] = 
				(short)(m_fDemodOutBuffer[i] * CSharedVariables::m_fVolume);
			CSharedVariables::m_nAudioBufferFullCounter++;
		}
		return TRUE;
	}//func

}//namespace