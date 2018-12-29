#include "StdAfx.h"
#include "AMDemodulator.h"
#include "../BCGP_HF/SharedVariables.h"

namespace NS_DSP
{
	//filters coefficients
	FILTER_COEFS<double>	CAMDemodulator::m_stFilterCoefBuffer;
	FILTER_COEFS<double>	CAMDemodulator::m_stFilterCoefHilbert;


	CAMDemodulator::CAMDemodulator(void):CDemodulatorBase()
	{
		m_fInputBuffer			=	NULL;
		m_nInputBufferLength	=	0;
		nType					=	eModulationAM;
		m_fDemodOutBuffer		=	NULL;
		m_iRecordBuffer			=	NULL;
		m_nDemodOutBufferLength	=	0;
		m_fDelayLine			=	NULL;
		m_nDelayLineLength		=	0;
	}

	CAMDemodulator::~CAMDemodulator(void)
	{
		Release();
	}

	BOOL CAMDemodulator::IsLoadedFIRCoef()
	{
		if (!m_stFilterCoefBuffer.IsEmpty() && 
			!m_stFilterCoefHilbert.IsEmpty() ) return TRUE;
		return FALSE;
	}

	BOOL CAMDemodulator::Create(int m_nID)
	{
		
		BOOL bRet = CDemodulatorBase::Create(m_nID);
		if (!bRet) return FALSE;
		if (!IsLoadedFIRCoef()) {
			Release();
			return FALSE;
		}
		//Demod input Buffer
		m_fInputBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fInputBuffer) {
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

		//Create Filters
		if (!m_InputFilter.Create(m_stFilterCoefBuffer.m_fCoefficients,
									m_stFilterCoefBuffer.m_nSize))
		{
			Release(); return FALSE;
		}
		if (!m_HilbertFilter.Create(m_stFilterCoefHilbert.m_fCoefficients,
			m_stFilterCoefHilbert.m_nSize))
		{
			Release(); return FALSE;
		}
		m_nDelayLineLength	=	m_stFilterCoefHilbert.m_nSize/2 ;
		m_fDelayLine = new double[m_nDelayLineLength];
		if (!m_fDelayLine)
		{
			Release();return FALSE;
		}

		Reset();
		return TRUE;
	}//func

	void CAMDemodulator::Reset()
	{
		CDemodulatorBase::Reset();
		m_nInputBufferLength	=	0;//empty buffer
		for(int i=0;i<m_nDelayLineLength;i++) m_fDelayLine[i]=0;
		m_HilbertFilter.Reset();
		m_InputFilter.Reset();
	}//func

	void CAMDemodulator::Release()
	{
		//CDemodulatorBase::Release();
		SAFERELEASE_ARRAY(m_fInputBuffer);
		SAFERELEASE_ARRAY(m_fDemodOutBuffer);
		SAFERELEASE_ARRAY(m_iRecordBuffer);
		SAFERELEASE_ARRAY(m_fDelayLine);
	}


	void CAMDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		int i , tmp;
		nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		m_nInputBufferLength=0;
		for (i=0;i<nSize;i++)
		{
			if (GetSampleID(m_pInBuffer[i]) != nID) 
				continue;//samples is not belong to this demodulator

			tmp=m_pInBuffer[i] & 0x0000ffff;
			if (m_pInBuffer[i] & 0x00008000)	tmp = tmp |0xffff0000;
			m_fInputBuffer[m_nInputBufferLength] = double(tmp);
			m_nInputBufferLength++;
		}
	}//func

	void CAMDemodulator::CalculateMSB()
	{
		int i , nVal=0;
		for (i =0;i<m_nInputBufferLength;i++)
		{
			if(nCalcMSBCounter >= nCalcMSBNumberSamples) {
				//nMSBPosition = log2(nMSBValue);
				break;//stop calculation
			}
			if ((nVal=(int)abs(m_fInputBuffer[i])) > nMSBValue)
				nMSBValue = nVal;
			nCalcMSBCounter++;
		}//for
	}//func

	//is called on app start to load all demodulators filters coefficients
	BOOL CAMDemodulator::LoadFiltersCoefficients()
	{
		BOOL bRet ;
		bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/AM_Buffer_FIR.txt");
		if (!bRet ) return FALSE;
		bRet = m_stFilterCoefHilbert.ReadCoefsFromFile("../FIR_Coef/AM_Hilbert_FIR.txt");
		if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CAMDemodulator::UnLoadFiltersCoefficients()
	{
		m_stFilterCoefBuffer.Release();
		m_stFilterCoefHilbert.Release();
	}//func

	BOOL CAMDemodulator::Demodulate()
	{
		int i,j;
		double firOut ,hilbertOut,delayOut;
		m_nDemodOutBufferLength = m_nInputBufferLength;
		for (i=0;i<m_nInputBufferLength;i++)
		{
			firOut = m_InputFilter.Filter(m_fInputBuffer[i]);
			hilbertOut = m_HilbertFilter.Filter(firOut);
			delayOut = m_fDelayLine[m_nDelayLineLength-1];
			for(j=m_nDelayLineLength-1;j>0;j--)
				m_fDelayLine[j]=m_fDelayLine[j-1];
			m_fDelayLine[0]=firOut;
			m_fDemodOutBuffer[i] = sqrt(hilbertOut*hilbertOut + delayOut*delayOut);
		}
		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
			m_iRecordBuffer[i] = (short)(m_fDemodOutBuffer[i]*CSharedVariables::m_fVolume);
		return TRUE;
	}

	BOOL CAMDemodulator::Record(BOOL bStart)
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

	BOOL CAMDemodulator::PlayOut(LPARAM lParam)
	{
		int i;
		if (!CSharedVariables::m_Audio.IsRunning()) return FALSE;
		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
		{
			if (CSharedVariables::m_nAudioBufferFullCounter == HF_AUDIO_BUFFER_LENGTH)
			{
				CSharedVariables::m_Audio.putData((UCHAR *)CSharedVariables::m_iAudioBuffer,
					CSharedVariables::m_nAudioBufferFullCounter*sizeof(short));
				CSharedVariables::m_nAudioBufferFullCounter	=	0;
			}
			CSharedVariables::m_iAudioBuffer[CSharedVariables::m_nAudioBufferFullCounter] = 
				(short)(m_fDemodOutBuffer[i]*CSharedVariables::m_fVolume);
			CSharedVariables::m_nAudioBufferFullCounter++;
		}
		return TRUE;
	}//func
}//namespace