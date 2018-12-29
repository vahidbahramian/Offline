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
#include "dsplib_fir_ola.h"
#include "dsplib_debug.h"

#define AMR_DIGITAL_ERROR 10
#define AMR_DIGITAL_2FSK 11
#define AMR_DIGITAL_BPSK 12
#define AMR_DIGITAL_QPSK 13
#define AMR_DIGITAL_OQPSK 14
#define AMR_DIGITAL_DBPSK 15
#define AMR_DIGITAL_Pi2DBPSK 16
#define AMR_DIGITAL_DQPSK 17
#define AMR_DIGITAL_8PSK 18
#define AMR_DIGITAL_QAM16 19
#define AMR_DIGITAL_QAM32 20
#define AMR_DIGITAL_QAM64 21
#define AMR_DIGITAL_QAM256 22
#define AMR_DIGITAL_4FSK 23
#define AMR_DIGITAL_NOT_FSK 24
#define AMR_DIGITAL_2_OR_4_FSK 25
#define AMR_DIGITAL_4_ASK 26
#define AMR_DIGITAL_UNKNOWN 27
#define AMR_DIGITAL_8FSK 28


namespace NS_DSP
{
	class CDigitalAMRvmls
	{
	public:
		CDigitalAMRvmls();
		~CDigitalAMRvmls();
		
		void Reset();
		void Release();
		BOOL Create(BOOL bFSK,int nSize);
		BOOL DoAMR(int *m_dwSamplesI,int *m_dwSampleQ,int nSize,float m_fSNR,int &nModType);
		BOOL DoAMR(double *m_fSamplesI,double *m_fSampleQ,int nSize,float m_fSNR,int &nModType);
		BOOL DoAMR(MKL_Complex16 *m_fSamples,int nSize,float m_fSNR,int &nModType);

	public:
		MKL_Complex8 *Com_Env1;
		float *phasor_2;
		float *Phase_1 ;
		MKL_Complex8 *C20_2;
		MKL_Complex8 *C21_2;
		MKL_Complex8 *C_2;
		MKL_Complex8 *C40_2;
		MKL_Complex8 *C42_2;
		float *R;
		float *I;
		MKL_Complex8 *C20_1;
		MKL_Complex8 *C21_1;
		MKL_Complex8 *C40_1;
		MKL_Complex8 *C42_1;
	};
}
