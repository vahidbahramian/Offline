#pragma once
#include "dsplib_general_func.h"

namespace NS_DSP
{
	class CAnalyzeChannel 
	{
	public:
		CAnalyzeChannel(void);
		~CAnalyzeChannel(void);

	public:
		static FILTER_COEFS	m_stCoefsBuffer;

		BOOL	Create();
		void	Release();
		void	EstimationCarrier(int nNFFT,double* dblF,int &nValidIdxStart,
			int &nValidIdxEnd,int &nAvrLen01,int &nAvrLen02,int &nAvrDelay01,int &nAvrDelay02,
			double *fInput,double &fFcEstimation,double &fBW,double &fSNR);
		void	InitEstimationCarrier(double fFs,int &nNFFT,double* dblF,int &nValidIdxStart,
			int &nValidIdxEnd,int &nAvrLen01,int &nAvrLen02,int &nAvrDelay01,int &nAvrDelay02);
		static BOOL	LoadFiltersCoefficients(CString strMainAddr);
		static void    UnLoadFiltersCoefficients();
		BOOL	LoadedFIRCoef();

		int		m_fBandStartIdx;
		int		m_fBandEndIdx;
		double	m_fFc;
		double	*m_fSTemp;
		double	*m_fA2Smoothed02;
		double	*m_fSTempAssist;
		MKL_Complex16 *m_cOutFFT;
		MKL_Complex16 *m_cOutConjFFT;
		DFTI_DESCRIPTOR_HANDLE m_pFFTDesc ;



	};
}
