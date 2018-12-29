#include "stdafx.h"
#include "dsplib_amr_module.h"

namespace NS_DSP
{
	CAMRModuleBase::CAMRModuleBase()
	{
		//return;
		m_pAMR		=	NULL;
		m_nAMRState = AMR_STATE_IDLE;//state variable
		m_bIsValidBufferOverThreshold=FALSE;//is true if a valid buffer has been captured
		m_fAMRBufferOverThreshold=NULL;//buffer for valid samples (if captured)
		m_nNumberAMRSamplesOverThreshold=0;//size of m_fAMRBufferOverThreshold

		m_fBuffer=NULL; // 
		m_fBufferCopy = NULL;
		m_nAMRState = 6;
		m_fPowerEstimationThreshold=5e-6;
		m_nPowerEstimationCntr=0; //counter for power estimation
		m_nMaxPowerEstimationCntr=5;//number power estimations for decision 
		m_bAMRPowerResult=NULL; //results of power estimation checks

		m_bArrValidChunk	= NULL;
		m_bArrValidChunkCopy= NULL;
		m_nSize=0;//maximum size of buufers		
		m_fFs=0;
		Reset();
	}

	CAMRModuleBase::~CAMRModuleBase()
	{
		Release();
	}

	void CAMRModuleBase::Reset()
	{
		m_nAMRState = AMR_STATE_IDLE;//state variable
		m_bIsValidBufferOverThreshold=FALSE;//is true if a valid buffer has been captured
		m_nNumberAMRSamplesOverThreshold=0;//size of m_fAMRBufferOverThreshold

		m_nPowerEstimationCntr=0; //counter for power estimation
	}

	void CAMRModuleBase::Release()
	{
		if(m_pAMR)
		{
			m_pAMR->Release();
			delete m_pAMR;
		}
		m_pAMR = NULL;
		if(m_bAMRPowerResult) delete[] m_bAMRPowerResult;
		if (m_fAMRBufferOverThreshold) delete[] m_fAMRBufferOverThreshold;
		if(m_fBuffer) delete[] m_fBuffer;
		if(m_fBufferCopy) delete[] m_fBufferCopy;
		if (m_bArrValidChunk) delete[] m_bArrValidChunk;
		if (m_bArrValidChunkCopy) delete[] m_bArrValidChunkCopy;
		m_bAMRPowerResult=NULL;
		m_fAMRBufferOverThreshold = NULL;
		m_fBuffer=NULL;
		m_fBufferCopy=NULL;
		m_bArrValidChunk=0;
		m_bArrValidChunkCopy=0;
	}

	BOOL CAMRModuleBase::Create(int nSize,int nTimes,double m_fThreshold,double fs)
	{
		m_nSize = nSize;
		m_fFs = fs;
		m_nMaxPowerEstimationCntr = nTimes;
		m_fPowerEstimationThreshold=m_fThreshold;
		m_bAMRPowerResult = new BOOL[m_nMaxPowerEstimationCntr];
		if(!m_bAMRPowerResult)
		{
			Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		m_fAMRBufferOverThreshold = new double[m_nSize];
		if(!m_fAMRBufferOverThreshold)
		{
			Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		m_fBuffer= new double[m_nSize];
		if(!m_fBuffer)
		{
			Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		m_fBufferCopy= new double[m_nSize];
		if(!m_fBufferCopy)
		{
			Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		m_pAMR = new NS_DSP::CHFAnalogAMRvmld;
		if (!m_pAMR)
		{
			Release();return FALSE;
		}
		m_nBlocks = m_nSize / 8192;
		m_bArrValidChunk = new BOOL[m_nBlocks];
		if (!m_bArrValidChunk)
		{
			Release();return FALSE;
		}
		m_bArrValidChunkCopy = new BOOL[m_nBlocks];
		if (!m_bArrValidChunkCopy)
		{
			Release();return FALSE;
		}
		BOOL ret = m_pAMR->Initialize(8192);
		if(ret != DSP_RET_SUCCESS) {
			Release();
			return ret;
		}
		return DSP_RET_SUCCESS;
	}

	//this virtual func must be overridden by card read process
	BOOL CAMRModuleBase::CaptureData(double *m_fSamples,int &nOutSize)
	{
		//null read proc
		memset(m_fSamples,0,m_nSize*sizeof(double));
		nOutSize = m_nSize ;
		return DSP_RET_SUCCESS;
	}

	//retValue values:
	//AMR_PROC_IN_PROCESS : this function must be called later 
	//AMR_PROC_ERROR      : error in process
	//AMR_PROC_TERMINATE  : the process has been terminated : modType = modulation 
	int CAMRModuleBase::AMRProcedure(int &retValue,int &modType)
	{
		int m_nBufferSize , ret=DSP_RET_SUCCESS,i ,Result;
		m_nBufferSize = m_nSize;
		int nInvalidChunks=0;
		modType = AMR_UNKNOWN;//default
		retValue = AMR_PROC_IN_PROCESS; 

		//amr state machine
		switch(m_nAMRState)
		{
		case AMR_STATE_IDLE:
			m_nPowerEstimationCntr=0;
			//wait for amr request 
			m_nAMRState = AMR_STATE_POWER_ESTIMATION;
			m_bIsValidBufferOverThreshold = FALSE;
			m_nMinNumberInvalidChunks=m_nBlocks;
			nInvalidChunks=0;
			break;

		case AMR_STATE_POWER_ESTIMATION:
		case AMR_STATE_SSB:
			{
				if(m_nPowerEstimationCntr == m_nMaxPowerEstimationCntr)
				{//decision after m_nMaxPowerEstimationCntr times sampling
					m_nAMRState = AMR_STATE_DIGITAL;
#ifdef _DEBUG
					afxDump << "------AMR Power Results---- \n";
					for (i=0;i<m_nMaxPowerEstimationCntr;i++)
					{
						afxDump << "Power"<<i<<"\t";
						switch(m_bAMRPowerResult[i] )
						{
						case AMR_POWER_COMPARISON_VALID: afxDump << "valid\n";break;
						case AMR_POWER_COMPARISON_INVALID: afxDump << "invalid\n";break;
						case AMR_POWER_COMPARISON_HIGH_VARIATION: afxDump << "high dev\n";break;
						}
					}
					afxDump << "number invalid chunks = " << m_nMinNumberInvalidChunks<<"\n";
#endif
					for(i=0;i<m_nMaxPowerEstimationCntr;i++){
						if(m_bAMRPowerResult[i] != AMR_POWER_COMPARISON_VALID)
						{
							m_nAMRState = AMR_STATE_SSB;
							if(m_bIsValidBufferOverThreshold==TRUE)
							{
								m_nAMRState = AMR_STATE_IDLE;
								ret = m_pAMR->DoAMR(m_fAMRBufferOverThreshold,m_fFs,
									m_nBufferSize,m_bArrValidChunk,m_nBlocks,modType);
								if(ret != DSP_RET_SUCCESS)
									retValue = AMR_PROC_ERROR;
								else
									retValue = AMR_PROC_TERMINATE; 
								return ret;
							}
						}
					}
					m_nPowerEstimationCntr = 0;//reset cntr
				}
				else
				{//sample for power estimation 
					ret = CaptureData(m_fBuffer,m_nBufferSize);
					m_nBufferSize = m_nSize;
					if (ret == DSP_RET_ABORT) {
						retValue = AMR_PROC_ABORT;
						return ret;
					}
					else if(ret != DSP_RET_SUCCESS){
						retValue = AMR_PROC_ERROR;
						return ret;
					}
					memcpy(m_fBufferCopy,m_fBuffer,m_nBufferSize*sizeof(double));
					ret =IsValidAMRSignal(
						m_fBufferCopy,m_nBufferSize,m_fPowerEstimationThreshold,
						m_bArrValidChunk,nInvalidChunks,Result);//5e-6 for amr tests
					if(ret != DSP_RET_SUCCESS) 
					{
						retValue = AMR_PROC_ERROR;
						return ret;
					}
					m_bAMRPowerResult[m_nPowerEstimationCntr] = Result;
					if (m_bAMRPowerResult[m_nPowerEstimationCntr]!=AMR_POWER_COMPARISON_INVALID)
					{
						m_bIsValidBufferOverThreshold = TRUE;
						//copy buffer
						if (m_nMinNumberInvalidChunks > nInvalidChunks)
						{
							memcpy(m_fAMRBufferOverThreshold,m_fBuffer,m_nBufferSize*sizeof(double));
							memcpy(m_bArrValidChunkCopy,m_bArrValidChunk,m_nBlocks*sizeof(BOOL));
							m_nMinNumberInvalidChunks=nInvalidChunks;
						}
					}
					/*if(m_nAMRState == AMR_STATE_SSB)
					{
					m_nAMRState = AMR_STATE_IDLE;
					ret = m_pAMR->DoAMR(m_fAMRBufferOverThreshold,m_fFs,
					m_nBufferSize,m_bArrValidChunk,m_nBlocks,modType);
					if(ret != DSP_RET_SUCCESS) retValue=AMR_PROC_ERROR;
					else retValue = AMR_PROC_TERMINATE; 
					return ret;
					}*/
					//if(m_nAMRState == AMR_STATE_POWER_ESTIMATION){
					m_nPowerEstimationCntr++;
					//}else{
					//	m_nPowerEstimationCntr=0;
					//}
				}//m_nPowerEstimationCntr

			}
			break;

		case AMR_STATE_DIGITAL:
			{
				ret = m_pAMR->DoAMR(m_fAMRBufferOverThreshold,m_fFs,
					m_nBufferSize,m_bArrValidChunk,m_nBlocks,modType);
				if(ret != DSP_RET_SUCCESS) retValue = AMR_PROC_ERROR;
				else retValue=AMR_PROC_TERMINATE;
				switch(modType)
				{
				case AMR_ANALOG_AM:modType = AMR_ANALOG_AM;break;
				case AMR_ANALOG_FM:modType = AMR_ANALOG_FM;break;
				default:modType = AMR_DIGITAL;break;
				}
				m_nAMRState = AMR_STATE_IDLE;
			}
			break;

		}//switch
		return ret;
	}//func

}//namespace