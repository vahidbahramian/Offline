#pragma once
#include "dsplib_fir_ola.h"

/*
	Hilbert Transform Using FFT
*/

#define _DSPLIB_HILBERT_OLA

namespace NS_DSP
{

////////////////////////////////////////////////////////////////////////
//Double Precision
class CHilbertTransformd 
{
public:
	CHilbertTransformd(void);
public:
	~CHilbertTransformd(void);
	void Reset();
	void Release();
	BOOL Create(int nSizeFFT);
	BOOL Filter(double *m_fInput, int nInputSize, MKL_Complex16 *m_cOutput,int &nOutSize);

	DFTI_DESCRIPTOR_HANDLE m_pDescFFT ;
	DFTI_DESCRIPTOR_HANDLE m_pDescIFFT ;

	MKL_Complex16 *m_cDataFFT;
	MKL_Complex16 *m_cOutFFT;
	MKL_Complex16 *m_cH;
	MKL_Complex16 *m_cOutIFFT;
	MKL_Complex16 *m_cLastOutOverlap;
	double *m_fInputBlock;

	int m_nFFT;//N 
	int m_N;//N-M+1 overlap length
	int m_nBlockIndex;
};

//Single Precision
class CHilbertTransforms 
{
public:
	CHilbertTransforms(void);
public:
	~CHilbertTransforms(void);
	void Reset();
	void Release();
	BOOL Create(int nSizeFFT);
	BOOL Filter(float *m_fInput, int nInputSize, MKL_Complex8 *m_cOutput,int &nOutSize);

	DFTI_DESCRIPTOR_HANDLE m_pDescFFT ;
	DFTI_DESCRIPTOR_HANDLE m_pDescIFFT ;

	MKL_Complex8 *m_cDataFFT;
	MKL_Complex8 *m_cOutFFT;
	MKL_Complex8 *m_cH;
	MKL_Complex8 *m_cOutIFFT;
	MKL_Complex8 *m_cLastOutOverlap;
	float *m_fInputBlock;

	int m_nFFT;//N 
	int m_N;//N-M+1 overlap length
	int m_nBlockIndex;
};
}