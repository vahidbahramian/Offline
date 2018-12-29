#include "../StdAfx.h"
#include "NewRsEstimation.h"
#include <math.h>
#include "dsplib_debug.h"

#define		__FSTOP__					25e3
#define		__NFFT16__					16384
#define		ROUND(x)					(floor((x)+0.5))
#define     __EPS__						2.22044604925031e-016
#define		__FIR_TAP_CNT__				128
#define		__INTERNAL_FIR_TAP_CNT__	128
#define		__EXTRABUFFSIZE__			16*1024
#define		__PI						3.1415926535897932384626433832795

namespace NS_DSP
{
	CNewRsEstimation::CNewRsEstimation(void)
	{
	}

	CNewRsEstimation::~CNewRsEstimation(void)
	{
	}

	void CNewRsEstimation::InitParams()
	{
		m_dbl_BwCoef =		0.6;
		m_n_Sample =		floor(m_dbl_Fs/m_dbl_PrimeRs);
		m_dbl_ERROR0 =		0.9;
		//m_dbl_WRange =	(1 + m_dbl_Rolloff)*m_dbl_PrimeRs*m_dbl_ERROR0;
		m_dbl_LpfBw =		(1 + m_dbl_Rolloff)*m_dbl_PrimeRs*m_dbl_BwCoef;

		m_b_LpfM[0] =		1;
		m_b_LpfM[1] =		1;
		m_b_LpfM[2] =		0;
		m_b_LpfM[3] =		0;

		m_bFSKFlag  =		FALSE;

		m_n_DelayLen =		100;
		//m_n_DownRate =	floor(m_dbl_Fs / (6 * m_dbl_LpfBw))+1;
		/*m_dbl_FsDown =	m_dbl_Fs / m_nDownSampleRate;*/
		m_dbl_FsDown =		m_dbl_Fs;
		m_n_NFFT =			__NFFT16__; 

		// 		m_n_StartOfRange =	floor(0.5 * m_dbl_WRange * m_n_NFFT / m_dbl_FsDown);
		// 		m_n_StopOfRange =	min(ROUND(2.1 * m_dbl_WRange * m_n_NFFT / m_dbl_FsDown), __NFFT16__/2);

		//m_n_StartOfRange =	floor(0.5 * m_dbl_PrimeRs * m_n_NFFT / m_dbl_FsDown);
		//m_n_StopOfRange =	min(ROUND(1.8 * m_dbl_PrimeRs * m_n_NFFT / m_dbl_FsDown), __NFFT16__/2);

		m_n_StartOfRange = 10;
		m_n_StopOfRange = m_n_NFFT/2;


		//------------------------------------------------------------
		// 		m_n_RangeArray = new int [m_n_StopOfRange - m_n_StartOfRange + 1];
		// 
		// 		for (int i=m_n_StartOfRange; i<=m_n_StopOfRange; i++)
		// 			m_n_RangeArray[i-m_n_StartOfRange] = i;
		//------------------------------------------------------------
		m_n_DelayArray = new int [m_n_DelayLen];

		for (int i=0;i<m_n_DelayLen;i++) // 1..100
			m_n_DelayArray[i] = i+1;
		//------------------------------------------------------------
		m_cmplx_DeNumArray = new MKL_Complex16 [16384];

		for (int i=0;i<8192;i++)
		{
			m_cmplx_DeNumArray[i].real = 16384 - i;
			m_cmplx_DeNumArray[i].imag = 0;
		}

		for (int i=0;i<8192;i++)
		{
			m_cmplx_DeNumArray[i + 8192].real = 8193 + i;
			m_cmplx_DeNumArray[i + 8192].imag = 0;
		}
		FILE *fp;
		fopen_s(&fp, "D:\\DeNum.txt","wt");
		for (int i=0;i<16384;i++){
			fprintf(fp,"%f\n",m_cmplx_DeNumArray[i].real );
		}
		//------------------------------------------------------------
		m_cmplx_Yout = new MKL_Complex16 [m_n_NFFT];
        m_dbl_Yout = new double [m_n_NFFT];
		m_cmplx_FFTOut = new MKL_Complex16[m_n_NFFT];
		//---------------------------------------------------------

		//------------------------------------------------------------
		m_dbl_YYout = new double* [100];
		for (int i=0;i<100;i++)
			m_dbl_YYout[i] =new double [m_n_NFFT];

		for (int i=0;i<100;i++)
			for (int j=0;j<m_n_NFFT;j++)
				m_dbl_YYout[i][j] = 0;
		//------------------------------------------------------------
		//Init FFT Struct----------------------
		long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_DOUBLE,DFTI_COMPLEX,1, m_n_NFFT);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//return FALSE;
		}
		//   Set placement of result DFTI_NOT_INPLACE
		Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR))
		{
			//return FALSE;
		}

		double Scale = 1.0/(double)m_n_NFFT;

		Status = DftiSetValue(m_pDescFFT, DFTI_BACKWARD_SCALE, Scale);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//return FALSE;
		}

		//   Commit Dfti descriptor
		Status = DftiCommitDescriptor( m_pDescFFT );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//return FALSE;
		}

		//------------------------------------------------------------
		//Init FFT Struct----------------------
		long StatusR = DftiCreateDescriptor( &m_pDescFFTR, DFTI_DOUBLE,DFTI_REAL,1, m_n_NFFT);
		if(! DftiErrorClass(StatusR, DFTI_NO_ERROR)){
			//return FALSE;
		}
		//   Set placement of result DFTI_NOT_INPLACE
		StatusR = DftiSetValue( m_pDescFFTR, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(StatusR, DFTI_NO_ERROR))
		{
			//return FALSE;
		}

		double ScaleR = 1.0/(double)m_n_NFFT;

		StatusR = DftiSetValue(m_pDescFFTR, DFTI_BACKWARD_SCALE, ScaleR);
		if(! DftiErrorClass(StatusR, DFTI_NO_ERROR)){
			//return FALSE;
		}

		//   Commit Dfti descriptor
		StatusR = DftiCommitDescriptor( m_pDescFFTR );
		if(! DftiErrorClass(StatusR, DFTI_NO_ERROR)){
			//return FALSE;
		}
	}

	void CNewRsEstimation::CreateEstimator(double dblFs, double dblRs, double dblFc, double dblBW, double dblRolloff)
	{
		//ASSERT(!(dblBW == -1 && dblRs == -1));

		m_dbl_Fs =			dblFs;
		m_dbl_Rolloff =		dblRolloff;

		if (dblRs == -1) m_dbl_PrimeRs = dblBW / ((1 + m_dbl_Rolloff));
		else	m_dbl_PrimeRs = dblRs;

		if (dblBW == -1) m_dbl_PrimBW = (1 + m_dbl_Rolloff)*dblRs;
		else m_dbl_PrimBW = dblBW;

		InitParams();
	}

	BOOL CNewRsEstimation::PrepareRsSamplesForFSK(MKL_Complex16 *cInputBuff,int nLen, double *cOutputBuff )
	{
		double *dbAngle = new double[nLen];
		double *dbDiffAngle = new double[nLen];
		double * WinBuff = new double[nLen];


		/*FILE* fp = fopen("D:\\angel_real.txt","rt");
		double fTemp00=0;
		for (int i=0;i<16384;i++){

			fscanf_s(fp,"%lg\n",&fTemp00);
			cInputBuff[i].real = fTemp00;
		}
		fclose(fp);
		fp = fopen("D:\\angel_imag.txt","rt");
		fTemp00=0;
		for (int i=0;i<16384;i++){
			fscanf_s(fp,"%lg\n",&fTemp00);
			cInputBuff[i].imag = fTemp00;
		}
		fclose(fp);*/

		for (int i=0;i<nLen;i++)
		{
			dbAngle[i]=atan2(cInputBuff[i].imag,cInputBuff[i].real);
		}
		//FILE *fp = fopen("D:\\angel.txt","wt");
		//for (int i=0;i<nLen;i++){
		//	fprintf(fp,"%lg\n",dbAngle[i]);
		//}
		//fclose(fp);


		///// unwrap

		for (int i=1;i<nLen;i++)
		{
			double dbDiff=(dbAngle[i]-dbAngle[i-1]);

				if (dbDiff>=__PI)
				{
					dbAngle[i]=dbAngle[i]-2*__PI;
				}
				else if (dbDiff<=-__PI)
				{
					dbAngle[i]=dbAngle[i]+2*__PI;
				}
				//dbDiff=(dbAngle[i]-dbAngle[i-1]);
		}

// 		FILE* fp = fopen("D:\\unwrap.txt","wt");
// 		for (int i=0;i<nLen;i++){
// 			fprintf(fp,"%lg\n",dbAngle[i]);
// 		}
// 		fclose(fp);

		////////////////diff

		for (int i=0;i<nLen-1;i++)
		{
			dbDiffAngle[i]=dbAngle[i+1]-dbAngle[i];
		}
		dbDiffAngle[nLen-1]=dbAngle[nLen-1]-dbAngle[16384-2];

		
		////////////////////////////////////
		for(int i=1;i<nLen;i++)
		{
			if(abs(dbDiffAngle[i])>=__PI/6)
			dbDiffAngle[i]=dbDiffAngle[i-1];
		}



		//// windoeing

		CreateFftWindow(WinBuff,nLen);
// 		fp = fopen("D:\\hamm.txt","wt");
// 		for (int i=0;i<nLen;i++){
// 			fprintf(fp,"%lg\n",WinBuff[i]);
// 		}
// 		fclose(fp);

		for (int i=0;i<nLen;i++)
		{
			cOutputBuff[i]=dbDiffAngle[i]*WinBuff[i];
		}

		SAFERELEASE_ARRAY(dbAngle);
		SAFERELEASE_ARRAY(dbDiffAngle);
		SAFERELEASE_ARRAY(WinBuff);

		return TRUE;
	}

	void CNewRsEstimation::CreateFftWindow(double * winBuff,int nLen)
	{
		hamming(nLen,winBuff); 
	}
	void CNewRsEstimation::FastSymbRateEst2(MKL_Complex16 * cmplxBuffIn, int * maxIndex1, double * maxval1, int * maxIndex2, double * maxval2, BOOL bFSKFlag=FALSE)
	{
		int nResCnt;
		int nCoef =0;

		memset(m_cmplx_Yout,0,sizeof(MKL_Complex16)*m_n_NFFT);

		m_cmplx_SumYout = new MKL_Complex16 [m_n_NFFT];
		memset(m_cmplx_SumYout,0,sizeof(MKL_Complex16)*m_n_NFFT);


		double * dblAbsSumYout = new double [m_n_NFFT];

		for (int i=0;i<m_n_DelayLen;i++)
		{
			m_cmplx_AutoCorrOut = new MKL_Complex16 [m_n_InputBuffCount + 2*m_n_DelayArray[i]];
			FastCyclicAutoCorr2(cmplxBuffIn, m_n_InputBuffCount, m_n_DelayArray[i], m_cmplx_AutoCorrOut, &nResCnt);
			nCoef = min(i+1,2);  // 1 to n delay
			long res = DftiComputeForward(m_pDescFFT, m_cmplx_AutoCorrOut, m_cmplx_FFTOut);
			vzMul(m_n_NFFT,m_cmplx_FFTOut, m_cmplx_FFTOut,m_cmplx_Yout);
			vzDiv(m_n_NFFT,m_cmplx_Yout, m_cmplx_DeNumArray,m_cmplx_Yout);
			vzAdd(m_n_NFFT,m_cmplx_SumYout,m_cmplx_Yout,m_cmplx_SumYout);

			for (int k=0;k<m_n_NFFT;k++)
			{
				m_cmplx_SumYout[k].imag = (double) (m_cmplx_SumYout[k].imag / nCoef);
				m_cmplx_SumYout[k].real = (double) (m_cmplx_SumYout[k].real / nCoef);
			}

			vzAbs(m_n_NFFT,m_cmplx_SumYout,dblAbsSumYout);
			memcpy(m_dbl_YYout[i], dblAbsSumYout, sizeof(double)*m_n_NFFT);

			delete[] m_cmplx_AutoCorrOut;
			m_cmplx_AutoCorrOut = NULL;
		}

		double * dblYYoutMax = new double [m_n_NFFT];
		double * dblYMedianOut = new double[m_n_NFFT];
		double * dblYMovingAveOut = new double[m_n_NFFT];
		double * dblYYoutMaxRange = new double [m_n_StopOfRange - m_n_StartOfRange +1];
//		CHFAMRAnalog *wndAmrAnalog = new CHFAMRAnalog;
		for (int i=0;i<m_n_NFFT;i++)
		{
			double dblMax = m_dbl_YYout[0][i];
			for (int j=1;j<100;j++)
			{
				if (dblMax < m_dbl_YYout[j][i]) dblMax = m_dbl_YYout[j][i];
			}
			dblYYoutMax[i] = dblMax;
		}
		Median(41,m_n_NFFT,dblYYoutMax,dblYMedianOut);
		movingavg(dblYMedianOut,m_n_NFFT,200,dblYMovingAveOut);
		for (int i = 0;i<m_n_NFFT;i++)
		{
			dblYYoutMax[i] = (double)dblYYoutMax[i]/dblYMovingAveOut[i];
		}
		for (int i = 0;i<m_n_NFFT;i++)
		{
			dblYYoutMax[i] = (double)dblYYoutMax[i];
		}
		memcpy(dblYYoutMaxRange, dblYYoutMax + m_n_StartOfRange, (m_n_StopOfRange - m_n_StartOfRange + 1)*sizeof(double));
		int nMaxIndex = cblas_idamax((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);
		int nMinIndex = cblas_idamin((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);

		double dblMaxVal = dblYYoutMaxRange[nMaxIndex];
		double dblMinVal = dblYYoutMaxRange[nMinIndex];

		dblYYoutMaxRange[nMaxIndex] = dblMinVal;

		int nMaxIndex_1 = cblas_idamax((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);
		double dblMaxVal_1 = dblYYoutMaxRange[nMaxIndex_1];

		//if (bFSKFlag)
		//{
		//	dblYYoutMaxRange[nMaxIndex_1] = dblMinVal;
		//	int nMaxIndex_2 = cblas_idamax((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);
		//	nMaxIndex = (nMaxIndex+nMaxIndex_1+nMaxIndex_2)/3;
		//}

		*maxIndex1 = nMaxIndex + m_n_StartOfRange;
		*maxIndex2 = nMaxIndex_1 + m_n_StartOfRange;
		*maxval1 = dblMaxVal;
		*maxval2 = dblMaxVal_1;

		delete [] dblAbsSumYout;
		dblAbsSumYout = NULL;
		delete [] dblYYoutMax;
		dblYYoutMax = NULL;
		delete [] dblYYoutMaxRange;
		dblYYoutMaxRange = NULL;
		delete [] dblAbsSumYout;
		dblAbsSumYout = NULL;
		delete[] m_cmplx_SumYout;
		m_cmplx_SumYout = NULL;
		delete[] dblYMedianOut;
		dblYMedianOut = NULL;
		delete[] dblYMovingAveOut;
		dblYMovingAveOut = NULL;
	}

	void CNewRsEstimation::FastSymbRateEstF(double * dblBuffIn, int * maxIndex1, double * maxval1, int * maxIndex2, double * maxval2, BOOL bFSKFlag)
	{
		int nResCnt;
		int nCoefF =0;

		memset(m_cmplx_Yout,0,sizeof(MKL_Complex16)*m_n_NFFT);

		m_cmplx_SumYout = new MKL_Complex16 [m_n_NFFT];
		memset(m_cmplx_SumYout,0,sizeof(MKL_Complex16)*m_n_NFFT);


		double * dblAbsSumYout = new double [m_n_NFFT];

		for (int i=0;i<m_n_DelayLen;i++)
		{
			m_dbl_AutoCorrOut = new double [m_n_InputBuffCount + 2*m_n_DelayArray[i]];
			FastCyclicAutoCorrF(dblBuffIn, m_n_InputBuffCount, m_n_DelayArray[i], m_dbl_AutoCorrOut, &nResCnt);
			nCoefF = min(i+1,2);  // 1 to n delay
			long res = DftiComputeForward(m_pDescFFTR, m_dbl_AutoCorrOut, m_cmplx_FFTOut);
			vzMul(m_n_NFFT,m_cmplx_FFTOut, m_cmplx_FFTOut,m_cmplx_Yout);
			vzDiv(m_n_NFFT,m_cmplx_Yout, m_cmplx_DeNumArray,m_cmplx_Yout);
			vzAdd(m_n_NFFT,m_cmplx_SumYout,m_cmplx_Yout,m_cmplx_SumYout);

			for (int k=0;k<m_n_NFFT;k++)
			{
				m_cmplx_SumYout[k].imag = (double) (m_cmplx_SumYout[k].imag / nCoefF);
				m_cmplx_SumYout[k].real = (double) (m_cmplx_SumYout[k].real / nCoefF);
			}

			vzAbs(m_n_NFFT,m_cmplx_SumYout,dblAbsSumYout);
			memcpy(m_dbl_YYout[i], dblAbsSumYout, sizeof(double)*m_n_NFFT);

			delete[] m_dbl_AutoCorrOut;
			m_dbl_AutoCorrOut = NULL;
		}

		double * dblYYoutMax = new double [m_n_NFFT];
		double * dblYYoutMaxRange = new double [m_n_StopOfRange - m_n_StartOfRange +1];
//		CHFAMRAnalog *wndAmrAnalog = new CHFAMRAnalog;
		for (int i=0;i<m_n_NFFT;i++)
		{
			double dblMax = m_dbl_YYout[0][i];
			for (int j=1;j<100;j++)
			{
				if (dblMax < m_dbl_YYout[j][i]) dblMax = m_dbl_YYout[j][i];
			}
			dblYYoutMax[i] = dblMax;
		}
		for (int i = 0;i<m_n_NFFT;i++)
		{
			dblYYoutMax[i] = (double)dblYYoutMax[i];
		}
		for (int i = 0;i<m_n_NFFT;i++)
		{
			dblYYoutMax[i] = (double)dblYYoutMax[i];
		}
		memcpy(dblYYoutMaxRange, dblYYoutMax + m_n_StartOfRange, (m_n_StopOfRange - m_n_StartOfRange + 1)*sizeof(double));
		int nMaxIndex = cblas_idamax((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);
		int nMinIndex = cblas_idamin((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);

		double dblMaxVal = dblYYoutMaxRange[nMaxIndex];
		double dblMinVal = dblYYoutMaxRange[nMinIndex];

		dblYYoutMaxRange[nMaxIndex] = dblMinVal;

		int nMaxIndex_1 = cblas_idamax((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);
		double dblMaxVal_1 = dblYYoutMaxRange[nMaxIndex_1];

		//if (bFSKFlag)
		//{
		//	dblYYoutMaxRange[nMaxIndex_1] = dblMinVal;
		//	int nMaxIndex_2 = cblas_idamax((m_n_StopOfRange - m_n_StartOfRange + 1), dblYYoutMaxRange, 1);
		//	nMaxIndex = (nMaxIndex+nMaxIndex_1+nMaxIndex_2)/3;
		//}

		*maxIndex1 = nMaxIndex + m_n_StartOfRange;
		*maxIndex2 = nMaxIndex_1 + m_n_StartOfRange;
		*maxval1 = dblMaxVal;
		*maxval2 = dblMaxVal_1;

		delete [] dblAbsSumYout;
		dblAbsSumYout = NULL;
		delete [] dblYYoutMax;
		dblYYoutMax = NULL;
		delete [] dblYYoutMaxRange;
		dblYYoutMaxRange = NULL;
		delete [] dblAbsSumYout;
		dblAbsSumYout = NULL;
		delete[] m_cmplx_SumYout;
		m_cmplx_SumYout = NULL;
	}

		void CNewRsEstimation::FastCyclicAutoCorr2(MKL_Complex16 * cmplxBuffIn, int nBuffInCount, int nTau, MKL_Complex16 * cmplxBuffOut, int * nBuffOutCount)
		{
			/*[x ; zeros(2*tau,1)] ====>>>> cmplxA*/

			int nZeroCnt = 2*nTau;
			int nOutBuffCnt = nZeroCnt + nBuffInCount;

			MKL_Complex16 * cmplxA = new MKL_Complex16 [nOutBuffCnt];

			memcpy(cmplxA, cmplxBuffIn, sizeof(MKL_Complex16)*nBuffInCount);
			memset(cmplxA + nBuffInCount, 0, sizeof(MKL_Complex16)*nZeroCnt);

			//---------------------------------------------

			/*[zeros(2*tau,1) ; conj(x)] ====>>>> cmplxB*/

			MKL_Complex16 * cmplxB = new MKL_Complex16 [nOutBuffCnt];
			MKL_Complex16 * cmplxConjOut = new MKL_Complex16 [nBuffInCount];

			vzConj(nBuffInCount, cmplxBuffIn, cmplxConjOut);
			memset(cmplxB, 0, sizeof(MKL_Complex16)*nZeroCnt);
			memcpy(cmplxB + nZeroCnt, cmplxConjOut, sizeof(MKL_Complex16)*nBuffInCount);

			//---------------------------------------------

			MKL_Complex16 * cmplxC = new MKL_Complex16 [nOutBuffCnt]; 
			double * dblAbsC = new double [nOutBuffCnt]; 
			MKL_Complex16 * cmplx_AbsC = new MKL_Complex16 [nOutBuffCnt]; 

			vzMul(nOutBuffCnt, cmplxA, cmplxB, cmplxC);

			MKL_Complex16 * cmplxR = new MKL_Complex16 [nOutBuffCnt]; 
			memset(cmplxR, 0, sizeof(MKL_Complex16)*nOutBuffCnt);

			vzAbs(nOutBuffCnt, cmplxC, dblAbsC);

			for (int i=0;i<nOutBuffCnt;i++)
			{
				cmplx_AbsC[i].real = 1 / (dblAbsC[i]+__EPS__);
				cmplx_AbsC[i].imag = 0;
			}

			int ndelta = 1;
			//cblas_zaxpy(nOutBuffCnt, dblAbsC, cmplxC, 1, cmplxR, 1);
			vzMul(nOutBuffCnt, cmplxC, cmplx_AbsC, cmplxR);

			memcpy(cmplxBuffOut, cmplxR, sizeof(MKL_Complex16)*nOutBuffCnt);
			*nBuffOutCount = nOutBuffCnt;

			delete [] cmplxA;
			cmplxA = NULL;
			delete [] cmplxB;
			cmplxB = NULL;
			delete [] cmplxC;
			cmplxC = NULL;
			delete [] cmplxR;
			cmplxR = NULL;
			delete [] cmplxConjOut;
			cmplxConjOut = NULL;
			delete [] cmplx_AbsC;
			cmplx_AbsC = NULL;
			delete [] dblAbsC;
			dblAbsC = NULL;
		}
		void CNewRsEstimation::FastCyclicAutoCorrF(double * dblBuffIn, int nBuffInCount, int nTau, double * dblBuffOut, int * nBuffOutCount)
		{
// 			/*[x ; zeros(2*tau,1)] ====>>>> cmplxA*/
// 
 			int nZeroCnt = 2*nTau;
 			int nOutBuffCnt = nZeroCnt + nBuffInCount;
 
 			double * dblA = new double [nOutBuffCnt];
 
 			memcpy(dblA, dblBuffIn, sizeof(double)*nBuffInCount);
 			memset(dblA + nBuffInCount, 0, sizeof(double)*nZeroCnt);
 
 			//---------------------------------------------
 
 			/*[zeros(2*tau,1) ; conj(x)] ====>>>> cmplxB*/
 
 			double * dblB = new double [nOutBuffCnt];
 
 			memset(dblB, 0, sizeof(double)*nZeroCnt);
 			memcpy(dblB + nZeroCnt, dblBuffIn, sizeof(double)*nBuffInCount);
 
 			//---------------------------------------------
 
 			double * dblC = new double [nOutBuffCnt]; 
 			double * dblAbsC = new double [nOutBuffCnt]; 
 			double * dbl_AbsC = new double [nOutBuffCnt]; 
 
 			vdMul(nOutBuffCnt, dblA, dblB, dblC);
 
 			double * dblR = new double [nOutBuffCnt]; 
 			memset(dblR, 0, sizeof(double)*nOutBuffCnt);
 
 			vdAbs(nOutBuffCnt, dblC, dblAbsC);
 
 			for (int i=0;i<nOutBuffCnt;i++)
 			{
 				dbl_AbsC[i] = 1 / (dblAbsC[i]+__EPS__);
			}
 
 			int ndelta = 1;
 			//cblas_zaxpy(nOutBuffCnt, dblAbsC, cmplxC, 1, cmplxR, 1);
 			vdMul(nOutBuffCnt, dblC, dbl_AbsC, dblR);
 
 			memcpy(dblBuffOut, dblR, sizeof(double)*nOutBuffCnt);
 			*nBuffOutCount = nOutBuffCnt;
 
 			delete [] dblA;
 			dblA = NULL;
 			delete [] dblB;
 			dblB = NULL;
 			delete [] dblC;
 			dblC = NULL;
 			delete [] dblR;
 			dblR = NULL;
 			delete [] dbl_AbsC;
 			dbl_AbsC = NULL;
 			delete [] dblAbsC;
 			dblAbsC = NULL;
		}

	
	double CNewRsEstimation::GetSymbolRate(BOOL bIsBFSKFlag)
	{
		// 		[ii_max1 ,ii_max2, mag_max1 , mag_max2] = FastSymbRateEst2 ((Sig), Fs, NFFT, Range, FSKFlag);
		// 		[ii_max1_O ,ii_max2_O, mag_max1_O , mag_max2_O] = FastSymbRateEst2 (sqrt(Sig), Fs, NFFT, Range, FSKFlag);  %==== add fastsymbolrate for sqrt
		// 
		// 			%%%% ===comparing and estimation
		// 			if (mag_max1>mag_max1_O) && (mag_max2>mag_max2_O)%%&& (mag_max1_O>2*mag_max2_O)
		// 				Rs_final =  ii_max1*Fs/NFFT
		// 			else
		// 			i_final = (ii_max1_O + ii_max2_O)/2;
		// 		Rs_final = i_final*Fs/NFFT
		// 			end
		// 			error = (Rs-Rs_final)/Rs

		int nMaxIndex0, nMaxIndex1, nMaxIndex2, nMaxIndex3;
		double dblVal0,dblVal1,dblVal2,dblVal3;
		double* m_dbl_Prepare_FSK=new double[16384];

// 		FILE *fp = fopen("D:\\RsEstimationI.txt","wt");
// 		for (int i=0;i<(m_n_NFFT);i++){
// 			fprintf(fp,"%f\n",m_cmplx_InputBuffer[i].real);
// 		}
// 		fclose(fp);
// 
// 		fp = fopen("D:\\RsEstimationQ.txt","wt");
// 		for (int i=0;i<(m_n_NFFT);i++){
// 			fprintf(fp,"%f\n",m_cmplx_InputBuffer[i].imag);
// 		}
// 		fclose(fp);

		if (bIsBFSKFlag)
		{
			PrepareRsSamplesForFSK(m_cmplx_InputBuffer,m_n_InputBuffCount,m_dbl_Prepare_FSK);
			FastSymbRateEstF(m_dbl_Prepare_FSK,&nMaxIndex0,&dblVal0,&nMaxIndex1,&dblVal1,m_bFSKFlag);
			dblVal2 = 0;
			dblVal3 = 0;
		}else
		{
			FastSymbRateEst2(m_cmplx_InputBuffer,&nMaxIndex0,&dblVal0,&nMaxIndex1,&dblVal1,m_bFSKFlag);
			MKL_Complex16 * m_cmplx_SqrtInpBuff = new MKL_Complex16 [m_n_InputBuffCount];
			vzSqrt(m_n_InputBuffCount, m_cmplx_InputBuffer, m_cmplx_SqrtInpBuff);
			FastSymbRateEst2(m_cmplx_SqrtInpBuff,&nMaxIndex2,&dblVal2,&nMaxIndex3,&dblVal3,m_bFSKFlag);
			delete [] m_cmplx_SqrtInpBuff;
		}

		double dblEstRs = 0;
		dblEstRs = nMaxIndex0*m_dbl_FsDown/m_n_NFFT;

		//if ((dblVal0>dblVal2) && (dblVal1>dblVal3)) 
		//{
		//	dblEstRs = nMaxIndex0*m_dbl_FsDown/m_n_NFFT;
		//	// 			bOQPSK = FALSE;
		//}
		//else 
		//{
		//	int nAveIndex = (nMaxIndex2 + nMaxIndex3)/2;
		//	dblEstRs = nAveIndex*m_dbl_FsDown/m_n_NFFT;
		//	// 			bOQPSK = TRUE;
		//}

		return dblEstRs;
	}

	void CNewRsEstimation::ResetEstimator()
	{

	}

	long CNewRsEstimation::CreateFFTDescriptor(int nFFT)
	{
		//////////////////////////////////////////////////////////////////////////
		if (m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
		long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_DOUBLE,DFTI_COMPLEX,1, nFFT);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR))
		{
			return -1;
		}
		//   Set placement of result DFTI_NOT_INPLACE
		Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR))
		{
			return -1;
		}
		//////////////////////////////////////////////////////////////////////////

		return 0;
	}

	void CNewRsEstimation::SetInputDataBuffer(MKL_Complex16 * databuff , int nsize)
	{
		m_cmplx_InputBuffer = new MKL_Complex16 [nsize];
		m_dbl_InputData_ImagePart = new double [nsize];
		m_dbl_InputData_RealPart = new double [nsize];

		CFIROLAvmld			*m_FirLowPassFilter;

		m_FirLowPassFilter = (new CFIROLAvmld);

		m_n_InputBuffCount = nsize;

		memcpy(m_cmplx_InputBuffer, databuff, sizeof(MKL_Complex16)*nsize);


		for (int i=0;i<nsize;i++)
		{
			m_dbl_InputData_RealPart[i] = m_cmplx_InputBuffer[i].real;
			m_dbl_InputData_ImagePart[i] = m_cmplx_InputBuffer[i].imag;
		}
		WriteToFile(_T("D:\\real1.txt"),m_dbl_InputData_RealPart,m_n_InputBuffCount);
		WriteToFile(_T("D:\\image1.txt"),m_dbl_InputData_ImagePart,m_n_InputBuffCount);

		double				dbl_WRad =		(2 * m_dbl_LpfBw / m_dbl_FsDown);
		//double				dbl_FStop =		min(m_dbl_LpfBw / 5 , __FSTOP__);
		double				dbl_FStop =		m_dbl_LpfBw / 5;
		double				dbl_FStopW =	(2 * dbl_FStop / m_dbl_FsDown);

		double dbl_LpfFreq[4];
		dbl_LpfFreq[0] =	0;
		dbl_LpfFreq[1] =	dbl_WRad;
		dbl_LpfFreq[2] =	dbl_WRad + dbl_FStopW;
		dbl_LpfFreq[3] =	1;

		double * dbl_INT_FirCoefBuff = new double [__INTERNAL_FIR_TAP_CNT__];

		Fir_Design(__INTERNAL_FIR_TAP_CNT__,dbl_LpfFreq,m_b_LpfM,4,dbl_INT_FirCoefBuff);
		//WriteToFile("c:\\inCoe.txt",dbl_INT_FirCoefBuff,128);

		double * dblBuffIn = new double [m_n_InputBuffCount + __EXTRABUFFSIZE__];
		double * dblBuffOut = new double [m_n_InputBuffCount + __EXTRABUFFSIZE__];

		int		nDataOutLen =0;

		memcpy(dblBuffIn,m_dbl_InputData_RealPart,m_n_InputBuffCount*sizeof(double));
		memset(dblBuffIn + m_n_InputBuffCount,0,sizeof(double)*__EXTRABUFFSIZE__);

		m_FirLowPassFilter->Create(__NFFT16__,__FIR_TAP_CNT__,dbl_INT_FirCoefBuff);
		m_FirLowPassFilter->Filter(dblBuffIn, m_n_InputBuffCount + __EXTRABUFFSIZE__ ,dblBuffOut,nDataOutLen);

		memcpy(m_dbl_InputData_RealPart, dblBuffOut, sizeof(double)*m_n_InputBuffCount);

		memcpy(dblBuffIn,m_dbl_InputData_ImagePart,m_n_InputBuffCount*sizeof(double));
		memset(dblBuffIn + m_n_InputBuffCount,0,sizeof(double)*__EXTRABUFFSIZE__);

		CFIROLAvmld			* m_FirLowPassFilter1;
		m_FirLowPassFilter1 = (new CFIROLAvmld);

		m_FirLowPassFilter1->Create(__NFFT16__,__FIR_TAP_CNT__,dbl_INT_FirCoefBuff);
		m_FirLowPassFilter1->Filter(dblBuffIn, m_n_InputBuffCount + __EXTRABUFFSIZE__ ,dblBuffOut,nDataOutLen);
		memcpy(m_dbl_InputData_ImagePart, dblBuffOut, sizeof(double)*m_n_InputBuffCount);

		for (int i=0;i<m_n_InputBuffCount;i++)
		{
			m_cmplx_InputBuffer[i].real = m_dbl_InputData_RealPart[i];
			m_cmplx_InputBuffer[i].imag = m_dbl_InputData_ImagePart[i];
		}

		WriteToFile(_T("D:\\real2.txt"),m_dbl_InputData_RealPart,m_n_InputBuffCount);
		WriteToFile(_T("D:\\image2.txt"),m_dbl_InputData_ImagePart,m_n_InputBuffCount);

		delete [] dblBuffIn;
		dblBuffIn = NULL;
		delete [] dblBuffOut;
		dblBuffOut = NULL;
		SAFERELEASE(m_FirLowPassFilter);
		SAFERELEASE(m_FirLowPassFilter1);
		//SAFERELEASE_ARRAY(m_FirLowPassFilter);
 		//delete [] dbl_INT_FirCoefBuff;
 		//dbl_INT_FirCoefBuff = NULL;

	}

	void CNewRsEstimation::Release()
	{
		SAFERELEASE_ARRAY(m_cmplx_InputBuffer);
		SAFERELEASE_ARRAY(m_cmplx_Yout);
		SAFERELEASE_ARRAY(m_cmplx_SumYout);
		SAFERELEASE_ARRAY(m_cmplx_DeNumArray);
		//SAFERELEASE_ARRAY(m_dbl_AutoCorrOut);
		SAFERELEASE_ARRAY(m_cmplx_FFTOut);
		SAFERELEASE_ARRAY(m_dbl_InputData_RealPart);
		SAFERELEASE_ARRAY(m_dbl_InputData_ImagePart);
		SAFERELEASE_ARRAY(m_n_DelayArray);
		SAFERELEASE_ARRAY(*m_dbl_YYout);
		for (int i=0;i<100;i++)
			SAFERELEASE_ARRAY(m_dbl_YYout[i]);

		if (m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
		m_pDescFFT = NULL;
	}



}

