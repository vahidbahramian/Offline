#pragma once
#include "func_interv.h"
#include "mkl_vml.h"
#include "mkl_types.h"
#include "mkl_cblas.h"
#include "mkl_dfti.h"
#include <Windows.h>
#define  _USE_MATH_DEFINES
#include <math.h>

#include "dsplib_general_func.h"
#include "dsplib_hilbert_ola.h"
#include "dsplib_fir_ola.h"

//#define _DSPLIB_POWER_EST_SAVE_S
//#define _DSPLIB_POWER_EST_SAVE_FILE "c:\\samples\\s.txt"
//#define _DSPLIB_SAVE_AMR_INPUT
//#define _DSPLIB_SAVE_AMR_INPUT_FILE "c:\\samples\\amr_in.txt"

/*=======================
* Useful Const
*=======================
*/
#define BW_CORRECTION   1.0190


/*=======================
* Utility functions
*=======================
*/
#define myMax(x,y)     ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define myMod(x,y)     ( ( x ) - ( y ) * floor ( ( x ) / ( y ) ) )
#define erb(x)         ( 24.7 * ( 4.37e-3 * ( x ) + 1.0 ) )

#define AMR_ANALOG_ERROR 0
#define AMR_ANALOG_AM 1
#define AMR_ANALOG_FM 2
#define AMR_ANALOG_DSB 3
#define AMR_ANALOG_USB 4
#define AMR_ANALOG_LSB 5
#define AMR_ANALOG_UNKNOWN 6
#define AMR_DIGITAL 7
#define AMR_UNKNOWN 8

namespace NS_DSP
{
	//double precision
	///////////////////////////////////////////////////////////////////////////
	class CHFAnalogAMRvmld
	{
	public:
		double m_fSigmaDP;
		double m_fMeanGamma;
		double m_fR;
		double P,PU,PL;

		DFTI_DESCRIPTOR_HANDLE m_pFFTDesc;
		DFTI_DESCRIPTOR_HANDLE m_pFFTDesc2;//fft 2N points
		int m_nFftSize;
		MKL_Complex16 *m_fSamplesFFT ;
		double *m_fFFTAbs;
		double *m_fFFTAbsSum;
		double *m_fBM ,*m_fEnvelope, *m_fEnvelope2, *m_fInstFreq,  *m_fInstPhase;
		double *m_fNL_phase,*m_fEnvPow2;

		CHilbertTransformd *m_pHilbert;
		CFIROLAvmld *m_pFirBPF;
		MKL_Complex16 *m_cHilbertOut;

	public:
		CHFAnalogAMRvmld(void);
		BOOL Initialize(int nFftSize);
		void Release();
		BOOL DoAMR(double *m_fSamples,double fs,int m_nNumberSamples,int &modType);//without bpf
		BOOL DoAMR(double *m_fSamples,double fs,double fStart,double fStop,int m_nNumberSamples,int &modType);//with bpf
		size_t CarrierFreqIndex(double *m_fSamples,int m_nNumberSamples);
		void ComputeP(double frequency,double fs,double *m_fSamples,
			int m_nNumberSamples,double &P,double &PL,double &PU);
		BOOL GammatoneFiter(double *m_fSamples,double fs,int m_nNumberSamples,double fc
			,int m_iHRect, double *m_fBM, double *m_fEnvelope,
			double *m_fInstFreq, double	*m_fInstPhase);
		BOOL ComputeGamma(double *m_fSamples,int m_nNumberSamples,double &m_fMeanGamma);

	public:
		~CHFAnalogAMRvmld(void);
	};
}



BOOL IsValidAMRSignal(float *m_fSamples,int m_nSize,float m_fThreshold,int &result);
BOOL IsValidAMRSignal(double *m_fSamples,int m_nSize,double m_fThreshold,int &result);
