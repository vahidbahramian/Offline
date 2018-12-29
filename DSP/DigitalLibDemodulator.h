#pragma once
#include "AnalyzeChannel.h"
#include "NewRsEstimation.h"
#include "func_interv.h"
#include "mkl_vml.h"
#include "mkl_types.h"
#include "mkl_cblas.h"
#include "mkl_dfti.h"
#include <Windows.h>
#define  _USE_MATH_DEFINES
#include <math.h>
#include "dsplib_general_func.h"
#include "../DemodulatorBase.h"
#include "AnalyzeChannel.h"

/*
#define ABS(x)		   ((x)>0 ? (x):-(x))
#define myMax(x,y)     ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define myMin(x,y)	   ( ( x ) < ( y ) ? ( x ) : ( y ) )
#define SIGN(x)		   ((x)>0 ? (1):(-1))
#define LOG2(x) (log((double)x)/log(2.0f))
*/

//#define HF_DIGITAL_FS				160e6//80e3
#define HF_DIGITAL_SPS_Target		8
#define HF_DIGITAL_SPS_Target_Rs	6
#define HF_DIGITAL_SPS_est			8.33
#define HF_DIGITAL_ALPHABET			2
#define HF_DIGITAL_ALPHABET_QPSK	4
#define HF_DIGITAL_ALPHABET_QAM16	16
#define HF_DIGITAL_ALPHABET_QAM32   32
#define HF_DIGITAL_ALPHABET_QAM64	64
#define HF_DIGITAL_ALPHABET_8PSK	8
#define HF_DIGITAL_CIC_M			2
#define HF_DIGITAL_CIC_N			3
#define HF_DIGITAL_P_GAIN_FACTOR	0.043
#define HF_DIGITAL_SPS_CORR_TSCO	0.999
#define HF_Fc_ESTIMATION_IT			10
#define HF_DIGITAL_BW_TO_RS_COEF	1.03
//inline long round(double x)		    	{ return long(floor(x+0.5)); }


namespace NS_DSP
{
	//class CDemodulatorBase;

	class CDigitalLibDemodulator : public CAnalyzeChannel //, public CDemodulatorBase
	{
	public:
		CDigitalLibDemodulator(void);
		~CDigitalLibDemodulator(void);

	public:
		BOOL	Create(DEMOD_PARAM stParam);
		void	Reset();
		void	Release();
		int		Remainder(int X,int Y);
		void	CumsumInt64(__int64 nPrimaryVal,__int64 *input,__int64 *Output,int nSize);
		void	CumsumDouble(double nPrimaryVal,double *input,double *Output,int nSize);
		void	Integrators(int nBufferLen,double *fInputBuffer,double *fInputDecimator,__int64 *fOutput,__int64 &nEndCIC01,__int64 &nEndCIC02,__int64 &nEndCIC03);
		void    IntegratorsQam(int nBufferLen,double *fInputBuffer,double *fInputDecimator,double *fOutput,double &nEndCIC01,double &nEndCIC02,double &nEndCIC03);
		void    IntegratorsQamother(int nBufferLen,double *fInputBuffer,double *fInputDecimator,double *fOutput,double &nEndCIC01,double &nEndCIC02,double &nEndCIC03);
		void	FirstFrameCIC(int nLength,__int64 *input,int nDownSampleRate,double *Output,double CICGain,__int64 *CICR,__int64 *CICC01,__int64 *CICC02,int &nBufferSize);
		void    FirstFrameCICDouble(int nLength,double *input,int nDownSampleRate,double *Output,double CICGain,double *CICR,double *CICC01,double *CICC02,int &nBufferSize);
		void	OtherFrameCIC(int nLength,__int64 *input,int nDownSampleRate,int nStartIndex,double *OutputBuffer,double CICGain,__int64 *CICR,__int64 *CICC01,__int64 *CICC02,int &nBufferSize);
		void    OtherFrameCICDouble(int nLength,double *input,int nDownSampleRate,int nStartIndex,double *OutputBuffer,double CICGain,double *CICR,double *CICC01,double *CICC02,int &nBufferSize);
		void	CaptureSamples(double *input,double *Output,int nLenght,int nCoefSize,BOOL &bStartDemod);
		void	CaptureSamples(double *input,double *Output,int nLenght,BOOL &bStartDemod);
		void    CaptureSamples(double *input,double *Output,int nLenght);
		void	CalculateAnalyzeItems(BOOL bisInitEst,double *m_fOutFilterBuffer,int nTotalSize,
			BOOL &m_bDoStmationStat1,double &m_fFcEstimation,double &dblFcEst,double &fBW,double &fSNR );
 		void	InterpretCardBuffer (DWORD *m_pInBuffer,int nSize);
		BOOL	FirstFrameInput(double fDF,double fRs,double fFcEstimation,
			double *fOutFilterBuffer,int nInputFilterOutBufferLength,double *fSI01,double *fSQ01);
		BOOL	OtherFramesInput(double fDF,double fRs,double fFcEstimation,
			double *fOutFilterBuffer,int nInputFilterOutBufferLength,double *fSI01,double *fSQ01);
		void	CalcRsEstimation(double fDF,double *m_pInBuffer,int nSize,
			BOOL bCalcFcEstimation,BOOL bCalcRsEstimation,BOOL bCalcBW,BOOL &bRsEst,
			double &fFcEst,double &fBW,double &fSNR,double &fRs,BOOL bIsBFSKFlag);
		void ProcessData(double fDF,double fRs,double fFcEstimation,
			double *fOutFilterBuffer,int nInputFilterOutBufferLength,double *fSI01,double *fSQ01);

		static BOOL		LoadFiltersCoefficients(CString strMainAddr);
		static void		UnLoadFiltersCoefficients();
		BOOL	IsLoadedFIRCoef();
		int	GetSampleID(DWORD m_dwSample);

	public:
		static FILTER_COEFS	m_stLPFCoefBuffer;
		static FILTER_COEFS	m_stBPFCoefBuffer;
		static FILTER_COEFS m_stMatchedFilter;

		CFIROLAvmld			m_InputFilter;	
		CFIROLAvmld			m_InputFilter1;	
		CFIROLAvmld			m_InputFilter2;
	public:	
		CNewRsEstimation		*m_pWndRsEstimation;

		double	*m_fBuffer;
		__int64	*m_nSTemp;
		__int64 *m_nBufferTemp;
		__int64 *m_nBufferTempAssist;
		double	*m_dSTemp;
		double *m_dBufferTemp;
		double *m_dBufferTempAssist;

	public:

		BOOL	m_bIfInitEst;
		BOOL	m_bStartDemod;
		BOOL	m_bStartRsEst;
		BOOL	m_bDoStmationStat1;
		BOOL	m_bIsFirstFrame;
		BOOL	m_bCalcFcEstimatin;
		BOOL	m_bCalcRsEstimation;

		double	m_fFs;
		double	m_fFc;
		double  m_nID;
		double	m_fBW;
		double  m_fSNR;
		double  m_fFcEstimation;
		double  m_fCICGain;
		double	m_fPhi01EndStr;
		double	*m_dblF;
		//double	*m_fInputBuffer;
		//double	*m_fInputFilterOutBuffer;
		double  *m_fOutFilterBuffer;
		double	*m_fRealInp;
		double	*m_fImgInp;
		double	*m_fRealMInput;
		double	*m_fImgMInput;
		double * m_fPhiIncTerm01;
		double * m_fPhi01;
		double * m_fLoI01;
		double * m_fLoQ01;
		double * m_dblTempBuffer;
		double * m_fBuffer1;
		double * m_fBuffer2;

		__int64 * m_fBufferTemp ;
		__int64 * m_fCICRI01Save;
		__int64 * m_fCICC01I01Save;
		__int64 * m_fCICC02I01Save;
		__int64 * m_fCICRQ01Save;
		__int64 * m_fCICC01Q01Save;
		__int64 * m_fCICC02Q01Save;

		__int64 m_nEndCICI01I01;
		__int64 m_nEndCICI02I01;
		__int64 m_nEndCICI03I01;
		__int64 m_nEndCICI01Q01;
		__int64 m_nEndCICI02Q01;
		__int64 m_nEndCICI03Q01;

		int		 m_nInputFilterOutBufferLength;
		int		 m_nLenghtBuffer;
		int		 nCICDecimatorLenght;
		int		 m_nInputBufferLength;
		int		 m_nValidIdxStart;
		int		 m_nValidIdxEnd;
		int		 m_nAvrLen01;
		int		 m_nAvrLen02;
		int		 m_nAvrDelay01;
		int		 m_nAvrDelay02;
		int		 m_nNFFT;
		int		 m_nTotalSize;
		int		 m_nDownSampleRate;
		int		 m_nSI01Lenght;
		int		 m_nSQ01Lenght;
		int		 m_nLenghtBuffer1;
		int		 m_nLenghtBuffer2;
		int		 m_nRem;

		double   m_fRsEstimation;//****ezafe shavad

		/////
		//double * fInp;
	};
}

