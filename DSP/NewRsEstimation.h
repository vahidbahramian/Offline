#pragma once
#include "DspBase.h"
#include "dsplib_fir_ola.h"
#include "dsplib_general_func.h"
//#include "Cdsplib_amr_analog.h"

// all input parameters based on MHZ

namespace NS_DSP
{
	class CNewRsEstimation : public CDspBase
	{
	public:
		CNewRsEstimation(void);
		~CNewRsEstimation(void);


	private:

		MKL_Complex16*		m_cmplx_InputBuffer;
		MKL_Complex16*		m_cmplx_Yout;
		MKL_Complex16*		m_cmplx_SumYout;
		MKL_Complex16*		m_cmplx_DeNumArray;
		// 		MKL_Complex16*      m_testbuff;
		double*				m_dbl_Yout;
		double*				m_dbl_AutoCorrOut;
		/*double*				m_dbl_SumYout;*/
		



		MKL_Complex16*		m_cmplx_AutoCorrOut;
		MKL_Complex16*		m_cmplx_FFTOut;

		double				m_dbl_Fs;
		double				m_dbl_FsDown;
		double				m_dbl_PrimeRs;
		double				m_dbl_PrimBW;
		//double				m_dbl_PrimFc;
		double				m_dbl_ERROR0;
		//double				m_dbl_WRange;
		double				m_dbl_LpfBw;
		//double				m_dbl_WRad;
		//double				m_dbl_FStop;
		//double				m_dbl_FStopW;
		//double				m_dbl_LpfFreq[4];
		BOOL				m_b_LpfM[4];
		double**			m_dbl_YYout;

		double*				m_dbl_InputData_RealPart;
		double*				m_dbl_InputData_ImagePart;
		double*				m_dbl_Prepare_FSK;

		double				m_dbl_BwCoef;
		int					m_n_Sample;
		int					m_n_NFFT;
		double				m_dbl_Rolloff;

		int					m_n_StartOfRange;
		int					m_n_StopOfRange;

		int*				m_n_RangeArray;
		int*				m_n_DelayArray;

		int					m_n_DelayLen;
		int					m_n_InputBuffCount;

		BOOL				m_bFSKFlag;


		DFTI_DESCRIPTOR_HANDLE	m_pDescFFT;
		DFTI_DESCRIPTOR_HANDLE  m_pDescFFTR;

		// 		CFIROLAvmld			m_FirLowPassFilter;



		void InitParams();
		void FastSymbRateEst2(MKL_Complex16 * cmplxBuffIn, int * maxIndex1, double * maxval1, int * maxindex2, double * maxval2, BOOL bFSKFlag);
		void FastSymbRateEstF(double * dblBuffIn, int * maxIndex1, double * maxval1, int * maxindex2, double * maxval2, BOOL bFSKFlag);
		void FastCyclicAutoCorr2(MKL_Complex16 * cmplxBuffIn, int nBuffInCount, int nTau, MKL_Complex16 * cmplxBuffOut, int * nBuffOutCount);
		void FastCyclicAutoCorrF(double * dblBuffIn, int nBuffInCount, int nTau, double * dblBuffOut, int * nBuffOutCount);
		long CreateFFTDescriptor(int nFFT);

		BOOL PrepareRsSamplesForFSK(MKL_Complex16 *cInputBuff,int nLen, double *cOutputBuff);
		void CreateFftWindow(double * winBuff,int nLen);

	public:
		//int					m_n_DownRate;
		DWORD*				m_dw_FirCoefBuff;
		double*				m_dbl_FirCoefBuff;

		inline long round(double x)	{ return long(floor(x+0.5));}
		void CreateEstimator(double dblFs, double dblRs, double dblFc, double dblBW = -1, double dblRolloff = 0.25);
		double GetSymbolRate(BOOL bIsBFSKFlag);
		void ResetEstimator();
		void SetInputDataBuffer(MKL_Complex16 * databuff , int nsize);
		void Release();
	};
}
