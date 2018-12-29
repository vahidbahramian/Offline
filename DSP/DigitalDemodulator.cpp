#include "StdAfx.h"
#include "DigitalDemodulator.h"
#include "../BCGP_HF/SharedVariables.h"
#include "../BCGP_HF/DigitalBar.h"

namespace NS_DSP
{

	CDigitalDemodulator::CDigitalDemodulator(void):CDemodulatorBase()
	{
		m_fInputBufferI			=	NULL;
		m_fInputBufferQ			=	NULL;
		m_nInputBufferLength	=	0;
		nType					=	eModulationBPSK;
		m_fDemodOutBuffer		=	NULL;
		m_iRecordBuffer			=	NULL;
		m_nDemodOutBufferLength	=	0;
		m_pWndDigital			=	NULL;
	}

	CDigitalDemodulator::~CDigitalDemodulator(void)
	{
		Release();
	}

	BOOL CDigitalDemodulator::IsLoadedFIRCoef()
	{
		return TRUE;
	}

	BOOL CDigitalDemodulator::Create(int m_nID)
	{
		BOOL bRet = CDemodulatorBase::Create(m_nID);
		if (!bRet) return FALSE;
		if (!IsLoadedFIRCoef()) {
			Release();
			return FALSE;
		}
		//Demod input Buffer (I)
		m_fInputBufferI = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fInputBufferI) {
			Release();
			return FALSE;
		}
		//Demod input Buffer (Q)
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

		Reset();
		return TRUE;
	}//func

	void CDigitalDemodulator::Reset()
	{
		CDemodulatorBase::Reset();
		m_nInputBufferLength	=	0;//empty buffer
	}//func

	void CDigitalDemodulator::Release()
	{
		//CDemodulatorBase::Release();
		SAFERELEASE_ARRAY(m_fInputBufferI);
		SAFERELEASE_ARRAY(m_fInputBufferQ);
		SAFERELEASE_ARRAY(m_fDemodOutBuffer);
		SAFERELEASE_ARRAY(m_iRecordBuffer);
	}


	void CDigitalDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		int i , tmp;
		nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		m_nInputBufferLength=0;
		for (i=0;i<nSize;i++)
		{
			if (!GetSampleID(m_pInBuffer[i])) 
				continue;//samples is not belong to this demodulator

			tmp=(m_pInBuffer[i] &0x00003FFF)<<2;//I
			if (tmp & 0x00008000)	tmp = tmp |0xffff0000;
			m_fInputBufferI[m_nInputBufferLength] = double(tmp);

			tmp=(((m_pInBuffer[i] &0x1FFFC000)>>14)<<1);//Q
			if (tmp & 0x00008000)	tmp = tmp |0xffff0000;
			m_fInputBufferQ[m_nInputBufferLength] = double(tmp);

			m_nInputBufferLength++;
		}
	}//func

	void CDigitalDemodulator::CalculateMSB()
	{
		int i , nVal=0;
		for (i =0;i<m_nInputBufferLength;i++)
		{
			if(nCalcMSBCounter >= nCalcMSBNumberSamples) {
				//nMSBPosition = log2(nMSBValue);
				break;//stop calculation
			}
			if ((nVal=abs(m_fInputBufferI[i])) > nMSBValue)
				nMSBValue = nVal;
		}//for
	}//func

	//is called on app start to load all demodulators filters coefficients
	BOOL CDigitalDemodulator::LoadFiltersCoefficients()
	{
		//BOOL bRet ;
		//bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/AF_Buffer_FIR.txt");
		//if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CDigitalDemodulator::UnLoadFiltersCoefficients()
	{
		/*m_stFilterCoefBuffer.Release();*/
	}//func

	BOOL CDigitalDemodulator::Demodulate()
	{
		int i;

		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
			m_iRecordBuffer[i] = (short)m_iRecordBuffer[i] = (short)(m_fDemodOutBuffer[i]*CSharedVariables::m_fVolume);
		return TRUE;
	}

	BOOL CDigitalDemodulator::Record(BOOL bStart)
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

	BOOL CDigitalDemodulator::PlayOut(LPARAM lParam)
	{
		ASSERT (m_pWndDigital);
		m_pWndDigital->SetScatterBuffer (m_fInputBufferI,m_fInputBufferQ,
			m_nInputBufferLength);
		return TRUE;

	}

}//namespace