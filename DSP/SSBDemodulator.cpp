#include "StdAfx.h"
#include "SSBDemodulator.h"
#include "../BCGP_HF/SharedVariables.h"

//HF Project Demodulator
namespace NS_DSP
{
	
	//filters coefficients
	FILTER_COEFS<double>	CSSBDemodulator::m_stFilterCoefBuffer;

	CSSBDemodulator::CSSBDemodulator(void)
	{
		m_fInputBuffer			=	NULL;
		m_nInputBufferLength	=	0;
		m_fFirOutBuffer			=	NULL;
		m_nFirOutBufferLength	=	0;
		nType					=	eModulationLSB;
		m_iRecordBuffer			=	NULL;
		m_fDemodOutBuffer		=	NULL;
		m_nDemodOutBufferLength	=	0;
		m_pSSB_hop				=	NULL;
		m_pSSB_fixed			=	NULL;
		nSSBType				=	DSPLIB_LSB_DEMOD;
		m_iHopsPositionBuffer	=	NULL;
	}

	CSSBDemodulator::~CSSBDemodulator(void)
	{
		Release();
	}

	BOOL CSSBDemodulator::IsLoadedFIRCoef() 
	{
		if (!m_stFilterCoefBuffer.IsEmpty())
			//&&	!m_stFilterCoefHilbert.IsEmpty() )
			return TRUE;
		return FALSE;
	}

	BOOL CSSBDemodulator::Create(int m_nID)
	{
		BOOL bRet = CDemodulatorBase::Create(m_nID);
		if (!bRet) return FALSE;
		if (!IsLoadedFIRCoef()){
			Release();
			return FALSE;
		}
		//Demod Input Buffer
		m_fInputBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fInputBuffer){
			Release();
			return FALSE;
		}
		//FIR Output Buffer
		m_fFirOutBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH*2];
		if (!m_fFirOutBuffer){
			Release();
			return FALSE;
		}

		//Demod output Buffer
		m_fDemodOutBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH*4];
		if (!m_fDemodOutBuffer) {
			Release();
			return FALSE;
		}
		//Record Buffer
		m_iRecordBuffer = new short[HF_MAX_DEMOD_BUFFER_LENGTH*4];
		if (!m_iRecordBuffer) {
			Release();
			return FALSE;
		}

		if (bFixed==FALSE){//Hopping ssb
			//Hop Positions Buffer
			m_iHopsPositionBuffer = new int[HF_MAX_DEMOD_BUFFER_LENGTH];
			if (!m_iHopsPositionBuffer) {
				Release();
				return FALSE;
			}

			m_pSSB_hop	=	new CSSBHopvmld;
			if (m_pSSB_hop==NULL)
			{
				Release();
				return FALSE;
			}
			m_pSSB_fixed=	NULL;
			bRet = m_pSSB_hop->Create(BULK_SIZE,nSSBType,40e3,10,TRUE);
			if (bRet!=DSP_RET_SUCCESS)
			{
				Release();
				return FALSE;
			}

		}else{//Fixed ssb
			m_pSSB_fixed=	new CSSBFixedvmld;
			m_pSSB_hop	=	NULL;
			if (m_pSSB_fixed==NULL)
			{
				Release();
				return FALSE;
			}
			bRet = m_pSSB_fixed->Create(BULK_SIZE,nSSBType,40e3,2);
			if (bRet!=DSP_RET_SUCCESS)
			{
				Release();
				return FALSE;
			}
		}//if bFixed 

		//create filter; (OLA)
	/*	bRet = m_filter.Create(BULK_SIZE,m_stFilterCoefBuffer.m_nSize,
			m_stFilterCoefBuffer.m_fCoefficients);
		if (bRet!=DSP_RET_SUCCESS)
		{
			Release();
			return FALSE;
		}*/

		//create filter; (OLD Filter)
		bRet = m_filter.Create(m_stFilterCoefBuffer.m_fCoefficients,
			m_stFilterCoefBuffer.m_nSize);
		if (bRet == FALSE)
		{
			Release();
			return FALSE;
		}

		Reset();
		return TRUE;
	}//func

	void CSSBDemodulator::Reset()
	{
		CDemodulatorBase::Reset();
		if (m_pSSB_fixed) m_pSSB_fixed->Reset();
		if (m_pSSB_hop) m_pSSB_hop->Reset();
		m_filter.Reset();
		m_nInputBufferLength	=	0;//empty buffer
	}//func

	void CSSBDemodulator::Release()
	{
		//CDemodulatorBase::Release();
		SAFERELEASE_ARRAY(m_fInputBuffer);
		SAFERELEASE_ARRAY(m_fFirOutBuffer);
		SAFERELEASE_ARRAY(m_fDemodOutBuffer);
		SAFERELEASE_ARRAY(m_iRecordBuffer);
		SAFERELEASE_ARRAY(m_iHopsPositionBuffer);
		//m_filter.Release(); //only for OLA
		SAFERELEASE(m_pSSB_fixed);
		SAFERELEASE(m_pSSB_hop);
	}


	void CSSBDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		int i , tmp;
		nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		m_nInputBufferLength=0;
		for (i=0;i<nSize;i++)
		{
			if (GetSampleID(m_pInBuffer[i]) != nID) 
				continue;//samples is not belong to this demodulator

			//if hopping get hop positions
			if (bFixed == FALSE)
			{
				//find hop positions
				tmp		=	m_pInBuffer[m_nInputBufferLength];
				m_iHopsPositionBuffer[m_nInputBufferLength] = (tmp & 0x10000) ? 1:0;				
				tmp		=	tmp & 0xffff;
				if(tmp & 0x8000) tmp |= 0xffff0000;
				else			 tmp &= 0xffff;
				m_fInputBuffer[m_nInputBufferLength] = double(tmp);
				m_nInputBufferLength++;
			}else {//fixed
				tmp=m_pInBuffer[i] & 0x0000ffff;
				if (m_pInBuffer[i] & 0x00008000)	tmp = tmp |0xffff0000;
				m_fInputBuffer[m_nInputBufferLength] = double(tmp);
				m_nInputBufferLength++;
			}//bFixed
		}
	}//func

	void CSSBDemodulator::CalculateMSB()
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
	BOOL CSSBDemodulator::LoadFiltersCoefficients()
	{
		BOOL bRet ;
		bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/SSB_Buffer_FIR.txt");
		if (!bRet ) return FALSE;
		//bRet = m_stFilterCoefHilbert.ReadCoefsFromFile("../FIR_Coef/AM_Hilbert_FIR.txt");
		//if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CSSBDemodulator::UnLoadFiltersCoefficients()
	{
		m_stFilterCoefBuffer.Release();
		//m_stFilterCoefOutput.Release();
	}//func

	BOOL CSSBDemodulator::Demodulate()
	{
		int i,nRet;

		if (bFixed){ //Fixed SSB
		/*	new_fc=m_ssb_fixed->GetFc();
			if (new_fc && m_ssb_fixed->GetState() == _DSPLIB_SSBFIXED_FINE_ESTIMATE_STATE
				&& f_tune!=prev_f_tune){
					new_fc-=prev_f_tune;
					new_fc+=f_tune;
					m_ssb_fixed->SetFc(new_fc);
					prev_f_tune=f_tune;
			}*/
			//OLA
		//	nRet = m_filter.Filter(m_fInputBuffer,m_nInputBufferLength,
		//		m_fFirOutBuffer,m_nFirOutBufferLength);

			//	OLD Filters
			for (i=0;i<m_nInputBufferLength;i++)
				m_fFirOutBuffer[i] = m_filter.Filter(m_fInputBuffer[i]);
			m_nFirOutBufferLength = m_nInputBufferLength;

		//	nRet = m_pSSB_fixed->Demodulate(m_fFirOutBuffer,m_nFirOutBufferLength,
		//		m_fDemodOutBuffer,m_nDemodOutBufferLength);
		
		}else{//Hopping
	/*		new_fc=m_ssb_hop->GetFc();
			if (f_tune!=prev_f_tune){
				new_fc-=prev_f_tune;
				new_fc+=f_tune;
				m_ssb_hop->SetFc(new_fc);
				prev_f_tune=f_tune;
			}*/
			//OLA
				//nRet = m_filter.Filter(m_fInputBuffer,m_nInputBufferLength,
				//	m_fFirOutBuffer,m_nFirOutBufferLength);

			//	OLD Filters
			for (i=0;i<m_nInputBufferLength;i++)
				m_fFirOutBuffer[i] = m_filter.Filter(m_fInputBuffer[i]);
			m_nFirOutBufferLength = m_nInputBufferLength;

			nRet = m_pSSB_hop->Demodulate(m_fFirOutBuffer,m_iHopsPositionBuffer,
				m_nFirOutBufferLength,
				&pVector,m_nDemodOutBufferLength);
			
		}//if Fixed

		//prepare Record Buffer
		for (i=0;i<m_nDemodOutBufferLength;i++)
			m_iRecordBuffer[i] = (short)m_iRecordBuffer[i] = (short)(m_fDemodOutBuffer[i]*CSharedVariables::m_fVolume);
		return TRUE;
	}//func

	BOOL CSSBDemodulator::Record(BOOL bStart)
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

	BOOL CSSBDemodulator::PlayOut(LPARAM lParam)
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
				(short)(m_fDemodOutBuffer[i] * CSharedVariables::m_fVolume);
			CSharedVariables::m_nAudioBufferFullCounter++;
		}
		return TRUE;
	}//func

}//namespace