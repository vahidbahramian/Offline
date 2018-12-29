#include "stdafx.h"
#include "dsplib_demod_am.h"

namespace NS_DSP
{
	//----------double precision-------------------------
	//---------------------------------------------------
	CAMvmld::CAMvmld()
	{
		m_cHilbertOut = NULL;
		m_nCoreSize = 0;
	}

	CAMvmld::~CAMvmld()
	{
		Release();
	}

	void CAMvmld::Reset()
	{

	}

	void CAMvmld::Release()
	{
		if(m_cHilbertOut) delete[] m_cHilbertOut;
		m_cHilbertOut = NULL;
	}

	BOOL CAMvmld::Create(int nCoreSize)
	{
		int ret ;
		m_nCoreSize = nCoreSize;
		m_cHilbertOut = new MKL_Complex16[m_nCoreSize*2];
		if(!m_cHilbertOut){ Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		ret = m_hilbert.Create(m_nCoreSize);
		if(ret != DSP_RET_SUCCESS)
		{
			Release(); return ret;
		}
		return DSP_RET_SUCCESS;
	}

	//y = abs(hilbert(x))
	BOOL CAMvmld::Demodulate(double *m_fInput, int nSizeInput, double *m_fOutput, int &nSizeOutput)
	{
		int ret,m_nHilbert;
		int i=0,blkNum = nSizeInput/m_nCoreSize;
		nSizeOutput = 0;
		for(i=0;i<blkNum ;i++)
		{
			ret = m_hilbert.Filter(m_fInput+i*m_nCoreSize,m_nCoreSize,m_cHilbertOut,
				m_nHilbert);
			if(ret != DSP_RET_SUCCESS) return ret;
			vzAbs(m_nHilbert,m_cHilbertOut,m_fOutput + nSizeOutput);
			nSizeOutput += m_nHilbert;
		}
		ret = m_hilbert.Filter(m_fInput+i*m_nCoreSize,
			(nSizeInput - i*m_nCoreSize),m_cHilbertOut,m_nHilbert);
		if(ret != DSP_RET_SUCCESS) return ret;
		vzAbs(m_nHilbert,m_cHilbertOut,m_fOutput + nSizeOutput);
		nSizeOutput += m_nHilbert;
		return DSP_RET_SUCCESS;
	}

	//----------single precision-------------------------
	//---------------------------------------------------
	CAMvmls::CAMvmls()
	{
		m_cHilbertOut = NULL;
		m_nCoreSize = 0;
	}

	CAMvmls::~CAMvmls()
	{
		Release();
	}

	void CAMvmls::Reset()
	{

	}

	void CAMvmls::Release()
	{
		if(m_cHilbertOut) delete[] m_cHilbertOut;
		m_cHilbertOut = NULL;
	}

	BOOL CAMvmls::Create(int nCoreSize)
	{
		int ret ;
		m_nCoreSize = nCoreSize;
		m_cHilbertOut = new MKL_Complex8[m_nCoreSize*2];
		if(!m_cHilbertOut){ Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
		ret = m_hilbert.Create(m_nCoreSize);
		if(ret != DSP_RET_SUCCESS)
		{
			Release(); return ret;
		}
		return DSP_RET_SUCCESS;
	}

	//y = abs(hilbert(x))
	BOOL CAMvmls::Demodulate(float *m_fInput, int nSizeInput, float *m_fOutput, int &nSizeOutput)
	{
		int ret,m_nHilbert;
		int i=0,blkNum = nSizeInput/m_nCoreSize;
		nSizeOutput = 0;
		for(i=0;i<blkNum ;i++)
		{
			ret = m_hilbert.Filter(m_fInput+i*m_nCoreSize,m_nCoreSize,m_cHilbertOut,
				m_nHilbert);
			if(ret != DSP_RET_SUCCESS) return ret;
			vcAbs(m_nHilbert,m_cHilbertOut,m_fOutput + nSizeOutput);
			nSizeOutput += m_nHilbert;
		}
		ret = m_hilbert.Filter(m_fInput+i*m_nCoreSize,
			(nSizeInput - i*m_nCoreSize),m_cHilbertOut,m_nHilbert);
		if(ret != DSP_RET_SUCCESS) return ret;
		vcAbs(m_nHilbert,m_cHilbertOut,m_fOutput + nSizeOutput);
		nSizeOutput += m_nHilbert;
		return DSP_RET_SUCCESS;
	}

}//namespace