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

#define _DSPLIB_SSBFIXED_BLK     8000
#define _DSPLIB_SSBFIXED_FFTSIZE 8192
#define _DSPLIB_SSBFIXED_NUM_FFT_FOR_OUTPUT 10
#define _DSPLIB_SSBFIXED_NUMBER_FFT 50

#define _DSPLIB_SSBFIXED_SEARCH_STATE 0
#define _DSPLIB_SSBFIXED_COARSE_ESTIMATE_STATE 1
#define _DSPLIB_SSBFIXED_FINE_ESTIMATE_STATE 2

extern double __ssb_hop_demod_hilbert_coef[501];

extern double __ssb_hop_demod_hop_lpf_audio[115];
namespace NS_DSP
{
	class CSSBFixedvmld
	{
	public:
		CSSBFixedvmld();
		~CSSBFixedvmld();
		void Reset();
		void Release();
		BOOL Create(int nSizeFFT,int modType,double fs,int iThreshold=2);
		inline void SetModType(int modType) {m_nModType = modType;}
		inline int GetModType() {return m_nModType;}

		BOOL FindCarrier(double *m_fSamples,int nInputSize, double &m_fCarrier);
		double FindPeak();
		BOOL Demodulate(double *m_fInputData,int nInputSize,double *m_fOutput,int &nOutputSize);
		BOOL DemodDDC(double *m_fInputData,int nInputSize,double *m_fOutput,int &nOutputSize);
		double GetFc() {return m_fFc;}
		void SetFc(double fc){m_fFc = fc;}
		int GetState() {return m_iState;}

	public:
		DFTI_DESCRIPTOR_HANDLE m_pDescFFTHann ;//fft for hann window
		int m_nModType;//lsb or usb
		int m_nSizeFFT;
		double m_fFs;
		//CHilbertTransformd m_hilbert;
		double m_fMultCos[_DSPLIB_SSBFIXED_FFTSIZE]; //x(n) * exp(-j*w0)
		double m_fMultSin[_DSPLIB_SSBFIXED_FFTSIZE]; //x(n) * exp(-j*w0)
		double m_fOutHilbert[_DSPLIB_SSBFIXED_FFTSIZE+2*8192]; 
		double m_fBaseBandLSB[_DSPLIB_SSBFIXED_FFTSIZE+2*8192];
		double m_fBaseBandUSB[_DSPLIB_SSBFIXED_FFTSIZE+2*8192];

		double *m_fZeroMemory;//dump memory for fill out fir & hilbert samples
		int m_timeDemodDDS;
		BOOL m_bF0;
		int m_iThreshold;
		int m_iState;

		double m_fInputBlock[_DSPLIB_SSBFIXED_FFTSIZE];
		double m_fHannWindow[_DSPLIB_SSBFIXED_BLK];
		double m_fMultHann[_DSPLIB_SSBFIXED_FFTSIZE];
		MKL_Complex16 m_cMultHannFFT[_DSPLIB_SSBFIXED_FFTSIZE/2];
		double m_fMultHannFFTAbs[_DSPLIB_SSBFIXED_FFTSIZE/2];
		double m_fMultHannFFTSum[_DSPLIB_SSBFIXED_FFTSIZE/2];

		CFIROLAvmld m_fir_hilbert;
		CFIROLAvmld m_fir_Demod;

		//----fixed----
		int m_iCarrierSearchCntr;
		int m_nBlockIndex;
		double m_fFc;

	};
}//namespace