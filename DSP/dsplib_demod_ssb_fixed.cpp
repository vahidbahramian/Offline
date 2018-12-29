#include "stdafx.h"
#include "dsplib_demod_ssb_fixed.h"

extern double __ssb_hop_demod_hilbert_coef[501];

extern double __ssb_hop_demod_hop_lpf_audio[115];
///////////////////////////////////////////////////////////
namespace NS_DSP
{

	CSSBFixedvmld::CSSBFixedvmld()
	{
		m_nModType = DSPLIB_LSB_DEMOD;
		m_pDescFFTHann = NULL;
		m_nSizeFFT = 0;
		m_fFs = 40e3;
		m_fZeroMemory = NULL;
	//	hanning(_DSPLIB_SSBFIXED_BLK,m_fHannWindow);//hanning window
	//	memset(m_fMultHann,0,_DSPLIB_SSBFIXED_BLK*sizeof(double));
		m_timeDemodDDS = 1;//initial phase
		m_iThreshold=8;
		m_iCarrierSearchCntr=0;
		m_nBlockIndex=0;
		m_iState = _DSPLIB_SSBFIXED_SEARCH_STATE;//search for fc
		m_fFc=0;


	}

	CSSBFixedvmld::~CSSBFixedvmld()
	{
		Release();
	}

	void CSSBFixedvmld::Reset()
	{
		m_fir_Demod.Reset();
		m_fir_hilbert.Reset();
		m_timeDemodDDS = 1;//initial phase
		m_iCarrierSearchCntr=0;
		m_nBlockIndex=0;
		memset(m_fZeroMemory,0,m_nSizeFFT*sizeof(double));
		m_iState = _DSPLIB_SSBFIXED_SEARCH_STATE;
		memset(m_fMultHannFFTSum,0,(_DSPLIB_SSBFIXED_FFTSIZE/2) * sizeof(double));

	}

	void CSSBFixedvmld::Release()
	{
		if(m_pDescFFTHann )DftiFreeDescriptor(&m_pDescFFTHann);
		m_pDescFFTHann =NULL;

		if(m_fZeroMemory) delete[] m_fZeroMemory;
		m_fZeroMemory = NULL;

		m_fir_hilbert.Release();
		m_fir_Demod.Release();
	}

	BOOL CSSBFixedvmld::Create(int nSizeFFT,int modType,double fs,int iThreshold/* =10 */)
	{
		BOOL ret;
		m_iThreshold=iThreshold;
		m_nSizeFFT = nSizeFFT;
		m_nModType = modType;
		m_fFs = fs;
		//Init FFT Hann Struct----------------------
		long Status = DftiCreateDescriptor( &m_pDescFFTHann, 
			DFTI_DOUBLE,DFTI_REAL, 1, _DSPLIB_SSBFIXED_FFTSIZE );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			return DSP_RET_FFT_INIT_ERROR;
		}
		//   Set placement of result DFTI_NOT_INPLACE
		Status = DftiSetValue( m_pDescFFTHann, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			return DSP_RET_FFT_INIT_ERROR;
		}
		double Scale = 1.0/(double)_DSPLIB_SSBFIXED_FFTSIZE;

		Status = DftiSetValue(m_pDescFFTHann, DFTI_BACKWARD_SCALE, Scale);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			return DSP_RET_FFT_INIT_ERROR;
		}

		//   Commit Dfti descriptor
		Status = DftiCommitDescriptor( m_pDescFFTHann );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			return DSP_RET_FFT_INIT_ERROR;
		}

		//-------------------------------------
		//----------LPFs------------------------
		ret = m_fir_Demod.Create(8192,115,__ssb_hop_demod_hop_lpf_audio);//__ssb_hop_demod_lpf_coef);
		if(ret != DSP_RET_SUCCESS) return ret;
		//---------------------------------------
		//--------Hilberts----------------------
		ret = m_fir_hilbert.Create(8192,501,__ssb_hop_demod_hilbert_coef);
		if(ret != DSP_RET_SUCCESS) return ret;
		//	ret = m_hilbert.Create(4000);
		//	if(ret != DSP_RET_SUCCESS) return ret;
		//---------------------------------------
		m_fZeroMemory= new double[nSizeFFT ];
		if(!m_fZeroMemory) {
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		memset(m_fZeroMemory,0,nSizeFFT*sizeof(double));//filled with 0


		Reset();
		return DSP_RET_SUCCESS;
	}

	//finds fc from _DSPLIB_SSBFIXED_NUMBER_FFT*_DSPLIB_SSBFIXED_BLK samples
	BOOL CSSBFixedvmld::FindCarrier(double *m_fSamples,int nInputSize, double &m_fCarrier)
	{
		int i,ret;
		int rez;
		int nInputIndex=0;
		int outHilbertIndex=0,nHilbertSizeOut=0;
		m_fCarrier=0;
		long status;
		int offset = m_fir_hilbert.m_nTapSize/2;
		if(nInputSize + m_nBlockIndex >= _DSPLIB_SSBFIXED_BLK)
		{//if there is at least one block
			while(nInputIndex < nInputSize)
			{
				//select one block
				rez = _DSPLIB_SSBFIXED_BLK - m_nBlockIndex;
				if(nInputIndex + rez > nInputSize )
				{
					rez = nInputSize - nInputIndex;
					memcpy(m_fInputBlock+m_nBlockIndex,m_fSamples+nInputIndex,rez*sizeof(double));
				}else{
					memcpy(m_fInputBlock+m_nBlockIndex,m_fSamples+nInputIndex,rez*sizeof(double));
					//WriteToFile("c:\\samples\\ssb_blk_in.txt",m_fInputBlock,_DSPLIB_SSBFIXED_BLK);
					//----------------coarse DDC ---------------
					m_fir_hilbert.Reset();
					for (i=0;i<_DSPLIB_SSBFIXED_BLK;i++)
					{
						m_fMultSin[i] = sin(m_timeDemodDDS*2*M_PI*7e3/m_fFs) * m_fInputBlock[i];
						m_fMultCos[i] = cos(m_timeDemodDDS*2*M_PI*7e3/m_fFs) * m_fInputBlock[i];
						m_timeDemodDDS++;
					}
					//WriteToFile("c:\\samples\\cos.txt",m_fMultCos,_DSPLIB_SSBFIXED_BLK);
					//----Do Hilbert Transform-------------//
					ret = m_fir_hilbert.Filter(m_fMultSin ,_DSPLIB_SSBFIXED_BLK,m_fOutHilbert
						,nHilbertSizeOut);
					if(ret != DSP_RET_SUCCESS)
					{
						return DSP_RET_MEMORY_ALLOCATE_ERROR;
					}
					outHilbertIndex += nHilbertSizeOut;
					ret = m_fir_hilbert.Filter(m_fZeroMemory ,m_fir_hilbert.m_nFFT
						,m_fOutHilbert + outHilbertIndex,nHilbertSizeOut);
					if(ret != DSP_RET_SUCCESS)
					{
						return DSP_RET_MEMORY_ALLOCATE_ERROR;
					}
					outHilbertIndex += nHilbertSizeOut;
					//WriteToFile("c:\\samples\\ssb_hilbert.txt",m_fOutHilbert,_DSPLIB_SSBFIXED_BLK+1000);
					//create i+imag(hilbert(q)) | i-imag(hilbert(q))
					if( m_nModType == DSPLIB_LSB_DEMOD)
					{//LSB
						//vdAdd(offset,m_fZeroMemory,m_fOutHilbert,m_fBaseBandLSB);
						vdAdd(_DSPLIB_SSBFIXED_BLK,m_fMultCos,m_fOutHilbert+offset,m_fBaseBandLSB);
						//WriteToFile("c:\\samples\\ssb_bllsb.txt",m_fBaseBandLSB,_DSPLIB_SSBFIXED_BLK);
						vdMul(_DSPLIB_SSBFIXED_BLK,m_fBaseBandLSB,m_fHannWindow,m_fMultHann);
					}else{//USB
						//vdSub(offset,m_fZeroMemory,m_fOutHilbert,m_fBaseBandUSB);
						vdSub(_DSPLIB_SSBFIXED_BLK,m_fMultCos,m_fOutHilbert+offset,m_fBaseBandUSB);
						vdMul(_DSPLIB_SSBFIXED_BLK,m_fBaseBandUSB,m_fHannWindow,m_fMultHann);
					}
					memset(m_fMultHann+_DSPLIB_SSBFIXED_BLK,0,
						(_DSPLIB_SSBFIXED_FFTSIZE-_DSPLIB_SSBFIXED_BLK)*sizeof(double));
					status = DftiComputeForward(m_pDescFFTHann,m_fMultHann,m_cMultHannFFT);
					vzAbs(_DSPLIB_SSBFIXED_FFTSIZE/2,m_cMultHannFFT,m_fMultHannFFTAbs);
					vdAdd(_DSPLIB_SSBFIXED_FFTSIZE/2,m_fMultHannFFTSum,m_fMultHannFFTAbs,m_fMultHannFFTSum);
					//WriteToFile("c:\\samples\\m_fMultHannFFTSum.txt",m_fMultHannFFTSum,_DSPLIB_SSBFIXED_FFTSIZE/2);
					m_iCarrierSearchCntr++;
					if (m_iCarrierSearchCntr > _DSPLIB_SSBFIXED_NUMBER_FFT)
					{
						m_fCarrier=FindPeak();
						return TRUE;
					}else if(m_iCarrierSearchCntr == _DSPLIB_SSBFIXED_NUM_FFT_FOR_OUTPUT)
					{
						m_fCarrier=FindPeak();
					}
				}
				m_nBlockIndex = (m_nBlockIndex + rez)%_DSPLIB_SSBFIXED_BLK;
				nInputIndex += rez;		
			}//while
		}else{//if there is not any block 
			memcpy(m_fInputBlock+m_nBlockIndex,m_fSamples,nInputSize*sizeof(double));
			m_nBlockIndex += nInputSize;
		}
		return FALSE;
	}

	//finds peak from m_fMultHannFFTSum array
	double CSSBFixedvmld::FindPeak()
	{
		int pos;
		int idx = cblas_idamax(_DSPLIB_SSBFIXED_FFTSIZE/2,m_fMultHannFFTSum,1);
		double m_fThreshold = m_fMultHannFFTSum[idx]/m_iThreshold;
		double fc = 0;

		//find max index by a 5 samples window
		for(pos=(int)floor(1600*_DSPLIB_SSBFIXED_FFTSIZE/m_fFs)+12;pos >=5 ;pos--)
		{
			idx = cblas_idamax(11,m_fMultHannFFTSum+pos,1);
			if(idx == 5)
			{
				//if HannZs(max_index(i))> Thresh && max_index(i)*Fs/L2<3000
				if(m_fMultHannFFTSum[pos+5] > m_fThreshold )
				{
					fc = ((pos+5)*m_fFs/_DSPLIB_SSBFIXED_FFTSIZE);
					break;
				}
			}
		}//for pos
		if(m_nModType == DSPLIB_LSB_DEMOD) fc = 7e3 + fc;
		else fc = 7e3 - fc;
		return fc;
	}

	BOOL CSSBFixedvmld::Demodulate(double *m_fInputData,int nInputSize,double *m_fOutput,int &nOutputSize)
	{
		int ret,nLen,nOutSize =0;
		nOutputSize =0;
		double fc;
		if (nInputSize < 1) return DSP_RET_SUCCESS;
		int i,blkNum = nInputSize / _DSPLIB_SSBFIXED_FFTSIZE;
		switch(m_iState)
		{
		case _DSPLIB_SSBFIXED_SEARCH_STATE://only search
			if(FindCarrier(m_fInputData,nInputSize,m_fFc) == TRUE)
				m_iState = _DSPLIB_SSBFIXED_FINE_ESTIMATE_STATE;
			else
				if(m_iCarrierSearchCntr == _DSPLIB_SSBFIXED_NUM_FFT_FOR_OUTPUT)
					m_iState = _DSPLIB_SSBFIXED_COARSE_ESTIMATE_STATE;
			break;

		case _DSPLIB_SSBFIXED_COARSE_ESTIMATE_STATE://search & demod
			if(FindCarrier(m_fInputData,nInputSize,fc) == TRUE)
			{
				m_fFc = fc;
				m_iState = _DSPLIB_SSBFIXED_FINE_ESTIMATE_STATE;
			}
			for(i=0;i<=blkNum;i++)
			{
				if (i<blkNum) nLen = _DSPLIB_SSBFIXED_FFTSIZE;
				else nLen = nInputSize % _DSPLIB_SSBFIXED_FFTSIZE;
				ret = DemodDDC(m_fInputData+(i*_DSPLIB_SSBFIXED_FFTSIZE),
					nLen,m_fOutput+nOutputSize,nOutSize);
				nOutputSize += nOutSize;

				if(ret != DSP_RET_SUCCESS) return ret;
			}
			break;

		case _DSPLIB_SSBFIXED_FINE_ESTIMATE_STATE://only demod
			for(i=0;i<=blkNum;i++)
			{
				if (i<blkNum) nLen = _DSPLIB_SSBFIXED_FFTSIZE;
				else nLen = nInputSize % _DSPLIB_SSBFIXED_FFTSIZE;
				ret = DemodDDC(m_fInputData+(i*_DSPLIB_SSBFIXED_FFTSIZE),
					nLen,m_fOutput+nOutputSize,nOutSize);
				nOutputSize += nOutSize;

				if(ret != DSP_RET_SUCCESS) return ret;
			}
			break;
		}//switch

		return DSP_RET_SUCCESS;

	}//func


	//the buffer must be created outside this function nInputSize <= 8192
	BOOL CSSBFixedvmld::DemodDDC(double *m_fInputData,int nInputSize,double *m_fOutput,int &nOutputSize)
	{
		int ret,i;
		nOutputSize =0;
		//Do DDC on pPointer(0:nLen-1)---------------------------
		//[a]-mult by DDS
		for(i=0;i<nInputSize;i++)
		{
			m_fMultCos[i] = cos(m_timeDemodDDS*2*M_PI*m_fFc/m_fFs) * m_fInputData[i];
			m_timeDemodDDS++;
		}
		//[b]-LPF
		if((ret = m_fir_Demod.Filter(m_fMultCos,nInputSize,m_fOutput,nOutputSize))!=DSP_RET_SUCCESS)
		{
			return ret;
		}
		return DSP_RET_SUCCESS;
	}//func

}//namespace