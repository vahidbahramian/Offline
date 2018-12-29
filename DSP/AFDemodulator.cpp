#include "StdAfx.h"
#include "AFDemodulator.h"
#include "../BCGP_HF/SharedVariables.h"

namespace NS_DSP
{
	//filters coefficients
	FILTER_COEFS<double>	CAFDemodulator::m_stFilterCoefBuffer;

	CAFDemodulator::CAFDemodulator(void):CDemodulatorBase()
	{
		m_fInputBuffer			=	NULL;
		m_nInputBufferLength	=	0;
		nType					=	eModulationAM;
		m_fDemodOutBuffer		=	NULL;
		m_iRecordBuffer			=	NULL;
		m_nDemodOutBufferLength	=	0;
	}

	CAFDemodulator::~CAFDemodulator(void)
	{
		Release();
	}

	BOOL CAFDemodulator::IsLoadedFIRCoef()
	{
		//if (!m_stFilterCoefBuffer.IsEmpty()) return TRUE;
		//return FALSE;
		return 1;
	}

	BOOL CAFDemodulator::Create(int m_nID)
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

		Reset();
		return TRUE;
	}//func

	void CAFDemodulator::Reset()
	{
		CDemodulatorBase::Reset();
		m_nInputBufferLength	=	0;//empty buffer
	}//func

	void CAFDemodulator::Release()
	{
		//CDemodulatorBase::Release();
		SAFERELEASE_ARRAY(m_fInputBuffer);
		SAFERELEASE_ARRAY(m_fDemodOutBuffer);
		SAFERELEASE_ARRAY(m_iRecordBuffer);
	}


	void CAFDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		int i , tmp;
		nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		m_nInputBufferLength=0;
		for (i=0;i<nSize;i++)
		{
			if (!GetSampleID(m_pInBuffer[i])) 
				continue;//samples is not belong to this demodulator

			tmp=m_pInBuffer[i] & 0x0000ffff;
			if (m_pInBuffer[i] & 0x00008000)	tmp = tmp |0xffff0000;
			m_fInputBuffer[m_nInputBufferLength] = double(tmp);
			m_nInputBufferLength++;
		}
	}//func

	void CAFDemodulator::CalculateMSB()
	{
		int i , nVal=0;
		for (i =0;i<m_nInputBufferLength;i++)
		{
			if(nCalcMSBCounter >= nCalcMSBNumberSamples) {
				//nMSBPosition = log2(nMSBValue);
				break;//stop calculation
			}
			if ((nVal=abs(m_fInputBuffer[i])) > nMSBValue)
				nMSBValue = nVal;
		}//for
	}//func

	//is called on app start to load all demodulators filters coefficients
	BOOL CAFDemodulator::LoadFiltersCoefficients()
	{
		BOOL bRet ;
		bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/AF_Buffer_FIR.txt");
		if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CAFDemodulator::UnLoadFiltersCoefficients()
	{
		m_stFilterCoefBuffer.Release();
	}//func

	BOOL CAFDemodulator::Demodulate()
	{
		int i;

		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
			m_iRecordBuffer[i] = (short)m_iRecordBuffer[i] = (short)(m_fDemodOutBuffer[i]*CSharedVariables::m_fVolume);
		return TRUE;
	}

	BOOL CAFDemodulator::Record(BOOL bStart)
	{
		int nWrite=0;
		if (!bStart)
		{//stop recording 
			m_Record.Close();
		}else{//start recording
			if (!IsRecording())
			{//if Begins Recording ,Create File First
				CString strName = CreateRecordFileName();
				return m_Record.Open(strName,1,40000,SF_FORMAT_PCM_16,SF_FORMAT_RAW);
			}
			//Record Samples
			nWrite = m_Record.Write(m_iRecordBuffer,m_nDemodOutBufferLength);
			if (nWrite != m_nDemodOutBufferLength) return FALSE;
		}//if
		return TRUE;
	}//func

	BOOL CAFDemodulator::PlayOut(LPARAM lParam)
	{
		return TRUE;

	}

}//namespace