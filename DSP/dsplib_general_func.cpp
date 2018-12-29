#include "../stdafx.h"
#include "dsplib_general_func.h"
#include "mkl.h"

char __dsplib_err_str[DSPLIB_MAX_ERRORS][100];

#define FIX(x) int(x)

void dsplibInitialize()
{
	//create error strings
	strcpy_s(__dsplib_err_str[DSP_RET_SUCCESS],"OK");
	strcpy_s(__dsplib_err_str[DSP_RET_MEMORY_ALLOCATE_ERROR],"Memory Allocation Error");
	strcpy_s(__dsplib_err_str[DSP_RET_FFT_INIT_ERROR],"MKL FFT Initialization Error");
	strcpy_s(__dsplib_err_str[DSP_RET_UNKNOWN_ERROR],"Unknown Error");
	strcpy_s(__dsplib_err_str[DSP_RET_FIR_DESIGN_ERROR],"FIR Design Error");

}
void dsplibTerminate()
{
	int t = 0;
	//MKL_FreeBuffers();
}

void dsplibGetErrorString(int error,char *m_sErrorStr)
{
	if(error>=0 && error<DSPLIB_MAX_ERRORS)
		strcpy_s(m_sErrorStr,strlen(__dsplib_err_str[error]),__dsplib_err_str[error]);
	else
		strcpy_s(m_sErrorStr,1,"");
}

//computes sum(x)/N single precision
BOOL vmlsMean(float *m_fBuffer,int nSize,float &result)
{
	//test
//	result=0;int i;
//	for(i=0;i<nSize;i++) result += m_fBuffer[i];
//	result /= nSize;
//	return TRUE;
	//test
	result =  cblas_sasum(nSize,m_fBuffer,1)/nSize;
	return DSP_RET_SUCCESS;
}

//computes sum(x)/N double precision
BOOL vmldMean(double *m_fBuffer,int nSize,double &result)
{
	//test
//	result=0;int i;
//	for(i=0;i<nSize;i++) result += m_fBuffer[i];
//	result /= nSize;
//	return TRUE;
	//test

	result=0;
	result = cblas_dasum(nSize,m_fBuffer,1)/nSize;
	return DSP_RET_SUCCESS;
}

//computes var(x)  single precision
BOOL vmlsVar(float *m_fBuffer,int nSize,float &result)
{
	int ret;
	float meanX,meanX2;
	float *X2 = new float[nSize];
	if(!X2) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	//test
	//	for(int i=0;i<nSize;i++) X2[i] = m_fBuffer[i]*m_fBuffer[i];
	//	if(!vmldMean(X2,nSize,meanX2)) {delete[] X2; return FALSE;}
	//	if(!vmldMean(m_fBuffer,nSize,meanX)){delete[] X2; return FALSE;}
	//	result = meanX2 - (meanX*meanX);
	//	delete[] X2;
	//	return TRUE;
	//test

	vsPowx(nSize,m_fBuffer,2,X2);
	if((ret = vmlsMean(X2,nSize,meanX2)) != DSP_RET_SUCCESS)
	{
		delete[] X2; 
		return ret;
	}
	if((ret=vmlsMean(m_fBuffer,nSize,meanX))!=DSP_RET_SUCCESS){
		delete[] X2; return ret;
	}
	result = meanX2 - (meanX*meanX);
	delete[] X2;
	return DSP_RET_SUCCESS;
}

//computes var(x)  double precision
BOOL vmldVar(double *m_fBuffer,int nSize,double &result)
{
	int ret;
	double meanX,meanX2;
	double *X2 = new double[nSize];
	if(!X2) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	//test
	//	for(int i=0;i<nSize;i++) X2[i] = m_fBuffer[i]*m_fBuffer[i];
	//	if(!vmldMean(X2,nSize,meanX2)) {delete[] X2; return FALSE;}
	//	if(!vmldMean(m_fBuffer,nSize,meanX)){delete[] X2; return FALSE;}
	//	result = meanX2 - (meanX*meanX);
	//	delete[] X2;
	//	return TRUE;
	//test

	vdPowx(nSize,m_fBuffer,2,X2);
	if((ret=vmldMean(X2,nSize,meanX2))!=DSP_RET_SUCCESS) {
		delete[] X2; return ret;
	}
	if((ret=vmldMean(m_fBuffer,nSize,meanX))!=DSP_RET_SUCCESS)
	{
		delete[] X2; return ret;
	}
	result = meanX2 - (meanX*meanX);
	delete[] X2;
	return DSP_RET_SUCCESS;
}

BOOL Fir_Design(int nTapSize,double *dblBandsFreq,BOOL *bBandsMask,
				int nBandsLen,double *dblFirCoefficients)
{
	nTapSize++;
	int npt = 1024;
	int n = nTapSize;
	int i;
	double *wind = NULL;
	MKL_Complex16 *H = NULL;
	MKL_Complex16 *h = NULL;
	MKL_Complex16 *exponent = NULL;
	try
	{
		wind = new double[n];
		ASSERT(wind);
		if (!wind) throw 1;
		H = new MKL_Complex16[2*npt];
		if (!H) throw 1;
		h = new MKL_Complex16[2*npt+1];
		if (!h) throw 1;

		//		kaiser(n, wind,6);
		blackmanHarris(n, wind);
		//hamming(n, wind);

		//WriteToFile("c:\\k.txt",wind,n); //OK
		int lap = FIX(npt/25);
		int nbrk = nBandsLen;
		// interpolate breakpoints onto large grid
		memset (H,0,npt * sizeof (double));
		int nint=nbrk-1;  // length(ff and aa)
		npt = npt + 1;   // Length of [dc 1 2 ... Nyquist] frequencies.
		int j,nb = 1,ne;
		double inc;
		BOOL *aa=bBandsMask;
		double *ff = dblBandsFreq;

		H[0].real=aa[0]; H[0].imag=0;
		for (i=1;i<=nint;i++)
		{
			ne = FIX(ff[i]*npt);
			if (nb == ne)	
			{
				inc = 0;
			}else{
				for (j=nb;j<=ne;j++)
				{
					inc = double(j-nb)/double(ne-nb);
					H[j-1].real = inc*aa[i] + (1 - inc)*aa[i-1];
					H[j-1].imag=0;
				}
			}
			nb = ne + 1;
		}//for i
		//WriteToFile("c:\\H.txt",H,npt);
		//Fourier time-shift.
		double rad, dt = 0.5 * (n - 1);
		exponent = new MKL_Complex16[2*npt];
		if (!exponent) throw 1;

		for (i=0;i<=(npt-1);i++)
		{
			exponent[i].real = cos(-dt * M_PI * i / (npt-1));
			exponent[i].imag = sin(-dt * M_PI * i / (npt-1));
		}

		vzMul((npt-1)+1,H,exponent,H); //H = H * exp()
		//		WriteToFile("c:\\H.txt",H,2*(npt-1));

		for (i=1;i<(npt-1);i++)
		{
			H[(npt-1)+i].real = H[(npt-1)-i].real;
			H[(npt-1)+i].imag = -H[(npt-1)-i].imag;
		}
		//WriteToFile("c:\\H.txt",H,2*(npt-1));

		//ht = real(ifft(H));            % Symmetric real series.
		//b = ht(1:nn);         % Raw numerator.
		//b = b .* wind(:).';   % Apply window.

		//-------init backward dft
		DFTI_DESCRIPTOR_HANDLE m_pDescFFT;
		long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_DOUBLE,DFTI_COMPLEX,1, 2*(npt-1) );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)) {
			throw 2;
		}
		/*
		/   Set placement of result DFTI_NOT_INPLACE
		*/
		Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			throw 2;
		}
		double Scale = 1.0/(double)(2*(npt-1));
		//printf("\n DFTI_BACKWARD_SCALE  = 1/n \n");

		Status = DftiSetValue(m_pDescFFT, DFTI_BACKWARD_SCALE, Scale);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			throw 2;
		}
		/*
		/   Commit Dfti descriptor
		*/
		Status = DftiCommitDescriptor( m_pDescFFT );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			throw 2;
		}
		Status = DftiComputeBackward( m_pDescFFT, H, h);
		//WriteToFile("c:\\imp.txt",h,2*(npt-1));
		if(m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
		for (i=0;i<n;i++)
			dblFirCoefficients[i] = h[i].real * wind[i];
		//	WriteToFile("c:\\ht.txt",dblFirCoefficients,n);
		nTapSize--;
	}
	catch (CException* e)
	{
		SAFERELEASE_ARRAY(wind);
		SAFERELEASE_ARRAY(H);
		SAFERELEASE_ARRAY(h);
		SAFERELEASE_ARRAY(exponent);
		return FALSE;
	}

	SAFERELEASE_ARRAY(wind);
	SAFERELEASE_ARRAY(H);
	SAFERELEASE_ARRAY(h);
	SAFERELEASE_ARRAY(exponent);
	return TRUE;

}
void blackmanHarris(int n,double* w)
{
	int      i;

	double k1 = 2*M_PI/((double)(n-1)); /* 2*pi/(N-1) */
	double k2 = 2*k1; /* 4*pi/(N-1) */
	double k3 = 3*k1; /* 6*pi/(N-1) */

	double a0 = 0.35875;
	double a1 = 0.48829;
	double a2 = 0.14128;
	double a3 = 0.01168;

	/* Calculate window coefficients */
	for (i=0; i<n; i++)
		*w++ = a0 - a1*cos(k1*(double)i) + a2 *cos(k2*(double)i) - a3 *cos(k3*(double)i);
}