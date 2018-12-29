#include "../StdAfx.h"
#include "DigitalLibDemodulator.h"
//#include "../dsp/DemodulatorBase.h"

namespace NS_DSP
{
	FILTER_COEFS	CDigitalLibDemodulator::m_stLPFCoefBuffer;
	FILTER_COEFS	CDigitalLibDemodulator::m_stBPFCoefBuffer;
	FILTER_COEFS	CDigitalLibDemodulator::m_stMatchedFilter;

	CDigitalLibDemodulator::CDigitalLibDemodulator(void)
	{
		m_fFs							=	80e3;
		m_fFc							=	20e3;
		m_fBW							=	0;
		m_fSNR							=	0.0;
		m_nID							=	-1;
		m_fCICGain						=	0.0;
		m_fPhi01EndStr					=	0.0;
		m_nTotalSize					=	0;
		m_nLenghtBuffer					=	0;
		nCICDecimatorLenght				=	0;
		m_nDownSampleRate					=	0;
		m_nInputBufferLength			=	0;
		m_nInputFilterOutBufferLength	=	0;

		m_nEndCICI01I01					=	0;
		m_nEndCICI02I01					=	0;
		m_nEndCICI03I01					=	0;
		m_nEndCICI01Q01					=	0;
		m_nEndCICI02Q01					=	0;
		m_nEndCICI03Q01					=	0;
		m_nSI01Lenght					=	0;
		m_nSQ01Lenght					=	0;
		m_nLenghtBuffer1				=	0;
		m_nLenghtBuffer2				=	0;

		m_fFcEstimation					=	0.0;
		m_fRsEstimation					=	0.0;

		m_fSTemp						= NULL;
		m_nSTemp						= NULL;
		m_nBufferTemp					= NULL;
		m_nBufferTempAssist				= NULL;
		m_fBuffer						= NULL;
		m_dSTemp						= NULL;
		m_dBufferTemp					= NULL;
		m_dBufferTempAssist				= NULL;
		//m_fInputBuffer					= NULL;
		//m_fInputFilterOutBuffer			= NULL;
		m_pWndRsEstimation				= NULL;
		m_fOutFilterBuffer				= NULL;
		m_fRealInp						= NULL;
		m_fRealMInput					= NULL;
		m_fImgInp						= NULL;
		m_fImgMInput					= NULL;
		
		m_fPhiIncTerm01					= NULL;
		m_fPhi01						= NULL;
		m_fLoI01						= NULL;
		m_fLoQ01						= NULL;
		m_dblTempBuffer					= NULL;
		m_fBuffer1						= NULL;
		m_fBuffer2						= NULL;

		m_fBufferTemp					= NULL ;
		m_fCICRI01Save					= NULL;
		m_fCICC01I01Save				= NULL;
		m_fCICC02I01Save				= NULL;
		m_fCICRQ01Save					= NULL;
		m_fCICC01Q01Save				= NULL;
		m_fCICC02Q01Save				= NULL;


		m_bIfInitEst					= FALSE;
		m_bStartDemod					= FALSE;
		m_bStartRsEst					= FALSE;
		m_bDoStmationStat1				= FALSE;
		m_bIsFirstFrame					= TRUE;
		m_bCalcFcEstimatin				= FALSE;
		m_bCalcRsEstimation				= FALSE;

		//*********** carrier estimation parameters
		m_dblF					= NULL;
		m_nValidIdxStart		= 0;
		m_nValidIdxEnd			= 0;
		m_nAvrLen01				= 0;
		m_nAvrLen02				= 0;
		m_nAvrDelay01			= 0;
		m_nAvrDelay02			= 0;
		m_nNFFT					= 0;
	}

	CDigitalLibDemodulator::~CDigitalLibDemodulator(void)
	{
		Release();
	}

	BOOL CDigitalLibDemodulator::IsLoadedFIRCoef()
	{
		if ((!m_stLPFCoefBuffer.IsEmpty()) && (!m_stBPFCoefBuffer.IsEmpty()) && (!m_stMatchedFilter.IsEmpty())) 
			return TRUE;
		return FALSE;
	}

	BOOL CDigitalLibDemodulator::Create(DEMOD_PARAM stParam)
	{
		m_fFs = stParam.Fs;
		m_fFc = stParam.Fc;
		CAnalyzeChannel::Create();
		//m_fSTemp = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		//if (!m_fSTemp)
		//{
		//	Release();
		//	return FALSE;
		//}

		if (!IsLoadedFIRCoef()) {
			Release();
			return FALSE;
		}

		m_nSTemp = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_nSTemp)
		{
			Release();
			return FALSE;
		}
		m_nBufferTemp = new __int64[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_nBufferTemp)
		{
			Release();
			return FALSE;
		}
		m_nBufferTempAssist = new __int64[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_nBufferTempAssist)
		{
			Release();
			return FALSE;
		} 
		m_dSTemp = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_nSTemp)
		{
			Release();
			return FALSE;
		}
		m_dBufferTemp = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_nBufferTemp)
		{
			Release();
			return FALSE;
		}
		m_dBufferTempAssist = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_nBufferTempAssist)
		{
			Release();
			return FALSE;
		} 

		m_bIfInitEst		= FALSE;

		m_fBuffer = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fBuffer)
		{
			Release();
			return FALSE;
		}

		////input Buffer
		//m_fInputBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH + 10];
		//if (!m_fInputBuffer) {
		//	Release();
		//	return FALSE;
		//}

		//m_fInputFilterOutBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH + 10];
		//if (!m_fInputFilterOutBuffer)
		//{
		//	Release();
		//	return FALSE;
		//}

		m_dblF = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_dblF)
		{
			Release();
			return FALSE;
		}

		m_fOutFilterBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fOutFilterBuffer)
		{
			Release();
			return FALSE;
		}

		m_fRealInp = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fRealInp)
		{
			Release();
			return FALSE;
		}
		m_fImgInp = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fImgInp)
		{
			Release();
			return FALSE;
		}


		m_fPhiIncTerm01 = new double [HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fPhiIncTerm01)
		{
			Release();
			return FALSE;
		}
		m_fPhi01 = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fPhi01)
		{
			Release();
			return FALSE;
		}
		m_fLoI01 = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fLoI01)
		{
			Release();
			return FALSE;
		}
		m_fLoQ01 = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fLoQ01)
		{
			Release();
			return FALSE;
		}
		m_dblTempBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_dblTempBuffer)
		{
			Release();
			return FALSE;
		}

		m_fBufferTemp = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fBufferTemp)
		{
			Release();
			return FALSE;
		}
		m_fCICRI01Save = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fCICRI01Save)
		{
			Release();
			return FALSE;
		}
		m_fCICC01I01Save = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fCICC01I01Save)
		{
			Release();
			return FALSE;
		}
		m_fCICC02I01Save = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fCICC02I01Save)
		{
			Release();
			return FALSE;
		}
		m_fCICRQ01Save = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fCICRQ01Save)
		{
			Release();
			return FALSE;
		}
		m_fCICC01Q01Save = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if(!m_fCICC01Q01Save)
		{
			Release();
			return FALSE;
		}
		m_fCICC02Q01Save = new __int64[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fCICC02Q01Save)
		{
			Release();
			return FALSE;
		}

		m_fRealMInput = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fRealMInput)
		{
			Release();
			return FALSE;
		}

		m_fImgMInput = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if(!m_fImgMInput)
		{
			Release();
			return FALSE;
		}


		m_fBuffer1 = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fBuffer1)
		{
			Release();
			return FALSE;
		}

		m_fBuffer2 = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fBuffer2)
		{
			Release();
			return FALSE;
		}

		//Create Filters
		if (m_InputFilter.Create((HF_MAX_DEMOD_BUFFER_LENGTH),m_stBPFCoefBuffer.m_nSize,
			m_stBPFCoefBuffer.m_fCoefficients) != DSP_RET_SUCCESS)
		{
			Release(); 
			return FALSE;
		}

		Reset();
		return TRUE;
	}

	void CDigitalLibDemodulator::Release()
	{
		CAnalyzeChannel::Release();
		m_InputFilter.Release();
		m_InputFilter1.Release();
		m_InputFilter2.Release();
		SAFERELEASE_ARRAY(m_fBuffer);
		SAFERELEASE_ARRAY(m_dblF);
		SAFERELEASE_ARRAY(m_fRealInp);
		SAFERELEASE_ARRAY(m_fImgInp);
		//SAFERELEASE_ARRAY(m_fInputBuffer);
		SAFERELEASE_ARRAY(m_fOutFilterBuffer);
		//SAFERELEASE_ARRAY(m_fInputFilterOutBuffer);

		SAFERELEASE_ARRAY(m_fPhiIncTerm01 );
		SAFERELEASE_ARRAY(m_fPhi01);
		SAFERELEASE_ARRAY(m_fLoI01);
		SAFERELEASE_ARRAY(m_fLoQ01);
		SAFERELEASE_ARRAY(m_dblTempBuffer);
		SAFERELEASE_ARRAY(m_fBufferTemp);
		SAFERELEASE_ARRAY(m_fCICRI01Save);
		SAFERELEASE_ARRAY(m_fCICC01I01Save);
		SAFERELEASE_ARRAY(m_fCICC02I01Save);
		SAFERELEASE_ARRAY(m_fCICRQ01Save);
		SAFERELEASE_ARRAY(m_fCICC01Q01Save);
		SAFERELEASE_ARRAY(m_fCICC02Q01Save);
		SAFERELEASE_ARRAY(m_fRealMInput);
		SAFERELEASE_ARRAY(m_fImgMInput);
		SAFERELEASE_ARRAY(m_fBuffer1);
		SAFERELEASE_ARRAY(m_fBuffer2);

		SAFERELEASE(m_pWndRsEstimation);
		
		if(m_fSTemp) {delete[] m_fSTemp; m_fSTemp=NULL;}
		if(m_nBufferTemp) {delete[] m_nBufferTemp; m_nBufferTemp=NULL;}
		if(m_nSTemp) {delete[] m_nSTemp; m_nSTemp=NULL;}
		if(m_nBufferTempAssist) {delete[] m_nBufferTempAssist; m_nBufferTempAssist=NULL;}
		if(m_dBufferTemp) {delete[] m_nBufferTemp; m_nBufferTemp=NULL;}
		if(m_dSTemp) {delete[] m_nSTemp; m_nSTemp=NULL;}
		if(m_dBufferTempAssist) {delete[] m_nBufferTempAssist; m_nBufferTempAssist=NULL;}

	}

	void CDigitalLibDemodulator::Integrators(int nBufferLen,double *fInputBuffer,double *fInputDecimator,__int64 *fOutput,__int64 &nEndCIC01,__int64 &nEndCIC02,__int64 &nEndCIC03)
	{
		vdMul(nBufferLen,fInputBuffer,fInputDecimator,m_fSTemp);
		for (int i=0;i<nBufferLen;i++)
		{
			__int64 nTemp10;
			nTemp10 = (__int64)m_fSTemp[i];
			m_nSTemp[i] = nTemp10;
		}

		CumsumInt64(nEndCIC01,m_nSTemp,m_nBufferTemp,nBufferLen);
		nEndCIC01 = m_nBufferTemp[nBufferLen-1];
		CumsumInt64(nEndCIC02,m_nBufferTemp,m_nBufferTempAssist,nBufferLen);
		memcpy(m_nBufferTemp,m_nBufferTempAssist,nBufferLen*sizeof(__int64));
		nEndCIC02 = m_nBufferTemp[nBufferLen-1];
		CumsumInt64(nEndCIC03,m_nBufferTemp,m_nBufferTempAssist,nBufferLen);
		memcpy(fOutput,m_nBufferTempAssist,nBufferLen*sizeof(__int64));
		nEndCIC03 = m_nBufferTempAssist[nBufferLen-1];
	}
	void CDigitalLibDemodulator::IntegratorsQam(int nBufferLen,double *fInputBuffer,double *fInputDecimator,double *fOutput,double &nEndCIC01,double &nEndCIC02,double &nEndCIC03)
	{
		vdMul(nBufferLen,fInputBuffer,fInputDecimator,m_fSTemp);
		CumsumDouble(nEndCIC01,m_fSTemp,m_dBufferTemp,nBufferLen);
		nEndCIC01 = m_dBufferTemp[nBufferLen-1];
		CumsumDouble(nEndCIC02,m_dBufferTemp,m_dBufferTempAssist,nBufferLen);
		memcpy(m_dBufferTemp,m_dBufferTempAssist,nBufferLen*sizeof(double));
		nEndCIC02 = m_dBufferTemp[nBufferLen-1];
		CumsumDouble(nEndCIC03,m_dBufferTemp,m_dBufferTempAssist,nBufferLen);
		memcpy(fOutput,m_dBufferTempAssist,nBufferLen*sizeof(double));
		nEndCIC03 = m_dBufferTempAssist[nBufferLen-1];
	}
	void CDigitalLibDemodulator::IntegratorsQamother(int nBufferLen,double *fInputBuffer,double *fInputDecimator,double *fOutput,double &nEndCIC01,double &nEndCIC02,double &nEndCIC03)
	{

	/*	vdMul(nBufferLen,fInputBuffer,fInputDecimator,m_fSTemp);
		m_dBufferTemp[0]= nEndCIC01;
		CumsumDouble(nEndCIC01,m_fSTemp,m_dBufferTemp+1,nBufferLen);
		nEndCIC01 = m_dBufferTemp[nBufferLen-1];
		m_dBufferTempAssist[0]= nEndCIC02;
		CumsumDouble(nEndCIC02,m_dBufferTemp+1,m_dBufferTempAssist+1,nBufferLen);
		memcpy(m_dBufferTemp,m_dBufferTempAssist,nBufferLen*sizeof(double));
		nEndCIC02 = m_dBufferTempAssist[nBufferLen-1];
		m_dBufferTempAssist[0]= nEndCIC03;
		CumsumDouble(nEndCIC03,m_dBufferTemp+1,m_dBufferTempAssist+1,nBufferLen);
		memcpy(fOutput,m_dBufferTempAssist,nBufferLen*sizeof(double));
		nEndCIC03 = m_dBufferTempAssist[nBufferLen-1];*/

		vdMul(nBufferLen,fInputBuffer,fInputDecimator,m_fSTemp);
		m_dBufferTemp[0]= nEndCIC01;
		CumsumDouble(nEndCIC01,m_fSTemp,m_dBufferTemp+1,nBufferLen);
		nEndCIC01 = m_dBufferTemp[nBufferLen];
		m_dBufferTempAssist[0]= nEndCIC02;
		CumsumDouble(nEndCIC02,m_dBufferTemp+1,m_dBufferTempAssist+1,nBufferLen);
		memcpy(m_dBufferTemp,m_dBufferTempAssist,(nBufferLen+1)*sizeof(double));
		nEndCIC02 = m_dBufferTempAssist[nBufferLen];
		m_dBufferTempAssist[0]= nEndCIC03;
		CumsumDouble(nEndCIC03,m_dBufferTemp+1,m_dBufferTempAssist+1,nBufferLen);
		memcpy(fOutput,m_dBufferTempAssist,(nBufferLen+1)*sizeof(double));
	/*	FILE *fpa3 = fopen("d:\\out.txt","at");
		for (int i=0;i<nBufferLen+1;i++){
		fprintf(fpa3,"%lg\n",(double)fOutput[i]);
		}
		fclose(fpa3);*/
		nEndCIC03 = m_dBufferTempAssist[nBufferLen];
	}

	void CDigitalLibDemodulator::FirstFrameCIC(int nLength,__int64 *input,int nDownSampleRate,double *Output, 
		double CICGain,__int64 *CICR,__int64 *CICC01,__int64 *CICC02,int &nBufferSize)
	{
		nBufferSize  = 0;
		//CIC_R
		for (int i=0;i<nLength;i+=nDownSampleRate)
		{
			m_nBufferTemp[i/nDownSampleRate] = input[i];
			nBufferSize++;
		}
		memset(m_nBufferTemp+nBufferSize,0,nBufferSize*sizeof(__int64));
		memcpy(CICR,m_nBufferTemp+nBufferSize-HF_DIGITAL_CIC_M,HF_DIGITAL_CIC_M*sizeof(__int64));

		//CIC_C01_I01
		m_nBufferTempAssist[0] = m_nBufferTemp[0];
		m_nBufferTempAssist[1] = m_nBufferTemp[1];
		for (int i=HF_DIGITAL_CIC_M;i<(nBufferSize);i++)
		{
			m_nBufferTempAssist[i] = m_nBufferTemp[i] - m_nBufferTemp[i-HF_DIGITAL_CIC_M];
		}
		memcpy(m_nBufferTemp,m_nBufferTempAssist,nBufferSize*sizeof(__int64));
		memcpy(CICC01,m_nBufferTemp+nBufferSize-HF_DIGITAL_CIC_M,HF_DIGITAL_CIC_M*sizeof(__int64));
		m_nBufferTempAssist[0] = m_nBufferTemp[0];
		m_nBufferTempAssist[1] = m_nBufferTemp[1];
		for (int i=HF_DIGITAL_CIC_M;i<(nBufferSize);i++)
		{
			m_nBufferTempAssist[i] = m_nBufferTemp[i] - m_nBufferTemp[i-HF_DIGITAL_CIC_M];
		}
		memcpy(m_nBufferTemp,m_nBufferTempAssist,nBufferSize*sizeof(__int64));
		memcpy(CICC02,m_nBufferTemp+nBufferSize-HF_DIGITAL_CIC_M,HF_DIGITAL_CIC_M*sizeof(__int64));

		Output[0] = 0;
		Output[1] = 0;
		for (int j=HF_DIGITAL_CIC_M;j<nBufferSize;j++)
		{
			Output[j] = (__int64)(m_nBufferTemp[j] - m_nBufferTemp[j-HF_DIGITAL_CIC_M])/CICGain;
		}
	}

	void CDigitalLibDemodulator::FirstFrameCICDouble(int nLength,double *input,int nDownSampleRate,double *Output, 
		double CICGain,double *CICR,double *CICC01,double *CICC02,int &nBufferSize)
	{
		nBufferSize  = 0;
		//CIC_R
		for (int i=0;i<nLength;i+=nDownSampleRate)
		{
			m_dBufferTemp[i/nDownSampleRate] = input[i];
			nBufferSize++;
		}
		memset(m_dBufferTemp+nBufferSize,0,nBufferSize*sizeof(double));
		memcpy(CICR,m_dBufferTemp+nBufferSize-HF_DIGITAL_CIC_M,HF_DIGITAL_CIC_M*sizeof(double));

		//CIC_C01_I01
		m_dBufferTempAssist[0] = m_dBufferTemp[0];
		m_dBufferTempAssist[1] = m_dBufferTemp[1];
		for (int i=HF_DIGITAL_CIC_M;i<(nBufferSize);i++)
		{
			m_dBufferTempAssist[i] = m_dBufferTemp[i] - m_dBufferTemp[i-HF_DIGITAL_CIC_M];
		}
		memcpy(m_dBufferTemp,m_dBufferTempAssist,nBufferSize*sizeof(double));
		memcpy(CICC01,m_dBufferTemp+nBufferSize-HF_DIGITAL_CIC_M,HF_DIGITAL_CIC_M*sizeof(double));
		m_dBufferTempAssist[0] = m_dBufferTemp[0];
		m_dBufferTempAssist[1] = m_dBufferTemp[1];
		for (int i=HF_DIGITAL_CIC_M;i<(nBufferSize);i++)
		{
			m_dBufferTempAssist[i] = m_dBufferTemp[i] - m_dBufferTemp[i-HF_DIGITAL_CIC_M];
		}
		memcpy(m_dBufferTemp,m_dBufferTempAssist,nBufferSize*sizeof(double));
		memcpy(CICC02,m_dBufferTemp+nBufferSize-HF_DIGITAL_CIC_M,HF_DIGITAL_CIC_M*sizeof(double));

		Output[0] = m_dBufferTemp[0]/CICGain;
		Output[1] = m_dBufferTemp[1]/CICGain;
		for (int j=HF_DIGITAL_CIC_M;j<nBufferSize;j++)
		{
			Output[j] = (double)(m_dBufferTemp[j] - m_dBufferTemp[j-HF_DIGITAL_CIC_M])/CICGain;
		}
	}
	void CDigitalLibDemodulator::OtherFrameCIC(int nLength,__int64 *input,int nDownSampleRate,int nStartIndex,double *OutputBuffer,
		double CICGain,__int64 *CICR,__int64 *CICC01,__int64 *CICC02,int &nBufferSize)
	{
		nBufferSize = 0;
		//CIC_R
		memcpy(m_nBufferTempAssist,CICR,HF_DIGITAL_CIC_M*sizeof(__int64));
		for (int i=0;i<(nLength-nStartIndex);i+=nDownSampleRate)
		{
			int nCnt = floor(double(i/nDownSampleRate))+HF_DIGITAL_CIC_M;
			m_nBufferTempAssist[nCnt] = input[i+nStartIndex];
			nBufferSize++;
		}
		memcpy(CICR,m_nBufferTempAssist+nBufferSize,HF_DIGITAL_CIC_M*sizeof(__int64));

		memcpy(m_nBufferTemp,m_nBufferTempAssist,((nBufferSize)+HF_DIGITAL_CIC_M)*sizeof(__int64));

		//CIC_C01
		memcpy(m_nBufferTempAssist,CICC01,HF_DIGITAL_CIC_M*sizeof(__int64));
		for (int i=0;i<(nBufferSize);i++)
		{
			m_nBufferTempAssist[i+HF_DIGITAL_CIC_M] = m_nBufferTemp[HF_DIGITAL_CIC_M+i] - m_nBufferTemp[i];
		}
		memcpy(m_nBufferTemp,m_nBufferTempAssist,((nBufferSize)+HF_DIGITAL_CIC_M)*sizeof(__int64));
		memcpy(CICC01,m_nBufferTemp+(nBufferSize),HF_DIGITAL_CIC_M*sizeof(__int64));

		//CIC_C02
		memcpy(m_nBufferTempAssist,CICC02,HF_DIGITAL_CIC_M*sizeof(__int64));
		for (int i=0;i<(nBufferSize);i++)
		{
			m_nBufferTempAssist[i+HF_DIGITAL_CIC_M] = m_nBufferTemp[HF_DIGITAL_CIC_M+i] - m_nBufferTemp[i];
		}
		memcpy(m_nBufferTemp,m_nBufferTempAssist,(nBufferSize+HF_DIGITAL_CIC_M)*sizeof(__int64));
		memcpy(CICC02,m_nBufferTemp+(nBufferSize),HF_DIGITAL_CIC_M*sizeof(__int64));

		//CIC_C03_I01
		for (int i=0;i<nBufferSize;i++)
		{
			OutputBuffer[i] = (__int64)(m_nBufferTemp[HF_DIGITAL_CIC_M+i] - m_nBufferTemp[i])/CICGain;
		}
	}
	void CDigitalLibDemodulator::OtherFrameCICDouble(int nLength,double *input,int nDownSampleRate,int nStartIndex,double *OutputBuffer,
		double CICGain,double *CICR,double *CICC01,double *CICC02,int &nBufferSize)
	{
		nBufferSize = 0;
		//CIC_R
		memcpy(m_dBufferTempAssist,CICR,HF_DIGITAL_CIC_M*sizeof(double));
		for (int i=0;i<(nLength-nStartIndex);i+=nDownSampleRate)
		{
			int nCnt = floor(double(i/nDownSampleRate))+HF_DIGITAL_CIC_M;
			m_dBufferTempAssist[nCnt] = input[i+nStartIndex];
			nBufferSize++;
		}
		memcpy(CICR,m_dBufferTempAssist+nBufferSize,HF_DIGITAL_CIC_M*sizeof(double));

		memcpy(m_dBufferTemp,m_dBufferTempAssist,((nBufferSize)+HF_DIGITAL_CIC_M)*sizeof(double));

			//CIC_C01
		memcpy(m_dBufferTempAssist,CICC01,HF_DIGITAL_CIC_M*sizeof(double));
		for (int i=0;i<(nBufferSize);i++)
		{
			m_dBufferTempAssist[i+HF_DIGITAL_CIC_M] = m_dBufferTemp[HF_DIGITAL_CIC_M+i] - m_dBufferTemp[i];
		}
		memcpy(m_dBufferTemp,m_dBufferTempAssist,((nBufferSize)+HF_DIGITAL_CIC_M)*sizeof(double));
		memcpy(CICC01,m_dBufferTemp+(nBufferSize),HF_DIGITAL_CIC_M*sizeof(double));

		
		//CIC_C02
		memcpy(m_dBufferTempAssist,CICC02,HF_DIGITAL_CIC_M*sizeof(double));
		for (int i=0;i<(nBufferSize);i++)
		{
			m_dBufferTempAssist[i+HF_DIGITAL_CIC_M] = m_dBufferTemp[HF_DIGITAL_CIC_M+i] - m_dBufferTemp[i];
		}
		memcpy(m_dBufferTemp,m_dBufferTempAssist,(nBufferSize+HF_DIGITAL_CIC_M)*sizeof(double));
		memcpy(CICC02,m_dBufferTemp+(nBufferSize),HF_DIGITAL_CIC_M*sizeof(double));

		
		//CIC_C03_I01
		for (int i=0;i<nBufferSize;i++)
		{
			OutputBuffer[i] = (double)(m_dBufferTemp[HF_DIGITAL_CIC_M+i] - m_dBufferTemp[i])/CICGain;
		}
	/*	 FILE * fpa3 = fopen("d:\\CIC3.txt","at");
			fprintf(fpa3,"%lg %lg %lg %lg %lg %lg\n",(double)CICR[0] ,CICR[1] ,CICC01[0] ,CICC01[1], CICC02[0], CICC02[1]);
		fclose(fpa3);*/
	}

	void CDigitalLibDemodulator::CumsumInt64(__int64 nPrimaryVal,__int64 *input,__int64 *Output,int nSize)
	{
		__int64 iSum = nPrimaryVal;
		for (int cnt=0;cnt<nSize;cnt++)
		{
			iSum = iSum + input[cnt];
			Output[cnt]=iSum;
		}
	}
	void CDigitalLibDemodulator::CumsumDouble(double nPrimaryVal,double *input,double *Output,int nSize)
	{
		double iSum = nPrimaryVal;
		for (int cnt=0;cnt<nSize;cnt++)
		{
			iSum = iSum + input[cnt];
			Output[cnt]=iSum;
		}
	}

	int CDigitalLibDemodulator::Remainder(int X,int Y)
	{
		ASSERT(Y != 0);
		int nDiv = floor((double)X/(double)Y);
		int nRem = X - nDiv * Y;
		return nRem;
	}

	void CDigitalLibDemodulator::InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)
	{
		//int i , tmp;
		//nSize = min(nSize,HF_MAX_DEMOD_BUFFER_LENGTH);
		//m_nInputBufferLength=0;
		//for (i=0;i<nSize;i++)
		//{
		//	tmp=m_pInBuffer[i] & 0x0000ffff;
		//	if (m_pInBuffer[i] & 0x00008000)	tmp = tmp |0xffff0000;
		//	m_fInputBuffer[m_nInputBufferLength] = double(tmp);
		//	m_nInputBufferLength++;
		//}
	}//func

	void CDigitalLibDemodulator::CalcRsEstimation(double fDF,double *m_pInBuffer,int nSize,
									BOOL bCalcFcEstimation,BOOL bCalcRsEstimation,BOOL bCalcBW,BOOL &bRsEst,
									double &fFcEst,double &fBW,double &fSNR,double &fRs,BOOL bIsBFSKFlag)
	{
		bRsEst = 0;
		double m_dblFcEst = 0;
		BOOL m_bisInitEst = FALSE;
 	//	CaptureSamples(m_pInBuffer,m_fInputFilterOutBuffer,nSize,0,m_bStartRsEst);

		//if (m_bStartRsEst == TRUE)
		//{
			m_InputFilter.Filter(m_pInBuffer,(HF_MAX_DEMOD_BUFFER_LENGTH-m_stBPFCoefBuffer.m_nSize+1),
				m_fOutFilterBuffer,m_nInputFilterOutBufferLength);

			m_nTotalSize++;

			//added for FcEstimation Calc or BW Calc
			if (!m_bCalcFcEstimatin){
				if ((bCalcFcEstimation) || (bCalcBW))
				{
					//m_bCalcFcEstimatin = FALSE;
					if ((m_nTotalSize < (HF_Fc_ESTIMATION_IT+1))&& (m_bDoStmationStat1 == FALSE))
					{
						CalculateAnalyzeItems(m_bisInitEst,m_fOutFilterBuffer,m_nTotalSize,
							m_bDoStmationStat1,m_fFcEstimation,m_dblFcEst,m_fBW,m_fSNR);
					}
					if (bCalcBW && m_nTotalSize == HF_Fc_ESTIMATION_IT)
					{
						m_bCalcFcEstimatin = TRUE;
						fBW = m_fBW;
						fFcEst = m_fFcEstimation;
						fSNR = m_fSNR;
					}
				}
			}
			if (bCalcRsEstimation && (m_bCalcFcEstimatin || !bCalcFcEstimation))
			{
				m_dblFcEst = fFcEst;
				double fSymbolRate = fBW/HF_DIGITAL_BW_TO_RS_COEF;
				ProcessData(fDF,fSymbolRate,fFcEst,m_fOutFilterBuffer,m_nInputFilterOutBufferLength,m_fRealInp,m_fImgInp);
			}

			if (bCalcRsEstimation && !bRsEst)
			{
				BOOL bTemp=0;

				CaptureSamples(m_fRealInp,m_fRealMInput,m_nSI01Lenght,bTemp);
				CaptureSamples(m_fImgInp,m_fImgMInput,m_nSQ01Lenght);
				if (bTemp)
				{
					//m_fBW=1600;
					m_pWndRsEstimation = new CNewRsEstimation;
					if (!m_pWndRsEstimation){Release();return;}
					m_pWndRsEstimation->CreateEstimator(m_fFs/m_nDownSampleRate,m_fBW/HF_DIGITAL_BW_TO_RS_COEF,m_fFcEstimation,m_fBW);
					MKL_Complex16 * m_cInp = new MKL_Complex16[HF_MAX_DEMOD_BUFFER_LENGTH];
					//RsEstimation
					for (int i=0;i<HF_MAX_DEMOD_BUFFER_LENGTH;i++)
					{
						m_cInp[i].imag = m_fImgMInput[i];
						m_cInp[i].real = m_fRealMInput[i];
					}

					m_pWndRsEstimation->SetInputDataBuffer(m_cInp,HF_MAX_DEMOD_BUFFER_LENGTH);
					double dblRs = m_pWndRsEstimation->GetSymbolRate(bIsBFSKFlag);
					fRs = dblRs;
					m_fRsEstimation = dblRs;
					bRsEst = 1;
					m_bCalcRsEstimation = TRUE;
					fFcEst = m_fFcEstimation;//****ezafe shavad
					m_pWndRsEstimation->Release();
					delete[] m_cInp;
					m_cInp = NULL;

				}
			}
		//}
	}

	//////////////////////////////////////////////////////////////////////////
	void CDigitalLibDemodulator::CaptureSamples(double *input,double *Output,int nLenght,
		BOOL &bStartDemod)
	{
		if (m_nLenghtBuffer1 < (HF_MAX_DEMOD_BUFFER_LENGTH+1))
		{
			memcpy(m_fBuffer1+m_nLenghtBuffer1,input,nLenght*sizeof(double));
			m_nLenghtBuffer1 += nLenght;
			m_bStartDemod = FALSE;
		}
		if (m_nLenghtBuffer1 >= (HF_MAX_DEMOD_BUFFER_LENGTH+1))
		{
			m_bStartDemod = TRUE;
			memcpy(Output,m_fBuffer1,(HF_MAX_DEMOD_BUFFER_LENGTH+1)*sizeof(double));
			m_nLenghtBuffer1 = m_nLenghtBuffer1 - (HF_MAX_DEMOD_BUFFER_LENGTH+1);
			memcpy(m_fBuffer1,m_fBuffer1+(HF_MAX_DEMOD_BUFFER_LENGTH+1),(m_nLenghtBuffer1)*sizeof(double));
			memset(m_fBuffer1+m_nLenghtBuffer1,0,(HF_MAX_DEMOD_BUFFER_LENGTH+1)*sizeof(double));
		}
		bStartDemod = m_bStartDemod;
	}

	void CDigitalLibDemodulator::CaptureSamples(double *input,double *Output,int nLenght)
	{
		if (m_nLenghtBuffer2 < (HF_MAX_DEMOD_BUFFER_LENGTH+1))
		{
			memcpy(m_fBuffer2+m_nLenghtBuffer2,input,nLenght*sizeof(double));
			m_nLenghtBuffer2 += nLenght;
			m_bStartDemod = FALSE;
		}
		if (m_nLenghtBuffer2 >= (HF_MAX_DEMOD_BUFFER_LENGTH+1))
		{
			m_bStartDemod = TRUE;
			memcpy(Output,m_fBuffer2,(HF_MAX_DEMOD_BUFFER_LENGTH+1)*sizeof(double));
			m_nLenghtBuffer2 = m_nLenghtBuffer2 - (HF_MAX_DEMOD_BUFFER_LENGTH+1);
			memcpy(m_fBuffer2,m_fBuffer2+(HF_MAX_DEMOD_BUFFER_LENGTH+1),(m_nLenghtBuffer2)*sizeof(double));
			memset(m_fBuffer2+m_nLenghtBuffer2,0,(HF_MAX_DEMOD_BUFFER_LENGTH+1)*sizeof(double));
		}
	}


	//////////////////////////////////////////////////////////////////////////

	void CDigitalLibDemodulator::CaptureSamples(double *input,double *Output,int nLenght,int nCoefSize,
									BOOL &bStartDemod)
	{
		if (m_nLenghtBuffer < (HF_MAX_DEMOD_BUFFER_LENGTH-nCoefSize+1))
		{
			memcpy(m_fBuffer+m_nLenghtBuffer,input,nLenght*sizeof(double));
			m_nLenghtBuffer += nLenght;
			m_bStartDemod = FALSE;
		}
		if (m_nLenghtBuffer >= (HF_MAX_DEMOD_BUFFER_LENGTH-nCoefSize+1))
		{
			m_bStartDemod = TRUE;
			memcpy(Output,m_fBuffer,(HF_MAX_DEMOD_BUFFER_LENGTH-nCoefSize+1)*sizeof(double));
			m_nLenghtBuffer = m_nLenghtBuffer - (HF_MAX_DEMOD_BUFFER_LENGTH-nCoefSize+1);
			memcpy(m_fBuffer,m_fBuffer+(HF_MAX_DEMOD_BUFFER_LENGTH-nCoefSize+1),(m_nLenghtBuffer)*sizeof(double));
			memset(m_fBuffer+m_nLenghtBuffer,0,(HF_MAX_DEMOD_BUFFER_LENGTH-nCoefSize+1)*sizeof(double));
		}
		bStartDemod = m_bStartDemod;
	}

	void CDigitalLibDemodulator::CalculateAnalyzeItems(BOOL bisInitEst,double *m_fOutFilterBuffer,int nTotalSize,
									BOOL &m_bDoStmationStat1,double &m_fFcEstimation,double &dblFcEst,double &fBW,double &fSNR)
	{
		double dblSNR = 0;
		if (!bisInitEst)
		{
			CAnalyzeChannel::InitEstimationCarrier(m_fFs,m_nNFFT,m_dblF,m_nValidIdxStart,
				m_nValidIdxEnd,m_nAvrLen01,m_nAvrLen02,m_nAvrDelay01,m_nAvrDelay02);
		}
		CAnalyzeChannel::EstimationCarrier(m_nNFFT,m_dblF,m_nValidIdxStart,
			m_nValidIdxEnd,m_nAvrLen01,m_nAvrLen02,m_nAvrDelay01,m_nAvrDelay02,
			m_fOutFilterBuffer,m_fFcEstimation,m_fBW,dblSNR);
		m_fSNR += dblSNR;

		if (nTotalSize == HF_Fc_ESTIMATION_IT)
		{
			m_bDoStmationStat1 = TRUE;
			m_fFcEstimation = m_fFcEstimation/(nTotalSize);
			fBW = 4*m_fBW;
			fSNR = m_fSNR/nTotalSize;
			dblFcEst = m_fFcEstimation;
		}

	}//func

	void CDigitalLibDemodulator::Reset()
	{

		m_bIsFirstFrame			=	TRUE;
		m_bStartDemod			=	FALSE;
		m_nTotalSize			=	0;
		m_nInputBufferLength	=	0;
		m_nLenghtBuffer			=	0;
		m_nLenghtBuffer			=	0;
		m_nSI01Lenght			=	0;
		m_nSQ01Lenght			=	0;
		m_nEndCICI01I01			=	0;
		m_nEndCICI02I01			=	0;
		m_nEndCICI03I01			=	0;
		m_nEndCICI01Q01			=	0;
		m_nEndCICI02Q01			=	0;
		m_nEndCICI03Q01			=	0;
		m_nTotalSize			= 0;
		m_nLenghtBuffer			= 0;
		m_nLenghtBuffer2	    = 0;
		m_nLenghtBuffer1		= 0;

		memset(m_fBuffer,0,HF_MAX_DEMOD_BUFFER_LENGTH*sizeof(double));
		memset(m_fBuffer2,0,HF_MAX_DEMOD_BUFFER_LENGTH*sizeof(double));
		memset(m_fBuffer1,0,HF_MAX_DEMOD_BUFFER_LENGTH*sizeof(double));

		m_InputFilter.Reset(); 
		m_InputFilter1.Reset();
		m_InputFilter2.Reset();
	}

	BOOL CDigitalLibDemodulator::LoadFiltersCoefficients(CString strMainAddr)
	{
		BOOL bRet ;
		CString strAddr;
		strAddr.Format("%sFIR_Coef\\BPSK_Buffer_LPF.txt", strMainAddr);
		bRet = m_stLPFCoefBuffer.ReadCoefsFromFile(strAddr.GetBuffer());
		if (!bRet ) return FALSE;

		strAddr.Format("%sFIR_Coef\\BFSK_Buffer_BPF.txt", strMainAddr);
		bRet = m_stBPFCoefBuffer.ReadCoefsFromFile(strAddr.GetBuffer());
		if (!bRet ) return FALSE;

		strAddr.Format("%sFIR_Coef\\MatchedFilterForRsEst.txt", strMainAddr);
		bRet = m_stMatchedFilter.ReadCoefsFromFile(strAddr.GetBuffer());
		if (!bRet) return FALSE;

		return TRUE;
	}//func

	void CDigitalLibDemodulator::UnLoadFiltersCoefficients()
	{
		m_stLPFCoefBuffer.Release();
		m_stBPFCoefBuffer.Release();
		m_stMatchedFilter.Release();
	}//func

	void CDigitalLibDemodulator::ProcessData(double fDF,double fRs,double fFcEstimation,
		double *fOutFilterBuffer,int nInputFilterOutBufferLength,double *fSI01,double *fSQ01)
	{
		if (m_bIsFirstFrame == TRUE	)
		{
			FirstFrameInput(fDF,fRs,m_fFcEstimation,fOutFilterBuffer,m_nInputFilterOutBufferLength,m_fRealInp,m_fImgInp);
			m_bIsFirstFrame = FALSE;
		}else
		{
			OtherFramesInput(fDF,fRs,m_fFcEstimation,fOutFilterBuffer,m_nInputFilterOutBufferLength,m_fRealInp,m_fImgInp);
		}
	}//func

	BOOL	CDigitalLibDemodulator::FirstFrameInput(double fDF,double fRs,double fFcEstimation,
				double *fOutFilterBuffer,int nInputFilterOutBufferLength,double *fSI01,double *fSQ01)
	{
		int nBufferSize = 0;
		int i;
		nCICDecimatorLenght = 0;
		double fIntegratorOutput = 0.0;

		double m_fPrevDF = fDF;
		double m_fPrevRs = fRs;

		double fSPSInput = m_fFs / fRs;
		m_nDownSampleRate = (int)floor(double(fSPSInput / HF_DIGITAL_SPS_Target_Rs)+0.5);
		if (m_nDownSampleRate<1)
		{
			m_nDownSampleRate = 1;
		}
		if(m_nDownSampleRate > (int)floor(double(HF_AMR_ANALOG_FRAME_SIZE*HF_AMR_ANALOG_FRAME_MAX_COUNT/HF_MAX_DEMOD_BUFFER_LENGTH))-1)
			m_nDownSampleRate = (int)floor(double(HF_AMR_ANALOG_FRAME_SIZE*HF_AMR_ANALOG_FRAME_MAX_COUNT/HF_MAX_DEMOD_BUFFER_LENGTH))-1;
		double fSPSEstimation = fSPSInput / m_nDownSampleRate;
		m_fCICGain = pow((double)(m_nDownSampleRate * HF_DIGITAL_CIC_M),(double)HF_DIGITAL_CIC_N);

		if (m_InputFilter1.Create(floor((double)(HF_MAX_DEMOD_BUFFER_LENGTH-127+1)/m_nDownSampleRate),m_stMatchedFilter.m_nSize,
			m_stMatchedFilter.m_fCoefficients) != DSP_RET_SUCCESS)
		{
			Release(); 
			return FALSE;
		}

		if (m_InputFilter2.Create(floor((double)(HF_MAX_DEMOD_BUFFER_LENGTH-127+1)/m_nDownSampleRate),m_stMatchedFilter.m_nSize,
			m_stMatchedFilter.m_fCoefficients) != DSP_RET_SUCCESS)
		{
			Release();
			return FALSE;
		}
		//Reset();
		////////////////////////////////////////////////////

		for (i =0; i<nInputFilterOutBufferLength; i++)
		{
			m_fPhiIncTerm01[i] = 2* M_PI * fFcEstimation * (i+1) / m_fFs;
			m_fPhi01[i] = m_fPhiIncTerm01[i] - 2 * M_PI * fFcEstimation / m_fFs;
		}
		double fPhi01EndStr = m_fPhi01[nInputFilterOutBufferLength-1];

		vdCos(nInputFilterOutBufferLength,m_fPhi01,m_fLoI01);
		vdSin(nInputFilterOutBufferLength,m_fPhi01,m_fLoQ01);

		for (int j = 0;j<nInputFilterOutBufferLength;j++)
		{
			m_fLoQ01[j] = m_fLoQ01[j] * (-1);
		}
		//--------begin I01 routines---------------------------------------------------------------------------
		Integrators(nInputFilterOutBufferLength,fOutFilterBuffer,m_fLoI01,
			m_fBufferTemp,m_nEndCICI01I01,m_nEndCICI02I01,m_nEndCICI03I01);

		FirstFrameCIC(nInputFilterOutBufferLength,m_fBufferTemp,m_nDownSampleRate,m_dblTempBuffer,
			m_fCICGain,m_fCICRI01Save,m_fCICC01I01Save,m_fCICC02I01Save,nCICDecimatorLenght);

		m_InputFilter1.Filter(m_dblTempBuffer,nCICDecimatorLenght,fSI01,m_nSI01Lenght);
		//--------I01 End--------------------------------------------------------------------------------------
		//--------begin Q01 routines---------------------------------------------------------------------------
		Integrators(nInputFilterOutBufferLength,fOutFilterBuffer,m_fLoQ01,
			m_fBufferTemp,m_nEndCICI01Q01,m_nEndCICI02Q01,m_nEndCICI03Q01);

		FirstFrameCIC(nInputFilterOutBufferLength,m_fBufferTemp,m_nDownSampleRate,m_dblTempBuffer,
			m_fCICGain,m_fCICRQ01Save,m_fCICC01Q01Save,m_fCICC02Q01Save,nCICDecimatorLenght);

		m_InputFilter2.Filter(m_dblTempBuffer,nCICDecimatorLenght,fSQ01,m_nSQ01Lenght);
		m_nRem = Remainder(m_nInputFilterOutBufferLength-1,m_nDownSampleRate);

		return TRUE;
	}


	BOOL	CDigitalLibDemodulator::OtherFramesInput(double fDF,double fRs,double fFcEstimation,
		double *fOutFilterBuffer,int nInputFilterOutBufferLength,double *fSI01,double *fSQ01)
	{
		int nBufferSize = 0;;
		int i;
// 		double fPowerTSCO = 0.0;//time smoothing coefficient
		double fIntegratorOutput = 0.0;
		int nDownStartIdx1 = 0;
		nDownStartIdx1 = (m_nDownSampleRate - m_nRem);
		nDownStartIdx1 = nDownStartIdx1- 1;//I0: start down sampling from this index
		m_nRem = Remainder(m_nInputFilterOutBufferLength-(nDownStartIdx1+1),m_nDownSampleRate);

		for (i = 0; i<m_nInputFilterOutBufferLength; i++)
		{
			m_fPhi01[i] = m_fPhiIncTerm01[i] + m_fPhi01EndStr;
		}
		m_fPhi01EndStr = m_fPhi01[m_nInputFilterOutBufferLength-1];

		vdCos(m_nInputFilterOutBufferLength,m_fPhi01,m_fLoI01);
		vdSin(m_nInputFilterOutBufferLength,m_fPhi01,m_fLoQ01);
		for (int j = 0;j<m_nInputFilterOutBufferLength;j++)
		{
			m_fLoQ01[j] = m_fLoQ01[j] * (-1);
		}
		//--------begin I01 routines---------------------------------------------------------------------------
		Integrators(nInputFilterOutBufferLength,fOutFilterBuffer,m_fLoI01,
			m_fBufferTemp,m_nEndCICI01I01,m_nEndCICI02I01,m_nEndCICI03I01);

		OtherFrameCIC(m_nInputFilterOutBufferLength,m_fBufferTemp,m_nDownSampleRate,nDownStartIdx1,
			m_dblTempBuffer,m_fCICGain,m_fCICRI01Save,m_fCICC01I01Save,m_fCICC02I01Save,nCICDecimatorLenght);

		m_InputFilter1.Filter(m_dblTempBuffer,nCICDecimatorLenght,fSI01,m_nSI01Lenght);
		//--------I01 End--------------------------------------------------------------------------------------
		//--------begin Q01 routines---------------------------------------------------------------------------
		Integrators(m_nInputFilterOutBufferLength,fOutFilterBuffer,m_fLoQ01,
			m_fBufferTemp,m_nEndCICI01Q01,m_nEndCICI02Q01,m_nEndCICI03Q01);

		OtherFrameCIC(m_nInputFilterOutBufferLength,m_fBufferTemp,m_nDownSampleRate,nDownStartIdx1,
			m_dblTempBuffer,m_fCICGain,m_fCICRQ01Save,m_fCICC01Q01Save,m_fCICC02Q01Save,nCICDecimatorLenght);

		m_InputFilter2.Filter(m_dblTempBuffer,nCICDecimatorLenght,fSQ01,m_nSQ01Lenght);
		 
		return TRUE;
	}

	 //int	CDigitalLibDemodulator::GetSampleID(DWORD m_dwSample)
	 //{ 
	 //	return (m_dwSample >> 29);
	 //}
}
