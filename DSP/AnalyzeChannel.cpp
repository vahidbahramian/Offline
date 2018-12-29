#include "../StdAfx.h"
#include "AnalyzeChannel.h"
#include "dsplib_debug.h"
namespace NS_DSP
{
	FILTER_COEFS	CAnalyzeChannel::m_stCoefsBuffer;

	CAnalyzeChannel::CAnalyzeChannel(void)
	{
		m_fBandStartIdx		=	0;
		m_fBandEndIdx		=	0;
		m_fFc				=	0;
		m_fSTemp			= NULL;
		m_cOutFFT			= NULL;
		m_fA2Smoothed02		= NULL;
		m_cOutConjFFT		= NULL;
		m_fSTempAssist		= NULL;
	}

	CAnalyzeChannel::~CAnalyzeChannel(void)
	{
	}

	BOOL CAnalyzeChannel::Create()
	{
		if (!LoadedFIRCoef()){
			Release();
			return FALSE;
		}

		m_fSTemp = new double[2*HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_fSTemp)
		{
			Release();
			return FALSE;
		}

		m_cOutFFT = new MKL_Complex16[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_cOutFFT)
		{
			Release();
			return FALSE;
		}

		m_fA2Smoothed02 = new double[HF_MAX_DEMOD_BUFFER_LENGTH/2+1];
		if (!m_fA2Smoothed02)
		{
			Release();
			return FALSE;
		}
		m_cOutConjFFT = new MKL_Complex16[HF_MAX_DEMOD_BUFFER_LENGTH];
		if (!m_cOutConjFFT)
		{
			Release();
			return FALSE;
		}

		m_fSTempAssist = new double[HF_MAX_DEMOD_BUFFER_LENGTH/2];
		if (!m_fSTempAssist)
		{
			Release();
			return FALSE;
		}
		int nFftSize = HF_MAX_DEMOD_BUFFER_LENGTH;
		//Init FFT------------------------------------ 
		//-------init forward dft
		long Status = DftiCreateDescriptor( &m_pFFTDesc, DFTI_DOUBLE,DFTI_REAL, 1, nFftSize );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}

		Status = DftiSetValue( m_pFFTDesc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		double Scale = 1.0/(double)(nFftSize);

		Status = DftiSetValue(m_pFFTDesc, DFTI_BACKWARD_SCALE, Scale);
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		Status = DftiCommitDescriptor( m_pFFTDesc );
		if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
			Release();
			return DSP_RET_FFT_INIT_ERROR;
		}
		//*************************************************************************
		return TRUE;
	}

	BOOL CAnalyzeChannel::LoadFiltersCoefficients(CString strMainAddr)
	{
		BOOL bRet ;
		CString strAddr;
		strAddr.Format("%sFIR_Coef\\WinHann.txt", strMainAddr);
		bRet = m_stCoefsBuffer.ReadCoefsFromFile(strAddr.GetBuffer());
		if (!bRet ) return FALSE;
		return TRUE;
	}//func

	void CAnalyzeChannel::UnLoadFiltersCoefficients()
	{
		m_stCoefsBuffer.Release();
	}//func

	BOOL CAnalyzeChannel::LoadedFIRCoef()
	{
		if (!m_stCoefsBuffer.IsEmpty()) 
			return TRUE;
		return FALSE;
	}

	void CAnalyzeChannel::Release()
	{
		SAFERELEASE_ARRAY(m_cOutFFT);
		SAFERELEASE_ARRAY(m_cOutConjFFT);
		SAFERELEASE_ARRAY(m_fSTempAssist);
		if(m_fSTemp) {delete[] m_fSTemp; m_fSTemp=NULL;}
		if(m_fA2Smoothed02) {delete[] m_fA2Smoothed02; m_fA2Smoothed02=NULL;}
	}

	void CAnalyzeChannel::EstimationCarrier(int nNFFT,double* dblF,int &nValidIdxStart,
		int &nValidIdxEnd,int &nAvrLen01,int &nAvrLen02,int &nAvrDelay01,int &nAvrDelay02,
		double *fInput,double &fFcEstimation,double &fBW,double &fSNR)
	{
		int nTemp = 0;
		long Status;
		memset(m_fSTemp,0,HF_MAX_DEMOD_BUFFER_LENGTH*sizeof(double));

		vdMul(nNFFT-HF_AMR_DIGITAL_OVERLAP,fInput,m_stCoefsBuffer.m_fCoefficients,m_fSTemp);
		Status=DftiComputeForward(m_pFFTDesc,m_fSTemp,m_cOutFFT);
		vzConj(nNFFT/2+1,m_cOutFFT,m_cOutConjFFT);
		vzMul(nNFFT/2+1,m_cOutFFT,m_cOutConjFFT,m_cOutFFT);
		for(int i = 0;i<nNFFT/2+1;i++)
		{
			m_fSTemp[i] = m_cOutFFT[i].real;
		}

		//WriteToFile("c:\\inp.txt",m_fSTemp,nNFFT/2);

		nTemp = nValidIdxStart-nAvrLen01-nAvrDelay01-1;
		memcpy(m_fSTempAssist,m_fSTemp+nTemp,(2*nAvrDelay01+1)*sizeof(double));
		nTemp = 2*nAvrDelay01+1;

		memset(m_fA2Smoothed02,0,(nNFFT/2+1)*sizeof(double));
		nTemp = nValidIdxStart-nAvrDelay02-1;
		memcpy(m_fSTempAssist,m_fSTemp+nTemp,(2*nAvrDelay02+1)*sizeof(double));
		nTemp = 2*nAvrDelay02+1;
		m_fA2Smoothed02[nValidIdxStart-1] = cblas_dasum(nTemp,m_fSTempAssist,1)/nAvrLen02;

		double fThreshold01 = m_fA2Smoothed02[nValidIdxStart-1];
		for (int k02 = (nValidIdxStart );k02<(nValidIdxEnd);k02++ )
		{
			m_fA2Smoothed02[k02] = m_fA2Smoothed02[k02-1]*nAvrLen02 - m_fSTemp[k02-nAvrDelay02-1]+ 
				m_fSTemp[k02+nAvrDelay02];
			m_fA2Smoothed02[k02] = m_fA2Smoothed02[k02] / nAvrLen02;

			fThreshold01 = myMax(m_fA2Smoothed02[k02],fThreshold01);
		}

		//fThreshold01 /= 10;
		fThreshold01 /= 5;

		m_fBandStartIdx = nValidIdxStart;
		m_fBandEndIdx = nValidIdxEnd;

		for (int k02 = (nValidIdxStart-1);k02< ((nNFFT/2)+1 - nAvrDelay01);k02++)
		{
			{
				if (m_fA2Smoothed02[k02] > fThreshold01)
				{
					m_fBandStartIdx = k02+1;
					break;
				}
			}
		}
		if (m_fBandStartIdx < nValidIdxEnd)
		{
			for (int k02 =(nValidIdxEnd-1);k02>(m_fBandStartIdx);k02--)
			{
				if (m_fA2Smoothed02[k02] > fThreshold01)
				{
					m_fBandEndIdx = k02+1;
					break;
				}
			}
		}
		nTemp = m_fBandEndIdx-m_fBandStartIdx+21;
		double fTotalEng = cblas_dasum(nTemp,m_fSTemp+int(m_fBandStartIdx-11),1);

		//Calc SNR
		double dblSNR = 0;

		int nNoiseSamplesCnt = 0;
		double fNoiseEng = 0;
		if( m_fBandStartIdx > (nValidIdxStart + 20) )
		{
			nNoiseSamplesCnt += int(m_fBandStartIdx-nValidIdxStart-20);
			fNoiseEng = cblas_dasum(nNoiseSamplesCnt,m_fSTemp+int(nValidIdxStart),1);
		}
		if( m_fBandEndIdx < (nValidIdxEnd - 20) )
		{
			nNoiseSamplesCnt += int(nValidIdxEnd-m_fBandEndIdx-20);
			fNoiseEng += cblas_dasum(int(nValidIdxEnd-m_fBandEndIdx-20),m_fSTemp+int(m_fBandEndIdx+20),1);
		}
		if( fNoiseEng > 0 )
		{
			double fNoisePower = (fNoiseEng/nNoiseSamplesCnt);
			dblSNR = 10*log10((fTotalEng/nTemp-fNoisePower)/fNoisePower);
			fSNR = dblSNR;
		}

		vdMul(nTemp,dblF+int(m_fBandStartIdx-11),m_fSTemp+int(m_fBandStartIdx-11),m_fSTempAssist);
		nTemp = m_fBandEndIdx-m_fBandStartIdx+21;
		double fFcTemp = cblas_dasum(nTemp,m_fSTempAssist,1)/fTotalEng;
		m_fFc = m_fFc + fFcTemp;
		fFcEstimation = m_fFc;

		// Calc BW
		memset(m_fSTempAssist,0,(nTemp)*sizeof(double));
		for (int i = 0;i<nTemp;i++)
		{
			m_fSTempAssist[i] = dblF[m_fBandStartIdx-11+i]-fFcTemp;
		}
		vdMul(nTemp,m_fSTempAssist,m_fSTemp+int(m_fBandStartIdx-11),m_fSTempAssist);
		double fSum = cblas_dasum(nTemp,m_fSTempAssist,1);
		double fNewBW = fSum/fTotalEng;
		if (fNewBW > fBW)
		{
			fBW = fNewBW;
		}
	}

	void CAnalyzeChannel::InitEstimationCarrier(double fFs,int &nNFFT,double* dblF,int &nValidIdxStart,
		int &nValidIdxEnd,int &nAvrLen01,int &nAvrLen02,int &nAvrDelay01,int &nAvrDelay02)
	{
		nNFFT = (pow((double)2.0 , (double)ceil(LOG2(HF_MAX_DEMOD_BUFFER_LENGTH-HF_AMR_DIGITAL_OVERLAP))));
		for (int i=0;i<(nNFFT/2+1);i++)
		{
			double fNum = (double) i / (double) nNFFT;
			dblF[i] = fNum * fFs;
		}
		nValidIdxStart = 1800 * nNFFT / (HF_MAX_DEMOD_BUFFER_LENGTH);
		nValidIdxEnd = 6400 * nNFFT / (HF_MAX_DEMOD_BUFFER_LENGTH);
		nAvrLen01 = 201 * nNFFT / (HF_MAX_DEMOD_BUFFER_LENGTH);
		nAvrLen02 = 51 * nNFFT / (HF_MAX_DEMOD_BUFFER_LENGTH);
		nAvrDelay01 = (nAvrLen01 - 1) / 2;
		nAvrDelay02 = (nAvrLen02 - 1) / 2;
	}

}
