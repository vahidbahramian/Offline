#include "StdAfx.h"
#include "stdlib.h"
#include "Cdsplib_amr_module_base.h"
#include "dsplib_debug.h"


namespace NS_DSP
{
	CHFAMRModuleBase::CHFAMRModuleBase()
	{
		//define primary values 
		//Constant Values for New AMR routine.
		//m_nSignalFrameNo=0;
		m_nTotalNo = 0;
		m_nIdxSig = 0;
		//m_nHilbertSize = m_stHilbertCoefBuffer.m_nSize - 1;
		m_nFrameSize = 0.0512*80e3;
		m_nSignalFramesNo = 0;
		m_nSTDPhiNLTh = 0.5;
		m_nGammaMaxTh = 4;
		m_nCarrierPowerTh01 = 80;
		m_nCarrierEngTh = 0.75;
		m_nMeanWin = 0.4999;
		m_nTsco01 = 0.3;   // smoothing coefficient
		m_nTscoNoiseEst = 0.8;    // smoothing coefficient for the final noise estimation
		m_nBETAmin = 4;    // minima tracking bias compensation factor
		m_nOsc = 4;        // over-subtraction coefficient
		m_nBiasCF = 3;  // final bias compensation factor
		m_nSubMinLen = 2;
		m_nMinDuration = 1.7; // Minima search length in seconds
		m_dblMx = double(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP+1)/2; // mean of Index01(1...4096)
		m_nModeType=6;
		m_fPMonHalfLen = 0.0;
		m_fPMon1stHalfEng = 0.0;
		m_fPMon2ndHalfEng = 0.0;
		m_fPMonEng = 0.0;

		m_CarrierEngCo = 20;

		m_nTotalCounts = 0;

		//Variables for New AMR routine
		m_fInputCopy = NULL;
		m_dblA2FFTSmoothed = NULL;
		m_dblNoisePSD = NULL;
		m_dblF = NULL;
		m_fPhi=NULL;
		m_fEnv=NULL;
		m_dblMinimaA2SubSeg = NULL;
		m_fMinimaA2 = NULL;
		m_nMinLen = 0;
		m_nSubMinSegs = 0;
		m_nSubSegIndex = 1;
		m_fDphiTemp=NULL;
		m_fDphiCorr=NULL;
		m_fOutSum = NULL;
		m_dblPhiNL = NULL;
		m_dblNoisePSDMed = NULL;
 		//m_dblNiosePSDAve = NULL;
		m_dblFav = NULL;
		m_fCounts=NULL;
		m_fFIROutBuffer=NULL;
		m_fHilbertOutBuffer=NULL;
		m_fDelayOutBuffer=NULL;
		m_nFIROutBufferLength=0;
		m_nHilbertOutBufferLenght=0;
		m_fInputBuffer	=	NULL;
		m_fCopyInputBuffer	=NULL;
		m_fInputNS = NULL;
		m_fTemp = NULL;
		m_dblFavMed = NULL;

		//Indexes for shift values
		nAddIdx03=HF_AMR_ANALOG_OVERLAP-1;
		nAddIdx04=HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP;
		nAddIdx05=HF_AMR_ANALOG_FRAME_SIZE-1;
		//m_pAMR = NULL;

		m_bFirstTime = TRUE;
		m_nNoiseEstSamplesCntr=0;
		m_nNumberAMRSamples=0;
		nAMRSampleCounter=0;
		m_bSendMod = FALSE;

 		m_pAMR		=	NULL;

		//Reset();
	}

	CHFAMRModuleBase::~CHFAMRModuleBase()
	{
		Release();
	}

	void CHFAMRModuleBase::Reset()
	{
		int i;
		m_nModeType = 6;
		for (int i=0;i<=HF_NUMBER_ANALOG_AMR_MODULATIONS;i++)
		{
			m_nModulationCounter[i]=0;
		}
		if (m_fDelayOutBuffer) 
			memset(m_fDelayOutBuffer,0,(3*HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP)*sizeof(double));
		m_fDphiTemp[0]=0;
		if(m_dblPhiNL)
			memset(m_dblPhiNL,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		m_bFirstTime = TRUE;
		m_nNoiseEstSamplesCntr=0;
		m_objMA7.Reset();
		m_objMA11.Reset();
		nAMRSampleCounter=0;
		m_nSignalFramesNo = 0;
		m_nIdxSig=0;
 		m_dblMx	=	double(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP+1)/2; // Mean of Index01(24..4096)

		////Indexes for shift values.
		nAddIdx03	=	0;
		nAddIdx04	=	0;
		nAddIdx05	=	0;

		m_fLastVal		=	0;
		memset(m_fDphiCorr,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		memset(m_fDphiTemp,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		memset(m_fOutSum,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		memset(m_fCounts,0,7*sizeof(double));

		m_nTotalNo		=	0;
		m_nIdxSig		=	0;
		for (i=0;i<9;i++) m_nModulationCounter[i];
		m_bFirstTime	=	TRUE;
		m_nNoiseEstSamplesCntr	=	0;
		nLenInputBuffer	=	0;	
		
	}

	void CHFAMRModuleBase::Release()
	{
		SAFERELEASE_ARRAY(m_fInputBuffer);
		SAFERELEASE_ARRAY(m_fInputCopy);
		SAFERELEASE_ARRAY(m_dblA2FFTSmoothed);
		SAFERELEASE_ARRAY(m_dblNoisePSD);
		SAFERELEASE_ARRAY(m_dblF);
		SAFERELEASE_ARRAY(m_dblMinimaA2SubSeg);
		SAFERELEASE_ARRAY(m_fMinimaA2);
		SAFERELEASE_ARRAY(m_fPhi);
		SAFERELEASE_ARRAY(m_fDphiTemp);
		SAFERELEASE_ARRAY(m_fDphiCorr);
		SAFERELEASE_ARRAY(m_fOutSum);
		SAFERELEASE_ARRAY(m_dblPhiNL);
		SAFERELEASE_ARRAY(m_dblNoisePSDMed);
		SAFERELEASE_ARRAY(m_fEnv);
		SAFERELEASE_ARRAY(m_dblFav);
		SAFERELEASE_ARRAY(m_fCounts);
		SAFERELEASE_ARRAY(m_fFIROutBuffer);
		SAFERELEASE_ARRAY(m_fHilbertOutBuffer);
		SAFERELEASE_ARRAY(m_fDelayOutBuffer);
		SAFERELEASE_ARRAY(m_fCopyInputBuffer);
		SAFERELEASE_ARRAY(m_fInputNS);
		SAFERELEASE_ARRAY(m_fTemp);
		//SAFERELEASE_ARRAY(m_dblNoisePSDMed);
		SAFERELEASE_ARRAY(m_dblFavMed);
 		//SAFERELEASE_ARRAY(m_fBuffer);
		//m_pAMR->Release();
		SAFERELEASE(m_pAMR);
		m_objMA7.Release();
		m_objMA11.Release();
		//m_pAMR->Release();
	}

	BOOL CHFAMRModuleBase::Create(int nSize)
	{
		m_nTotalCounts = 0;

		int nFftSize = HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP;
		m_fInputBuffer = new double[HF_MAX_DEMOD_BUFFER_LENGTH*4/*100*/];
		if (!m_fInputBuffer) {Release();return FALSE;}
		
		m_fCopyInputBuffer = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if(!m_fCopyInputBuffer){Release();return FALSE;}

		m_fTemp = new double[HF_AMR_ANALOG_FRAME_SIZE*2];
		if (!m_fTemp){Release();return FALSE;}

		//Copy of Input Buffer
		m_fInputCopy = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if(!m_fInputCopy){Release();return FALSE;}

		//input Noise Estimation
		m_fInputNS = new double[HF_MAX_DEMOD_BUFFER_LENGTH*4/*100*/];
		if (!m_fInputNS){Release();return FALSE;}

		//Demod output Buffer
		m_fFIROutBuffer = new double[HF_AMR_ANALOG_FRAME_SIZE*2];
		if (!m_fFIROutBuffer) {Release();return FALSE;}

		//Demod output Hilbert
		m_fHilbertOutBuffer = new double[3*HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_fHilbertOutBuffer) {Release();return FALSE;}

		m_fDelayOutBuffer = new double[3*HF_AMR_ANALOG_FRAME_SIZE];
		if (!m_fDelayOutBuffer){Release();return FALSE;}

		//a2_fft_smoothed
		m_dblA2FFTSmoothed = new double[HF_AMR_ANALOG_subMinLen*HF_AMR_ANALOG_TotalIdx];
		if (!m_dblA2FFTSmoothed){Release();return FALSE;}
		memset(m_dblA2FFTSmoothed,0,(HF_AMR_ANALOG_subMinLen*HF_AMR_ANALOG_TotalIdx)*sizeof(double));

		//NoisePSD
		m_dblNoisePSD = new double[3*HF_AMR_ANALOG_TotalIdx];
		if (!m_dblNoisePSD){Release();return FALSE;}
		memset(m_dblNoisePSD,0,3*HF_AMR_ANALOG_TotalIdx*sizeof(double));

		//NoisePSD Median
		m_dblNoisePSDMed = new double[HF_AMR_ANALOG_FRAME_SIZE/2+10];
		if (!m_dblNoisePSDMed){Release();return FALSE;}

		//f
		m_dblF=new double[HF_AMR_ANALOG_FRAME_SIZE/2+10];
		if (!m_dblF){Release();return FALSE;}

		//m_fMinimaA2
		m_fMinimaA2 = new double[3*HF_AMR_ANALOG_TotalIdx];
		if (!m_fMinimaA2){Release();return FALSE;}
		for (int i = 0;i<(3*HF_AMR_ANALOG_TotalIdx);i++)
		{
			m_fMinimaA2[i] = 1e12;
		}

		//minima_a2_subSeg
		m_dblMinimaA2SubSeg = new double[30*HF_AMR_ANALOG_TotalIdx];
		if (!m_dblMinimaA2SubSeg){Release();return FALSE;}
		memset(m_dblMinimaA2SubSeg,0,(28*HF_AMR_ANALOG_TotalIdx)*sizeof(double));

 		m_nSize = nSize;

		//dphi_temp
		m_fDphiTemp = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if (!m_fDphiTemp){Release();return FALSE;}

		//dphi_corr
		m_fDphiCorr = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if (!m_fDphiCorr){Release();return FALSE;}

		//Out of Cumsum Func.
		m_fOutSum = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if (!m_fOutSum)	{Release();return FALSE;}

		//phiNL
		m_dblPhiNL = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
		if (!m_dblPhiNL){ Release(); return FALSE;}

// 		//NoisePSD_med
// 		m_dblNoisePSDMed = new double[HF_AMR_ANALOG_FRAME_SIZE/2];
// 		if (!m_dblNoisePSDMed){Release();return FALSE;}

		//Envelope
		m_fEnv = new double[HF_AMR_ANALOG_FRAME_SIZE] ;
		if (!m_fEnv){Release(); return FALSE;}

		//f_av
		m_dblFav = new double[HF_AMR_ANALOG_f_av_med_len+10];
		if (!m_dblFav){Release(); return FALSE;}

		m_dblFavMed = new double[HF_AMR_ANALOG_TotalIdx+10];
		if (!m_dblFavMed)
		{
			Release();
			return FALSE;
		}

		//Counts
		m_fCounts = new double[AMR_ANALOG_TYPE_COUNT+10];
		if(!m_fCounts){Release();return FALSE;}

		// /m_nNumberAMRSamples = nSize;
		//m_fBuffer= new double[m_nSize+100];
		//if(!m_fBuffer){Release(); return FALSE;}
		
 		m_fPhi = new double[HF_AMR_ANALOG_FRAME_SIZE+10];
 		if (!m_fPhi){Release(); return FALSE;}

 		m_pAMR = new NS_DSP::CHFAMRAnalog;
 		if (!m_pAMR){Release();return FALSE;}

 		BOOL ret = m_pAMR->Initialize(HF_AMR_ANALOG_FRAME_SIZE/*8192*/);
 		if(ret != DSP_RET_SUCCESS){Release();return ret;}

		ret = m_objMA7.Create(HF_AMR_ANALOG_FRAME_SIZE,HF_AMR_ANALOG_Moving_AVE_7_Len);
		if (ret !=DSP_RET_SUCCESS){Release();return ret;}

		ret = m_objMA11.Create(HF_AMR_ANALOG_FRAME_SIZE,HF_AMR_ANALOG_Moving_AVE_11_Len);
		if (ret !=DSP_RET_SUCCESS){Release();return ret;}

		Reset();
		return DSP_RET_SUCCESS;
	}

	double CHFAMRModuleBase::CalcMed(int nMinRange,int nMaxRange,double *input)
	{
		int i;
		int j;
		double fMaxVal=0;
		int nMaxIdx=0;
		int nBulkSize=(nMaxRange+nMinRange)/2;
// 		int nIndx;
		for (j=0;j<nBulkSize+1;j++)
		{
			for(i=nMinRange;i<=nMaxRange;i++)
			{
				if(fMaxVal<input[i])
				{
					fMaxVal=input[i];
					nMaxIdx=i;
				}
			}
			if (j!=(nBulkSize))
				input[nMaxIdx]=0;
			else if(j==(nBulkSize))
				return fMaxVal;
		}
	}


	void CHFAMRModuleBase::InitAMRParam()
	{
// 		int i;
		m_pAMR->InitAMRParam(m_fCopyInputBuffer,m_dblA2FFTSmoothed,m_dblNoisePSD,m_dblMinimaA2SubSeg,m_fMinimaA2);
		//memset(m_fCounts,0,7*sizeof(double));
		//m_nSignalFramesNo = 0;
		for (int k=0;k<HF_AMR_ANALOG_FRAME_SIZE/2;k++)
			m_dblF[k]=double(k*(double)(HF_AMR_ANALOG_Fs/HF_AMR_ANALOG_FRAME_SIZE));

		m_fPMonHalfLen = round(1.5*HF_AMR_ANALOG_Fs/(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP));
		m_fPMon1stHalfEng = HF_AMR_ANALOG_epsilon0;
		m_fPMon2ndHalfEng = HF_AMR_ANALOG_epsilon0;
		
		m_bFirstTime=FALSE;

	}

	void CHFAMRModuleBase::PreExecute(void)
	{
// 		afxDump << "AMR base: preExecute \n";

		// 		int nSignalFrameNo=0;
		double dblSumN;
		double dblSumD;
		double dblMy;//Mean of Phi;
		double dblAB1;
		double dblAB2;

// 		afxDump << "AMR base: EnvelopeAndPhaseEst \n";
		m_pAMR->EnvelopeAndPhaseEst(m_fDelayOutBuffer,m_fHilbertOutBuffer,m_fPhi,m_fEnv);

// 		afxDump << "AMR base: vdSubt \n";
		vdSub(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_fPhi+1,m_fPhi,m_fDphiTemp);

// 		afxDump << "AMR base: CorrectionPhase \n";
		m_pAMR->CorrectionPhase(m_fDphiTemp,m_fDphiCorr);

// 		afxDump << "AMR base: Cumsum \n";
		m_pAMR->Cumsum(m_fDphiCorr,m_fOutSum,HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);

// 		afxDump << "AMR base: for \n";
		for (int i=1;i<HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP;i++)
		{
			m_fPhi[i]=m_fPhi[i]+m_fOutSum[i-1];
		}

		//////// 		//test
		//FILE *fp3 = fopen("c:\\phi.txt","at");
		//for (int i=0;i<HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP;i++){
		//	fprintf(fp3,"%40.40f\t",m_fPhi[i]);
		//}
		//fprintf(fp3,"\n");
		//fclose(fp3);
		//////// 		////test

// 		afxDump << "AMR base: AfterEnvelopeAndPhaseEst \n";

		dblSumN=0;
		for (int i=0;i<(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);i++)
			dblSumN = dblSumN + m_fPhi[i];
		dblMy = dblSumN/(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);
		memset(m_fDphiTemp,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		memset(m_fDphiCorr,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));
		for(int i=0;i<HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP;i++)
		{
			m_fDphiTemp[i]=(i+1)-m_dblMx;
			m_fDphiCorr[i]=m_fPhi[i]-dblMy;
		}
		vdMul(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_fDphiTemp,m_fDphiCorr,m_fDphiCorr);
		vdSqr(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_fDphiTemp,m_fDphiTemp);
		dblSumD = cblas_dasum(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_fDphiTemp,1);
		// 			dblSumD=0;
		dblSumN=0;
		for (int i=0;i<HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP;i++)
		{
			dblSumN = dblSumN+m_fDphiCorr[i];
			// 				dblSumD = dblSumD+m_fDphiTemp[i];
		}
		if (dblSumD)
			dblAB1 = dblSumN/dblSumD;
		else
			dblAB1=1.5621;
		dblAB2 = dblMy - dblAB1 *m_dblMx;
		
		for (int i=0;i<(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);i++)
			m_dblPhiNL[i]=m_fPhi[i]-dblAB1*(i+1)-dblAB2;

		/////////////////////////////////////
		/*m_pAMR->Median(71,HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_dblPhiNL,m_fPhi);
		for (int i=0;i<(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);i++)
			m_dblPhiNL[i]=m_dblPhiNL[i] - m_fPhi[i];*/
		/////////////////////////////////////

// 		afxDump << "AMR base: End of PreExecute \n";

	}

	int CHFAMRModuleBase::Execute(void)
	{
		int j=0;
		double SNR;
// 		int nNumberSteps;
// 		int nSignalFrameNo=0;
		double dblSumN;
		double dblSumD;
		double dblMy;//Mean of Phi;
		double dblAB1;
		double dblAB2;
		BOOL   bSignalFlag=FALSE;
		double m_fCenter;
		double fCentralEng;

		//Main loop
		//m_nNumFrames=1;
		m_fDphiTemp[0]=0;
		for (int nNumberSteps=0;nNumberSteps<m_nNumFrames;nNumberSteps++)
		{
// 			afxDump << "AMR base: Start Execute \n";
			m_nTotalNo++;
			dblSumN=0;
			dblSumD=0;
			dblMy = 0;
			dblAB1 = 0;
			dblAB2 = 0;
			SNR=0;
			m_fCenter=0;
			fCentralEng=0;
			int nInputLen;
			int nMaxIdx = -1;
			nInputLen = nLenInputBuffer;

			memset(m_fDphiTemp,0,HF_AMR_ANALOG_FRAME_SIZE*sizeof(double));

			int nLen;
			if (m_nNoiseEstSamplesCntr + nInputLen > HF_AMR_ANALOG_FRAME_SIZE)
			{
				nLen = HF_AMR_ANALOG_FRAME_SIZE - m_nNoiseEstSamplesCntr;
			}else{
				nLen = nInputLen ;
			}
			cblas_dcopy(nLen,m_fInputBuffer,1,m_fInputCopy+m_nNoiseEstSamplesCntr,1);
			m_nNoiseEstSamplesCntr+=nLen;

			m_pAMR->NoiseEstimation(m_nTotalNo,m_nSubMinSegs,nNumberSteps,m_fInputNS,m_dblNoisePSD,m_dblA2FFTSmoothed,
				m_dblMinimaA2SubSeg,m_dblNoisePSDMed,m_fDphiTemp,m_fMinimaA2);//m_fDphiTemp-->a2fft
			memmove(m_fInputCopy,m_fInputCopy+HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,
				HF_AMR_ANALOG_OVERLAP * sizeof(double));
			m_nNoiseEstSamplesCntr=HF_AMR_ANALOG_OVERLAP;

			double fNoiseEng=0;
			if (m_nTotalNo<5)
			{
				fNoiseEng = cblas_dasum(HF_AMR_ANALOG_TotalIdx,m_fDphiTemp+HF_AMR_ANALOG_BandStartIdx,1);
			}else
			{
				fNoiseEng = cblas_dasum(HF_AMR_ANALOG_TotalIdx,m_dblNoisePSDMed,1);
			}

			double fSignalEng =0;

			fSignalEng = cblas_dasum(HF_AMR_ANALOG_TotalIdx,m_fDphiTemp+HF_AMR_ANALOG_BandStartIdx,1);

			if (fSignalEng > m_fPMon2ndHalfEng)
				m_fPMon2ndHalfEng = fSignalEng;

			int nMod = (m_nTotalNo % (int)m_fPMonHalfLen) ;

			if (nMod == 0)
			{
				m_fPMon1stHalfEng = m_fPMon2ndHalfEng;
				m_fPMon2ndHalfEng = HF_AMR_ANALOG_epsilon0;
			}
			m_fPMonEng = max(m_fPMon1stHalfEng,m_fPMon2ndHalfEng);

			SNR = myMax(fSignalEng,HF_AMR_ANALOG_epsilon0)/(myMax(m_nBiasCF*fNoiseEng,HF_AMR_ANALOG_epsilon0))-1;
			if (SNR>1.5)
				bSignalFlag=TRUE;
			else
				bSignalFlag=FALSE;

			memset(m_fOutSum,0,(HF_AMR_ANALOG_FRAME_SIZE/2)*sizeof(double));
			memset(m_fDphiCorr,0,(HF_AMR_ANALOG_FRAME_SIZE/2)*sizeof(double));
			if (bSignalFlag)
			{
				int nOutSize;
 				double *pBuffer;
				m_objMA7.Reset();
				m_objMA7.AddBuffer(m_fDphiTemp+HF_AMR_ANALOG_BandStartIdx,HF_AMR_ANALOG_TotalIdx,&pBuffer,nOutSize);
 				cblas_dcopy(HF_AMR_ANALOG_TotalIdx,pBuffer,1,m_fOutSum,1);

				m_objMA11.AddBuffer(m_dblNoisePSDMed,HF_AMR_ANALOG_TotalIdx,&pBuffer,nOutSize);
				cblas_dcopy(HF_AMR_ANALOG_TotalIdx,pBuffer,1,m_fDphiCorr,1);
				//delete[] pBuffer;
				//pBuffer = NULL;
				memset(m_fPhi,0,(HF_AMR_ANALOG_FRAME_SIZE)*sizeof(double));
				for (int i=0;i<HF_AMR_ANALOG_TotalIdx;i++)
				{
					m_fPhi[i]=myMax(0,m_fOutSum[i+(HF_AMR_ANALOG_Moving_AVE_7_Len-1)/2]-HF_AMR_ANALOG_osc*HF_AMR_ANALOG_Bias_CF*m_fDphiCorr[i+(HF_AMR_ANALOG_Moving_AVE_11_Len-1)/2]);
				}

				//double fSumA2FFtNR=0;
				/*for (int i=0;i<HF_AMR_ANALOG_TotalIdx;i++)
					fSumA2FFtNR= fSumA2FFtNR+m_fPhi[i];*/

				double fSumA2FFtNR = cblas_dasum(HF_AMR_ANALOG_TotalIdx,m_fPhi,1);

				double fCarrierPower = 0;
				int	nCarrierBin = 0;

				for(int i=0;i<HF_AMR_ANALOG_TotalIdx;i++)
				{
					if (fCarrierPower<m_dblNoisePSD[i])
					{
						fCarrierPower=m_dblNoisePSD[i];
						nCarrierBin = i;
					}
				}

				double fCarrierToNoise = fCarrierPower/myMax(HF_AMR_ANALOG_epsilon0,m_dblNoisePSDMed[nCarrierBin]);

				vdSqr(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_dblPhiNL,m_dblPhiNL);

				//dblSumD = 0;
				dblSumD = cblas_dasum(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP,m_dblPhiNL,1);
				double fSTDPhiNL = dblSumD/(HF_AMR_ANALOG_FRAME_SIZE-HF_AMR_ANALOG_OVERLAP);
				fSTDPhiNL = sqrt((double)fSTDPhiNL);

				double fGammaMax=m_pAMR->ComputeGammaMax(m_fEnv);

				vdMul(HF_AMR_ANALOG_TotalIdx,m_dblF+HF_AMR_ANALOG_BandStartIdx,m_fPhi,m_fTemp);

				dblSumD = cblas_dasum(HF_AMR_ANALOG_TotalIdx,m_fTemp,1);

				m_dblFav[m_nIdxSig]=dblSumD/fSumA2FFtNR;

				m_pAMR->Median(1,myMin((int)m_nSignalFramesNo+1,(int)HF_AMR_ANALOG_f_av_med_len),m_dblFav,m_dblFavMed);

				m_dblFavMed[0]=round((m_dblFavMed[0]*HF_AMR_ANALOG_FRAME_SIZE)/HF_AMR_ANALOG_Fs);
				//m_nSignalFrameNo++;

				double fCarrierPowerTh01 =sqrt(SNR)/40 ;

				if (((fCarrierToNoise>fCarrierPowerTh01)||(m_fCounts[1]>9 && fCarrierToNoise>fCarrierPowerTh01/2))
					&& (fSTDPhiNL<HF_AMR_ANALOG_STD_phiNL_th_hi)&& (fGammaMax<HF_AMR_ANALOG_Gamma_Max_th_hi)
					|| (fCarrierToNoise>fCarrierPowerTh01*10000 && (fGammaMax<500))
					|| (fCarrierToNoise>fCarrierPowerTh01*1000 && (fGammaMax<100))
					|| (fCarrierToNoise>fCarrierPowerTh01*100 && (fGammaMax<50))
					|| (fCarrierToNoise>fCarrierPowerTh01*10 && (fGammaMax<25))
					|| (fCarrierToNoise>fCarrierPowerTh01 && fGammaMax < 10) )

				{

					m_fCounts[1]=m_fCounts[1]+1;
					dblSumD=0;
					for (int i=myMax(nCarrierBin-10,1);i<(myMin(nCarrierBin+10,HF_AMR_ANALOG_TotalIdx));i++)
						dblSumD = dblSumD+m_fPhi[i];
					double fCarrierEng = dblSumD/fSumA2FFtNR;
					if (fSTDPhiNL<HF_AMR_ANALOG_STD_phiNL_th_lo && fGammaMax>HF_AMR_ANALOG_Gamma_Max_th_lo)
					{
						m_fCounts[2]=m_fCounts[2]+2;
						m_nTotalCounts += 2;
					}else if (fSTDPhiNL>HF_AMR_ANALOG_STD_phiNL_th_lo && fGammaMax<HF_AMR_ANALOG_Gamma_Max_th_lo
						&& (fCarrierEng<HF_AMR_ANALOG_Carrier_Eng_th || m_fCounts[3]>9))
					{
						m_fCounts[3]=m_fCounts[3]+2;
						m_nTotalCounts += 2;
					}else if (fSTDPhiNL>(HF_AMR_ANALOG_STD_phiNL_th_lo*2)&&(fGammaMax<HF_AMR_ANALOG_Gamma_Max_th_lo/2))
					{
						m_fCounts[3] = m_fCounts[3]+2;
						m_nTotalCounts += 2;
					}
					else if(m_fCounts[2]>9)
					{
						if (fSTDPhiNL<(1.5*HF_AMR_ANALOG_STD_phiNL_th_lo) && fGammaMax>(HF_AMR_ANALOG_Gamma_Max_th_lo/1.5))
						{	
							m_fCounts[2]=m_fCounts[2]+1;
							m_nTotalCounts += 1;
						}
						else if (fSTDPhiNL < HF_AMR_ANALOG_STD_phiNL_th_lo*10 && fGammaMax < HF_AMR_ANALOG_Gamma_Max_th_lo*2.5 && SNR < 4)
						{
							m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
							m_fCounts[3] = m_fCounts[3] + 1;  // increment FM count 
							m_nTotalCounts += 1;
						}
					}else if (m_fCounts[3]>9)
					{
						if (fSTDPhiNL>(0.75*HF_AMR_ANALOG_STD_phiNL_th_lo) && fGammaMax>(HF_AMR_ANALOG_Gamma_Max_th_lo/0.75))
						{
							m_fCounts[3]=m_fCounts[3]+1;
							m_nTotalCounts += 1;
						}
						else if (fSTDPhiNL < HF_AMR_ANALOG_STD_phiNL_th_lo*10 && fGammaMax < HF_AMR_ANALOG_Gamma_Max_th_lo*2.5 && SNR < 4)
						{
							m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
							m_fCounts[3] = m_fCounts[3] + 1;  // increment FM count 
							m_nTotalCounts += 1;
						}
					}else if (fSTDPhiNL > HF_AMR_ANALOG_STD_phiNL_th_lo && fGammaMax < (HF_AMR_ANALOG_Gamma_Max_th_lo/4))
					{
						m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
						m_fCounts[3] = m_fCounts[3] + 3;  // increment FM count
						m_nTotalCounts += 3;
					}else if (fSTDPhiNL > (HF_AMR_ANALOG_STD_phiNL_th_lo*4) && fGammaMax < (HF_AMR_ANALOG_Gamma_Max_th_lo/2))
					{
						m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
						m_fCounts[3] = m_fCounts[3] + 2;  // increment FM count 
						m_nTotalCounts += 2;
					}else if (fSTDPhiNL < (HF_AMR_ANALOG_STD_phiNL_th_lo/3) && fGammaMax > (HF_AMR_ANALOG_Gamma_Max_th_lo/2.5))
					{
						m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
						m_fCounts[2] = m_fCounts[2] + 3;  // increment AM count
						m_nTotalCounts += 3;
					}else if (fSTDPhiNL < HF_AMR_ANALOG_STD_phiNL_th_lo && fGammaMax > HF_AMR_ANALOG_Gamma_Max_th_lo && fGammaMax < 20)
					{
						m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
						m_fCounts[2] = m_fCounts[2] + 1;  // increment AM count
						m_nTotalCounts += 1;
					}
					else if (fSTDPhiNL < HF_AMR_ANALOG_STD_phiNL_th_lo*10 && fGammaMax < HF_AMR_ANALOG_Gamma_Max_th_lo*2.5 && SNR < 4)
					{
						m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
						m_fCounts[3] = m_fCounts[3] + 1;  // increment FM count 
						m_nTotalCounts += 1;
					}

				}else if (fSTDPhiNL > HF_AMR_ANALOG_STD_phiNL_th_lo && fGammaMax < (HF_AMR_ANALOG_Gamma_Max_th_lo/4))
				{
					m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
					m_fCounts[3] = m_fCounts[3] + 3;  // increment FM count
					m_nTotalCounts += 3;
				}else if (fSTDPhiNL > (HF_AMR_ANALOG_STD_phiNL_th_lo*4) && fGammaMax < (HF_AMR_ANALOG_Gamma_Max_th_lo/2))
				{
					m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
					m_fCounts[3] = m_fCounts[3] + 2;  // increment FM count 
					m_nTotalCounts += 2;
				}else if (fSTDPhiNL < (HF_AMR_ANALOG_STD_phiNL_th_lo/3) && fGammaMax > (HF_AMR_ANALOG_Gamma_Max_th_lo/2.5))
				{
					m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
					m_fCounts[2] = m_fCounts[2] + 3;  // increment AM count
					m_nTotalCounts += 3;
				}else if (fSTDPhiNL < HF_AMR_ANALOG_STD_phiNL_th_lo && fGammaMax > HF_AMR_ANALOG_Gamma_Max_th_lo && fGammaMax < 20)
				{
					m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
					m_fCounts[2] = m_fCounts[2] + 1;  // increment AM count
					m_nTotalCounts += 1;
				}else if (fSTDPhiNL < HF_AMR_ANALOG_STD_phiNL_th_lo*10 && fGammaMax < HF_AMR_ANALOG_Gamma_Max_th_lo*2.5 && SNR < 4)
				{
					m_fCounts[1] = m_fCounts[1] + 1;  // increment carrier count
					m_fCounts[3] = m_fCounts[3] + 1;  // increment FM count 
					m_nTotalCounts += 1;
				}
				else if (fGammaMax>5 && fSTDPhiNL > 4)
				{
					//added in New version of new AMR routine!!
					//double *pBuffer;
					m_fCenter = m_dblFavMed[0];
					if (m_fCenter < HF_AMR_ANALOG_BandStartIdx) 
						m_fCenter = HF_AMR_ANALOG_BandStartIdx;
					
					int nCenterIdxStart = myMax(0,m_dblFavMed[0]-7);
					int nCenterIdxEnd = myMin(m_dblFavMed[0]+7,HF_AMR_ANALOG_FRAME_SIZE/2-1);
					double fSum1 = cblas_dasum(nCenterIdxEnd-nCenterIdxStart+1,m_fDphiTemp+nCenterIdxStart,1);

					nCenterIdxStart = myMax(m_dblFavMed[0]-HF_AMR_ANALOG_BandStartIdx-7,0);
					nCenterIdxEnd = myMin(m_dblFavMed[0]-HF_AMR_ANALOG_BandStartIdx+7,HF_AMR_ANALOG_TotalIdx-1);
					double fSum3 = cblas_dasum(nCenterIdxEnd-nCenterIdxStart+1,m_dblNoisePSD+nCenterIdxStart,1);

					double fTotalSum = cblas_dasum(HF_AMR_ANALOG_TotalIdx,m_dblNoisePSD,1);

					fCentralEng = fSum1/fSum3/(m_fPMonEng/fTotalSum);

					//double fSumHalf1=0;
					//for (int i=0;i<=m_fCenter-HF_AMR_ANALOG_BandStartIdx;i++)
					//	fSumHalf1 = fSumHalf1+m_fPhi[i];
					double fSumHalf1 = cblas_dasum((int)m_fCenter-HF_AMR_ANALOG_BandStartIdx+1,m_fPhi,1);

					//double fSumHalf2=0;
					//for (int i=m_fCenter-HF_AMR_ANALOG_BandStartIdx+1;i<HF_AMR_ANALOG_TotalIdx/*HF_AMR_ANALOG_FRAME_SIZE/2*/;i++)
					//	fSumHalf2=fSumHalf2+m_fPhi[i];
					double fSumHalf2 = cblas_dasum(HF_AMR_ANALOG_TotalIdx-((int)m_fCenter-HF_AMR_ANALOG_BandStartIdx+1),m_fPhi+(int)m_fCenter-HF_AMR_ANALOG_BandStartIdx,1);

					double fSpectralSymmetry=(fSumHalf1-fSumHalf2)/fSumA2FFtNR;
					if (fCentralEng>0.25)
					{
						if (fSpectralSymmetry>0.3)
						{
							m_fCounts[6]=m_fCounts[6]+2;
							m_nTotalCounts += 2;
						}
						else if (fSpectralSymmetry < -0.3)
						{
							m_fCounts[5]=m_fCounts[5]+2;
							m_nTotalCounts += 2;
						}
						else if (fSpectralSymmetry > 0.2)
						{
							m_fCounts[6]=m_fCounts[6]+1;
							m_nTotalCounts += 1;
						}
						else if (fSpectralSymmetry < -0.2)
						{
							m_fCounts[5]=m_fCounts[5]+1;
							m_nTotalCounts += 1;
						}
						else if (ABS(fSpectralSymmetry) < 0.08 && fCentralEng < 0.5)
						{
							m_fCounts[4] = m_fCounts[4] + 1;
							m_nTotalCounts += 1;
						}
						else if (ABS(fSpectralSymmetry) < 0.1 && fCentralEng > 0.5)
						{
							m_fCounts[1] = m_fCounts[1] + 1;
							m_nTotalCounts += 1;
							if (fSTDPhiNL < 7.5)
								m_fCounts[2] = m_fCounts[2] + 1;
							else if (fGammaMax < 10) 
								m_fCounts[3] = m_fCounts[3] + 1;
						}
					}else{
						if (fSpectralSymmetry> 0.5)
						{
							m_fCounts[6]=m_fCounts[6]+2;
							m_nTotalCounts += 2;
						}
						else if (fSpectralSymmetry < -0.5)
						{
							m_fCounts[5]=m_fCounts[5]+2;
							m_nTotalCounts += 2;
						}
						else if (fSpectralSymmetry > 0.1)
						{
							m_fCounts[6]=m_fCounts[6]+1;
							m_nTotalCounts += 1;
						}
						else if (fSpectralSymmetry < -0.1)
						{
							m_fCounts[5]=m_fCounts[5]+1;
							m_nTotalCounts += 1;
						}
						else
						{
							m_fCounts[4]=m_fCounts[4]+1;
							m_nTotalCounts += 1;
						}
					}
				}

				m_nIdxSig = m_nIdxSig % HF_AMR_ANALOG_f_av_med_len + 1;
				m_nSignalFramesNo++;

				//m_nModeType = m_pAMR->recognitionAMR(m_fCounts);

				double	fMaxCount = -1;
				nMaxIdx = -1;
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
						return AMR_ANALOG_NO_SIGNAL;
						break;
					case 1:
						{
							if (m_fCounts[2] >= m_fCounts[3])
								m_nModeType = AMR_ANALOG_AMFM_AM;
							else
								m_nModeType = AMR_ANALOG_AMFM_FM;
						}
						break;
					case 2:
						m_nModeType = AMR_ANALOG_AM;
						break;
					case 3:
						m_nModeType = AMR_ANALOG_FM;
						break;
					case 4:
						m_nModeType = AMR_ANALOG_DSB;
						break;
					case 5:
						m_nModeType = AMR_ANALOG_SSB;//AMR_ANALOG_LSB;
						break;
					case 6:
						m_nModeType = AMR_ANALOG_SSB;//AMR_ANALOG_USB;
						break;
					}
				}else
					m_nModeType = AMR_ANALOG_NO_SIGNAL;


				//SAFERELEASE(m_pAMR);
				//m_pAMR->Release();
				m_nModulationCounter[m_nModeType]++;

			}
			if (m_nTotalNo > 40 && m_nSignalFramesNo > 10 && nMaxIdx > -1 )
			{

				//for(j=0;j<=HF_NUMBER_ANALOG_AMR_MODULATIONS;j++)
				//{
					//double fMod = double((double)m_nModulationCounter[j]/(double)m_nSignalFramesNo);
					double fMod = double((double)m_fCounts[nMaxIdx]/(double)m_nTotalCounts);
					if (fMod >= 0.6)
					{
						m_bSendMod = TRUE;
						return m_nModeType;
						//break;
					}
				//}
				//if(m_bSendMod == TRUE)
				//	return j;

				//if (m_nTotalNo == 120)//10Sec For Unknown status.
				//{
				//	m_bSendMod=TRUE;
				//	return AMR_ANALOG_NO_SIGNAL;
				//}
			}
			if ((m_nTotalNo * HF_AMR_ANALOG_FRAME_SIZE) >= m_nNumberAMRSamples)
			{
				m_bSendMod=TRUE;
				return AMR_ANALOG_NO_SIGNAL;
			}


		}//for
	}//sub

}//namespace