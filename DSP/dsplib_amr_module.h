#pragma once
/*
AMR Module :
A procedure for modulation identification
*/

#include "dsplib_amr_analog_hf.h"

//AMR State Machine 
#define  AMR_STATE_IDLE 0
#define  AMR_STATE_POWER_ESTIMATION 1
#define AMR_STATE_SSB 2
#define AMR_STATE_DIGITAL 3
#define AMR_STATE_UPDATE_FFT 4


#define AMR_PROC_IN_PROCESS 0
#define AMR_PROC_ERROR      1
#define AMR_PROC_TERMINATE  2
#define AMR_PROC_ABORT		3

#define AMR_POWER_COMPARISON_INVALID 0 
#define AMR_POWER_COMPARISON_VALID 1 
#define AMR_POWER_COMPARISON_HIGH_VARIATION 2 

namespace NS_DSP
{

	class CAMRModuleBase
	{
	public:
		CAMRModuleBase();
		~CAMRModuleBase();

		void Reset();
		void Release();
		BOOL Create(int nSize,int nTimes,double m_fThreshold,double fs);
		//virtual function : this func must be overridden by card read procedure

		int AMRProcedure(int &retValue,int &modType);
		virtual BOOL CaptureData(double *m_fSamples,int &m_nSize);

	public:
		double m_fPowerEstimationThreshold;
		double m_fFs;
		double *m_fBuffer; // 
		double *m_fBufferCopy; // for use in power estimation
		BOOL m_nPowerEstimationCntr; //counter for power estimation
		BOOL m_nMaxPowerEstimationCntr; //number power estimations for decision 
		BOOL *m_bAMRPowerResult; //results of power estimation checks

		int m_nAMRState;//state variable
		BOOL m_bIsValidBufferOverThreshold;//is true if a valid buffer has been captured
		double *m_fAMRBufferOverThreshold;//buffer for valid samples (if captured)
		int m_nNumberAMRSamplesOverThreshold;//size of m_fAMRBufferOverThreshold
		BOOL *m_bArrValidChunk;
		BOOL *m_bArrValidChunkCopy;
		int m_nSize;//maximum size of buufers
		int m_nBlocks;
		int m_nMinNumberInvalidChunks;

		//CHFAnalogAMRvmld *m_pAMR;
		NS_DSP::CHFAnalogAMRvmld *m_pAMR;
	};

}//namespace
