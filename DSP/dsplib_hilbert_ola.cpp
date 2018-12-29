#include "StdAfx.h"
#include "dsplib_hilbert_ola.h"

namespace NS_DSP
{
////////////////////////////////////////////////////////////////////////////////////////////
//Double Precision
////////////////////////////////////////////////////////////////////////////////////////////
CHilbertTransformd::CHilbertTransformd(void)
{
	m_cH = NULL;
	m_fInputBlock = NULL;
	m_cDataFFT = NULL;
	m_cOutFFT = NULL;
	m_cOutIFFT = NULL;
	m_N = 0;
	m_nFFT=0;
	m_nBlockIndex = 0;
	m_cLastOutOverlap = NULL;
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
}


CHilbertTransformd::~CHilbertTransformd(void)
{
	Release();
}

void CHilbertTransformd::Release()
{
	if(m_cH) {delete[] m_cH; m_cH=NULL;}
	if(m_fInputBlock) {delete[] m_fInputBlock; m_fInputBlock=NULL;}
	if(m_cDataFFT) {delete[] m_cDataFFT; m_cDataFFT = NULL;}
	if(m_cOutFFT) {delete[] m_cOutFFT; m_cOutFFT=NULL;}
	if(m_cOutIFFT) {delete[] m_cOutIFFT; m_cOutIFFT=NULL;}
	if(m_cLastOutOverlap) {delete[] m_cLastOutOverlap;m_cLastOutOverlap=NULL;}
	if(m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
	if(m_pDescIFFT) DftiFreeDescriptor(&m_pDescIFFT);
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
}

void CHilbertTransformd::Reset()
{
	m_nBlockIndex = 0;
	if(m_fInputBlock)
		memset(m_fInputBlock,0,m_nFFT*sizeof(double));//zero pad input block
	if(m_cLastOutOverlap)
		memset(m_cLastOutOverlap,0,(m_nFFT-m_N)*sizeof(MKL_Complex16));//zero pad input block

}

BOOL CHilbertTransformd::Create(int nSizeFFT)
{
	int i;
	m_cH = new MKL_Complex16[nSizeFFT];
	if(!m_cH) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	memset(m_cH,0,nSizeFFT*sizeof(MKL_Complex16));//zero pad
	m_nFFT = nSizeFFT;
	m_N = nSizeFFT - 0;//HILBERT_FFT_BOUND_GAP;
	//	H = ones(1,N)';       (zero-padded FFT) no filter
	//		H(2:N/2)=H(2:N/2)*2;
	//	H(N/2+2:N)=0;
	//	H(1)=1;H(1+N/2)=1;
	m_cH[0].real = m_cH[m_N/2].real=1;
	for(i=1;i<m_N/2;i++) m_cH[i].real=2;

	m_fInputBlock = new double[nSizeFFT];
	if(!m_fInputBlock) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cDataFFT = new MKL_Complex16[nSizeFFT];
	if(!m_cDataFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cOutFFT = new MKL_Complex16[nSizeFFT];
	if(!m_cOutFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cOutIFFT = new MKL_Complex16[nSizeFFT];
	if(!m_cOutIFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cLastOutOverlap = new MKL_Complex16[nSizeFFT-m_N];
	if(!m_cLastOutOverlap) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	memset(m_cOutFFT,0,nSizeFFT*sizeof(MKL_Complex16));//zero pad input block
	memset(m_fInputBlock,0,nSizeFFT*sizeof(double));//zero pad input block
	memset(m_cLastOutOverlap,0,(m_nFFT-m_N)*sizeof(MKL_Complex16));//zero pad input block

	//-------init forward dft
	long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_DOUBLE,DFTI_REAL, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//dfti_example_status_print(Status);
		//printf(" DftiSetValue TEST FAIL\n");  
		return DSP_RET_FFT_INIT_ERROR;
	}
	float Scale = 1.0/(float)nSizeFFT;
	//printf("\n DFTI_BACKWARD_SCALE  = 1/n \n");

	Status = DftiSetValue(m_pDescFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}


	/*
	/   Commit Dfti descriptor
	*/
	Status = DftiCommitDescriptor( m_pDescFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}


	//-------init backward dft
	Status = DftiCreateDescriptor( &m_pDescIFFT, DFTI_DOUBLE,DFTI_COMPLEX, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescIFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiSetValue TEST FAIL\n");  
		return DSP_RET_FFT_INIT_ERROR;
	}
	Scale = 1.0/(float)nSizeFFT;

	Status = DftiSetValue(m_pDescIFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}


	/*
	/   Commit Dfti descriptor
	*/
	Status = DftiCommitDescriptor( m_pDescIFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set Scale number for Backward transform
	*/



	//test
	//	for(int i=0;i<nSizeFFT;i++){
	//		m_cDataFFT[i].real = 0;
	//		m_cDataFFT[i].imag = 0;
	//	}
	///	m_cDataFFT[10].real = 100;m_cDataFFT[100].imag = 0;
	//	m_cDataFFT[nSizeFFT-10].real =100;m_cDataFFT[nSizeFFT-100].imag = 0;


	//	Status = DftiComputeBackward( m_pDescFFT, m_cDataFFT, m_cOutIFFT);
	///	FILE *ofp = fopen("c:\\samples\\ifft.txt","wt");
	//	for(int i=0;i<nSizeFFT;i++) fprintf(ofp,"%f\n",m_cOutIFFT[i].real);
	//	for(int i=0;i<nSizeFFT;i++) fprintf(ofp,"%f\n",m_cOutIFFT[i].imag);
	//	fclose(ofp);
	//test

	Reset();
	return DSP_RET_SUCCESS;

}

BOOL CHilbertTransformd::Filter(double *m_fInput, int nInputSize, MKL_Complex16 *m_cOutput,int &nOutSize)
{
	int rez;
	int nInputIndex=0;
	nOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex>= m_N)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_N - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(float));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vzMul(m_nFFT,m_cDataFFT,m_cH,m_cOutFFT);
				Status = DftiComputeBackward( m_pDescIFFT, m_cOutFFT, m_cOutIFFT);
				memcpy(m_cOutput+nOutSize,m_cOutIFFT,m_N*sizeof(MKL_Complex16));
				//vcAdd(m_nFFT-m_N,m_cOutput+nOutSize,m_cLastOutOverlap,m_cOutput+nOutSize);
				//memcpy(m_cLastOutOverlap,m_cOutIFFT+m_N,(m_nFFT-m_N)*sizeof(MKL_Complex8));
				nOutSize += m_N;

			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_N;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(double));
		m_nBlockIndex += nInputSize;
	}
	return DSP_RET_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////////////
//Single Precision
////////////////////////////////////////////////////////////////////////////////////////////
CHilbertTransforms::CHilbertTransforms(void)
{
	m_cH = NULL;
	m_fInputBlock = NULL;
	m_cDataFFT = NULL;
	m_cOutFFT = NULL;
	m_cOutIFFT = NULL;
	m_N = 0;
	m_nFFT=0;
	m_nBlockIndex = 0;
	m_cLastOutOverlap = NULL;
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
}


CHilbertTransforms::~CHilbertTransforms(void)
{
	Release();
}

void CHilbertTransforms::Release()
{
	if(m_cH) {delete[] m_cH; m_cH=NULL;}
	if(m_fInputBlock) {delete[] m_fInputBlock; m_fInputBlock=NULL;}
	if(m_cDataFFT) {delete[] m_cDataFFT; m_cDataFFT = NULL;}
	if(m_cOutFFT) {delete[] m_cOutFFT; m_cOutFFT=NULL;}
	if(m_cOutIFFT) {delete[] m_cOutIFFT; m_cOutIFFT=NULL;}
	if(m_cLastOutOverlap) {delete[] m_cLastOutOverlap;m_cLastOutOverlap=NULL;}
	if(m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
	if(m_pDescIFFT) DftiFreeDescriptor(&m_pDescIFFT);
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
}

void CHilbertTransforms::Reset()
{
	m_nBlockIndex = 0;
	if(m_fInputBlock)
		memset(m_fInputBlock,0,m_nFFT*sizeof(float));//zero pad input block
	if(m_cLastOutOverlap)
		memset(m_cLastOutOverlap,0,(m_nFFT-m_N)*sizeof(MKL_Complex8));//zero pad input block

}

BOOL CHilbertTransforms::Create(int nSizeFFT)
{
	int i;
	m_cH = new MKL_Complex8[nSizeFFT];
	if(!m_cH) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	memset(m_cH,0,nSizeFFT*sizeof(MKL_Complex8));//zero pad
	m_nFFT = nSizeFFT;
	m_N = nSizeFFT - 0;//HILBERT_FFT_BOUND_GAP;
	//	H = ones(1,N)';       (zero-padded FFT) no filter
	//		H(2:N/2)=H(2:N/2)*2;
	//	H(N/2+2:N)=0;
	//	H(1)=1;H(1+N/2)=1;
	m_cH[0].real = m_cH[m_N/2].real=1;
	for(i=1;i<m_N/2;i++) m_cH[i].real=2;

	m_fInputBlock = new float[nSizeFFT];
	if(!m_fInputBlock) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cDataFFT = new MKL_Complex8[nSizeFFT];
	if(!m_cDataFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cOutFFT = new MKL_Complex8[nSizeFFT];
	if(!m_cOutFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cOutIFFT = new MKL_Complex8[nSizeFFT];
	if(!m_cOutIFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cLastOutOverlap = new MKL_Complex8[nSizeFFT-m_N];
	if(!m_cLastOutOverlap) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	memset(m_cOutFFT,0,nSizeFFT*sizeof(MKL_Complex8));//zero pad input block
	memset(m_fInputBlock,0,nSizeFFT*sizeof(float));//zero pad input block
	memset(m_cLastOutOverlap,0,(m_nFFT-m_N)*sizeof(MKL_Complex8));//zero pad input block

	//-------init forward dft
	long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_SINGLE,DFTI_REAL, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//dfti_example_status_print(Status);
		//printf(" DftiSetValue TEST FAIL\n");  
		return DSP_RET_FFT_INIT_ERROR;
	}
	float Scale = 1.0/(float)nSizeFFT;
	//printf("\n DFTI_BACKWARD_SCALE  = 1/n \n");

	Status = DftiSetValue(m_pDescFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}


	/*
	/   Commit Dfti descriptor
	*/
	Status = DftiCommitDescriptor( m_pDescFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}


	//-------init backward dft
	Status = DftiCreateDescriptor( &m_pDescIFFT, DFTI_SINGLE,DFTI_COMPLEX, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescIFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiSetValue TEST FAIL\n");  
		return DSP_RET_FFT_INIT_ERROR;
	}
	Scale = 1.0/(float)nSizeFFT;

	Status = DftiSetValue(m_pDescIFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}


	/*
	/   Commit Dfti descriptor
	*/
	Status = DftiCommitDescriptor( m_pDescIFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set Scale number for Backward transform
	*/



	//test
	//	for(int i=0;i<nSizeFFT;i++){
	//		m_cDataFFT[i].real = 0;
	//		m_cDataFFT[i].imag = 0;
	//	}
	///	m_cDataFFT[10].real = 100;m_cDataFFT[100].imag = 0;
	//	m_cDataFFT[nSizeFFT-10].real =100;m_cDataFFT[nSizeFFT-100].imag = 0;


	//	Status = DftiComputeBackward( m_pDescFFT, m_cDataFFT, m_cOutIFFT);
	///	FILE *ofp = fopen("c:\\samples\\ifft.txt","wt");
	//	for(int i=0;i<nSizeFFT;i++) fprintf(ofp,"%f\n",m_cOutIFFT[i].real);
	//	for(int i=0;i<nSizeFFT;i++) fprintf(ofp,"%f\n",m_cOutIFFT[i].imag);
	//	fclose(ofp);
	//test

	Reset();
	return DSP_RET_SUCCESS;

}

BOOL CHilbertTransforms::Filter(float *m_fInput, int nInputSize, MKL_Complex8 *m_cOutput,int &nOutSize)
{
	int rez;
	int nInputIndex=0;
	nOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex>= m_N)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_N - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(float));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(float));
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(float));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vcMul(m_nFFT,m_cDataFFT,m_cH,m_cOutFFT);
				Status = DftiComputeBackward( m_pDescIFFT, m_cOutFFT, m_cOutIFFT);
				memcpy(m_cOutput+nOutSize,m_cOutIFFT,m_N*sizeof(MKL_Complex8));
				//vcAdd(m_nFFT-m_N,m_cOutput+nOutSize,m_cLastOutOverlap,m_cOutput+nOutSize);
				//memcpy(m_cLastOutOverlap,m_cOutIFFT+m_N,(m_nFFT-m_N)*sizeof(MKL_Complex8));
				nOutSize += m_N;

			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_N;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(float));
		m_nBlockIndex += nInputSize;
	}
	return DSP_RET_SUCCESS;
}

}//namespace