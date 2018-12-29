#pragma once
/*
	SSB Demodulator Hopping
*/

#include "dsplib_fir_ola.h"
#include "dsplib_hilbert_ola.h"
#include "dsplib_debug.h"

#include <vector>

#define _DSPLIB_DEMOD_SSB_HOP

#define DSPLIB_USB_DEMOD 0
#define DSPLIB_LSB_DEMOD 1

//#define _DSPLIB_DEMOD_SSB_SAVE_FIR_I
//#define _DSPLIB_DEMOD_SSB_SAVE_FIR_I_FILE "c:\\samples\\ssb_fir_i.txt"
//#define _DSPLIB_DEMOD_SSB_SAVE_FFT_PEAK_IN
//#define _DSPLIB_DEMOD_SSB_SAVE_FFT_PEAK_IN_FILE "c:\\samples\\ssb_pd_in_fft.txt"
//#define _DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC
//#define _DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC_FILE "c:\\samples\\ssb_fc.txt"
//#define  _DSPLIB_DEMOD_SSB_SAVE_HILBERT
//#define _DSPLIB_DEMOD_SSB_SAVE_HILBERT_FILE "c:\\samples\\ssb_hilbert_out.txt"
//#define  _DSPLIB_DEMOD_SSB_SAVE_DEVIATION
//#define _DSPLIB_DEMOD_SSB_SAVE_DEVIATION_FILE "c:\\samples\\ssb_deviation_out.txt"
//#define _DSPLIB_DEMOD_SSB_SAVE_CEPTRUM_IN
//#define _DSPLIB_DEMOD_SSB_SAVE_CEPTRUM_IN_FILE "c:\\samples\\ssb_cep_in.txt"

#define _DSPLIB_DELTA_FREQ_L1 4000
#define _DSPLIB_DELTA_FREQ_L2 4096
#define _DSPLIB_HOP_MAX_SAMPLES (3*8192)
extern double __ssb_hop_demod_hilbert_coef[501];

extern double __ssb_hop_demod_hop_lpf_audio[115];
namespace NS_DSP
{

	struct __SSB_DOUBLE_VECTOR
	{
		__SSB_DOUBLE_VECTOR() {m_iHops=0;m_fSamples=0;m_nSize = 0;}
		~__SSB_DOUBLE_VECTOR(){
			if(m_fSamples) delete[] m_fSamples; 
			m_fSamples=NULL;
			if(m_iHops) delete[] m_iHops; 
			m_iHops=NULL;
		}
		double* m_fSamples;
		int *m_iHops;
		int m_nSize;
	};

	class CSSBHopvmld 
	{
	public:
		CSSBHopvmld();
		~CSSBHopvmld();
		void Reset();
		void Release();
		BOOL Create(int nSizeFFT,int modType,double fs,int iThreshold=10,BOOL bF0=TRUE);
		inline void SetModType(int modType) {m_nModType = modType;}
		inline int GetModType() {return m_nModType;}
		inline double GetFc() {return m_fFineTune;}
		inline void   SetFc(double x) {m_fFineTune=x;}

		//peak finder : find  max index in fft(m_fSample,8192)
		//m_fSamples = from vector[m_iVectorStart][m_iVectorStartPos] to vector[m_iVectorEnd][m_iVectorEndPos]
		BOOL PeakFinder(int m_iVectorStart,
			int m_iVectorStartPos,
			int m_iVectorEnd,
			int m_iVectorEndPos,
			double &m_fFout //out freq
			);
		void FreeUsedVectors();

		BOOL DDC(int m_iVectorStart,
			int m_iVectorStartPos,
			int m_iVectorEnd,
			int m_iVectorEndPos,
			double m_fFc,
			double **m_fFcCorrection1,
			double **m_fFcCorrection2,
			int &m_nOutSize
			);

		BOOL DemodDDC(int m_iVectorStart,
			int m_iVectorStartPos,
			int m_iVectorEnd,
			int m_iVectorEndPos,
			double m_fFc,
			double *m_fDemodOut,//must be created outside
			int &m_nOutSize
			);

		BOOL Delta_Freq_Estimator(double *m_fSamples,
			int m_nInputSize,double &m_fDeltaFreq);

		BOOL Demodulate(double *m_fInputData,int *m_bInputHopsPosition,
			int nInputSize,	std::vector<__SSB_DOUBLE_VECTOR *> *m_vOutput,
			int &nOutputSize);
		int GetNextHopPosition(int *m_iArray,int nSize,int nFirstIndex);

	public:
		DFTI_DESCRIPTOR_HANDLE m_pDescFFT ;
		DFTI_DESCRIPTOR_HANDLE m_pDescFFTHann ;//fft for hann window
		DFTI_DESCRIPTOR_HANDLE m_pDescIFFTHann ;//fft for hann window
		int m_nModType;//lsb or usb
		int m_nSizeFFT;
		double m_fFs;
		double *m_fPeakFinderBuffer;
		MKL_Complex16 *m_cPeakFinderOut;

		std::vector<__SSB_DOUBLE_VECTOR *> m_vSamplesVector;
		//hop start & end pointers 
		//hop interval is from vector[m_iHopVectorStart][m_iHopVectorStartPos] 
		//     to vector[m_iHopVectorEnd][m_iHopVectorEndPos]
		int m_iHopVectorStart;
		int m_iHopVectorStartPos;
		int m_iHopVectorEnd;
		int m_iHopVectorEndPos;
		int m_iHopNumberSamples;//number samples btwn 2 hops

		double m_fPeakFinderFc;//the carrier freq estimated by peakfinder
		CFIROLAvmld m_firLPF_I,m_firLPF_Q;
		CFIROLAvmld m_fir_Demod;
		CFIROLAvmld m_fir_hilbert;
		//CHilbertTransformd m_hilbert;
		double *m_fMultSin,*m_fMultCos; //x(n) * exp(-j*w0)

		double *m_fZeroMemory;//dump memory for fill out fir & hilbert samples
		int m_timeDDS;
		int m_iDistance;//distance btwn this and last hop
		int m_iHopSamplesCntr ;
		BOOL m_bF0;
		int m_iThreshold;
		double m_fFineTune;

		//Hann Window
		double m_fHannWindow[_DSPLIB_DELTA_FREQ_L1];
		double m_fMultHann[_DSPLIB_DELTA_FREQ_L2];
		MKL_Complex16 m_cMultHannFFT[_DSPLIB_DELTA_FREQ_L2/2];
		double m_fMultHannFFTAbs[_DSPLIB_DELTA_FREQ_L2/2];
		double m_fMultHannFFTSum[_DSPLIB_DELTA_FREQ_L2/2];
		double m_fMultHannLn[_DSPLIB_DELTA_FREQ_L2];
		MKL_Complex16 m_cMultHannLn[_DSPLIB_DELTA_FREQ_L2];
		MKL_Complex16 m_cIFFTHannLn[_DSPLIB_DELTA_FREQ_L2];
		double m_fCeptrum[_DSPLIB_DELTA_FREQ_L2];

		int m_timeDemodDDS;

#ifdef _DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC
		FILE *fp_DSPLIB_DEMOD_SSB_SAVE_PEAK_DETECTOR_FC ;
#endif
#ifdef _DSPLIB_DEMOD_SSB_SAVE_DEVIATION
		FILE *fp_DSPLIB_DEMOD_SSB_SAVE_DEVIATION ;
#endif
	};

}//namespace

