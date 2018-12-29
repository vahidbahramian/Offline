#pragma once
#include "Cdsplib_amr_analog.h"
#include "MovingAverage.h"
/*
AMR Module :
A procedure for modulation identification
*/
//#define AMR_ANALOG_ERROR 0
//#define AMR_ANALOG_AM 1
//#define AMR_ANALOG_FM 2
//#define AMR_ANALOG_DSB 3
//#define AMR_ANALOG_USB 4
//#define AMR_ANALOG_LSB 5
//#define AMR_ANALOG_UNKNOWN 6
//#define AMR_DIGITAL 7
//#define AMR_UNKNOWN 8
//#define AMR_ANALOG_SSB 9
#define AMR_ANALOG_NO_SIGNAL 0
#define AMR_ANALOG_AMFM_AM 1
#define AMR_ANALOG_AMFM_FM 2
#define AMR_ANALOG_AM 3
#define AMR_ANALOG_FM 4
#define AMR_ANALOG_DSB 5
#define AMR_ANALOG_SSB 6
#define AMR_ANALOG_TYPE_COUNT 7

#define HF_AMR_ANALOG_Fs 80e3
#define HF_AMR_ANALOG_subMinLen  5
#define HF_AMR_ANALOG_BandStartIdx round(10e3*HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_Fs)
#define HF_AMR_ANALOG_BandStopIdx round(30e3*HF_AMR_ANALOG_FRAME_SIZE/HF_AMR_ANALOG_Fs)
#define HF_AMR_ANALOG_TotalIdx (HF_AMR_ANALOG_BandStopIdx - HF_AMR_ANALOG_BandStartIdx + 1)

#define HF_NUMBER_ANALOG_AMR_MODULATIONS 7
#define myMax(x,y)     ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define myMin(x,y)	   ( ( x ) < ( y ) ? ( x ) : ( y ) )
#define ABS(x)		   ((x)>0 ? (x):-(x))

namespace NS_DSP
{

	class CHFAMRModuleBase
	{
	public:
		CHFAMRModuleBase();
		~CHFAMRModuleBase();

		void Reset();
		void Release();
		BOOL Create(int nSize);
		//virtual function : this func must be overridden by card read procedure
		void InitAMRParam();
		int Execute(void);
		double CalcMed(int nMinRange,int nMaxRange,double *input);
		void PreExecute(void);
		inline long round(double x)	{ return long(floor(x+0.5));}
		//inline BOOL IsFinished() {return (m_nSignalFramesNo >= 20) ? TRUE : FALSE;}
		inline BOOL IsFinished() {return (m_nSignalFramesNo >= 20) ? TRUE : FALSE;}

	public:

		 //double		*m_fBuffer; 
		 int		m_nSize;//maximum size of buffers
		 int		m_nNumberAMRSamples;
		 int		nAMRSampleCounter;
		//Constant Values for New AMR routine.
		int			m_nHilbertSize;
		double		m_nFrameSize;
		int         m_nSignalFramesNo;
		double		m_nSTDPhiNLTh;
		double	    m_fPMonHalfLen;
		double		m_fPMon1stHalfEng;
		double		m_fPMon2ndHalfEng;

		double		m_CarrierEngCo;

		int			m_nGammaMaxTh;
		int			m_nCarrierPowerTh01;
		double		m_nCarrierEngTh;
		double		m_nMeanWin;
		double		m_nTsco01;   // smoothing coefficient
		double		m_nTscoNoiseEst;    // smoothing coefficient for the final noise estimation
		int			m_nBETAmin;    // minima tracking bias compensation factor
		int			m_nOsc;        // over-subtraction coefficient
		int			m_nBiasCF;  // final bias compensation factor
		int			m_nSubMinLen;
		double		m_nMinDuration; // Minima search length in seconds
		double		m_dblMx; // Mean of Index01(24..4096)
		double		m_fPMonEng;
		BOOL		m_bSendMod;

		//Indexes for shift values.
		int nAddIdx03;
		int nAddIdx04;
		int	nAddIdx05;

		double		*m_fInputBuffer;
		int			m_nInputBufferLength;
		double		*m_dblA2FFTSmoothed;
		double		*m_dblNoisePSD;
		double		*m_dblF;
		double		*m_dblMinimaA2SubSeg;
		double		*m_fMinimaA2;
		double		*m_dblPhiNL;
		double		*m_dblNoisePSDMed;
		//double		*m_dblNiosePSDAve;
		double		*m_fEnv;
		double		*m_dblFav;
		double		*m_dblFavMed;
		double		*m_fCounts;
		int			m_nTotalCounts;
		int			m_nMinLen;
		int			m_nSubMinSegs;
		int			m_nSubSegIndex;
		int			m_nNumFrames;
		int			m_nModeType;
		//int			m_nSignalFrameNo;

 		double		*m_fPhi;
		double		m_fLastVal;
 		double		*m_fDphiCorr;
 		double		*m_fDphiTemp;
 		double		*m_fOutSum;
		double		*m_fCopyInputBuffer;
		double		*m_fInputNS;
		double		*m_fTemp;

		double		*m_fFIROutBuffer;
		double		*m_fHilbertOutBuffer;
		double		*m_fDelayOutBuffer;
		double		*m_fInputCopy;
		int			m_nFIROutBufferLength;
		int			m_nHilbertOutBufferLenght;
		int			m_nTotalNo;
		int			m_nIdxSig;
		int			m_nModulationCounter[9];

		BOOL		m_bFirstTime;
		int			m_nNoiseEstSamplesCntr;
		int			nLenInputBuffer;

    	NS_DSP::CHFAMRAnalog *m_pAMR;
		CMovingAverage<double> m_objMA7,m_objMA11;

	};

}//namespace
