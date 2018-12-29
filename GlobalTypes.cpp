#include "StdAfx.h"
#include "GlobalTypes.h"
//#include "SharedVariables.h"

namespace NS_TOOLS
{
	char CGlobalTypes::strEnumEventsCategory[eNumberCatEvent][30]=
	{"Error","Warning","Event"};
	char CGlobalTypes::strEnumFormsCategory[eNumberCatForms][30]=
	{"Monitoring","Scan","Database","Vor"};
	char CGlobalTypes::strenumModulationType[eNumberModulation][30]=
	{"AM","FM","SSB","USB","LSB","CW","BFSK","4FSK","BPSK","QPSK","8PSK","OQPSK","DBPSK","pi2-DBPSK","DQPSK","Direct","QAM16","QAM32","QAM64","Unknown"};
	char CGlobalTypes::strenumScanPriorityType[eNumberScanPriority][30]=
	{"High","Low","None"};
	char CGlobalTypes::strenumFilterBandwidth[eNumberFilter][30]=
	{"1kHz","2kHz","3kHz","4kHz","6kHz","8kHz","10kHz","15kHz","20kHz"};
	char CGlobalTypes::strenumPriorityMode[eNumberPriority][30] = 
	{"1","2","3","4","No Priority"};
	char  CGlobalTypes::strenumChannelSpacing[eNumberChannelSpacing][30]=
	{"5kHz","7kHz","10kHz","15kHz","20kHz"};
	char CGlobalTypes::strEnumChannelBandwidth[eNumberChannelBW][30] =
		//{"5.00 KHz","10.0 KHz","25.0 KHz","50.0 MHz","0.10 MHz","0.25 MHz","0.50 MHz","1.00 MHz","2.50 MHz","5.00 MHz"};
	{"7.00KHz","20.0KHz","30.0KHz","0.25MHz","0.50MHz","1.00MHz","5.00MHz"};//

	char CGlobalTypes::strEnumSynthName[eNumberSynthNames][30] = 
	{"Synth_HF_01","Synth_HF_02","Synth_HF_03","Synth_HF_04","Synth_HF_05","Synth_HF_06"};

	char CGlobalTypes::strEnumTunerName[eNumberTunerNames][30] = 
	{"HF-TUNER","HF-TUNER01","HF-TUNER02","HF-TUNER03","HF-TUNER04","HF-TUNER05","HF-TUNER06"};


	char CGlobalTypes::strEnumAnttenaPolarization[eNumberAnttenaPolarization][30]=
	{"Horizontal","Vertical"};
	char CGlobalTypes::strEnumChannelMode[eNumberChannelModes][30]=
	{"Analog","Digital"};

	char CGlobalTypes::strEnumAmrAcc[eNumberAmrAcc][30]=
	{"Low","Normal","High"};
	int  CGlobalTypes::nNumberEnumAmrAcc[eNumberAmrAcc]=
	{
		1,5,10
	};

	char CGlobalTypes::strEnumAMRBandwidth[eNumberAMRBandWidth][30]=
	{"4 KHz","8 KHz","16 KHz","20 KHz","40 KHz"};
	char CGlobalTypes::strEnumChannelFFT[eNumberChannelFFT][30]=
	{"16K","8K","4K","2K","1K"};
	char CGlobalTypes::strEnumQueryFilter[eNumberQueryFilter][30]=
	{
		"ID","UserID","SessionID","ModulationType","HardwareIndex","StartTime","StopTime","Center","Bandwidth","HopRate",
		"FilePath","Description"};

		char CGlobalTypes::strEnumQueryCondition[eNumberQueryCondition][30]=
		{
			"=",">",">=","<","<=","Between","Like"
		};

		//static functions
		char *CGlobalTypes::GetEnumEventStr(enumEventsCategory nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strEnumEventsCategory[(UINT) nEvent];
		}

		char *CGlobalTypes::GetEnumFormStr(enumFormsCategory nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strEnumFormsCategory[(UINT) nEvent];
		}

		char *CGlobalTypes::GetEnumModulationStr(enumModulationType nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strenumModulationType[(UINT) nEvent];
		}
		char *CGlobalTypes::GetEnumScanPriority(enumScanPriorityType nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strenumScanPriorityType[(UINT) nEvent];
		}

		char *CGlobalTypes::GetEnumFilterBW(enumFilterBandwidth nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strenumFilterBandwidth[(UINT) nEvent];
		}

		char *CGlobalTypes::GetEnumPriorityStr(enumPriorityMode nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strenumPriorityMode[(UINT) nEvent];
		}

		char *CGlobalTypes::GetEnumChannelSpacing(enumFullbandChannelSpacing nEvent)
		{
			if (nEvent<0) return _T("");
			return CGlobalTypes::strenumChannelSpacing[(UINT) nEvent];
		}

		char *CGlobalTypes::GetEnumChannelBandwidthStr(enumChannelBandwith nBW)
		{
			if (nBW<0) return _T("");
			return CGlobalTypes::strEnumChannelBandwidth[(UINT) nBW];
		}

		char *CGlobalTypes::GetEnumSynthesizerName(enumSynthesizerName nSynthName)
		{
			if (nSynthName<0) return _T("");
			return CGlobalTypes::strEnumSynthName[(UINT) nSynthName];
		}

		char *CGlobalTypes::GetEnumTunerName(enumTunerName nTunerName)
		{
			if (nTunerName<0) return _T("");
			return CGlobalTypes::strEnumTunerName[(UINT) nTunerName];
		}

		char *CGlobalTypes::GetEnumAnttenaPolarizationStr(enumAnttenaPolarization nPolar)
		{
			if (nPolar<0) return _T("");
			return CGlobalTypes::strEnumAnttenaPolarization[(UINT) nPolar];
		}
		char *CGlobalTypes::GetEnumChannelModeStr(enumChannelMode nMode)
		{
			if (nMode<0) return _T("");
			return CGlobalTypes::strEnumChannelMode[(UINT) nMode];
		}

		char *CGlobalTypes::GetAccuracyAmr(enumAmrAcc nMode)
		{
			if (nMode<0) return _T("");
			return CGlobalTypes::strEnumAmrAcc[(UINT) nMode];
		}


		char *CGlobalTypes::GetEnumAMRBandwidthStr(enumAMRBandwidth nBW)
		{
			if (nBW<0) return _T("");
			return CGlobalTypes::strEnumAMRBandwidth[(UINT) nBW];
		}
		char *CGlobalTypes::GetEnumChannelResolutionStr(enumChannelResolution nRes)
		{
			if (nRes<0) return _T("");
			return CGlobalTypes::strEnumChannelFFT[(UINT) nRes];
		}
		char *CGlobalTypes::GetEnumQueryFilterStr(enumQueryFilter nRes)
		{
			if (nRes<0) return _T("");
			return CGlobalTypes::strEnumQueryFilter[(UINT) nRes];
		}
		char *CGlobalTypes::GetEnumQueryConditionStr(enumQueryCondition nRes)
		{
			if (nRes<0) return _T("");
			return CGlobalTypes::strEnumQueryCondition[(UINT) nRes];
		}

		int CGlobalTypes::StrToForm(CString str)
		{
			for (int i=0;i<(eNumberCatForms-1);i++)
			{
				if (str.Compare(CGlobalTypes::strEnumFormsCategory[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}
		int CGlobalTypes::StrToChannelBandwidth(CString str)
		{
			for (int i=0;i<eNumberChannelBW;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumChannelBandwidth[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}

		//int CGlobalTypes::StrToSynthesizerName(CString str)
		//{
		//	for (int i=0;i<eNumberSynthNames;i++)
		//	{
		//		if (str.Compare(CGlobalTypes::strEnumSynthName[i]) == 0)
		//			return (i);
		//	}
		//	return -1;//invalid string
		//}

		int CGlobalTypes::StrToModulationType(CString str)
		{
			for (int i=0;i<eNumberModulation;i++)
			{
				if (str.Compare(CGlobalTypes::strenumModulationType[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}

		int CGlobalTypes::StrToScanPriorityType(CString str)
		{
			for (int i=0;i<eNumberScanPriority;i++)
			{
				if (str.Compare(CGlobalTypes::strenumScanPriorityType[i]) == 0)
					return (i);
			}
			return -1;//invalid string

		}

		int CGlobalTypes::StrToBWFilter(CString str)
		{
			for (int i=0;i<eNumberFilter;i++)
			{
				if (str.Compare(CGlobalTypes::strenumFilterBandwidth[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}

		int CGlobalTypes::StrToPriorityMode(CString str)
		{
			for (int i=0;i<eNumberPriority;i++)
			{
				if (str.Compare(CGlobalTypes::strenumPriorityMode[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}

		int CGlobalTypes::StrToAnttenaPolarization(CString str)
		{
			for (int i=0;i<eNumberChannelBW;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumAnttenaPolarization[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}

		int CGlobalTypes::StrToChannelMode(CString str)
		{
			for (int i=0;i<eNumberChannelModes;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumChannelMode[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}
		int CGlobalTypes::StrToAMRBandwidth(CString str)
		{
			for (int i=0;i<eNumberAMRBandWidth;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumAMRBandwidth[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}
		int CGlobalTypes::StrToChannelResolution(CString str)
		{
			for (int i=0;i<eNumberChannelFFT;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumChannelFFT[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}
		int CGlobalTypes::StrToQueryFilter(CString str)
		{
			for (int i=0;i<eNumberQueryFilter;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumQueryFilter[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}
		int CGlobalTypes::StrToQueryCondition(CString str)
		{
			for (int i=0;i<eNumberQueryCondition;i++)
			{
				if (str.Compare(CGlobalTypes::strEnumQueryCondition[i]) == 0)
					return (i);
			}
			return -1;//invalid string
		}

		enumChannelBandwith CGlobalTypes::Bandwidth2Enum(double m_fBW)
		{//nearest BW
			if (m_fBW<=7e3)	return eChannelBW7x0KHz;
			else if (m_fBW<=20e3) return eChannelBW20x0KHz;
			else if (m_fBW<=30e3) return eChannelBW30x0KHz;
			else if (m_fBW<=50e3) return eChannelBW50x0KHz;
			//else if (m_fBW<=0.g_iINPUT_FREQ_UNIT[(int)m_pConfig->m_stInputFile.enFreqUnit]) return eChannelBW0x1MHz;
			else if (m_fBW<=0.25e6) return eChannelBW0x25MHz;
			else if (m_fBW<=0.5e6) return eChannelBW0x5MHz;
			else if (m_fBW<=1.0e6) return eChannelBW1x0MHz;
			//else if (m_fBW<=2.5e6) return eChannelBW2x5MHz;
			else return eChannelBW5x0MHz;
		}

		enumModulationCategory CGlobalTypes::GetModulationCategory(enumModulationType nType)
		{
			switch(nType)
			{
			//case eModulationAF:
			case eModulationAM:
			case eModulationFM:
			case eModulationSSB:
			case eModulationLSB:
			case eModulationUSB:
			case eModulationDirect:
			case eModulationCW:
				// 		case eModulationUSB:
				return eModulationCatAnalog;
				break;
			//case eModulationCW:
			//	return eModulationCatMorse;
			//	break;
			case eModulationQPSK:
			case eModulation8PSK:
			case eModulationD8PSK:
			case eModulationOQPSK:
			case eModulationBFSK:
			case eModulation4FSK:
			case eModulationBPSK:
			case eModulationDBPSK:
			case eModulationPi2DBPSK:
			case eModulationDQPSK:
			case eModulationQAM16:
			case eModulationQAM32:
			case eModulationQAM64:
				return eModulationCatDigital;
				break;
			default:
				return eModulationCatUnknown;
				break;
			}
		}

		int CGlobalTypes::GetFFTSize(enumChannelResolution nRes)
		{
			switch (nRes)
			{
			case eChannelFFT16K : return 16384; break;
			case eChannelFFT8K : return 8192; break;
			case eChannelFFT4K : return 4096; break;
			case eChannelFFT2K : return 2048; break;
			case eChannelFFT1K : return 1024; break;
			}
		}

		double CGlobalTypes::GetFilterBW(enumFilterBandwidth nBW)
		{
			switch (nBW)
			{
			case eFilter1k: return (1.0e3);break;
			case eFilter2k: return(2.0e3);break;
			case eFilter3k: return(3.0e3);break;
			case eFilter4k: return(4.0e3);break;
			case eFilter6k: return (6.0e3);break;
			case eFilter8k: return (8.0e3);break;
			case eFilter10k: return (10.0e3);break;
			case eFilter15k: return (15.0e3);break;
			case eFilter20k: return (20.0e3);break;
			}
			return 0.0;//error
		}

		double CGlobalTypes::GetBandwidth(enumChannelBandwith nBW)
		{//based on Hz
			switch (nBW)
			{
			case eChannelBW7x0KHz: return (7.0e3);break;
			case eChannelBW20x0KHz: return(20.0e3);break;
			case eChannelBW30x0KHz: return(30.0e3);break;
			case eChannelBW50x0KHz: return(50.0e3);break;
			case eChannelBW0x25MHz: return (0.25e6);break;
			case eChannelBW0x5MHz: return (0.5e6);break;
			case eChannelBW1x0MHz: return (1.0e6);break;
			case eChannelBW5x0MHz: return (5.0e6);break;
			}
			return 0.0;//error
		}

		double CGlobalTypes::GetChannelSpacing(enumFullbandChannelSpacing nChannelSpacing)
		{//based on kHz
			switch(nChannelSpacing)
			{
			case eChannelSpacing5k: return (5.0e3);break;
			case eChannelSpacing7k: return (7.0e3);break;
			case eChannelSpacing10k: return (10.0e3);break;
			case eChannelSpacing15k: return (15.0e3);break;
			case eChannelSpacing20k: return (20.0e3);break;
			}
			return 0.0;//error
		}

		BOOL CGlobalTypes::IsValidRangeChannel(double center,double bw)
		{
			double m_start = center - bw/2;
			double m_stop = center + bw/2;

//--- chech osa
//			if (m_start < HF_VALID_RANGE_FREQ_MIN || 
//				m_start > HF_VALID_RANGE_FREQ_MAX) return FALSE;
//			if (m_stop < HF_VALID_RANGE_FREQ_MIN || 
//				m_stop > HF_VALID_RANGE_FREQ_MAX) return FALSE;
			return TRUE;
		}

		//m_pOutBuffer must be created outside this func
		void CGlobalTypes::ConvertFFTSamples(DWORD *m_pdwBuffer,int nSize,
			double *m_pOutBuffer)
		{
			int i;
//--- chech osa
//			ASSERT (CSharedVariables::m_FPGA.m_pHardwareProp);
			for (i=0;i<nSize;i++)
			{
//--- chech osa
//				m_pOutBuffer[i] = 20*log10(0.5+double(m_pdwBuffer[i]))-
//					HF_FFT_LEVEL_OFFSET(CSharedVariables::m_FPGA.m_pHardwareProp->nDataWidth);//-
			}
		}

		BOOL CGlobalTypes::IsModulationAnalog(enumModulationType nModulation)
		{
			switch (nModulation)
			{
			case eModulationAM:
			case eModulationFM:
			case eModulationSSB:
			case eModulationLSB:
			case eModulationUSB:
			case eModulationDirect:
			case eModulationCW:
				return TRUE;break;
			}
			return FALSE;
		}

		BOOL CGlobalTypes::IsModulationDigital(enumModulationType nModulation)
		{
			switch (nModulation)
			{
			case eModulationBPSK:
			case eModulationBFSK:
			case eModulation4FSK:
			case eModulationQPSK:
			case eModulation8PSK:
			case eModulationD8PSK:
			case eModulationOQPSK:
			case eModulationDBPSK:
			case eModulationPi2DBPSK:
			case eModulationDQPSK:
			case eModulationQAM16:
			case eModulationQAM32:
			case eModulationQAM64:
				return TRUE;break;
			}
			return FALSE;
		}

		BOOL CGlobalTypes::IsModulationCW(enumModulationType nModulation)
		{
			return (nModulation == eModulationCW);
		}

		BOOL CGlobalTypes::IsAudioModulation(enumModulationType nModulation)
		{
			switch (nModulation)
			{

			case eModulationAM:
			case eModulationFM:
			case eModulationSSB:
			case eModulationLSB:
			case eModulationUSB:
			case eModulationDirect:
			case eModulationCW:
				return TRUE;break;
			}
			return FALSE;
			//return TRUE;
		}

		int CGlobalTypes::DigitalModulationEnum2Type(enumModulationType nModulation)
		{//-1 on error
			switch(nModulation)
			{
			case eModulationBFSK:  return 0;break;
			case eModulationBPSK:  return 1;break;
			case eModulationOQPSK: return 2;break;
			case eModulationQPSK:  return 3;break;
			case eModulation8PSK:  return 4;break;
			case eModulationDBPSK: return 5;break;
			case eModulationPi2DBPSK: return 6;break;
			case eModulationDQPSK: return 7;break;
			case eModulationD8PSK: return 8;break;
			}
			return -1;
		}


		enumModulationType CGlobalTypes::DigitalModulationType2Enum(int nType)
		{//unknown on error
			switch (nType)
			{
			case 0:return eModulationBFSK;break;
			case 1:return eModulationBPSK;break;
			case 2:return eModulationOQPSK;break;
			case 3:return eModulationQPSK;break;
			case 4:return eModulation8PSK;break;
			case 5:return eModulationDBPSK;break;
			case 6:return eModulationPi2DBPSK;break;
			case 7:return eModulationDQPSK;break;
			case 8:return eModulationD8PSK;break;
			}
			return eModulationUnknown;
		}

		static enumModulationType ModulationType2Enum(int nType)
		{
			switch (nType)
			{
			//case 0:return eModulationAF;break;
			case 0:return eModulationAM;break;
			case 1:return eModulationFM;break;
			case 2:return eModulationSSB;break;
			case 3:return eModulationLSB;break;
			case 4:return eModulationUSB;break;
			case 5:return eModulationCW;break;
			case 6:return eModulationBFSK;break;
			case 7:return eModulationBPSK;break;
			case 8:return eModulationQPSK;break;
			case 9:return eModulation8PSK;break;
			case 10:return eModulationOQPSK;break;
			case 11:return eModulationDBPSK;break;
			case 12:return eModulationPi2DBPSK;break;
			case 13:return eModulationDQPSK;break;
			case 14:return eModulationDirect;break;
			case 15:return eModulationD8PSK;break;
			}
			return eModulationUnknown;
		}



		CGlobalTypes::CGlobalTypes(void)
		{
		} 

		CGlobalTypes::~CGlobalTypes(void)
		{
		}

		//////////////////GENERAL_FORM_PROP////////////////////
		GENERAL_FORM_PROP::GENERAL_FORM_PROP()
		{

			//advanced settings
			m_ADCOffset			=	0;
			m_mask				=	0xC003C;
			m_history_vector	=	0xC0000;
			m_DecisionHistory	=	0xFFF;
			m_HopBand			=	220;
			m_HopAcc			=	6;
			m_HopWaitPeriod		=	10;
			m_hoprateavg		=	2;
			m_actlimit			=	500;
			m_nDelayBeforeDDS	=	16384;
		}

		GENERAL_FORM_PROP::GENERAL_FORM_PROP(const GENERAL_FORM_PROP& m_copy)
		{
			int i ,nSize = m_copy.m_ListHopRates.size();
			m_ListHopRates.clear();
			for (i=0;i< nSize;i++)
				m_ListHopRates.push_back(m_copy.m_ListHopRates[i]);

			//advanced settings
			m_ADCOffset			=	m_copy.m_ADCOffset;
			m_mask				=	m_copy.m_mask;
			m_history_vector	=	m_copy.m_history_vector;
			m_DecisionHistory	=	m_copy.m_DecisionHistory;
			m_HopBand			=	m_copy.m_HopBand;
			m_HopAcc			=	m_copy.m_HopAcc;
			m_HopWaitPeriod		=	m_copy.m_HopWaitPeriod;
			m_hoprateavg		=	m_copy.m_hoprateavg;
			m_actlimit			=	m_copy.m_actlimit;
			m_hoprateavg		=	m_copy.m_hoprateavg;
			m_nDelayBeforeDDS	=	m_copy.m_nDelayBeforeDDS;
		}

		GENERAL_FORM_PROP::~GENERAL_FORM_PROP()
		{
			m_ListHopRates.clear();	
		}

		////////////CHANNEL_FORM_PROP///////////////////////
		CHANNEL_FORM_PROP::CHANNEL_FORM_PROP()
		{
			bLoading	=	FALSE;
			nIndex		=	-1;//form index (added to tab title)
			m_fCenter	=	10.0e6;//MHz
			m_iCenterUnit=	3;//MHz
			nChannelBW	=	eChannelBW1x0MHz;
			nTunerGain	=	0;
			nPolarization=	eAnttenaHoriz;
			nVolume		=	10;
			bMute		=	FALSE;
			nSpectrumNumberAverages	=	1;
			bSpectrumShowThreshold	=	TRUE;
			nChannelMode=	eChannelModeAnalog;
			nAMRBandwidth=	eAMRBandWidth4KHz;
			nChannelResolution	=	eChannelFFT16K;
			nTransmitterID		=	0;//auto
			fRs					=	3.0e3;//KHz
			fDf					=	0.0;
			nTime				=	1;
			//CSharedVariables::m_nTunerChannelBW = nChannelBW;
		}

		CHANNEL_FORM_PROP::CHANNEL_FORM_PROP(const CHANNEL_FORM_PROP& m_copy)
		{
			*this = m_copy;
		}

		const CHANNEL_FORM_PROP &CHANNEL_FORM_PROP::operator=(const CHANNEL_FORM_PROP& m_copy)
		{
			bLoading	=	m_copy.bLoading;
			nIndex		=	m_copy.nIndex;//form index (added to tab title)
			m_fCenter	=	m_copy.m_fCenter;//MHz
			m_iCenterUnit=	m_copy.m_iCenterUnit;
			nChannelBW	=	eChannelBW1x0MHz/*m_copy.nChannelBW*/;
			nTunerGain	=	m_copy.nTunerGain;
			nPolarization=	eAnttenaHoriz;
			nVolume		=	m_copy.nVolume;
			bMute		=	m_copy.bMute;
			fRs			=	m_copy.fRs;
			fDf			=	m_copy.fDf;
			nSpectrumNumberAverages	=	m_copy.nSpectrumNumberAverages;
			bSpectrumShowThreshold	=	m_copy.bSpectrumShowThreshold;
			nChannelMode=	m_copy.nChannelMode;
			nAMRBandwidth=	m_copy.nAMRBandwidth;
			nChannelResolution	=	m_copy.nChannelResolution;
			nTransmitterID		=	m_copy.nTransmitterID;
			nTime				=	m_copy.nTime;
//--- chech osa
//			CSharedVariables::m_nTunerChannelBW = eChannelBW1x0MHz;//m_copy.nChannelBW;
			return *this;
		}

		////////////FULLBAND_FORM_PROP///////////////////////
		FULLBAND_FORM_PROP::FULLBAND_FORM_PROP()
		{
			nIndex		=	-1;//form index (added to tab title)
			m_fStart	=	9.0e6;//MHz
			m_iStartUnit=	3;//MHz
			m_fStop		=	10.0e6;//MHz
			m_iStopUnit =	3;//MHz
			nTunerGain	=	0;
			nChannelBW	=	eChannelBW1x0MHz;
			nPolarization=	enumAnttenaPolarization::eAnttenaHoriz;
			nVolume		=	10;
			bMute		=	FALSE;
			nSpectrumNumberAverages	=	1;
			bSpectrumShowThreshold	=	TRUE;
			nChannelResolution	=	eChannelFFT16K;
			nChannelSpacing = eChannelSpacing20k;
		}

		FULLBAND_FORM_PROP::FULLBAND_FORM_PROP(const FULLBAND_FORM_PROP& m_copy)
		{
			nIndex			=	m_copy.nIndex;//form index (added to tab title)
			m_fStart		=	m_copy.m_fStart;//MHz
			m_iStartUnit	=	m_copy.m_iStartUnit;
			m_fStop			=	m_copy.m_fStop;//MHz
			m_iStopUnit		=	m_copy.m_iStopUnit;
			nTunerGain		=	m_copy.nTunerGain;
			nChannelBW		=	m_copy.nChannelBW;
			nPolarization	=	m_copy.nPolarization;
			nVolume			=	m_copy.nVolume;
			bMute			=	m_copy.bMute;
			nChannelSpacing = m_copy.nChannelSpacing;
			nSpectrumNumberAverages	=	m_copy.nSpectrumNumberAverages;
			bSpectrumShowThreshold	=	m_copy.bSpectrumShowThreshold;
			nChannelResolution		=	m_copy.nChannelResolution;
		}

		////////////////////SESSION_PROP/////////////////////////////
		SESSION_PROP::SESSION_PROP()
		{
			//-------------Options------------------
			//[General]
			//[Appearance]
			m_strPathDefaultLayoutChannel	=	_T("");
			m_strPathDefaultLayoutFullBand	=	_T("");
			m_strPathDefaultLayoutDatabase	=	_T("");
			//[Hardware]
			m_strPathBitFileClock	=	_T("");
			m_strPathBitFileAnalong[0]	=	_T("");
			m_strPathBitFileAnalong[1]	=	_T("");
			m_strPathBitFileDigital	=	_T("");
			m_strPathW61			=	_T("");
			m_strPathMediaPlayer	=	_T("");
			m_strPathDatabase		=	_T("");
			m_strPathOutput			=	_T("");
			m_strDatabaseConnection	=	_T("Provider= SQLOLEDB;Data Source=.\\SQLEXPRESS;AttachDbFilename=F:\\Projects\\HF\\HF_Revise\\BCGPHFdb\\BCGPHFdb_Data.MDF;Integrated Security=SSPI");
			m_nSynthesizerComPort	=	10;
			m_nSynthesizerComSpeed	=	38400;

		}

/*		//////////////////////////////////////////////////
		CConfigListItem::CConfigListItem()
		{
			m_fFrequency		=	10000;
			m_strName			=	_T("New Channel");
			m_iId				=	0;
			m_fHopRate			=	0;
			m_nAntennaPolarization=	eAnttenaHoriz;
			m_fBandwidth		=	3.5;
			m_nModulation		=	eModulationUnknown;
			m_nPriority			-	eScanNone;
			m_fFcEst			=	1200;
			m_fFineTune			=	0;
			m_strUser			=	_T("");
			m_strDescp			=	_T(" ");
			m_bSave				=	FALSE;
		}

		CConfigListItem::CConfigListItem(const CConfigListItem& stItem)
		{
			m_fFrequency			=	stItem.m_fFrequency;
			m_strName				=	stItem.m_strName;
			m_strDescp				=	stItem.m_strDescp;
			m_iId					=	stItem.m_iId;
			m_ObjTime				=	stItem.m_ObjTime;
			m_strName				=	stItem.m_strName;
			m_nAntennaPolarization	=	stItem.m_nAntennaPolarization;
			m_fBandwidth			=	stItem.m_fBandwidth;
			m_nModulation			=	stItem.m_nModulation;
			m_nPriority				=	stItem.m_nPriority;
			m_fHopRate				=	stItem.m_fHopRate;
			m_fFcEst				=	stItem.m_fFcEst;
			m_fFineTune				=	stItem.m_fFineTune;
			m_strUser				=	stItem.m_strUser;
			m_bSave					=	stItem.m_bSave;
		}

		CConfigListItem::CConfigListItem(const FullbandTable& stTable)
		{
			m_iId					=	stTable.m_iId;
			m_ObjTime				=	stTable.m_ObjTime;
			m_fFrequency			=	stTable.m_iFreqCount;
			m_strName				=	stTable.m_strName;
			m_fBandwidth			=	stTable.m_fBandwidth ;
			m_nAntennaPolarization	= (enumAnttenaPolarization)stTable.m_iAntenna;
			m_nModulation			=	 stTable.m_nModulation;
			m_nPriority				=	 stTable.m_nPriority;
			m_fHopRate				=	 stTable.m_fHopRate;
			m_fFcEst				=	 stTable.m_fSymbolRate;
			m_fFineTune				=	 stTable.m_fFineTune;
			m_strUser				=	 stTable.m_strUser;
			m_bSave					=	 stTable.m_bSave;
		}
		///////////////////////////////////////////////////
		/////////User Log Cell List///////////////////////
		CUserLogListItem::CUserLogListItem()
		{
			m_iId = 0;
			m_strUser = _T("");
			m_UserStartTime.SetDateTime(1,1,1,1,1,1);
			m_UserStopTime.SetDateTime(1,1,1,1,1,1);
			m_nPlayNumber = 0;
			m_dblPlayTime = 0;
			m_nRecordNumber = 0;
			m_dblRecordTime = 0;
		}

		CUserLogListItem::CUserLogListItem(const CUserLogListItem& stItem)
		{
			m_iId			= stItem.m_iId;
			m_strUser	    = stItem.m_strUser;
			m_UserStartTime = stItem.m_UserStartTime;
			m_UserStopTime  = stItem.m_UserStopTime;
			m_nPlayNumber   = stItem.m_nPlayNumber;
			m_dblPlayTime   = stItem.m_dblPlayTime;
			m_nRecordNumber = stItem.m_nRecordNumber;
			m_dblRecordTime = stItem.m_dblRecordTime;	
		}

		CUserLogListItem::CUserLogListItem(const StatisticJobUsers& stTable)
		{
			m_iId					=	stTable.m_iId;
			m_strUser				=	stTable.m_strUser;
			m_UserStartTime			=	stTable.m_UserStartTime;
			m_UserStopTime			=	stTable.m_UserStopTime;
			m_nPlayNumber			=	stTable.m_nPlayNumber ;
			m_dblPlayTime			=   stTable.m_dblPlayTime;
			m_nRecordNumber			=	stTable.m_nRecordNumber;
			m_dblRecordTime			=	stTable.m_dblRecordTime;
		}
		///////////////////////////////////////////////////

		//////////////////////////////////////////////////
		CRecordListItem::CRecordListItem()
		{
			m_bPlay				=	FALSE;
			m_nID				=	-1; //records ID
			m_nModulationType	=	12;//modulation Type
			m_fCenter			=	0; //center frequency
			m_fBandwidth		=	0; //bandwidth
			m_fHopRate			=	0; //hop rate
			m_fDuration			=	0.0; //duration
			m_fFineTune			=	0.0;//Fine tune
			m_fSymbolRate		=	0.0;//symbol rate
			m_DateTime.SetDateTime(1,1,1,1,1,1);//start time
			m_strName		    =	_T("Unknown");//Name
			m_sFilePath			=	_T("");//file path
			m_sDescription		=	_T(" ");//description
			m_nRecordNumbers	=	1;
			m_sUser             =   _T(" ");//user name
		}

		CRecordListItem::CRecordListItem(const CRecordListItem& stItem)
		{
			m_bPlay				=	FALSE;
			m_nID				=	stItem.m_nID;
			m_nModulationType	=	stItem.m_nModulationType;
			m_fCenter			=	stItem.m_fCenter;
			m_fBandwidth		=	stItem.m_fBandwidth;
			m_fHopRate			=	stItem.m_fHopRate;
			m_fDuration			=	stItem.m_fDuration;
			m_fFineTune			=	stItem.m_fFineTune;
			m_fSymbolRate		=	stItem.m_fSymbolRate;
			m_strName			=	stItem.m_strName;
			m_sFilePath			=	stItem.m_sFilePath;
			m_sDescription		=   stItem.m_sDescription;
			m_sUser             =   stItem.m_sUser;
			m_DateTime			=	stItem.m_DateTime;
			m_nRecordNumbers		=	stItem.m_nRecordNumbers;
		}

		CRecordListItem::CRecordListItem(const RecordsTable&  stTable)
		{
			m_bPlay				=	FALSE;
			m_nID				=	stTable.m_nID;
			m_nModulationType	=	stTable.m_nModulationType;
			m_fCenter			=	stTable.m_fCenter;
			m_fBandwidth		=	stTable.m_fBandwidth;
			m_fHopRate			=	stTable.m_fHopRate;
			m_fDuration			=	stTable.m_fDuration;
			m_fSymbolRate		=	stTable.m_fSymbolRate;
			m_strName			=	stTable.m_strName;
			m_sFilePath			=	stTable.m_sFilePath;
			m_sDescription		=   stTable.m_sDescription;
			m_sUser             =   stTable.m_sUser;
			m_DateTime			=	stTable.m_DateTime;
			m_nRecordNumbers	=	stTable.m_nRecordNumbers;
			m_fFineTune			=	stTable.m_fFineTune;
		}
		///////////////////////////////////////////////////
		CFullbandFreqTableItem::CFullbandFreqTableItem()
		{
			m_nIdx				=	0;
			m_fFrequency		=	10000000;
			m_fFcEst			=	1200;
			m_fBandwidth		=	0.5;
			m_fHopRate			=	0;
			m_nPersistence		=	0;
			m_nModulation		=	eModulationUnknown;
			m_nPriority			=	eScanNone;
			m_nAntenna			=	eAnttenaHoriz;
			m_strName			=	_T("NewChannel");
			m_strDescription	=	_T(" ");
			m_bSave				=	FALSE;
			m_nRecordsNumber	=	1;
		}

		CFullbandFreqTableItem::CFullbandFreqTableItem(const CFullbandFreqTableItem& stItem)
		{
			m_fFrequency		=	stItem.m_fFrequency;
			m_fBandwidth		=	stItem.m_fBandwidth;
			m_nModulation		=	stItem.m_nModulation;
			m_fHopRate			=	stItem.m_fHopRate;
			m_fFcEst			=	stItem.m_fFcEst;
			m_fFineTune			=	stItem.m_fFineTune;
			m_nAntenna			=	stItem.m_nAntenna;
			m_ObjTime			=	stItem.m_ObjTime;
			m_sttTimeDate		=	stItem.m_sttTimeDate;
			m_nIdx				=	stItem.m_nIdx;
			m_nPriority			=	stItem.m_nPriority;
			m_strName			=	stItem.m_strName;
			m_strDescription	=	stItem.m_strDescription;
			m_nPersistence		=	stItem.m_nPersistence;
			m_bSave				=	stItem.m_bSave;
			m_nRecordsNumber	=	stItem.m_nRecordsNumber;
		}

		CFullbandFreqTableItem::CFullbandFreqTableItem(const FullbandTable& stTable)
		{
			m_fFrequency		=	stTable.m_iFreqCount;
			m_fBandwidth		=	stTable.m_fBandwidth;
			m_nModulation		=	stTable.m_nModulation;
			m_fHopRate			=	stTable.m_fHopRate;
			m_fFcEst			=	stTable.m_fSymbolRate;
			m_fFineTune			=	stTable.m_fFineTune;
			m_ObjTime			=	stTable.m_ObjTime;
			m_sttTimeDate		=	stTable.m_strTimeDate;
			m_nIdx				=	stTable.m_iId;
			m_strName			=	stTable.m_strName;
			m_strDescription	=	stTable.m_strDescp;
			m_nPriority			=	stTable.m_nPriority;
			m_bSave				=	stTable.m_bSave;
			m_nRecordsNumber	=	stTable.m_nRecordsNumber;
		}

		void CFullbandFreqTableItem::FillDBTable(FullbandTable &stTable,int nConfigID,int nFreqTableID)
		{
			stTable.m_iFreqCount	=	m_fFrequency;	
			stTable.m_fBandwidth	=	m_fBandwidth;
			//stTable.m_iId			=	nFreqTableID;
			stTable.m_iId			=	nConfigID;
			stTable.m_nPriority		=	m_nPriority;
			stTable.m_nRecordsNumber =	m_nRecordsNumber;
		}

		COLORREF CFullbandFreqTableItem::GetColor(int nPersistence)
		{
			if (nPersistence == 1)
			{
				return RGB(140,140,140);
			}else if (nPersistence==2){
				return RGB(220,107,119);
			}else if (nPersistence==3){
				return RGB(0,0,0);
			}else if (nPersistence==4){
				return RGB(74,156,35);
			}else{
				return RGB(250,100,100);
			}
		}

		//-------------------Database----------------------------------//
		//-------User Policy List-------------//
		//A Record of database user policy table
		CUserPolicyList::CUserPolicyList()
		{
			m_bCanOpenNewFormChannel	=	TRUE; //can open new form Monitoring
			m_bCanOpenNewFormFullBand	=	TRUE; //can open new form Scan
			m_bCanOpenNewFormDatabase	=	TRUE; //can open new form database
			//user policies
			m_bCanEditUsers = FALSE;//Admin
			m_bCanEditDatabase = FALSE;//Can Edit Item Saved by Database			
			m_bCanEditAdvancedSettings = FALSE;//Can Edit Advanced Settings
			m_bViewOtherUsersRecordedFiles = FALSE;//Can View Other Users Files
			m_bViewOtherUsersConfigurations = FALSE;
		}//func

		CUserPolicyList::CUserPolicyList(const CUserPolicyList& stItem)
		{
			m_bCanOpenNewFormChannel	=	stItem.m_bCanOpenNewFormChannel; //can open new form Monitoring
			m_bCanOpenNewFormFullBand	=	stItem.m_bCanOpenNewFormFullBand; //can open new form Scan
			m_bCanOpenNewFormDatabase	=	stItem.m_bCanOpenNewFormDatabase; //can open new form database
			//user policies
			m_bCanEditUsers = stItem.m_bCanEditUsers;//Admin
			m_bCanEditDatabase = stItem.m_bCanEditDatabase;//Can Edit Item Saved by Database			
			m_bCanEditAdvancedSettings = stItem.m_bCanEditAdvancedSettings;//Can Edit Advanced Settings
			m_bViewOtherUsersRecordedFiles = stItem.m_bViewOtherUsersRecordedFiles;//Can View Other Users Files
			m_bViewOtherUsersConfigurations = stItem.m_bViewOtherUsersConfigurations;
		}//func


		CUserPolicyList::CUserPolicyList(const UserPolicyTable& stTable)
		{
			m_bCanOpenNewFormChannel	=	stTable.m_bCanOpenNewFormChannel; //can open new form Monitoring
			m_bCanOpenNewFormFullBand	=	stTable.m_bCanOpenNewFormFullBand; //can open new form Scan
			m_bCanOpenNewFormDatabase	=	stTable.m_bCanOpenNewFormDatabase; //can open new form database
			//user policies
			m_bCanEditUsers = stTable.m_bCanEditUsers;//Admin
			m_bCanEditDatabase = stTable.m_bCanEditDatabase;//Can Edit Item Saved by Database			
			m_bCanEditAdvancedSettings = stTable.m_bCanEditAdvancedSettings;//Can Edit Advanced Settings
			m_bViewOtherUsersRecordedFiles = stTable.m_bViewOtherUsersRecordedFiles;//Can View Other Users Files
			m_bViewOtherUsersConfigurations = stTable.m_bViewOtherUsersConfigurations;//Can View Other Users Configurations

		}//func


		//-------------Query---------------------//
		CQueryFilter::CQueryFilter()
		{
			m_nFilter	=	eQueryID;
			m_nCondition=	eConditionEQ;
			m_nStrValue[0]=_T("");//used for string/date conditions
			m_nStrValue[1]=_T("");//used for string/date conditions
		}
		CQueryFilter::CQueryFilter(const CQueryFilter &stCopy)
		{
			CopyFrom(&stCopy);
		}

		void CQueryFilter::CopyFrom(const CQueryFilter *pSource)
		{
			m_nFilter		=	pSource->m_nFilter;
			m_nCondition	=	pSource->m_nCondition;
			m_nStrValue[0]	=pSource->m_nStrValue[0];//used for string/date conditions
			m_nStrValue[1]	=pSource->m_nStrValue[1];//used for string/date conditions	
		}

		CQueryFilter::~CQueryFilter()
		{
		}

		BOOL CQueryFilter::Load(TiXmlElement *pElement)
		{
			int iVal ;
			const char *pStr;
			if (pElement->QueryIntAttribute( "Type", &iVal ) != TIXML_SUCCESS)
				return FALSE;
			m_nFilter = (enumQueryFilter) iVal;
			if (pElement->QueryIntAttribute( "Condition", &iVal ) != TIXML_SUCCESS)
				return FALSE;
			m_nCondition = (enumQueryCondition) iVal;
			if ((pStr = pElement->Attribute( "Param1")) == NULL)
				return FALSE;
			m_nStrValue[0] = pStr;
			if ((pStr = pElement->Attribute( "Param2")) == NULL)
				return FALSE;
			m_nStrValue[1] = pStr;
		}//func

		TiXmlElement *CQueryFilter::Save(TiXmlElement *pRoot,LPCTSTR lpszNodeName )
		{
			TiXmlElement *pElement = new TiXmlElement(lpszNodeName);
			if (!pElement) return FALSE;
			pElement->SetAttribute("Type", m_nFilter);
			pElement->SetAttribute("Condition", m_nCondition);
			pElement->SetAttribute("Param1", m_nStrValue[0]);
			pElement->SetAttribute("Param2", m_nStrValue[1]);
			if (pRoot)
				pRoot->LinkEndChild( pElement );
			return pElement;
		}//func


		BOOL CQueryFilter::CorrectQueryValue(const CString &str,CString &strResult)
		{
			switch (m_nFilter)
			{
			case eQueryModulationType:
				{
					int nType = CGlobalTypes::StrToModulationType(str);
					if (nType < 0 || nType>= eNumberModulation) return FALSE;
					strResult.Format("%d",(int)nType);
				}
				break;
			default:
				strResult = str;
				break;
			}//switch
			return TRUE;
		}//func

		BOOL CQueryFilter::CreateQuery(CString &strResult,BOOL bTitle)
		{
			CString strTemp;
			strResult = CGlobalTypes::GetEnumQueryFilterStr(m_nFilter);
			if (strResult.IsEmpty()) return FALSE;
			if (!IsValidParameter(0)) return FALSE;
			strResult += _T(" ");
			strResult += CGlobalTypes::GetEnumQueryConditionStr(m_nCondition);
			strResult += _T(" ");
			if (!bTitle)
			{
				if (!CorrectQueryValue(m_nStrValue[0],strTemp)) return FALSE;
				strResult += strTemp;
			}else{
				strResult += m_nStrValue[0];
			}
			strResult += _T(" ");
			if(m_nCondition == eConditionBetween)
			{
				strResult += _T("AND ");
				if (!bTitle)
				{
					if (!CorrectQueryValue(m_nStrValue[1],strTemp)) return FALSE;
					strResult += strTemp;
				}else{
					strResult += m_nStrValue[1];
				}
				strResult += _T(" ");
			}
			return TRUE;
		}//func

		BOOL CQueryFilter::IsValidParameter(int nIndex)
		{
			if (m_nCondition != eConditionBetween && nIndex>0) return FALSE;

			switch (m_nFilter)
			{
			case eQueryID://int queries
			case eQueryHardwareIndex:
				if (m_nCondition == eConditionLike) return FALSE;
				break;//int queries

			case eQueryModulationType://mod type
				if (m_nCondition != eConditionEQ) return FALSE;
				break;

			case eQueryUser://string queries
			case eQuerySession:
			case eQueryPath:
			case eQueryDescription:
				if (m_nCondition != eConditionEQ &&
					m_nCondition != eConditionLike) return FALSE;
				break;//string queries

			case eQueryCenter://double queries
			case eQueryBandwidth:
			case eQueryHopRate:
				if (m_nCondition == eConditionLike) return FALSE;
				break;//double queries

			case eQueryStart://time queries
			case eQueryStop:
				if (m_nCondition == eConditionLike) return FALSE;
				break;//time queries
			default:
				return FALSE;
			}
			return TRUE;
		}///func

		BOOL CQueryFilter::IsTime()
		{
			switch (m_nFilter)
			{
			case eQueryStart:
			case eQueryStop:
				return TRUE;break;
			}
			return FALSE;
		}
		BOOL CQueryFilter::IsInteger()
		{
			switch (m_nFilter)
			{
			case eQueryID://int queries
			case eQueryModulationType:
			case eQueryHardwareIndex:
				return TRUE;break;
			}
			return FALSE;
		}
		BOOL CQueryFilter::IsDouble()
		{
			switch (m_nFilter)
			{
			case eQueryCenter://double queries
			case eQueryBandwidth:
			case eQueryHopRate:
				return TRUE;break;
			}
			return FALSE;
		} 

		BOOL CQueryFilter::IsString()
		{
			switch (m_nFilter)
			{
			case eQueryUser://string queries
			case eQuerySession:
			case eQueryPath:
			case eQueryDescription:
				return TRUE;break;
			}
			return FALSE;
		}

*/
}//namespace

int	GetSampleID(DWORD m_dwSample)
{ 
	return (m_dwSample >> 29);
}