#pragma once
#include "func_interv.h"
#include "mkl_vml.h"
#include "mkl_types.h"
#include "mkl_cblas.h"
#include "mkl_dfti.h"
#include <Windows.h>
#define  _USE_MATH_DEFINES
#include <math.h>
#include "dsplib_fir_design.h"
#include "dsplib_general_func.h"

#define _DSPLIB_DELAY
/*
Delay Line : y(n) = x(n-L)    H(z)=z^-L
*/

namespace NS_DSP
{
	//double precision
	class CDelayLined 
	{
	public:
		CDelayLined();
		~CDelayLined();
		BOOL Create(int nDelay);
		void Reset();
		double Delay(double m_fInput);

		double *m_fBuffer;
		int m_nDelay;
		int m_nPos;
	};

	//single precision
	class CDelayLines 
	{
	public:
		CDelayLines();
		~CDelayLines();
		BOOL Create(int nDelay);
		void Reset();
		float Delay(float m_fInput);

		float *m_fBuffer;
		int m_nDelay;
		int m_nPos;
	};

}
