#include "StdAfx.h"
#include "Cdsplib_amr_analog.h"
#include "dsplib_delay.h"
#include "dsplib_amr_module.h"
#include "dsplib_debug.h"

namespace NS_DSP
{
	//filters coefficients
	FILTER_COEFS	CHFAMRAnalog::m_stFilterCoefBuffer;
	FILTER_COEFS	CHFAMRAnalog::m_stHilbertCoefBuffer;
	FILTER_COEFS	CHFAMRAnalog::m_stWindowHannBuffer;

	//Double Precision
	//----------------------------//
	CHFAMRAnalog::CHFAMRAnalog(void)
	{
		m_nSubSegIdx	 = 0;
		m_nNDFT			 = 0;
		m_fFrameSize	 = 0.0f;
		m_fOverlap		 = 0.0f;
		m_cCoefFIR		 = NULL;
		m_fSFrame		 = NULL;
		m_cOutFFT		 = NULL;
		m_cOutEnv		 = NULL;
		m_fSample		 = NULL;
		m_fdPhi		     = NULL;
		//m_fFreqNoiseFlag = NULL;
		m_fCoefficients	 = NULL;
		m_cSamplesFFT	 = NULL;
		m_fTemp			 = NULL;
		m_fTempAssist	 = NULL;
		m_fMinimaA2		 = NULL;
		m_fNoisePSDTot	 = NULL;
		bIsCalcMinima	 = TRUE;

	}

	CHFAMRAnalog::~CHFAMRAnalog(void)
	{
		Release();
	}
	BOOL CHFAMRAnalog::IsLoadedFIRCoef()
	{
		if ((!m_stFilterCoefBuffer.IsEmpty()) && 
			(!m_stHilbertCoefBuffer.IsEmpty()) &&
			(!m_stWindowHannBuffer.IsEmpty())) return TRUE;
		return FALSE;
	}

	BOOL CHFAMRAnalog::Initialize(int nFftSize)
	{
// 		int ret;

		if (!IsLoadedFIRCoef()) {
			Release();
			return FALSE;
		}

		//////////Noise Estimation Initialize////////
		m_fFrameSize = 0.0512 * HF_AMR_ANALOG_Fs;
		m_fOverlap = HF_AMR_ANALOG_OVERLAP;
		m_nNDFT = m_fFrameSize * 8;
		////////////////////////////////////////////

		m_fCoefficients = new double[nFftSize];
		if(!m_fCoefficients) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		memset(m_fCoefficients,0,nFftSize*sizeof(double));//zero pad
		memcpy(m_fCoefficients,m_stFilterCoefBuffer.m_fCoefficients,
			m_stFilterCoefBuffer.m_nSize * sizeof(double));
		m_cCoefFIR=new MKL_Complex16[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_cCoefFIR){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}

		m_nFftSize = nFftSize;
		//Init FFT------------------------------------ 
		//-------init forward dft
		long Status = DftiCreateDescriptor( &m_pFFTDesc, DFTI_DOUBLE,DFTI_REAL, 1, nFftSize );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		/*
		/   Set placement of result DFTI_NOT_INPLACE
		*/
		Status = DftiSetValue( m_pFFTDesc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//dfti_example_status_print(Status);
			//printf(" DftiSetValue TEST FAIL\n");  
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		double Scale = 1.0/(double)(nFftSize);
		//printf("\n DFTI_BACKWARD_SCALE  = 1/n \n");

		Status = DftiSetValue(m_pFFTDesc, DFTI_BACKWARD_SCALE, Scale);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//printf(" TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
/////==========================
		long Status1 = DftiCreateDescriptor( &m_pFFTDesc1, DFTI_DOUBLE,DFTI_REAL, 1, (HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP) );
		if(! DftiErrorClass(Status1, DFTI_NO_ERROR)){
			//		dfti_example_status_print(Status);
			//printf(" DftiCreateDescriptor TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		/*
		/   Set placement of result DFTI_NOT_INPLACE
		*/
		Status1 = DftiSetValue( m_pFFTDesc1, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status1, DFTI_NO_ERROR)){
			//dfti_example_status_print(Status);
			//printf(" DftiSetValue TEST FAIL\n");  
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		double Scale1 = 1.0/(double)(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);
		//printf("\n DFTI_BACKWARD_SCALE  = 1/n \n");

		Status1 = DftiSetValue(m_pFFTDesc1, DFTI_BACKWARD_SCALE, Scale1);
		if(! DftiErrorClass(Status1, DFTI_NO_ERROR)){
			//printf(" TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}

///============================

		/*
		/   Commit Dfti descriptor
		*/
		Status = DftiCommitDescriptor( m_pFFTDesc );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//		dfti_example_status_print(Status);
			//printf(" DftiCommitDescriptor TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		/*
		/   Set Scale number for Backward transform
		*/

		Status1 = DftiCommitDescriptor( m_pFFTDesc1 );
		if(! DftiErrorClass(Status1, DFTI_NO_ERROR)){
			//		dfti_example_status_print(Status);
			//printf(" DftiCommitDescriptor TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}


		//--------init backward dft
		Status = DftiCreateDescriptor( &m_pIFFTDesc, DFTI_DOUBLE,DFTI_REAL, 1, nFftSize );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//		dfti_example_status_print(Status);
			//printf(" DftiCreateDescriptor TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		/*
		/   Set placement of result DFTI_NOT_INPLACE
		*/
		Status = DftiSetValue( m_pIFFTDesc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//dfti_example_status_print(Status);
			//printf(" DftiSetValue TEST FAIL\n");  
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}

		Scale = 1.0/double(nFftSize);

		Status = DftiSetValue(m_pIFFTDesc, DFTI_BACKWARD_SCALE, Scale);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//printf(" TEST FAIL\n"); 
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}

		/*
		/   Commit Dfti descriptor
		*/
		Status = DftiCommitDescriptor( m_pIFFTDesc );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			//		dfti_example_status_print(Status);
			//printf(" DftiCommitDescriptor TEST FAIL\n");
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}


		Status = DftiComputeForward( m_pFFTDesc, m_fCoefficients,m_cCoefFIR);


		//-------------Allocate Memory--------------------------------
// 		m_fSamplesFFT = new MKL_Complex16[m_nFftSize*2];
// 		if(!m_fFFTAbs) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
// 		m_fFFTSqr = new double[m_nFftSize];
// 		if (!m_fFFTSqr)	{Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		//----------------Allocated memory for Coefs-------------------
		m_fSFrame = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if (!m_fSFrame)	{Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		m_cOutFFT = new MKL_Complex16[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_cOutFFT)	{Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		m_cOutEnv = new MKL_Complex16[HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP];
		if (!m_cOutEnv) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		m_fSample = new double[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_fSample){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		m_fdPhi=new double[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_fdPhi){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		//FreqNoiseFlag 
		//m_fFreqNoiseFlag = new double[HF_AMR_ANALOG_FRAME_SIZE/2];
		//if (!m_fFreqNoiseFlag){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		//
		m_cSamplesFFT = new MKL_Complex16[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_cSamplesFFT){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}

		m_fMinimaA2 = new double[m_nNDFT/4+10];
		if (!m_fMinimaA2)
		{
			Release();
			return FALSE;
		}
		for (int i = 0;i<m_nNDFT/4;i++)
		{
			m_fMinimaA2[i] = 1e12;
		}

		m_fTempAssist = new double[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_fTempAssist)
		{
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		m_fTemp = new double[HF_AMR_ANALOG_FRAME_SIZE];
		if(!m_fTemp){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}

		m_fNoisePSDTot = new double[HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_fNoisePSDTot){Release();return FALSE;}

		return DSP_RET_SUCCESS;
	}
	
	void CHFAMRAnalog::Release()
	{
		SAFERELEASE_ARRAY(m_fTemp);
		SAFERELEASE_ARRAY(m_fSFrame);
		SAFERELEASE_ARRAY(m_cOutFFT);
		SAFERELEASE_ARRAY(m_cCoefFIR);
		SAFERELEASE_ARRAY(m_fdPhi);
		//SAFERELEASE_ARRAY(m_fFreqNoiseFlag);
		SAFERELEASE_ARRAY(m_fCoefficients);
		SAFERELEASE_ARRAY(m_cSamplesFFT);
		SAFERELEASE_ARRAY(m_cOutEnv);
		SAFERELEASE_ARRAY(m_fTempAssist);
		SAFERELEASE_ARRAY(m_fMinimaA2);
		SAFERELEASE_ARRAY(m_fNoisePSDTot);
		SAFERELEASE_ARRAY(m_fSample);
		if(m_pFFTDesc) DftiFreeDescriptor(&m_pFFTDesc);
		m_pFFTDesc = NULL;
		if (m_pIFFTDesc) DftiFreeDescriptor(&m_pIFFTDesc);
		m_pIFFTDesc = NULL;
		if (m_pFFTDesc1) DftiFreeDescriptor(&m_pFFTDesc1);
		m_pFFTDesc1 = NULL;

	}


	void  CHFAMRAnalog::InitAMRParam(double *m_fInputBuffer,double *m_dblA2FFTSmoothed,double *m_dblNoisePSD,
		double *m_dblMinimaA2SubSeg,double *m_fMinimaA2)
	{
		int i;
		MKL_LONG Status;
		memset(m_fSFrame,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		Status=DftiComputeForward(m_pFFTDesc,m_fInputBuffer,m_cOutFFT);
		vzMul(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_cOutFFT,m_cCoefFIR,m_cOutFFT);
		vzAbs(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_cOutFFT,m_fSFrame);
		for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2+1;i++)
			m_fSFrame[i]=m_fSFrame[i]/HF_AMR_ANALOG_FRAME_SIZE;
		vdSqr(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_fSFrame,m_fSFrame);

		//Added 
		memset(m_fTempAssist,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		for (int i=HF_AMR_ANALOG_BandStartIdx;i<=HF_AMR_ANALOG_BandStopIdx;i++)
		{
			m_fTempAssist[i] = m_fSFrame[i];//a2_fft DownSample
		}
		cblas_dcopy(HF_AMR_ANALOG_TotalIdx,m_fTempAssist+HF_AMR_ANALOG_BandStartIdx,1,m_dblA2FFTSmoothed,1);
// 		memcpy(m_fA2FFTSmoothed,m_fTempAssist,(nBufferSize+1)*sizeof(double));
//  	memcpy(m_dblA2FFTSmoothed,m_fSFrame,(HF_AMR_ANALOG_FRAME_SIZE/2)*sizeof(double));
		int m_nMinLen = round((double) HF_AMR_ANALOG_minDuration*HF_AMR_ANALOG_Fs/(m_fFrameSize - m_fOverlap));
		m_nSubSegIdx = ceil((double) m_nMinLen / HF_AMR_ANALOG_subMinLen);

		for(int k=0;k<m_nSubSegIdx;k++)
		{
			for (int j=0;j<(HF_AMR_ANALOG_TotalIdx);j++)
			{
				m_dblMinimaA2SubSeg[j+k*(HF_AMR_ANALOG_TotalIdx)]=4*m_fTempAssist[j+HF_AMR_ANALOG_BandStartIdx];
			}//for2
		}//for1

//  		memcpy(m_dblNoisePSD,m_fSFrame,(HF_AMR_ANALOG_FRAME_SIZE/2)*sizeof(double));
		memcpy(m_dblNoisePSD,m_fTempAssist+HF_AMR_ANALOG_BandStartIdx,(HF_AMR_ANALOG_TotalIdx)*sizeof(double));
		memcpy(m_fMinimaA2,m_dblMinimaA2SubSeg,(HF_AMR_ANALOG_TotalIdx)*sizeof(double));
	}


	void CHFAMRAnalog::EnvelopeAndPhaseEst(double *fFIROutBuffer,double *fHilbertOutBuffer,	double *m_fPhiOut,double *m_fEnv)
	{
// 		int i;
		vdAtan2(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,fHilbertOutBuffer,fFIROutBuffer,m_fPhiOut);
		vdSqr(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,fFIROutBuffer,fFIROutBuffer);
		vdSqr(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,fHilbertOutBuffer,fHilbertOutBuffer);
		vdAdd(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,fFIROutBuffer,fHilbertOutBuffer,m_fEnv);
		vdSqrt(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_fEnv,m_fEnv);

	}

	void CHFAMRAnalog::CorrectionPhase(double *input,double *output)
	{
		int i;
		double dblDPhi;
		for(i=0;i<HF_AMR_ANALOG_FRAME_SIZE-1;i++)
		{
			dblDPhi=input[i];
			if (input[i]<-M_PI)
				dblDPhi=input[i]+2.*M_PI;
			else if(input[i]>M_PI)
				dblDPhi=input[i]-2.*M_PI;

			m_fdPhi[i]=dblDPhi;
		}

		for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE-1;i++)
			if (m_fdPhi[i])
				if ((m_fdPhi[i]==-M_PI)&&(input[i]>0))
					m_fdPhi[i]==M_PI;
		vdSub(HF_AMR_ANALOG_FRAME_SIZE-1,m_fdPhi,input,output);
	}

	void CHFAMRAnalog::Cumsum(double *input,double *Output,int nSize)
	{
		double dblSum = 0;
		for (int cnt=0;cnt<nSize;cnt++)
		{
			dblSum = dblSum + input[cnt];
			Output[cnt]=dblSum;
		}

	}
	BOOL CHFAMRAnalog::NoiseEstimation(int totalSteps,int SubMinSegs,int NumberSetps,double *input,double *noisePSD
						,double *a2FFTSmoothed,double *minimaA2SubSeg,double *noisePSDMed,double *a2FFT,double *minimaA2)
	{
		int nBufferSize = (int)m_nNDFT/8;
		int i;
		MKL_LONG Status;
// 		memset(m_fSFrame,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
// 		Status=DftiComputeForward(m_pFFTDesc,input,m_cOutFFT);
// 		vzMul(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_cOutFFT,m_cCoefFIR,m_cSamplesFFT);
// 		Status=DftiComputeBackward(m_pFFTDesc,m_cSamplesFFT,m_fSFrame);
// 		afxDump << "AMR base: vdMul-NoiseEstimate \n";
		vdMul(HF_AMR_ANALOG_FRAME_SIZE,m_stWindowHannBuffer.m_fCoefficients,input,m_fSFrame);
		Status=DftiComputeForward(m_pFFTDesc, m_fSFrame, m_cOutFFT);
		memset(m_fSFrame,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
// 		afxDump << "AMR base: vzAbs-NoiseEstimate \n";
		vzAbs(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_cOutFFT,m_fSFrame);
		for (i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2+1/*(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP)*/;i++)
			m_fSFrame[i]=m_fSFrame[i]/HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_MEAN_HANN;

// 		afxDump << "AMR base: vdSqr-NoiseEstimate \n";
		vdSqr(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_fSFrame,m_fSFrame);//a2_fft->MATLAB
		cblas_dcopy(HF_AMR_ANALOG_FRAME_SIZE/2+1,m_fSFrame,1,a2FFT,1);

		//calc Noise Estimation	
		/*memset(m_fTempAssist,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));*/
		
//  		for (int i=HF_AMR_ANALOG_BandStartIdx;i<=HF_AMR_ANALOG_BandStopIdx;i++)
//  		{
//  			m_fTempAssist[i] = a2FFT[i];
//  		}
		double fMinVal = 1e9;
// 		int nReduce1 = (int) (NumberSetps+1)%HF_AMR_ANALOG_subMinLen;
// 		int nReduce2 = (int) (NumberSetps)%HF_AMR_ANALOG_subMinLen;

		int nReduce1 = (int) (totalSteps)%HF_AMR_ANALOG_subMinLen;
		int nReduce2 = (int) (totalSteps-1)%HF_AMR_ANALOG_subMinLen;

// 		afxDump << "AMR base: forSmooth-NoiseEstimate \n";
		for (int i=0;i<HF_AMR_ANALOG_TotalIdx;i++)
		{
			a2FFTSmoothed[i+nReduce1*(HF_AMR_ANALOG_TotalIdx)] = 
				(1-HF_AMR_ANALOG_tsco01)*a2FFT[i+HF_AMR_ANALOG_BandStartIdx] 
			    + HF_AMR_ANALOG_tsco01*a2FFTSmoothed[i+nReduce2*(HF_AMR_ANALOG_TotalIdx)];
		}

		////test
		//FILE *fp1 = fopen("c:\\smooth.txt","at");
		//for (int i=nReduce1*(HF_AMR_ANALOG_TotalIdx);i<(HF_AMR_ANALOG_TotalIdx+nReduce1*(HF_AMR_ANALOG_TotalIdx));i++){
		//	fprintf(fp1,"%40.40f\t",a2FFTSmoothed[i]);
		//}
		//fprintf(fp1,"\n");
		//fclose(fp1);
		//////test

// 		memcpy(minimaA2+HF_AMR_ANALOG_TotalIdx,minimaA2,(HF_AMR_ANALOG_TotalIdx)*sizeof(double));
// 		afxDump << "AMR base: cblas_dcopy-NoiseEstimate \n";
		cblas_dcopy(HF_AMR_ANALOG_TotalIdx,minimaA2,1,minimaA2+HF_AMR_ANALOG_TotalIdx,1);

// 		afxDump << "AMR base: forSubSeg-NoiseEstimate \n";
		if (((totalSteps+1)%HF_AMR_ANALOG_subMinLen)==0)
		{
			for (int i=0;i<(HF_AMR_ANALOG_TotalIdx);i++)
			{
				for (int j=0;j<HF_AMR_ANALOG_subMinLen;j++)
				{
					fMinVal = myMin(fMinVal,a2FFTSmoothed[i+j*(HF_AMR_ANALOG_TotalIdx)]);
				}
				minimaA2SubSeg[i+m_nSubSegIdx*(HF_AMR_ANALOG_TotalIdx)] = fMinVal;
				fMinVal = 1e9;

				if (minimaA2[i]>minimaA2SubSeg[i+m_nSubSegIdx*(HF_AMR_ANALOG_TotalIdx)])
				{
					minimaA2[i] = minimaA2SubSeg[i+m_nSubSegIdx*(HF_AMR_ANALOG_TotalIdx)];
				}
			}

			if (m_nSubSegIdx == SubMinSegs)
			{
				m_nSubSegIdx = 0;
			}
			else
				m_nSubSegIdx += 1;
		}else
		{
			cblas_dcopy(HF_AMR_ANALOG_TotalIdx,minimaA2+HF_AMR_ANALOG_TotalIdx,1,minimaA2,1);
// 			memcpy(minimaA2,minimaA2+HF_AMR_ANALOG_TotalIdx,HF_AMR_ANALOG_TotalIdx*sizeof(double));
		}

// 		afxDump << "AMR base: noisePSD-NoiseEstimate \n";
		if ( totalSteps<5 )
		{
			for(int i=0;i<(HF_AMR_ANALOG_TotalIdx);i++)
			{
				double TempVal01 = HF_AMR_ANALOG_tsco_noise_est*noisePSD[i];
				noisePSD[i] = (1-HF_AMR_ANALOG_tsco_noise_est)*a2FFT[i+HF_AMR_ANALOG_BandStartIdx] + TempVal01;
			}
		}
		else
		{
			for(int i=0;i<(HF_AMR_ANALOG_TotalIdx);i++)
			{
				double fTempVal01 = double(a2FFTSmoothed[i+nReduce1*(HF_AMR_ANALOG_TotalIdx)]/
					(0.95*HF_AMR_ANALOG_BETAmin*minimaA2[i]+0.25*HF_AMR_ANALOG_Bias_CF*noisePSD[i]));
				if ( fTempVal01 < 1 )
				{
					noisePSD[i] = (1-HF_AMR_ANALOG_tsco_noise_est)*a2FFT[i+HF_AMR_ANALOG_BandStartIdx] 
					+ HF_AMR_ANALOG_tsco_noise_est*noisePSD[i];

				}else
				{
					noisePSD[i] = noisePSD[i];
				}
			}
		}

		////test
		//FILE *fp2 = fopen("c:\\noisePSD.txt","at");
		//for (int i=0;i<HF_AMR_ANALOG_TotalIdx;i++){
		//	fprintf(fp2,"%40.40f\t",noisePSD[i]);
		//}
		//fprintf(fp2,"\n");
		//fclose(fp2);
		//////test

//		memset(m_fTempAssist,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
//		int nCnt = 0;
//		//for (int i=0;i<(HF_AMR_ANALOG_TotalIdx);i++)
//		//{
//		//	m_fTempAssist[nCnt] = noisePSD[i];
//		//	double fValue = (noisePSD[i]+noisePSD[i+1])/2;
//		//	m_fTempAssist[nCnt+1] = fValue;
//		//	nCnt+=2;
//		//}
//		//m_fTempAssist[nCnt]=m_fTempAssist[nCnt+1]=noisePSD[HF_AMR_ANALOG_TotalIdx-1];
//		//memcpy(m_fNoisePSDTot,m_fTempAssist,(HF_AMR_ANALOG_FRAME_SIZE/2+1)*sizeof(double));
//
//		for (int i=HF_AMR_ANALOG_BandStartIdx;i<(HF_AMR_ANALOG_BandStopIdx);i++)
//		{
//			m_fTempAssist[i] = noisePSD[i-HF_AMR_ANALOG_BandStartIdx];
//// 			m_fTempAssist[nCnt] = noisePSD[i];
//// 			double fValue = (noisePSD[i]+noisePSD[i+1])/2;
//// 			m_fTempAssist[nCnt+1] = fValue;
//// 			nCnt+=2;
//		}
//		//m_fTempAssist[nCnt]=m_fTempAssist[nCnt+1]=noisePSD[HF_AMR_ANALOG_TotalIdx-1];
//		memcpy(m_fNoisePSDTot,m_fTempAssist,(HF_AMR_ANALOG_FRAME_SIZE/2+1)*sizeof(double));
//
//
//		//////test
//		FILE *fp2 = fopen("c:\\noisePSDTot.txt","at");
//		for (int i=0;i<HF_AMR_ANALOG_FRAME_SIZE/2+1;i++){
//			fprintf(fp2,"%40.40f\t",m_fNoisePSDTot[i]);
//		}
//		fprintf(fp2,"\n");
//		fclose(fp2);
//		////////test
// 		afxDump << "AMR base: Median-NoiseEstimate \n";
		CHFAMRAnalog::Median(41,HF_AMR_ANALOG_TotalIdx,noisePSD,noisePSDMed);
		//////////test
		//FILE *fp2 = fopen("c:\\noisePSDMed.txt","at");
		//for (int i=0;i<HF_AMR_ANALOG_TotalIdx;i++){
		//	fprintf(fp2,"%40.40f\t",noisePSDMed[i]);
		//}
		//fprintf(fp2,"\n");
		//fclose(fp2);
		////////////test

		//CHFAMRAnalog::Median(41,HF_AMR_ANALOG_TotalIdx,noisePSD,noisePSDMed);

		return DSP_RET_SUCCESS;
	}

	void CHFAMRAnalog::Median(int blksz,int filterSize,double *m_fInputBuff,double *m_fMedBuffer)
	{
		int k;
		int i;
		int j;
		double dblMax=0;
		int	   nIndxMax=2049;
		int nMinRange=0;
		int nMaxRange=0;
		int nLen=0;
		memset(m_fSFrame,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));

		if (blksz==1)
		{
			cblas_dcopy(filterSize,m_fInputBuff,1,m_fSFrame,1);

			nLen = filterSize;

			double fTemp;
			for(i=0; i < ceil(double(nLen+1)/2); i++)
			{
				for(j=i; j<nLen; j++)
				{
					if( m_fSFrame[i]>m_fSFrame[j] )
					{
						fTemp = m_fSFrame[i];
						m_fSFrame[i] = m_fSFrame[j];
						m_fSFrame[j] = fTemp;
					}
				}
			}

			if( nLen%2 )
				m_fMedBuffer[0] = m_fSFrame[ (nLen-1)/2 ];
			else
				m_fMedBuffer[0] = ( m_fSFrame[ nLen/2 ]+m_fSFrame[ nLen/2-1 ] )/2;
		}
		else
		{
			for (k=0; k<filterSize; k++)
			{
				nLen = blksz;
				if (nLen%2)
				{
					nLen = (nLen-1)/2;
				}else
				{
					nLen=nLen/2-1;
				}

				nMinRange = k - nLen;
				nMaxRange = k + nLen;

				if (nMinRange < 0)
					nMinRange = 0;

				if (nMaxRange > filterSize)
					nMaxRange = filterSize;

				nLen = nMaxRange-nMinRange+1;

				//if (blksz==1)
				//	cblas_dcopy(filterSize,m_fInputBuff,1,m_fSFrame+1,1);
				//else
				//	cblas_dcopy(filterSize,m_fInputBuff,1,m_fSFrame,1);

				cblas_dcopy(nLen, m_fInputBuff+nMinRange, 1, m_fSFrame, 1);

				/*for (j=0;j<=nLen;j++)
				{
					for (i=nMinRange;i<=nMaxRange;i++)
					{
						if (m_fSFrame[i]>dblMax)
						{
							dblMax = m_fSFrame[i];
							nIndxMax = i;
						}
					}
					if (j!=(nLen))
					{
						m_fSFrame[nIndxMax]=0;
						dblMax=0;
						nIndxMax=0;
					}
					else if (j==(nLen))
						m_fMedBuffer[k]=dblMax;
				}*/

				double fTemp;
				for(i=0; i<(nLen+1)/2; i++)
				{
					for(j=i; j<nLen; j++)
					{
						if( m_fSFrame[i]>m_fSFrame[j] )
						{
							fTemp = m_fSFrame[i];
							m_fSFrame[i] = m_fSFrame[j];
							m_fSFrame[j] = fTemp;
						}
					}
				}

				m_fMedBuffer[k] = m_fSFrame[(nLen-1)/2];
			}
		}
	}
	void CHFAMRAnalog::MovingAverage(int nMaxRange,int filterSize,double *input,double *output)
	{
		int i;
		int j;
		double dblSum=0;
		memset(output,0,filterSize*sizeof(double));
		for (i=0;i<filterSize;i++)
		{
			for(j=i;j<i+nMaxRange;j++)
				dblSum = dblSum + input[j];
			output[((nMaxRange-1)/2)+i]=dblSum/nMaxRange;
		}
	}

	double CHFAMRAnalog::ComputeGammaMax(double *m_fSample)
	{
		int i;
		double dblSum=0;
		double fMeanEnv=0;
		double fMaxVal=0;
		MKL_LONG Status;
 		memset(m_cOutEnv,0,(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP)*sizeof(double));
		for (i=0;i<(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);i++)
			dblSum = dblSum + m_fSample[i];
		fMeanEnv = dblSum/(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);
		for(i=0;i<(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);i++)
			m_fSample[i]=(m_fSample[i]/fMeanEnv)-1;
		Status=DftiComputeForward(m_pFFTDesc1, m_fSample, m_cOutEnv);
		vzAbs((HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP)/2,m_cOutEnv,m_fSample);
		for (i=0;i<((HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP)/2);i++)
		{
			if (fMaxVal<m_fSample[i])
				fMaxVal=m_fSample[i];
		}
		fMaxVal = pow((double)fMaxVal,2.0);
		return (fMaxVal/((HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP)));
	}

	void CHFAMRAnalog::DoAMR(double *m_fCounts,double fCarrierToNoise,double fSTDPhiNL,double fGammaMax,
		double fCarrierEng,double fCentralEng,double fSpectralSymmetry)
	{
	}


	int CHFAMRAnalog::recognitionAMR(double *m_fCounts)
	{
		double	fMaxCount=-1;
		int	nMaxIdx=-1;
		for(int i=2;i<7;i++)
		{
			if (m_fCounts[i]>fMaxCount)
			{
				fMaxCount = m_fCounts[i];
				nMaxIdx = i;
			}
		}
		fMaxCount = fMaxCount + 2;
		if (m_fCounts[nMaxIdx]<3)
		{
			if(m_fCounts[1]>0)
				nMaxIdx = 1;
			else
				nMaxIdx = 0;
		}
		else if ( (m_fCounts[5]+m_fCounts[6]) < m_fCounts[1] )
		{
			fMaxCount=-1;
			nMaxIdx=-1;
			for(int i=1;i<7;i++)
			{
				if (m_fCounts[i]>fMaxCount)
				{
					fMaxCount = m_fCounts[i];
					nMaxIdx = i;
				}
			}
			fMaxCount = fMaxCount + 1;
		}
		if (nMaxIdx<10)
		{
			switch(nMaxIdx)
			{
			case 0:
				return AMR_ANALOG_NO_SIGNAL;break;
			case 1:
				{
					if (m_fCounts[2]>=m_fCounts[3])
						return AMR_ANALOG_AMFM_AM;
					else
						return AMR_ANALOG_AMFM_FM;
				}
				break;
			case 2:
				return AMR_ANALOG_AM;
				break;
			case 3:
				return AMR_ANALOG_FM;
				break;
			case 4:
				return AMR_ANALOG_DSB;
				break;
			case 5:
				return AMR_ANALOG_SSB;//AMR_ANALOG_LSB;
				break;
			case 6:
				return AMR_ANALOG_SSB;//AMR_ANALOG_USB;
				break;

			}
		}
		return AMR_ANALOG_NO_SIGNAL;

		//int i;
		//double	fMaxCount=0;
		//int	nMaxIdx=0;
		//for(i=2;i<7;i++)
		//{
		//	if (m_fCounts[i]>fMaxCount)
		//	{
		//		fMaxCount = m_fCounts[i];
		//		nMaxIdx = i;
		//	}
		//}
		//if (m_fCounts[nMaxIdx]<3)
		//{
		//	if(m_fCounts[0]>m_fCounts[1])
		//		nMaxIdx = 0;
		//	else
		//		nMaxIdx = 1;
		//}
		//if (nMaxIdx<10)
		//{
		//	switch(nMaxIdx)
		//	{
		//	case 0:
		//		return AMR_ANALOG_NO_SIGNAL;break;
		//	case 1:
		//		{
		//			if (m_fCounts[2]>=m_fCounts[3])
		//				return AMR_ANALOG_AMFM_AM;
		//			else
		//				return AMR_ANALOG_AMFM_FM;
		//		}
		//		break;
		//	case 2:
		//		return AMR_ANALOG_AM;
		//		break;
		//	case 3:
		//		return AMR_ANALOG_FM;
		//		break;
		//	case 4:
		//		return AMR_ANALOG_DSB;
		//		break;
		//	case 5:
		//		return AMR_ANALOG_SSB;//AMR_ANALOG_LSB;
		//		break;
		//	case 6:
		//		return AMR_ANALOG_SSB;//AMR_ANALOG_USB;
		//		break;

		//	}
		//}
		//return AMR_ANALOG_NO_SIGNAL;
	}

	//is called on app start to load all demodulators filters coefficients
	BOOL CHFAMRAnalog::LoadFiltersCoefficients()
	{
		BOOL bRet ;
		bRet = m_stFilterCoefBuffer.ReadCoefsFromFile("../FIR_Coef/new_AMR_Buffer_FIR.txt");
		if (!bRet ) return FALSE;
		bRet = m_stHilbertCoefBuffer.ReadCoefsFromFile("../FIR_Coef/new_AMR_Hilbert_Buffer.txt");
		if (!bRet ) return FALSE;
		bRet = m_stWindowHannBuffer.ReadCoefsFromFile("../FIR_Coef/AMR_Hann_Win.txt");
		if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CHFAMRAnalog::UnLoadFiltersCoefficients()
	{
		m_stFilterCoefBuffer.Release();
		m_stHilbertCoefBuffer.Release();
		m_stWindowHannBuffer.Release();
	}//func
}//name space