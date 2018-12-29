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

#define AMR_ANALOG_NO_SIGNAL 0
#define AMR_ANALOG_AMFM_AM 1
#define AMR_ANALOG_AMFM_FM 2
#define AMR_ANALOG_AM 3
#define AMR_ANALOG_FM 4
#define AMR_ANALOG_DSB 5
#define AMR_ANALOG_SSB 6
#define AMR_ANALOG_TYPE_COUNT 7

#define HF_AMR_ANALOG_Fs 80e3

#define HF_AMR_ANALOG_BandStartIdx round(10e3*HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_Fs)
#define HF_AMR_ANALOG_BandStopIdx round(30e3*HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_Fs)
#define HF_AMR_ANALOG_TotalIdx (HF_AMR_ANALOG_BandStopIdx - HF_AMR_ANALOG_BandStartIdx + 1)

#define HF_AMR_ANALOG_subMinLen  5
#define HF_AMR_ANALOG_minDuration  2.2
#define HF_AMR_ANALOG_tsco01  0.3
#define HF_AMR_ANALOG_tsco_noise_est  0.8
#define HF_AMR_ANALOG_BETAmin  4
#define HF_AMR_ANALOG_osc 4
#define HF_AMR_ANALOG_Bias_CF 3

#define HF_AMR_ANALOG_epsilon0  1e-18

#define myMax(x,y)     ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define myMin(x,y)	   ( ( x ) < ( y ) ? ( x ) : ( y ) )

namespace NS_DSP
{
	//double precision
	///////////////////////////////////////////////////////////////////////////
	class CHFAMRAnalog
	{
	public:
		double m_fSigmaDP;
		double m_fMeanGamma;
		double m_fR;
		double P,PU,PL;

		DFTI_DESCRIPTOR_HANDLE m_pFFTDesc ;
		DFTI_DESCRIPTOR_HANDLE m_pIFFTDesc;
		DFTI_DESCRIPTOR_HANDLE m_pFFTDesc1;
// 		DFTI_DESCRIPTOR_HANDLE m_pFFTDesc2;//fft 2N points
		int m_nFftSize;

		static FILTER_COEFS	m_stFilterCoefBuffer;
		static FILTER_COEFS	m_stHilbertCoefBuffer;
		static FILTER_COEFS m_stWindowHannBuffer;
// 		double				*m_dPhi;
// 		double			    *m_fAbsFFT;
		double				*m_fCoefficients;

	public:
		CHFAMRAnalog(void);
		BOOL Initialize(int nFftSize);
		inline long round(double x)	{ return long(floor(x+0.5));}

		//=================================Add for new AMR=======================================
		void EnvelopeAndPhaseEst(double *fFIROutBuffer,double *fHilbertOutBuffer,double *m_fPhiOut,double *m_fEnv);
		void CorrectionPhase(double *input,double *output);
		void Cumsum(double *input,double *Output,int nSize);
		BOOL NoiseEstimation(int totalSteps,int SubMinSegs,int NumberSetps,double *input,double *noisePSD,double *a2FFTSmoothed,
			double *minimaA2SubSeg,double *noisePSDMed,double *a2FFT,double *minimaA2);
		void Median(int blksz,int filterSize,double *m_fInputBuff,double *m_fMedBuffer);
		void MovingAverage(int nMaxRange,int filterSize,double *input,double *output);
		double ComputeGammaMax(double *m_fSample);
		void DoAMR(double *m_fCounts,double fCarrierToNoise,double fSTDPhiNL,double fGammaMax,
			double fCarrierEng,double fCentralEng,double fSpectralSymmetry);
		int recognitionAMR(double *m_fCounts);
		void InitAMRParam(double *m_fInputBuffer,double *m_dblA2FFTSmoothed,double *m_dblNoisePSD,
			double *m_dblMinimaA2SubSeg,double *m_fMinimaA2);
		//=======================================================================================

		void Release();

		static BOOL LoadFiltersCoefficients();
		static void UnLoadFiltersCoefficients();
		BOOL	IsLoadedFIRCoef() ;
		
		//Add for new AMR
		double		*m_fSFrame;
		double		*m_fTemp;
		double		*m_fTempAssist;
		double		*m_fNoisePSDTot;

		MKL_Complex16 *m_cOutFFT;
		MKL_Complex16 *m_cOutEnv;
		MKL_Complex16 *m_cSamplesFFT ;
		MKL_Complex16 *m_cCoefFIR;
		double		*m_fSample;
		double		*m_fdPhi;
		//double		*m_fFreqNoiseFlag;
		double		*m_fMinimaA2;

		double		m_fFrameSize;
		double      m_fOverlap;
		int			m_nSubSegIdx;
		int			m_nNDFT;
		BOOL		bIsCalcMinima;



	public:
		~CHFAMRAnalog(void);
	};
}



// BOOL IsValidAMRSignal(float *m_fSamples,int m_nSize,float m_fThreshold,int &result);
// BOOL IsValidAMRSignal(double *m_fSamples,int m_nSize,double m_fThreshold,BOOL *bArrValidChunk,
// 					  int &nInvalidCntr,int &result);
