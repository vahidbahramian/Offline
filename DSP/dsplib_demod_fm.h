#pragma once
/*
FM Demodulator Implementation MKL
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
#include "dsplib_cic.h"
#include "dsplib_general_func.h"

#define _DSPLIB_DEMOD_FM

namespace NS_DSP
{
//double precision-----------------------------
//---------------------------------------------
	/*
class CFMvmld
{
public:
	CFMvmld(void);
	~CFMvmld(void);

	void Reset();
	void Release();
	BOOL Create(int nSize);
	int Demodulate(double *m_inputX,double *m_inputY, double *output, int nSize);

	CFIROLAvmld fir43[2];
	double m_fFir43Coefs[43];
	CCICvmld cic;
	int nBulkSize;
	double *firX, *firY, *atanOut;
	double *atanX1, *atanY1 ;
	double *atanX2, *atanY2 ;
	double firX_old ,firY_old ;
};*/

//single precision-----------------------------
//---------------------------------------------
class CFMvmls
{
public:
	CFMvmls(void);
	~CFMvmls(void);

	void Reset();
	void Release();
	BOOL Create(int nSize);
	int Demodulate(float *m_inputX,float *m_inputY, float *output, int nSize);

	CFIROLAvmls fir43[2];
	float m_fFir43Coefs[43];
	CCICvmls cic;
	int nBulkSize;
	float *firX, *firY, *atanOut;
	float *atanX1, *atanY1 ;
	float *atanX2, *atanY2 ;
	float firX_old ,firY_old ;
};

}//namespace
