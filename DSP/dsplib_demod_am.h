#pragma once
/*
AM Demodulator Implementation MKL  :  y = abs(hilbert(x))
*/

#include "func_interv.h"
#include "mkl_vml.h"
#include "mkl_types.h"
#include "mkl_cblas.h"
#include "mkl_dfti.h"
#include <Windows.h>
#define  _USE_MATH_DEFINES
#include <math.h>
#include "dsplib_fir_ola.h"
#include "dsplib_hilbert_ola.h"
#include "dsplib_general_func.h"

#define _DSPLIB_DEMOD_AM

namespace NS_DSP
{
	//double precision-----------------------------
	//---------------------------------------------
	class CAMvmld
	{
	public:
		CAMvmld(void);
		~CAMvmld(void);
		BOOL Create(int nCoreSize);
		BOOL Demodulate(double *m_fInput,int nSizeInput,double *m_fOutput,int &nSizeOutput);
		void Reset();
		void Release();

	public:
		CHilbertTransformd m_hilbert;
		MKL_Complex16 *m_cHilbertOut;
		int m_nCoreSize;
	};

	//single precision-----------------------------
	//---------------------------------------------
	class CAMvmls
	{
	public:
		CAMvmls(void);
		~CAMvmls(void);
		BOOL Create(int nCoreSize);
		BOOL Demodulate(float *m_fInput,int nSizeInput,float *m_fOutput,int &nSizeOutput);
		void Reset();
		void Release();

	public:
		CHilbertTransforms m_hilbert;
		MKL_Complex8 *m_cHilbertOut;
		int m_nCoreSize;
	};
}//namespace