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

#define _DSPLIB_CIC_FILTER
#define _DSPLIB_CIC_FIXED_CONST 4096.0f //must be fixed point

/*
CIC Filter Implementation
Note : Lu = 2^nTrunc 
*/

namespace NS_DSP
{
	//double precision
	class CCICvmld
	{
	public:
		CCICvmld(void);
		~CCICvmld(void);

	public:
		void Reset();
		BOOL Create(int m_nDownSample,int m_nTrunc,int m_nSize);
		int Filter(double *input,double *output,int nSize);

		int nBulkSize;
		int nTrunc;
		int nDownSample;

		int m_intergral[2];
		int m_comb[2];
		int m_nDnsampleCntr;
	};

	//single precision
	class CCICvmls
	{
	public:
		CCICvmls(void);
		~CCICvmls(void);

	public:
		void Reset();
		BOOL Create(int m_nDownSample,int m_nTrunc,int m_nSize);
		int Filter(float *input,float *output,int nSize);
		int Filter(double *input,double *output,int nSize);
		void ConstrainRange(double *input,double *output,int nSize);

		int nBulkSize;
		int nTrunc;
		int nDownSample;

		int m_intergral[2];
		int m_comb[2];
		int m_nDnsampleCntr;
	};


}//namespace