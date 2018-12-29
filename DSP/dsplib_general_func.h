#pragma once
#include "func_interv.h"
#include "mkl_vml.h"
#include "mkl_types.h"
#include "mkl_cblas.h"
#include "mkl_dfti.h"
#include <Windows.h>
#define  _USE_MATH_DEFINES
#include <math.h>

//return values
#define DSP_RET_SUCCESS					0
#define DSP_RET_MEMORY_ALLOCATE_ERROR	1
#define DSP_RET_UNKNOWN_ERROR			2
#define DSP_RET_FFT_INIT_ERROR			3
#define DSP_RET_FIR_DESIGN_ERROR		4
#define DSP_RET_ABORT					5


#define DSPLIB_MAX_ERRORS				20

#define HF_MAX_DEMOD_BUFFER_LENGTH  16384
#define	HF_MAX_MATCHFILTER_BUFFER	200

#define OFFLINE_MAX_BIT_BUFFER_LENGTH  16384

#define OFFLINE_MAX_BIT__DS_BUFFER_LENGTH  512

//GHazal:

#define REF_SYMBOL_APSK_32			32

//return constants for new AMR routine
#define  HF_AMR_DIGITAL_OVERLAP   126
#define  HF_AMR_ANALOG_OVERLAP	  150
#define  HF_AMR_ANALOG_FRAME_SIZE 4096
#define  HF_AMR_ANALOG_FRAME_MAX_COUNT 50
#define  HF_AMR_ANALOG_MEAN_HANN 0.4999
#define  HF_AMR_ANALOG_SMOOTING_COEFF 0.3
#define	 HF_AMR_ANALOG_SMOOTH_FINAL_NOISE 0.8
#define  BETA_min 4
#define  HF_AMR_ANALOG_SUB_MIN_LEN 2
#define  HF_AMR_ANALOG_MIN_DURATION 1.5
#define  HF_AMR_ANALOG_STD_phiNL_th_lo  0.5
#define  HF_AMR_ANALOG_STD_phiNL_th_hi  3.5
#define	 HF_AMR_ANALOG_Gamma_Max_th_lo  4
#define	 HF_AMR_ANALOG_Gamma_Max_th_hi  80
// #define  HF_AMR_ANALOG_Carrier_Power_th01 80
#define  HF_AMR_ANALOG_Carrier_Eng_th 0.75
#define  HF_AMR_ANALOG_f_av_med_len  21
#define  HF_AMR_ANALOG_Moving_AVE_7_Len	7
#define  HF_AMR_ANALOG_Moving_AVE_11_Len 11
#define  HF_AMR_ANALOG_Moving_AVE_500_Len 500



#define ABS(x)		   ((x)>0 ? (x):-(x))
#define myMax(x,y)     ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define myMin(x,y)	   ( ( x ) < ( y ) ? ( x ) : ( y ) )
#define SIGN(x)		   ((x)>0 ? (1):(-1))
#define LOG2(x)			(log((double)x)/log(2.0f))


#ifndef SAFERELEASE
	#define SAFERELEASE(x) \
		{\
		if (x) delete x;\
		x=NULL;\
		}
#endif

#ifndef SAFERELEASE_ARRAY
	#define SAFERELEASE_ARRAY(x) \
		{\
		if (x) delete[] x;\
		x=NULL;\
		}
#endif

BOOL vmlsMean(float *m_fBuffer,int nSize,float &result);//computes sum(x)/N single precision
BOOL vmldMean(double *m_fBuffer,int nSize,double &result);//computes sum(x)/N double precision
BOOL vmlsVar(float *m_fBuffer,int nSize,float &result);//computes var(x)  single precision
BOOL vmldVar(double *m_fBuffer,int nSize,double &result);//computes var(x)  double precision
BOOL Fir_Design(int nTapSize,double *dblBandsFreq,BOOL *bBandsMask,
				int nBandsLen,double *dblFirCoefficients);

void blackmanHarris(int n,double* w);


//complex functions
inline float vmlsABS(MKL_Complex8 c) {return (sqrt(c.real*c.real + c.imag*c.imag));}
inline double vmldABS(MKL_Complex16 c) {return (sqrt(c.real*c.real + c.imag*c.imag));}
inline MKL_Complex8 vmlsConj(MKL_Complex8 a)
{
	MKL_Complex8 r;r.real = a.real;r.imag = -a.imag;
	return r;
}
inline MKL_Complex8 vmlsMul(MKL_Complex8 a,MKL_Complex8 b)
{
	MKL_Complex8 c;
	c.real = a.real * b.real - a.imag*b.imag;
	c.imag = a.real * b.imag + a.imag*b.real;
	return c;
}


void dsplibGetErrorString(int error,char *m_sErrorStr);
void dsplibInitialize();
void dsplibTerminate();

struct FILTER_COEFS
{
	double		*m_fCoefficients;
	int		m_nSize;
	FILTER_COEFS() {m_nSize=0;m_fCoefficients=NULL;}
	~FILTER_COEFS(){Release();}
	inline BOOL IsEmpty() {return (m_nSize==0 || m_fCoefficients==NULL);}
	void Release(){m_nSize=0;SAFERELEASE_ARRAY(m_fCoefficients);}
	BOOL ReadCoefsFromFile(char *lpszPath)
	{
		Release();
		FILE *fp;
		fopen_s(&fp, lpszPath,"rt");
		if (!fp){
			m_nSize	=	0;
			return	FALSE;
		}
		fscanf_s(fp,"%d\n",&m_nSize);
		if (!m_nSize)
		{
			fclose(fp); return FALSE;
		}
		m_fCoefficients = new double[m_nSize];
		if (!m_fCoefficients ) return FALSE;
		//T tmp;
		double tmp;
		for (register int i=0;i<m_nSize;i++){
			fscanf_s(fp,"%lf\n",&tmp);
			m_fCoefficients[i] = tmp;
		}
		fclose(fp);
		return TRUE;
	}
};
namespace NS_DSP
{
	class IQSample
	{
	public:
		IQSample(){I=0;Q=0;}
		double	I;
		double	Q;
	};
}