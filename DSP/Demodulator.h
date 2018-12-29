#pragma once

#include "DemodulatorBase.h"

namespace NS_DSP
{
	class ANALOG_MODULATION_PROP
	{
	public:
		UINT	nRegControl;
		int		nRegFreqOffset;
		double	nRegCICRating;
		UINT	nRegMSBTrunc;
//		UINT	nRegCWConstant;
//		UINT	nRegCWThreshold;

		ANALOG_MODULATION_PROP();
		ANALOG_MODULATION_PROP(const ANALOG_MODULATION_PROP& m_copy);
	};

	class DIGITAL_MODULATION_PROP
	{
	public:
		UINT	nRegControl;
		UINT	nRegOptimumBit;
		//UINT	nRegNumberSample;
		UINT	nRegR;
		UINT	nRegRInt;
		UINT	nRegMu0;
		UINT	nRegLu;
		UINT	nRegNCombDelay;
		UINT	nRegG1;
		UINT	nRegG2;
		UINT	nRegGShift;
		UINT	nRegFc;
		UINT	nRegKP;
		UINT	nRegKI1;
		UINT	nRegKI2;
		UINT	nRegFreqEstimationCoef;
		UINT	nRegFreqEstimationTrunc;

		DIGITAL_MODULATION_PROP();
		DIGITAL_MODULATION_PROP(const DIGITAL_MODULATION_PROP& m_copy);
	};

	class CDemodulatorItem
	{
	public:
		CDemodulatorItem(void);
		~CDemodulatorItem(void);
		static BOOL LoadFIRCoefs();
		static void UnLoadFIRCoefs();
		void StopDemodulator();
		void Release();
		BOOL Create(int m_nID,enumModulationType m_nType,int m_nRow,int bHopping,int m_nHWTableIndex=3);
		void CalcAnalog(double m_fCenter,double m_fBW,double m_fDeltaF);
		void CalcDigital(double m_fCenter,double m_fBW,double m_fDeltaF,double m_fBitRate);
		void SetHardwareParameters();
		void Reset();
		BOOL Demodulate(DWORD *m_pRawBuffer,int nSize);
		BOOL Record(BOOL bStart=TRUE);

		static CDemodulatorItem* GetAudioPlayingItem() {return m_pAudioPlayingItem;}
		static void SetAudioPlaynigItem(CDemodulatorItem *pItem) {m_pAudioPlayingItem=pItem;} 
		void StartAudio();
		void StopAudio();
		void NormalizeAudio();

	public:
		enumModulationCategory		nCategory; 
		enumModulationType			nType;
		BOOL						bIsPlaying;
		BOOL						bIsRecording;
		BOOL						bIsHopping;
		int							nID; //our Demod ID 
		int							nHWTableIndex;//id in FPGA (for hopping only)
		int							nRow;//Row in channels list

		ANALOG_MODULATION_PROP		*m_pAnalogProp;
		DIGITAL_MODULATION_PROP		*m_pDigitalProp;
		CDemodulatorBase			*m_pDemodulator;
		CBCGPDockingControlBar		*m_pWndContents;//pointer to morse/digital window
		static CDemodulatorItem		*m_pAudioPlayingItem;//pointer to active playing audio
		
		//Loaded FIR Coefs Status
		static BOOL					m_bIsLoadedFIRCoefsAM;
		static BOOL					m_bIsLoadedFIRCoefsAF;
		static BOOL					m_bIsLoadedFIRCoefsFM;
		static BOOL					m_bIsLoadedFIRCoefsSSB;
		static BOOL					m_bIsLoadedFIRCoefsCW;
		static BOOL					m_bIsLoadedFIRCoefsAMR;
	};

}//namespace
