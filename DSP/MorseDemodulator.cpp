#include "StdAfx.h"
#include "MorseDemodulator.h"

#define MORSE_FIRSTLEN  (8192*1)
#define MORSE_SECONDLEN (8192*5)



namespace NS_DSP
{
	//filters coefficients
	FILTER_COEFS<double>	CMorseDemodulator::m_stFilterCoefBuffer;


	CMorseDemodulator::CMorseDemodulator(void):CDemodulatorBase()
	{
		m_fInputBufferI			=	NULL;
		m_fInputBufferQ			=	NULL;
		m_nInputBufferLength	=	0;
		nType					=	eModulationCW;
		m_fThreshold			=	0;//morse threshold
		m_iLenAdd				=	0;
		m_iLenDataNeed			=	0;
		m_pStoreData			=	NULL;

	}

	CMorseDemodulator::~CMorseDemodulator(void)
	{
		Release();
	}

	BOOL CMorseDemodulator::IsLoadedFIRCoef()
	{
		if (!m_stFilterCoefBuffer.IsEmpty()) return TRUE;
		return FALSE;
	}

	BOOL CMorseDemodulator::Create(int m_nID)
	{
		BOOL bRet = CDemodulatorBase::Create(m_nID);
		if (!bRet) return FALSE;
		if (!IsLoadedFIRCoef()) return FALSE;
		try
		{
		//	m_pMorseCode = new CMorseCode;
		//	if (!m_pMorseCode) throw (1);

			m_fInputBufferI = new double[16384];//HF_MAX_DEMOD_BUFFER_LENGTH];
			if (!m_fInputBufferI) throw (1);
			m_fInputBufferQ = new double[16384];//HF_MAX_DEMOD_BUFFER_LENGTH];
			if (!m_fInputBufferQ) throw (1);
			m_iLenDataNeed =MORSE_FIRSTLEN;
			m_cMC.Init(m_iLenDataNeed);
			m_cMorseCode.setStatus(CMorseCode::START);
			m_pStoreData = new double [m_iLenDataNeed+1];
			if (!m_pStoreData) throw (1);
			bRet = m_filterI.Create(m_stFilterCoefBuffer.m_fCoefficients,m_stFilterCoefBuffer.m_nSize);
			if (!bRet) throw (1);
			bRet = m_filterQ.Create(m_stFilterCoefBuffer.m_fCoefficients,m_stFilterCoefBuffer.m_nSize);
			if (!bRet) throw (1);

		}
		catch (int nError){
			Release();
			bRet	=	FALSE;
		}

		if (bRet) Reset();
		return bRet;
	}//func

	void CMorseDemodulator::Reset()
	{
		CDemodulatorBase::Reset();
		m_filterI.Reset();
		m_filterQ.Reset();
		m_nInputBufferLength	=	0;//empty buffer
	}//func

	void CMorseDemodulator::Release()
	{
		SAFERELEASE_ARRAY(m_fInputBufferI);
		SAFERELEASE_ARRAY(m_fInputBufferQ);
		SAFERELEASE_ARRAY(m_pStoreData);
	}//func


	void CMorseDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		int i , tmpI,tmpQ;
		nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		m_nInputBufferLength=0;
		for (i=0;i<nSize;i++)
		{
			if (GetSampleID(m_pInBuffer[i])!=nID) 
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

	void CMorseDemodulator::CalculateMSB()
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
	BOOL CMorseDemodulator::LoadFiltersCoefficients()
	{
		BOOL bRet ;
		bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/CW_Buffer_FIR.txt");
		if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CMorseDemodulator::UnLoadFiltersCoefficients()
	{
		m_stFilterCoefBuffer.Release();
	}//func

	BOOL CMorseDemodulator::Demodulate()
	{
		return TRUE;
	}

	BOOL CMorseDemodulator::Record(BOOL bStart)
	{
		return TRUE;
	}//func

	BOOL CMorseDemodulator::PlayOut(LPARAM lParam)
	{
		return TRUE;
	}

}//namespace