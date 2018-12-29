#include "StdAfx.h"
#include "Demodulator.h"

#include "AFDemodulator.h"
#include "AMDemodulator.h"
#include "FMDemodulator.h"
#include "SSBDemodulator.h"
#include "AFDemodulator.h"
#include "MorseDemodulator.h"
#include "DigitalDemodulator.h"

#include "../BCGP_HF/BCGP_HF.h"
#include "../BCGP_HF/SharedVariables.h"

namespace NS_DSP
{
	////////////////ANALOG_MODULATION_PROP///////////////////////////

	ANALOG_MODULATION_PROP::ANALOG_MODULATION_PROP()
	{
		nRegControl		=	0;
		nRegFreqOffset	=	0;
		nRegCICRating	=	0;
		nRegMSBTrunc	=	0;
		//nRegCWConstant	=	0;
		//nRegCWThreshold	=	0;
	}
	ANALOG_MODULATION_PROP::ANALOG_MODULATION_PROP(const ANALOG_MODULATION_PROP& m_copy)
	{
		nRegControl		=	m_copy.nRegControl;
		nRegFreqOffset	=	m_copy.nRegFreqOffset;
		nRegCICRating	=	m_copy.nRegCICRating;
		nRegMSBTrunc	=	m_copy.nRegMSBTrunc;
		//nRegCWConstant	=	m_copy.nRegCWConstant;
		//nRegCWThreshold	=	m_copy.nRegCWThreshold;
	}
	/////////////////////////////////////////////////////////////////
	
	////////////////DIGITAL_MODULATION_PROP///////////////////////////
	DIGITAL_MODULATION_PROP::DIGITAL_MODULATION_PROP()
	{
		nRegControl				=	0;
		nRegOptimumBit			=	0;
		//nRegNumberSample		=	0;
		nRegR					=	0;
		nRegRInt				=	0;
		nRegMu0					=	0;
		nRegLu					=	0;
		nRegNCombDelay			=	0;
		nRegG1					=	0;
		nRegG2					=	0;	
		nRegGShift				=	0;
		nRegFc					=	0;
		nRegKP					=	0;
		nRegKI1					=	0;
		nRegKI2					=	0;
		nRegFreqEstimationCoef	=	0;
		nRegFreqEstimationTrunc	=	0;
	}
	DIGITAL_MODULATION_PROP::DIGITAL_MODULATION_PROP(const DIGITAL_MODULATION_PROP& m_copy)
	{
		nRegControl				=	m_copy.nRegControl;
		nRegOptimumBit			=	m_copy.nRegOptimumBit;
		//nRegNumberSample		=	m_copy.;
		nRegR					=	m_copy.nRegR;
		nRegRInt				=	m_copy.nRegRInt;
		nRegMu0					=	m_copy.nRegMu0;
		nRegLu					=	m_copy.nRegLu;
		nRegNCombDelay			=	m_copy.nRegNCombDelay;
		nRegG1					=	m_copy.nRegG1;
		nRegG2					=	m_copy.nRegG2;	
		nRegGShift				=	m_copy.nRegGShift;
		nRegFc					=	m_copy.nRegFc;
		nRegKP					=	m_copy.nRegKP;
		nRegKI1					=	m_copy.nRegKI1;
		nRegKI2					=	m_copy.nRegKI2;
		nRegFreqEstimationCoef	=	m_copy.nRegFreqEstimationCoef;
		nRegFreqEstimationTrunc	=	m_copy.nRegFreqEstimationTrunc;
	}
	/////////////////////////////////////////////////////////////////

	//Loaded FIR Coefs Status
	BOOL CDemodulatorItem::m_bIsLoadedFIRCoefsAM=0;
	BOOL CDemodulatorItem::m_bIsLoadedFIRCoefsFM=0;
	BOOL CDemodulatorItem::m_bIsLoadedFIRCoefsSSB=0;
	BOOL CDemodulatorItem::m_bIsLoadedFIRCoefsAF=0;
	BOOL CDemodulatorItem::m_bIsLoadedFIRCoefsCW=0;
	BOOL CDemodulatorItem::m_bIsLoadedFIRCoefsAMR=0;
	CDemodulatorItem	*CDemodulatorItem::m_pAudioPlayingItem=NULL;//pointer to active plating audio

	CDemodulatorItem::CDemodulatorItem(void)
	{
		nCategory		=	eModulationCatUnknown; 
		nType			=	eModulationUnknown;
		bIsPlaying		=	FALSE;
		bIsRecording	=	FALSE;
		nID				=	-1; //ID in FPGA
		nRow			=	-1;//Row in channels list
		m_pAnalogProp	=	NULL;
		m_pDigitalProp	=	NULL;
		m_pDemodulator	=	NULL;
		bIsHopping		=	FALSE;
		nHWTableIndex	=	3;//default fixed table index
		m_pWndContents	=	NULL;
	}

	CDemodulatorItem::~CDemodulatorItem(void)
	{
		Release();
	}

	BOOL CDemodulatorItem::Create(int m_nID,enumModulationType m_nType,int m_nRow,int bHopping,int m_nHWTableIndex)
	{
		ASSERT(m_nID!=-1);
		enumModulationCategory nCat = CGlobalTypes::GetModulationCategory(m_nType);
		switch (nCat)
		{
		case eModulationCatAnalog:
		case eModulationCatMorse:
			m_pAnalogProp = new ANALOG_MODULATION_PROP;
			if (!m_pAnalogProp) return FALSE;
			break;

		case eModulationCatDigital:
			m_pDigitalProp = new DIGITAL_MODULATION_PROP;
			if (!m_pDigitalProp) return FALSE;
			break;

		}
		switch(m_nType)
		{
		case eModulationAF:
			m_pDemodulator	=	new CAFDemodulator;
			if (!m_pDemodulator) return FALSE;
			if(!m_pDemodulator->Create(m_nID)) return FALSE;
			break;

		case eModulationAM:
			m_pDemodulator	=	new CAMDemodulator;
			if (!m_pDemodulator) return FALSE;
			if(!m_pDemodulator->Create(m_nID)) return FALSE;
			break;

		case eModulationFM:
			m_pDemodulator	=	new CFMDemodulator;
			if (!m_pDemodulator) return FALSE;
			if(!m_pDemodulator->Create(m_nID)) return FALSE;
			break;

		case eModulationLSB:
		case eModulationUSB:
			m_pDemodulator	=	new CSSBDemodulator;
			if (!m_pDemodulator) return FALSE;
			m_pDemodulator->SetType(nType);
			m_pDemodulator->SetHopping(bHopping);
			if(!m_pDemodulator->Create(m_nID)) return FALSE;	
			break;

		case eModulationCW:
			{
				CMorseDemodulator *pMod = new CMorseDemodulator;
				if (!pMod ) return FALSE;
				//m_pDemodulator->SetType(nType);
				if(!pMod->Create(m_nID)) return FALSE;	
				m_pDemodulator = pMod;
				m_pDemodulator = pMod;
			}
			break;

		case eModulationBPSK:
		case eModulationQPSK:
		case eModulationOQPSK:
			{
				CDigitalDemodulator *pMod = new CDigitalDemodulator;
				if (!pMod ) return FALSE;
				//m_pDemodulator->SetType(nType);
				if(!pMod->Create(m_nID)) return FALSE;	
				m_pDemodulator = pMod;
				m_pDemodulator = pMod;
			}
			break;

		}//switch

		nID			=	m_nID;
		nCategory	=	nCat;
		nType		=	m_nType;
		nRow		=	m_nRow;
		bIsHopping	=	bHopping;
		nHWTableIndex = m_nHWTableIndex;
		Reset();
		return TRUE;
	}//func

	void CDemodulatorItem::StopDemodulator()
	{
		bIsPlaying	=	FALSE;
		if (m_pAudioPlayingItem == this)
		{//if this item is playing audio
			StopAudio();
		}
		if (m_pWndContents && ::IsWindow(m_pWndContents->GetSafeHwnd()) )
		{//close morse/digital window
			m_pWndContents->PostMessage(WM_CLOSE);
		}
	}//func

	void CDemodulatorItem::Release()
	{
		StopDemodulator();

		//free allocated hardware
		switch (nCategory)
		{
		case eModulationCatAnalog:
		case eModulationCatMorse:
			CSharedVariables::m_FPGA.DeleteAnalogIndex(nID);
			break;
		case eModulationCatDigital:
			CSharedVariables::m_FPGA.DeleteDigitalIndex(0);//nID);
			break;
		}//switch nCategory
		
		CSharedVariables::EnableDemodulatorBuffer();
		if (m_pAnalogProp) HF_DISABLE_DEMOD(m_pAnalogProp->nRegControl);//Enable Demod
		SetHardwareParameters();
		SAFERELEASE(m_pAnalogProp);
		SAFERELEASE(m_pDigitalProp);
		SAFERELEASE(m_pDemodulator);
	}

	void CDemodulatorItem::Reset()
	{
		if(m_pDemodulator) m_pDemodulator->Reset();
		bIsPlaying		=	FALSE;
		bIsRecording	=	FALSE;
	}

	BOOL CDemodulatorItem::LoadFIRCoefs()
	{
		m_bIsLoadedFIRCoefsAM=CAMDemodulator::LoadFiltersCoefficients();
		m_bIsLoadedFIRCoefsFM=CFMDemodulator::LoadFiltersCoefficients();
		m_bIsLoadedFIRCoefsSSB=CSSBDemodulator::LoadFiltersCoefficients();
		m_bIsLoadedFIRCoefsAF=CAFDemodulator::LoadFiltersCoefficients();
		m_bIsLoadedFIRCoefsCW=CMorseDemodulator::LoadFiltersCoefficients();
		m_bIsLoadedFIRCoefsAMR=CHFAnalogAMR::LoadFiltersCoefficients();

		if (!m_bIsLoadedFIRCoefsAM)
		{
			theApp.LogEvent(eCatError,eFormChannel,"Error Loading AM Filter Coefficients",
				"[CDemodulatorItem::LoadFIRCoefs]");
		}
		if (!m_bIsLoadedFIRCoefsFM)
		{
			theApp.LogEvent(eCatError,eFormChannel,"Error Loading FM Filter Coefficients",
				"[CDemodulatorItem::LoadFIRCoefs]");
		}
		if (!m_bIsLoadedFIRCoefsSSB)
		{
			theApp.LogEvent(eCatError,eFormChannel,"Error Loading SSB Filter Coefficients",
				"[CDemodulatorItem::LoadFIRCoefs]");
		}
		if (!m_bIsLoadedFIRCoefsAF)
		{
			theApp.LogEvent(eCatError,eFormChannel,"Error Loading AF Filter Coefficients",
				"[CDemodulatorItem::LoadFIRCoefs]");
		}
		if (!m_bIsLoadedFIRCoefsCW)
		{
			theApp.LogEvent(eCatError,eFormChannel,"Error Loading CW Filter Coefficients",
				"[CDemodulatorItem::LoadFIRCoefs]");
		}
		if (!m_bIsLoadedFIRCoefsAMR)
		{
			theApp.LogEvent(eCatError,eFormChannel,"Error Loading AMR Filter Coefficients",
				"[CDemodulatorItem::LoadFIRCoefs]");
		}

		if (m_bIsLoadedFIRCoefsAM && m_bIsLoadedFIRCoefsFM
			&& m_bIsLoadedFIRCoefsSSB && m_bIsLoadedFIRCoefsAF &&
			m_bIsLoadedFIRCoefsCW && m_bIsLoadedFIRCoefsAMR) return TRUE;
		return FALSE;
	}
	
	void CDemodulatorItem::UnLoadFIRCoefs()
	{
		CAMDemodulator::UnLoadFiltersCoefficients();
		CAFDemodulator::UnLoadFiltersCoefficients();
		CFMDemodulator::UnLoadFiltersCoefficients();
		CSSBDemodulator::UnLoadFiltersCoefficients();
		CMorseDemodulator::UnLoadFiltersCoefficients();
		CHFAnalogAMR::UnLoadFiltersCoefficients();
	}

	BOOL CDemodulatorItem::Demodulate(DWORD *m_pRawBuffer,int nSize)
	{
		if(!m_pDemodulator) return FALSE;
		m_pDemodulator->InterpretCardBuffer(m_pRawBuffer,nSize);
		m_pDemodulator->CalculateMSB();
		BOOL bRet = m_pDemodulator->Demodulate();
		if (!bRet) return FALSE;
		bRet = m_pDemodulator->Record(bIsRecording);
		if (!bRet)
		{
			CString str;
			str.Format("Error Recording File : %s",m_pDemodulator->GetLastRecordError());
			theApp.LogEvent(eCatError,eFormChannel,str,"[CDemodulatorItem::Demodulate]");
		}
		if (bIsPlaying)
		{//play-out channel
			m_pDemodulator->PlayOut();
		}//play-out channel
		return bRet;
	}

	BOOL CDemodulatorItem::Record(BOOL bStart)
	{
		if(!m_pDemodulator) return FALSE;
		BOOL bRet = m_pDemodulator->Record(bStart);
		bRet = m_pDemodulator->Record(bIsRecording);
		if (!bRet)
		{
			CString str;
			str.Format("Error Recording File : %s",m_pDemodulator->GetLastRecordError());
			theApp.LogEvent(eCatError,eFormChannel,str,"[CDemodulatorItem::Demodulate]");
		}
		return bRet;
	}

	void CDemodulatorItem::CalcAnalog(double m_fCenter,double m_fBW,double m_fDeltaF)
	{
		ASSERT(nID!=-1);
		enumModulationCategory nCat = CGlobalTypes::GetModulationCategory(nType);
		double fs = CSharedVariables::m_FPGA.GetFs(m_fCenter,m_fBW);
		int nModeIndex = CSharedVariables::m_FPGA.GetHFMode(m_fCenter,m_fBW);
		int n,nFFTIndex  = HF_MACRO_RAW_fft_index(nModeIndex);
		switch (nType)
		{
		case eModulationAM:
		case eModulationLSB:
		case eModulationUSB:
			m_pAnalogProp->nRegCICRating = (fs / HF_AUDIO_RATE);
			n	=  (HF_CTE_CICACCESSBITS-
				ceil(HF_CIC_ORDER*LOG2((double)m_pAnalogProp->nRegCICRating)));
			ASSERT (n>=0);
			m_pAnalogProp->nRegMSBTrunc = UINT (n);
			m_pAnalogProp->nRegCICRating = m_pAnalogProp->nRegCICRating *65536; // This CicRate is for write to register
			HF_ENABLE_DEMOD(m_pAnalogProp->nRegControl);//Enable Demod
			if (bIsHopping)
			{
				ASSERT(nHWTableIndex>=0 && nHWTableIndex<3);
				m_pAnalogProp->nRegFreqOffset	=	-HF_FIXED_DEMOD_OFFSET;
				m_pAnalogProp->nRegControl &= 0x000f;
				m_pAnalogProp->nRegControl |= (nHWTableIndex << 5);
			}else{//is fixed
				m_pAnalogProp->nRegFreqOffset	=	int(m_fDeltaF - 
					HF_FIXED_DEMOD_OFFSET+((nFFTIndex*fs/CSharedVariables::GetFFTSize())));
				m_pAnalogProp->nRegControl |= 16;
			}		
			break;

		case eModulationFM:
			m_pAnalogProp->nRegCICRating = (fs / (HF_AUDIO_RATE *HF_FM_CIC_DOWNSAMPLE));
			n	=  (HF_CTE_CICACCESSBITS-
				ceil(HF_CIC_ORDER*LOG2((double)m_pAnalogProp->nRegCICRating)));
			ASSERT (n>=0);
			m_pAnalogProp->nRegMSBTrunc = UINT (n);
			m_pAnalogProp->nRegCICRating = m_pAnalogProp->nRegCICRating *65536; // This CicRate is for write to register
			HF_ENABLE_DEMOD(m_pAnalogProp->nRegControl);//Enable Demod
			m_pAnalogProp->nRegControl |= 0x0002;
			if (bIsHopping)
			{
				ASSERT(nHWTableIndex>=0 && nHWTableIndex<3);
				m_pAnalogProp->nRegFreqOffset	=	-0;
				m_pAnalogProp->nRegControl &= 0x000f;
				m_pAnalogProp->nRegControl |= (nHWTableIndex << 5);
			}else{//is fixed
				m_pAnalogProp->nRegFreqOffset	=	int(m_fDeltaF - 
					0+(nFFTIndex*fs/CSharedVariables::GetFFTSize()));
				m_pAnalogProp->nRegControl |= 16;
			}		
			break;

		case eModulationCW:
			m_pAnalogProp->nRegCICRating = (fs / (HF_AUDIO_RATE ));
			n	=  (HF_CTE_CICACCESSBITS-
				ceil(HF_CIC_ORDER*LOG2((double)m_pAnalogProp->nRegCICRating)));
			ASSERT (n>=0);
			m_pAnalogProp->nRegMSBTrunc = UINT (n);
			m_pAnalogProp->nRegCICRating = m_pAnalogProp->nRegCICRating *65536; // This CicRate is for write to register
			HF_ENABLE_DEMOD(m_pAnalogProp->nRegControl);//Enable Demod
			m_pAnalogProp->nRegControl |= 0x0004;
			if (bIsHopping)
			{
				ASSERT(nHWTableIndex>=0 && nHWTableIndex<3);
				m_pAnalogProp->nRegFreqOffset	=	-0;
				m_pAnalogProp->nRegControl &= 0x000f;
				m_pAnalogProp->nRegControl |= (nHWTableIndex << 5);
			}else{//is fixed
				m_pAnalogProp->nRegFreqOffset	=	int(m_fDeltaF - 
					0+(nFFTIndex*fs/CSharedVariables::GetFFTSize()));
				m_pAnalogProp->nRegControl |= 16;
			}		
			break;

		}//switch
		m_pAnalogProp->nRegFreqOffset = (m_pAnalogProp->nRegFreqOffset)*(1048576.0/fs);

	}//func

	void CDemodulatorItem::CalcDigital(double m_fCenter,double m_fBW,double m_fDeltaF,double m_fBitRate)
	{
		int m_nHWType = CGlobalTypes::DigitalModulationEnum2Type(nType);
		double fs = CSharedVariables::m_FPGA.GetFs(m_fCenter,m_fBW);
		int nModeIndex = CSharedVariables::m_FPGA.GetHFMode(m_fCenter,m_fBW);
		int nFFTIndex  = HF_MACRO_RAW_fft_index(nModeIndex);
		double CLOCKRATE = CSharedVariables::m_FPGA.GetClockA();

		DWORD dg_control=50177 | (m_nHWType << 1);
		//dFreq=500000;
		double dg_carrierfreq = m_fDeltaF+((double)nFFTIndex/(double)CSharedVariables::GetFFTSize())*fs;
		//Compute
		//double rate = m_dg_bw;///(double)1.35;
		double Rs =  m_fBitRate;//*1000000 ;/// 1.35;//rolloff = 0.35
		double nsamp = fs/Rs;
		double Rs_dev = (1 - 0.000)*Rs;
		double dec = fs/(6*Rs_dev);

		//Digital Demodulator CIC parameters
		double R=(double)fs/(double)(6*Rs_dev);
		double R_first=ceil(R/10);
		double A=R/R_first;
		double downrate=floor(A);
		double mu0=(A-downrate);
		while (mu0<.1 || mu0>.6)
		{
			R_first=R_first+1;
			downrate=floor((double)fs/(double)(6*R_first*Rs_dev));
			mu0=((double)fs/(double)(6*R_first*Rs_dev))-downrate;
		}	
		R = R_first;
		double dec2 = dec/R;
		double Rint = downrate;
		if (Rint<=0) Rint=1;
		double mu0_fixed=floor(mu0*pow((double)2,(double)17));
		double M=1;
		double N=2;
		double Lu=floor(N*LOG2(R*Rint*M));

		//Timing Parameters
		double delayN = M * (Rint);
		double g1_fixed = 104860;
		double g2_fixed = 2097;
		double g_shift = 2;
		double KI = .0004*.7;
		double KI_fix = floor(KI*pow((double)2,(double)15));
		double Kp = 0.03*.7;
		double Kp_fix = floor(Kp*pow((double)2,(double)15));

		//DDS parameters
		double fc = dg_carrierfreq;
		double fc_fixed = floor(2*M_PI*fc/CLOCKRATE*pow((double)2,(double)29));
		//double fc_fixed = floor(2*M_PI*fc/fs*pow((double)2,(double)29));

		double freqstm_gain =pow(2.0f,15)*Rs_dev*4/CLOCKRATE;//4 is for integrating the deltafc
		int Endrunc=10-(int)floor(LOG2(freqstm_gain));
		if (Endrunc<0 )
			Endrunc=0;
		int fixed_freq_gain = round(freqstm_gain*pow(2.0f,Endrunc));
		if(R==1 && Rint==1)
		{
			Lu=3;
			delayN = 3;
			g1_fixed = 85000;
			g2_fixed = 4100;
			g_shift = 0;
		}
		R-=1;Rint-=1;
		DWORD dg_g1=(int)g1_fixed;
		DWORD dg_g2=(int)g2_fixed;
		DWORD dg_lu=(int) Lu;
		DWORD dg_fc=(DWORD)floor(fc_fixed);
		DWORD dg_gshift=(int)g_shift;
		DWORD dg_downsample=(DWORD)((double)fs/(double)(6*Rs_dev));
		DWORD dg_ncombdelay=(int)delayN;
		DWORD dg_freqstmcoef=fixed_freq_gain;
		DWORD dg_freqstmtrunc=Endrunc;

		//	if(m_arrModulation.GetCount() >=8)  // We can not create new modulation 
		//		return FALSE;
		int dg_optimum_bit=12;
		int dg_number_sample=100;
		int dg_ki1=7;
		int dg_ki2=11;
		int dg_kp=650;

		//---------------------------------------//
		m_pDigitalProp->nRegControl = dg_control;
		m_pDigitalProp->nRegFc		=UINT(dg_fc);
		m_pDigitalProp->nRegFreqEstimationCoef = dg_freqstmcoef;
		m_pDigitalProp->nRegFreqEstimationTrunc = dg_freqstmtrunc;
		m_pDigitalProp->nRegG1 = dg_g1;
		m_pDigitalProp->nRegG2 = dg_g2;
		m_pDigitalProp->nRegGShift = dg_gshift;
		m_pDigitalProp->nRegKI1 = dg_ki1;
		m_pDigitalProp->nRegKI2 = dg_ki2;
		m_pDigitalProp->nRegKP = dg_kp;
		m_pDigitalProp->nRegLu = dg_lu;
		m_pDigitalProp->nRegMu0  = UINT(mu0_fixed);
		m_pDigitalProp->nRegNCombDelay = dg_ncombdelay;
		m_pDigitalProp->nRegOptimumBit = dg_optimum_bit;
		m_pDigitalProp->nRegR = UINT(R);//dg_downsample;
		m_pDigitalProp->nRegRInt = UINT(Rint);
		
	}//func

	void CDemodulatorItem::SetHardwareParameters()
	{
		if(nID==-1) return;
		enumModulationCategory nCat = CGlobalTypes::GetModulationCategory(nType);
		switch(nCat)
		{
		case eModulationCatAnalog:
		case eModulationCatMorse:
			{
			int nDemodStartAddr = nID * 6;
			ASSERT(m_pAnalogProp);
			/*CSharedVariables::m_FPGA.SetRegister(
				nDemodStartAddr+HF_REG_DEMOD_CONTROL,m_pAnalogProp->nRegControl);
			CSharedVariables::m_FPGA.SetRegister(
				nDemodStartAddr+HF_REG_DEMOD_FREQOFFSET,m_pAnalogProp->nRegFreqOffset);
			CSharedVariables::m_FPGA.SetRegister(
				nDemodStartAddr+HF_REG_DEMOD_CICRATING,m_pAnalogProp->nRegCICRating);
			CSharedVariables::m_FPGA.SetRegister(
				nDemodStartAddr+HF_REG_DEMOD_MSBTRUNC,m_pAnalogProp->nRegMSBTrunc);
			CSharedVariables::m_FPGA.SetRegister(
				nDemodStartAddr+HF_REG_DEMOD_CW_CONSTANT,50);*/
			CSharedVariables::m_FPGA.SetAnalogDemodHardWare(nDemodStartAddr,m_pAnalogProp);
			}
			break;

		case eModulationCatDigital:
			ASSERT(m_pDigitalProp);
			{
				/*CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_OPTIMUMBIT,m_pDigitalProp->nRegOptimumBit);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_NUMBERSAMPLE,100);//Note ****
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_CONTROL,m_pDigitalProp->nRegControl + 32);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_CONTROL,m_pDigitalProp->nRegControl);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_R,m_pDigitalProp->nRegR);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_RINT,m_pDigitalProp->nRegRInt);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_MU0FIXED,m_pDigitalProp->nRegMu0);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_LU,m_pDigitalProp->nRegLu);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_NCOMBDELAY,m_pDigitalProp->nRegNCombDelay);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_G1,m_pDigitalProp->nRegG1);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_G2,m_pDigitalProp->nRegG2);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_GSHIFT,m_pDigitalProp->nRegGShift);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_FC,m_pDigitalProp->nRegFc);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_KP,m_pDigitalProp->nRegKP);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_KI1,m_pDigitalProp->nRegKI1);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_KI2,m_pDigitalProp->nRegKI2);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_FREQSTMCOEF,m_pDigitalProp->nRegFreqEstimationCoef);
				CSharedVariables::m_FPGA.SetRegister(
					HF_REG_DG_FREQSTMTRUNC,m_pDigitalProp->nRegFreqEstimationTrunc);*/
				CSharedVariables::m_FPGA.SetDigitalDemodHardWare(0,m_pDigitalProp);
			}
			break;
		}
	}

	void CDemodulatorItem::StartAudio()
	{
		if (CSharedVariables::m_Audio.IsRunning())
			CSharedVariables::m_Audio.Stop();
		CSharedVariables::m_nAudioBufferFullCounter = 0;
		CSharedVariables::m_Audio.init(HF_AUDIO_BUFFER_LENGTH,1,HF_AUDIO_RATE,BDT_Int16);
		CSharedVariables::m_Audio.Start();
		CDemodulatorItem::m_pAudioPlayingItem = this;
	}

	void CDemodulatorItem::StopAudio()
	{
		if (CSharedVariables::m_Audio.IsRunning())
			CSharedVariables::m_Audio.Stop();
		CSharedVariables::m_nAudioBufferFullCounter = 0;
		CDemodulatorItem::m_pAudioPlayingItem = NULL;
	}//func


	void CDemodulatorItem::NormalizeAudio()
	{
		double nIncrement;
		if (m_pDemodulator	==	NULL) return;
		if (nCategory	!= eModulationCatAnalog) return;
		if (m_pAnalogProp	==	NULL) return;
		//------ONLY FOR ANALOG MODULATIONS--------
		if (m_pDemodulator->nMSBValue == 0 ) nIncrement =12;
		else nIncrement = pow(2.0,12.0) / (m_pDemodulator->nMSBValue);
		nIncrement = LOG2(nIncrement);
		int nMin = 19 - 2*LOG2((m_pAnalogProp->nRegCICRating)/65536.0);
		//if (nInc <  -nMin) nInc = -nMin;
		//else if (nInc >  (19-(pMod->m_Property.MsbTrunc))) nInc = (19-(pMod->m_Property.MsbTrunc));
		int iGain = (m_pAnalogProp->nRegMSBTrunc + nIncrement);
		if (iGain < nMin ) iGain = nMin;
		else if (iGain > 18) iGain=18;
		m_pDemodulator->ResetCalcMSB();
		if (iGain == m_pAnalogProp->nRegMSBTrunc ){
			return;//the same trunc
		}

		m_pAnalogProp->nRegMSBTrunc  = iGain;
		SetHardwareParameters();
	}//func
}//namespace