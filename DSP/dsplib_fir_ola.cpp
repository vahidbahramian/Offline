#include "../StdAfx.h"
#include "dsplib_fir_ola.h"
namespace NS_DSP
{

////////////////////////////////////////////////////////////////////////////
/////////////////////FIR OLA Double precision
////////////////////////////////////////////////////////////////////////////
CFIROLAvmld::CFIROLAvmld(void)
{
	m_nSubSegIdx = 1;
	bIsCalcMinima=TRUE;


	m_L=0;
	m_fCoefficients = NULL;
	m_cCoefsFFT = NULL;
	m_cDataFFT=NULL;
	m_cOutFFT = NULL;
	m_fOutIFFT = NULL;
	m_fInputBlock=NULL;
	m_fOurBufferMA=NULL;
// 	m_fOutAbs=NULL;
// 	m_fOutSqr=NULL;
	m_fLastOutOverlap=NULL;
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
/*	m_fFreqNoiseFlag = NULL;*/
// 	m_fSFrame = NULL;
// 	a2FFTSmoothed=NULL;
// 	minimaA2SubSeg=NULL;
	m_nDelayIndex=0;
	m_bResetDelayBuffer=TRUE;
	m_nLastOutSize=0;
	m_fDCFftBackward=0;
	m_bIsFilter = FALSE;
	m_fFreqNoiseFlag=NULL;
	Reset();
}

CFIROLAvmld::~CFIROLAvmld(void)
{
	Release();
}

void CFIROLAvmld::Release()
{
	///Moving Ave
	//m_objMA500.Release();
	//SAFERELEASE_ARRAY(m_fOutBufferMA);
	//////////////

	if(m_fCoefficients) {delete[] m_fCoefficients; m_fCoefficients=NULL;}
	if(m_cCoefsFFT) {delete[] m_cCoefsFFT; m_cCoefsFFT =NULL;}
	if(m_cDataFFT) {delete[] m_cDataFFT; m_cDataFFT = NULL;}
	if(m_cOutFFT) {delete[] m_cOutFFT; m_cOutFFT=NULL;}
	if(m_fOutIFFT) {delete[] m_fOutIFFT; m_fOutIFFT=NULL;}
	if(m_fInputBlock){delete[] m_fInputBlock;m_fInputBlock=NULL;}
	if (m_fOurBufferMA){delete[] m_fOurBufferMA;m_fOurBufferMA=NULL;}
	if(m_fFreqNoiseFlag){delete[] m_fFreqNoiseFlag;m_fFreqNoiseFlag=NULL;}
	if(m_fLastOutOverlap) {delete[] m_fLastOutOverlap; m_fLastOutOverlap=NULL;}
// 	if(m_fDCFftBackward) {delete[] m_fDCFftBackward; m_fDCFftBackward=NULL;}
	if(m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
	if(m_pDescIFFT) DftiFreeDescriptor(&m_pDescIFFT);
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
}

void CFIROLAvmld::Reset()
{
	////Moving Ave
	//m_objMA500.Reset();
	/////////////////
	m_nBlockIndex = 0;
	if(m_fInputBlock)
		memset(m_fInputBlock,0,m_nFFT*sizeof(double));//zero pad input block
	if(m_fLastOutOverlap)
		memset(m_fLastOutOverlap,0,m_nTapSize*sizeof(double));//reset output overlap
	if(m_fOurBufferMA)
		memset(m_fOurBufferMA,0,m_nFFT*sizeof(double));//zero pad input block

	m_nDelayIndex=0;
	m_bResetDelayBuffer=TRUE;
	m_bIsFilter= FALSE;
	m_nLastOutSize=0;
	m_nDelayIndex = ((m_nTapSize-1)/2);
}

BOOL CFIROLAvmld::Create(int nSizeFFT,int nTapSize , double *m_fCoeffs)
{
	m_fCoefficients = new double[nSizeFFT];
	if(!m_fCoefficients) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	memset(m_fCoefficients,0,nSizeFFT*sizeof(double));//zero pad
	m_nTapSize = nTapSize;
	m_nFFT = nSizeFFT;
	m_L = m_nFFT - m_nTapSize + 1;
	m_cCoefsFFT = new MKL_Complex16[nSizeFFT];
	if(!m_cCoefsFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cDataFFT = new MKL_Complex16[nSizeFFT];
	if(!m_cDataFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cOutFFT = new MKL_Complex16[nSizeFFT];
	if(!m_cOutFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fOutIFFT = new double[nSizeFFT];
	if(!m_fOutIFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fInputBlock = new double[nSizeFFT];
	if(!m_fInputBlock) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fOurBufferMA = new double[nSizeFFT];
	if (!m_fOurBufferMA){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}

	////Moving Ave///////////////////
	//BOOL bRet = m_objMA500.Create(nSizeFFT,HF_AMR_ANALOG_Moving_AVE_500_Len);
	//if (!bRet){
	//	Release();
	//	return DSP_RET_MEMORY_ALLOCATE_ERROR;
	//}
	///////////////////////////////////


// 	m_fDCFftBackward = new double[1];
// 	//FreqNoiseFlag 
// 	m_fFreqNoiseFlag = new double[HF_AMR_ANALOG_FRAME_SIZE/2];
// 	if (!m_fFreqNoiseFlag){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
// 	//Temp
 	m_fSFrame = new double[HF_AMR_ANALOG_FRAME_SIZE/2];
 	if (!m_fSFrame){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
// 
// 	a2FFTSmoothed=new double[HF_AMR_ANALOG_FRAME_SIZE];
// 	if(!a2FFTSmoothed){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
// 
// 	minimaA2SubSeg=new double[HF_AMR_ANALOG_FRAME_SIZE];
// 	if (!minimaA2SubSeg){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
// 	m_fOutAbs = new double[nSizeFFT];
// 	if (!m_fOutAbs){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
// 	m_fOutSqr = new double[nSizeFFT];
// 	if (!m_fOutSqr){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fLastOutOverlap = new double[nTapSize ];//M-1 overlap
	if(!m_fLastOutOverlap) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}

	memset(m_fInputBlock,0,nSizeFFT*sizeof(double));//zero pad input block
// 	memset(m_fOutAbs,0,nSizeFFT*sizeof(double));
	memset(m_cOutFFT,0,nSizeFFT*sizeof(MKL_Complex16));//zero pad input block

	memcpy(m_fCoefficients,m_fCoeffs,nTapSize * sizeof(double));

	//-------init forward dft
	long Status = DftiCreateDescriptor( &m_pDescFFT, DFTI_DOUBLE,DFTI_REAL, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCreateDescriptor TEST FAIL\n");
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//dfti_example_status_print(Status);
		//printf(" DftiSetValue TEST FAIL\n");  
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}
	double Scale = 1.0/(double)nSizeFFT;
	//printf("\n DFTI_BACKWARD_SCALE  = 1/n \n");

	Status = DftiSetValue(m_pDescFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" TEST FAIL\n");
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}


	/*
	/   Commit Dfti descriptor
	*/
	Status = DftiCommitDescriptor( m_pDescFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCommitDescriptor TEST FAIL\n");
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set Scale number for Backward transform
	*/


	//--------init backward dft
	Status = DftiCreateDescriptor( &m_pDescIFFT, DFTI_DOUBLE,DFTI_REAL, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCreateDescriptor TEST FAIL\n");
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescIFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//dfti_example_status_print(Status);
		//printf(" DftiSetValue TEST FAIL\n");  
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}

	Scale = 1.0/(double)nSizeFFT;

	Status = DftiSetValue(m_pDescIFFT, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" TEST FAIL\n"); 
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}

	/*
	/   Commit Dfti descriptor
	*/
	Status = DftiCommitDescriptor( m_pDescIFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCommitDescriptor TEST FAIL\n");
		Release();
		return DSP_RET_FFT_INIT_ERROR;
	}



	//compute H(w)
	Status = DftiComputeForward( m_pDescFFT, m_fCoefficients, m_cCoefsFFT);

	//test
//	for(int i=0;i<nSizeFFT;i++){
//		m_cDataFFT[i].real = 0;
//		m_cDataFFT[i].imag = 0;
//	}
//	m_cDataFFT[10].real = 100;m_cDataFFT[100].imag = 0;
//	m_cDataFFT[nSizeFFT-10].real = 230;m_cDataFFT[nSizeFFT-100].imag = 0;


	//	Status = DftiComputeBackward( m_pDescFFT, m_cDataFFT, m_fOutIFFT);
	//	FILE *ofp = fopen("c:\\samples\\ifft.txt","wt");
	//	for(int i=0;i<nSizeFFT;i++) fprintf(ofp,"%f\n",m_fOutIFFT[i]);
	//	fclose(ofp);
	//test

	Reset();
	m_nDelayIndex = ((m_nTapSize-1)/2);

	return DSP_RET_SUCCESS;

}

BOOL CFIROLAvmld::CreateBPF(int nSizeFFT,int nTapSize , double fStart,double fStop,double fs,
							int flags)
{
	/*	DFTI_DESCRIPTOR_HANDLE m_pDesc ;
	int i,k1 = floor(fStart*nTapSize/fs);
	int k2 = floor(fStop*nTapSize/fs);
	MKL_Complex16 *a1= new MKL_Complex16[nTapSize/2 -1];
	if(!a1) {Release();return FALSE;}
	MKL_Complex16 *freqz=new MKL_Complex16[nTapSize];
	if(!freqz) {delete[] a1; Release();return FALSE;}
	memset(a1,0,(nTapSize/2-1)*sizeof(MKL_Complex16));
	memset(freqz,0,nTapSize*sizeof(MKL_Complex16));
	freqz[0].real=1; freqz[0].imag=0;//dc component
	//	a1=[zeros(1,k1) ones(1,k2-k1) zeros(1,N/2-k2-1)];
	//	freqz=[1 a1 0 fliplr(a1)];
	//	ff=ifft(a);
	//	coefs=[real(ff(N/2+1:N)) real(ff(1:N/2))];
	for(i=k1;i<=k2;i++) {a1[i].real=1;a1[i].imag=0;}//f1 to f2
	for(i=0;i<nTapSize/2-1;i++) freqz[i+1] = a1[i];
	freqz[nTapSize/2].real=0;freqz[nTapSize/2].imag=0;
	for(i=0;i<nTapSize/2-1;i++) 
	freqz[nTapSize/2+i+1] = a1[nTapSize/2-2-i];
	MKL_Complex16 *coefs=new MKL_Complex16[nTapSize];
	if(!coefs) {delete[] a1;delete[] freqz; Release();return FALSE;}
	//coefs = fft.ifft(freqz,nTapSize);
	double *c = new double [nTapSize];
	if(!c) {delete[] a1; delete[] freqz;delete[] coefs; Release();return FALSE;}
	//compute H(w)
	//-------init forward dft
	long Status = DftiCreateDescriptor( &m_pDesc, DFTI_DOUBLE,DFTI_REAL, 1, nTapSize );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	Status = DftiSetValue( m_pDesc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	double Scale = 1.0/(double)nTapSize;

	Status = DftiSetValue(m_pDesc, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	Status = DftiCommitDescriptor( m_pDesc );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	Status = DftiComputeBackward( m_pDesc,  freqz,c);
	double tmp;
	for(i=0;i<nTapSize/2;i++){
	tmp = c[i];
	c[i] =c[nTapSize/2+i];
	c[nTapSize/2+i] = tmp;
	}
	//use Hamming Window
	double w;
	for(i=0;i<nTapSize;i++){
	w = 0.54 - 0.46 * cos(2*M_PI *i/nTapSize);
	c[i] *= w;
	}

	//
	FILE *fp = fopen("c:\\samples\\coef.txt","wt");
	for(i=0;i<nTapSize;i++) fprintf(fp,"%f\n",c[i]);
	fclose(fp);
	//
	DftiFreeDescriptor(&m_pDesc);

	BOOL res = Create(nSizeFFT,nTapSize,c);
	delete[] a1;
	delete[] freqz; 
	delete[] c;
	delete[] coefs;
	return res;*/

	double *coef = new double [nTapSize];
	if(!coef) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	double f[2];
	if(flags & LP)
	{
		f[0] = 2*fStop/fs;
	}else if(flags & HP)
	{
		f[0] = 2*fStart/fs;
	}else{
		f[0]=2 * fStart / fs;f[1]=2 * fStop / fs;
	}
	int ret = design_fir(nTapSize,coef,f,flags,0);
	if(ret == -1) {delete[] coef;Release();return DSP_RET_FIR_DESIGN_ERROR;}
	//
#ifdef _FIR_OLA_SAVE_COEFS
	FILE *fp = fopen(_FIR_OLA_COEFS_FILE,"wt");
	for(int i=0;i<nTapSize;i++) fprintf(fp,"%f\n",coef[i]);
	fclose(fp);
#endif
	//
	BOOL res = Create(nSizeFFT,nTapSize,coef);
	delete[] coef;
	return res;
}

BOOL CFIROLAvmld::Filter(double *m_fInput, int nInputSize, double *m_fOutput,int &nOutSize
							,BOOL bDivDcFft)
{
	int rez;
	int nInputIndex=0;
	nOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex >= m_L)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_L - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
				
				//////add Moving Average///
				//int m_nOutMASize=0;
				//m_objMA500.Reset();
				//m_objMA500.AddBuffer(m_fInputBlock,m_nFFT,&m_fOurBufferMA,m_nOutMASize);
				/////////////////////////////


				/////////////////////////
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(double));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vzMul(m_nFFT/2,m_cDataFFT,m_cCoefsFFT,m_cOutFFT);
				Status = DftiComputeBackward( m_pDescFFT, m_cOutFFT, m_fOutIFFT);
				memcpy(m_fOutput+nOutSize,m_fOutIFFT,m_L*sizeof(double));
				vdAdd((m_nTapSize-1),m_fOutput+nOutSize,m_fLastOutOverlap,m_fOutput+nOutSize);
				//=================================
				if (bDivDcFft)
				{
					m_fDCFftBackward = m_cOutFFT[0].real;
					m_fDCFftBackward = m_fDCFftBackward/m_L;
					if (m_fDCFftBackward<550 || m_fDCFftBackward>600)
					{
						int i=0;
					}
					for (int i=0;i<m_L;i++)
						m_fOutput[i]=m_fOutput[i]-m_fDCFftBackward;
				}
				//==================================
				memcpy(m_fLastOutOverlap,m_fOutIFFT+m_L,(m_nTapSize-1)*sizeof(double));
				nOutSize += m_L;
			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_L;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(double));
		m_nBlockIndex += nInputSize;
	}
	return DSP_RET_SUCCESS;
} 
//Init Primary Values For AMR
BOOL CFIROLAvmld::FilterForFirstFrame(double *m_fInput, int nInputSize
									  , double *m_fOutput,MKL_Complex16 *cOutFFT,int &nOutSize)
{
	int rez;
	int nInputIndex=0;
	nOutSize=0;
	long Status;
	if (m_bIsFilter == FALSE)
	{
		if(nInputSize + m_nBlockIndex >= 4096)
		{
			//rez = m_L - m_nBlockIndex;
			memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,4096*sizeof(double));
			//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(double));//zero pad input block
			//block = m_fInputBlock(0:L-1)
			Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
			vzMul(m_nFFT/2,m_cDataFFT,m_cCoefsFFT,cOutFFT);
			Status = DftiComputeBackward( m_pDescFFT, m_cOutFFT, m_fOutIFFT);
			memcpy(m_fOutput+nOutSize,m_fOutIFFT,4096*sizeof(double));
			nOutSize+=4096;
			m_bIsFilter = TRUE;

		}else{//if there is not any block 
			memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(double));
			m_nBlockIndex += nInputSize;
		}

	}
	return DSP_RET_SUCCESS;
}

BOOL CFIROLAvmld::FilterAndDelay(double *m_fInput, int nInputSize,
								 double *m_fOutput,double *m_fDelay,
								 int &nOutSize)
{
	int rez;
	int nInputIndex=0;
	if (m_bResetDelayBuffer)
	{
		ASSERT(m_nDelayIndex>=m_nLastOutSize);
		memcpy(m_fDelay,m_fDelay+m_nLastOutSize,(m_nDelayIndex-m_nLastOutSize+1)*sizeof(double));
		m_nDelayIndex -= m_nLastOutSize;
	}
	memcpy(m_fDelay+m_nDelayIndex,m_fInput,nInputSize * sizeof(double));
	m_nDelayIndex += nInputSize;
	m_bResetDelayBuffer=FALSE;
	nOutSize=0;
	m_nLastOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex >= m_L)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_L - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(double));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vzMul(m_nFFT/2,m_cDataFFT,m_cCoefsFFT,m_cOutFFT);
				Status = DftiComputeBackward( m_pDescFFT, m_cOutFFT, m_fOutIFFT);
				memcpy(m_fOutput+nOutSize,m_fOutIFFT,m_L*sizeof(double));
				vdAdd((m_nTapSize-1),m_fOutput+nOutSize,m_fLastOutOverlap,m_fOutput+nOutSize);
				memcpy(m_fLastOutOverlap,m_fOutIFFT+m_L,(m_nTapSize-1)*sizeof(double));
				nOutSize += m_L;
			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_L;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(double));
		m_nBlockIndex += nInputSize;
	}
	if(nOutSize>0) m_bResetDelayBuffer=TRUE;
	m_nLastOutSize=nOutSize;
	return DSP_RET_SUCCESS;
}

BOOL CFIROLAvmld::FilterAndDelay(double *m_fInput, int nInputSize,double *m_fOutput,
								 double *m_fDelay,int &nOutSize,double *m_fOutAbs,
								 double *m_fOutSqr)
{
	int rez;
	int nInputIndex=0;
	if (m_bResetDelayBuffer)
	{
		ASSERT(m_nDelayIndex>=m_nLastOutSize);
		memcpy(m_fDelay,m_fDelay+m_nLastOutSize,(m_nDelayIndex-m_nLastOutSize+1)*sizeof(double));
		m_nDelayIndex -= m_nLastOutSize;
	}
	memcpy(m_fDelay+m_nDelayIndex,m_fInput,nInputSize * sizeof(double));
	m_nDelayIndex += nInputSize;
	m_bResetDelayBuffer=FALSE;
	nOutSize=0;
	m_nLastOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex >= m_L)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_L - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(double));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vzMul(m_nFFT/2,m_cDataFFT,m_cCoefsFFT,m_cOutFFT);
				vzAbs(2048,m_cOutFFT,m_fOutAbs);
				vdSqr(2048,m_fOutAbs,m_fOutSqr);
				Status = DftiComputeBackward( m_pDescFFT, m_cOutFFT, m_fOutIFFT);
				memcpy(m_fOutput+nOutSize,m_fOutIFFT,m_L*sizeof(double));
				vdAdd(m_nTapSize-1,m_fOutput+nOutSize,m_fLastOutOverlap,m_fOutput+nOutSize);
				memcpy(m_fLastOutOverlap,m_fOutIFFT+m_L,(m_nTapSize-1)*sizeof(double));
				nOutSize += m_L;
			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_L;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(double));
		m_nBlockIndex += nInputSize;
	}
	if(nOutSize>0) m_bResetDelayBuffer=TRUE;
	m_nLastOutSize=nOutSize;
	return DSP_RET_SUCCESS;
}

BOOL CFIROLAvmld::FilterAndDelaySSBHopping(double *m_fInput, int nInputSize, 
										   double *m_fOutput,int *m_iHopSeq,
										   int *m_iHopSeqDelay,int &nOutSize
										   )
{
	int rez;
	int nInputIndex=0;
	if (m_bResetDelayBuffer)
	{
		ASSERT(m_nDelayIndex>=m_nLastOutSize);
		memmove(m_iHopSeqDelay,m_iHopSeqDelay+m_nLastOutSize,(m_nDelayIndex-m_nLastOutSize)*sizeof(int));
		m_nDelayIndex -= m_nLastOutSize;
	}
	memmove(m_iHopSeqDelay+m_nDelayIndex,m_iHopSeq,nInputSize * sizeof(int));
	m_nDelayIndex += nInputSize;
	m_bResetDelayBuffer=FALSE;
	nOutSize=0;
	m_nLastOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex >= m_L)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_L - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(double));
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(double));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vzMul(m_nFFT/2,m_cDataFFT,m_cCoefsFFT,m_cOutFFT);
				Status = DftiComputeBackward( m_pDescFFT, m_cOutFFT, m_fOutIFFT);
				memcpy(m_fOutput+nOutSize,m_fOutIFFT,m_L*sizeof(double));
				vdAdd(m_nTapSize-1,m_fOutput+nOutSize,m_fLastOutOverlap,m_fOutput+nOutSize);
				memcpy(m_fLastOutOverlap,m_fOutIFFT+m_L,(m_nTapSize-1)*sizeof(double));
				nOutSize += m_L;
			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_L;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(double));
		m_nBlockIndex += nInputSize;
	}
	if(nOutSize>0) m_bResetDelayBuffer=TRUE;
	m_nLastOutSize=nOutSize;
	return DSP_RET_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////
/////////////////////Add Noise Estimation after output card
// BOOL CFIROLAvmld::NoiseEstimation(int SubMinSegs,int NumberSetps,double *Coef,double *input,
// 				double *noisePSD,double *a2FFT)
// {
// 	int i;
// 	MKL_LONG Status;
// 	vdMul(HF_AMR_ANALOG_FRAME_SIZE,Coef,input,input);
// 	Status=DftiComputeForward(m_pDescFFT, input, m_cOutFFT);
// 	vzAbs(HF_AMR_ANALOG_FRAME_SIZE/2,m_cOutFFT,m_fSFrame);
// 	//for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 	//	m_fSFrame[i]=m_fSFrame[i]/HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_MEAN_HANN;
// 	cblas_dscal(HF_AMR_ANALOG_FRAME_SIZE/2,1/HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_MEAN_HANN,m_fSFrame,1);
// 	vdSqr(HF_AMR_ANALOG_FRAME_SIZE/2,m_fSFrame,a2FFT);
// 
// 	//cblas_dscal(HF_AMR_ANALOG_FRAME_SIZE/2,1-HF_AMR_ANALOG_SMOOTING_COEFF,m_fSFrame,1);
// 	for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 		a2FFTSmoothed[i+2048]=m_fSFrame[i]*(1-HF_AMR_ANALOG_SMOOTING_COEFF)+HF_AMR_ANALOG_SMOOTING_COEFF*a2FFTSmoothed[i];
// 
// 	if (bIsCalcMinima == TRUE)
// 	{
// 		for(i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 		{
// 			if (a2FFTSmoothed[i]<a2FFTSmoothed[i+2048])
// 				minimaA2SubSeg[i]=a2FFTSmoothed[i];
// 			else
// 				minimaA2SubSeg[i]=a2FFTSmoothed[i+2048];
// 		}
// 	}else
// 	{
// 		for(i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 		{
// 			if (a2FFTSmoothed[i]<a2FFTSmoothed[i+2048])
// 			{
// 				minimaA2SubSeg[i]=a2FFTSmoothed[i];
// 				minimaA2SubSeg[i+2048]=a2FFTSmoothed[i];
// 			}
// 			else
// 			{
// 				minimaA2SubSeg[i]=a2FFTSmoothed[i+2048];
// 				minimaA2SubSeg[i+2048]=a2FFTSmoothed[i+2048];
// 			}
// 		}
// 	}
// 
// 	for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 		if (minimaA2SubSeg[i]<minimaA2SubSeg[i+2048])
// 			minimaA2SubSeg[i+2048]=minimaA2SubSeg[i];
// 	for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 	{
// 		if ((a2FFTSmoothed[i+2048]/(BETA_min*minimaA2SubSeg[i+2048]+0.25*3*noisePSD[i]))<1)
// 			m_fFreqNoiseFlag[i]=0;
// 		else
// 			m_fFreqNoiseFlag[i]=1;
// 	}
// 	for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2;i++)
// 	{
// 		if (m_fFreqNoiseFlag[i])
// 			noisePSD[i]=(1-HF_AMR_ANALOG_SMOOTH_FINAL_NOISE)*m_fSFrame[i]+(HF_AMR_ANALOG_SMOOTH_FINAL_NOISE)*noisePSD[i];
// 		else
// 			noisePSD[i]=noisePSD[i];
// 	}
// 	if (!NumberSetps%2)
// 	{
// 		if (m_nSubSegIdx==SubMinSegs)
// 		{
// 			m_nSubSegIdx=1;
// 			memset(minimaA2SubSeg,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
// 			bIsCalcMinima=FALSE;
// 		}else{
// 			m_nSubSegIdx++;
// 			bIsCalcMinima=TRUE;
// 		}
// 	}
// 		return DSP_RET_SUCCESS;
// }


////////////////////////////////////////////////////////////////////////////
/////////////////////FIR OLA Single precision
////////////////////////////////////////////////////////////////////////////
CFIROLAvmls::CFIROLAvmls(void)
{
	m_L=0;
	m_fCoefficients = NULL;
	m_cCoefsFFT = NULL;
	m_cDataFFT=NULL;
	m_cOutFFT = NULL;
	m_fOutIFFT = NULL;
	m_fInputBlock=NULL;
	m_fLastOutOverlap=NULL;
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
	Reset();
}

CFIROLAvmls::~CFIROLAvmls(void)
{
	Release();
}

void CFIROLAvmls::Release()
{
	if(m_fCoefficients) {delete[] m_fCoefficients; m_fCoefficients=NULL;}
	if(m_cCoefsFFT) {delete[] m_cCoefsFFT; m_cCoefsFFT =NULL;}
	if(m_cDataFFT) {delete[] m_cDataFFT; m_cDataFFT = NULL;}
	if(m_cOutFFT) {delete[] m_cOutFFT; m_cOutFFT=NULL;}
	if(m_fOutIFFT) {delete[] m_fOutIFFT; m_fOutIFFT=NULL;}
	if(m_fInputBlock){ delete[] m_fInputBlock;m_fInputBlock=NULL;}
	if(m_fLastOutOverlap) {delete[] m_fLastOutOverlap; m_fLastOutOverlap=NULL;}
	if(m_pDescFFT) DftiFreeDescriptor(&m_pDescFFT);
	if(m_pDescIFFT) DftiFreeDescriptor(&m_pDescIFFT);
	m_pDescFFT = NULL;
	m_pDescIFFT = NULL;
}

void CFIROLAvmls::Reset()
{
	m_nBlockIndex = 0;
	if(m_fInputBlock)
		memset(m_fInputBlock,0,m_nFFT*sizeof(float));//zero pad input block
	if(m_fLastOutOverlap)
		memset(m_fLastOutOverlap,0,m_nTapSize*sizeof(float));//reset output overlap
}

BOOL CFIROLAvmls::Create(int nSizeFFT,int nTapSize , float *m_fCoeffs)
{
	m_fCoefficients = new float[nSizeFFT];
	if(!m_fCoefficients) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	memset(m_fCoefficients,0,nSizeFFT*sizeof(float));//zero pad
	m_nTapSize = nTapSize;
	m_nFFT = nSizeFFT;
	m_L = m_nFFT - m_nTapSize + 1;
	m_cCoefsFFT = new MKL_Complex8[nSizeFFT];
	if(!m_cCoefsFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cDataFFT = new MKL_Complex8[nSizeFFT];
	if(!m_cDataFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cOutFFT = new MKL_Complex8[nSizeFFT];
	if(!m_cOutFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fOutIFFT = new float[nSizeFFT];
	if(!m_fOutIFFT) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fInputBlock = new float[nSizeFFT];
	if(!m_fInputBlock) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fLastOutOverlap = new float[nTapSize ];//M-1 overlap
	if(!m_fLastOutOverlap) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}

	memset(m_fInputBlock,0,nSizeFFT*sizeof(float));//zero pad input block
	memset(m_cOutFFT,0,nSizeFFT*sizeof(MKL_Complex8));//zero pad input block

	memcpy(m_fCoefficients,m_fCoeffs,nTapSize * sizeof(float));

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
		//		dfti_example_status_print(Status);
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set Scale number for Backward transform
	*/


	//--------init backward dft
	Status = DftiCreateDescriptor( &m_pDescIFFT, DFTI_SINGLE,DFTI_REAL, 1, nSizeFFT );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//		dfti_example_status_print(Status);
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	/*
	/   Set placement of result DFTI_NOT_INPLACE
	*/
	Status = DftiSetValue( m_pDescIFFT, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//dfti_example_status_print(Status);
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
		//		dfti_example_status_print(Status);
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}



	//compute H(w)
	Status = DftiComputeForward( m_pDescFFT, m_fCoefficients, m_cCoefsFFT);

	//test
	//	for(int i=0;i<nSizeFFT;i++){
	//		m_cDataFFT[i].real = 0;
	//		m_cDataFFT[i].imag = 0;
	//	}
	//	m_cDataFFT[10].real = 100;m_cDataFFT[100].imag = 0;
	//	m_cDataFFT[nSizeFFT-10].real = 230;m_cDataFFT[nSizeFFT-100].imag = 0;


	//	Status = DftiComputeBackward( m_pDescFFT, m_cDataFFT, m_fOutIFFT);
	//	FILE *ofp = fopen("c:\\samples\\ifft.txt","wt");
	//	for(int i=0;i<nSizeFFT;i++) fprintf(ofp,"%f\n",m_fOutIFFT[i]);
	//	fclose(ofp);
	//test

	Reset();
	return TRUE;

}

BOOL CFIROLAvmls::CreateBPF(int nSizeFFT,int nTapSize , float fStart,float fStop,float fs,
							int flags)
{
	/*	DFTI_DESCRIPTOR_HANDLE m_pDesc ;
	int i,k1 = floor(fStart*nTapSize/fs);
	int k2 = floor(fStop*nTapSize/fs);
	MKL_Complex8 *a1= new MKL_Complex8[nTapSize/2 -1];
	if(!a1) {Release();return FALSE;}
	MKL_Complex8 *freqz=new MKL_Complex8[nTapSize];
	if(!freqz) {delete[] a1; Release();return FALSE;}
	memset(a1,0,(nTapSize/2-1)*sizeof(MKL_Complex8));
	memset(freqz,0,nTapSize*sizeof(MKL_Complex8));
	freqz[0].real=1; freqz[0].imag=0;//dc component
	//	a1=[zeros(1,k1) ones(1,k2-k1) zeros(1,N/2-k2-1)];
	//	freqz=[1 a1 0 fliplr(a1)];
	//	ff=ifft(a);
	//	coefs=[real(ff(N/2+1:N)) real(ff(1:N/2))];
	for(i=k1;i<=k2;i++) {a1[i].real=1;a1[i].imag=0;}//f1 to f2
	for(i=0;i<nTapSize/2-1;i++) freqz[i+1] = a1[i];
	freqz[nTapSize/2].real=0;freqz[nTapSize/2].imag=0;
	for(i=0;i<nTapSize/2-1;i++) 
	freqz[nTapSize/2+i+1] = a1[nTapSize/2-2-i];
	MKL_Complex8 *coefs=new MKL_Complex8[nTapSize];
	if(!coefs) {delete[] a1;delete[] freqz; Release();return FALSE;}
	//coefs = fft.ifft(freqz,nTapSize);
	float *c = new float [nTapSize];
	if(!c) {delete[] a1; delete[] freqz;delete[] coefs; Release();return FALSE;}
	//compute H(w)
	//-------init forward dft
	long Status = DftiCreateDescriptor( &m_pDesc, DFTI_float,DFTI_REAL, 1, nTapSize );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	Status = DftiSetValue( m_pDesc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	float Scale = 1.0/(float)nTapSize;

	Status = DftiSetValue(m_pDesc, DFTI_BACKWARD_SCALE, Scale);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	Status = DftiCommitDescriptor( m_pDesc );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
	return FALSE;
	}
	Status = DftiComputeBackward( m_pDesc,  freqz,c);
	float tmp;
	for(i=0;i<nTapSize/2;i++){
	tmp = c[i];
	c[i] =c[nTapSize/2+i];
	c[nTapSize/2+i] = tmp;
	}
	//use Hamming Window
	float w;
	for(i=0;i<nTapSize;i++){
	w = 0.54 - 0.46 * cos(2*M_PI *i/nTapSize);
	c[i] *= w;
	}

	//
	FILE *fp = fopen("c:\\samples\\coef.txt","wt");
	for(i=0;i<nTapSize;i++) fprintf(fp,"%f\n",c[i]);
	fclose(fp);
	//
	DftiFreeDescriptor(&m_pDesc);

	BOOL res = Create(nSizeFFT,nTapSize,c);
	delete[] a1;
	delete[] freqz; 
	delete[] c;
	delete[] coefs;
	return res;*/

	double *coef = new double [nTapSize];
	if(!coef) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	float *coeff = new float [nTapSize];
	if(!coeff) {delete[] coef;Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	double f[2];
	if(flags & LP)
	{
		f[0] = 2*fStop/fs;
	}else if(flags & HP)
	{
		f[0] = 2*fStart/fs;
	}else{
		f[0]=2 * fStart / fs;f[1]=2 * fStop / fs;
	}
	int ret = design_fir(nTapSize,coef,f,flags,0);
	if(ret == -1) {delete[] coef;Release();return DSP_RET_FIR_DESIGN_ERROR;}
	//
#ifdef _FIR_OLA_SAVE_COEFS
	FILE *fp = fopen(_FIR_OLA_COEFS_FILE,"wt");
	for(int i=0;i<nTapSize;i++) fprintf(fp,"%f\n",coef[i]);
	fclose(fp);
#endif
	//
	BOOL res = Create(nSizeFFT,nTapSize,coeff);
	delete[] coef;
	delete[] coeff;
	return res;
}

BOOL CFIROLAvmls::Filter(float *m_fInput, int nInputSize, float *m_fOutput,int &nOutSize)
{
	int rez;
	int nInputIndex=0;
	nOutSize=0;
	long Status;
	if(nInputSize + m_nBlockIndex >= m_L)
	{//if there is at least one block
		while(nInputIndex < nInputSize)
		{
			//select one block
			rez = m_L - m_nBlockIndex;
			if(nInputIndex + rez > nInputSize )
			{
				rez = nInputSize - nInputIndex;
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(float));
			}else{
				memcpy(m_fInputBlock+m_nBlockIndex,m_fInput+nInputIndex,rez*sizeof(float));
				//memset(m_fInputBlock+m_L,0,(m_nFFT-m_L)*sizeof(float));//zero pad input block
				//block = m_fInputBlock(0:L-1)
				Status = DftiComputeForward( m_pDescFFT, m_fInputBlock, m_cDataFFT);
				vcMul(m_nFFT/2,m_cDataFFT,m_cCoefsFFT,m_cOutFFT);
				Status = DftiComputeBackward( m_pDescFFT, m_cOutFFT, m_fOutIFFT);
				memcpy(m_fOutput+nOutSize,m_fOutIFFT,m_L*sizeof(float));
				vsAdd(m_nTapSize-1,m_fOutput+nOutSize,m_fLastOutOverlap,m_fOutput+nOutSize);
				memcpy(m_fLastOutOverlap,m_fOutIFFT+m_L,(m_nTapSize-1)*sizeof(float));
				nOutSize += m_L;
			}
			m_nBlockIndex = (m_nBlockIndex + rez)%m_L;
			nInputIndex += rez;		
		}//while
	}else{//if there is not any block 
		memcpy(m_fInputBlock+m_nBlockIndex,m_fInput,nInputSize*sizeof(float));
		m_nBlockIndex += nInputSize;
	}
	return DSP_RET_SUCCESS;
}

}//namespace