#pragma once

#include "DSP/dsplib.h"
#include "GlobalTypes.h"
//#include "../BCGP_HF/SoundRecord.h"

#define myMax(x,y)     ( ( x ) > ( y ) ? ( x ) : ( y ) )
#define myMin(x,y)	   ( ( x ) < ( y ) ? ( x ) : ( y ) )
#define PI 3.1415926535897932384626433832795

struct DEMOD_PARAM 
{
	double Fs;
	double Fc;
	double Rs;	 
	double Rs2;	 
	int M;
	int P;
};

using namespace NS_TOOLS;

//HF Project Demodulator
namespace NS_DSP
{
	class CDemodulatorBase
	{
	public:
		CDemodulatorBase(void);
		virtual ~CDemodulatorBase(void);

		//calculate max MSB
		inline int GetCalcMSBNumberSamples() {return nCalcMSBNumberSamples;}
		inline void SetCalcMSBNumberSamples(int n) {nCalcMSBNumberSamples=n;}
		inline BOOL IsCalculatingMSB() {return (nCalcMSBCounter<nCalcMSBNumberSamples);}
		void		ResetCalcMSB(); 

		//record
		static CString  GetRecordRootPath() {return m_strRecordRootPath;}
		static CString  CreateRecordDate();
		void			SetRecordRootPath(CString path){m_strRecordRootPath=path;} 
		CString			CreateRecordFileName(double fFreq);
//		inline BOOL		IsRecording(){return m_Record.IsOpen();}
//		inline char*	GetLastRecordError() {return m_Record.GetLastError();}
		static BOOL		CreateRecordRootDirectory();

		//overrides
		virtual BOOL	IsLoadedFIRCoef() =0;
		virtual BOOL	Create(DEMOD_PARAM stParam);
		virtual void	Reset();
		virtual void	Release()=0;
		virtual void	InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)=0;
		virtual void	AddRecordedBuffer(double *pdInBuffer, int nSize)=0;
		virtual void	CalculateMSB()=0;
		virtual	void	NormalizeAudio()=0;
		virtual BOOL	Demodulate()=0;
		virtual BOOL	Record(BOOL bStart,double fFreq = 0);
		virtual BOOL	RecordSamples();
		virtual BOOL	PlayOut(LPARAM lParam=NULL)=0;
		virtual void	DigitStat(BOOL bIsRecording);//,BOOL bIsPlaying,double fRs,double fFineTune,double fDevFreq,double fFreq);
		virtual double  CarrierEstimate();
		virtual void	SelectFilter(enumFilterBandwidth nFilter,double fFineTune,enumModulationType nModulation,double fNewInpFrequency,BOOL bIsSetCarrier = FALSE);

		inline enumModulationType GetType() {return nType;}
		inline void SetType(enumModulationType t){nType = t;}
		inline void SetHopping(BOOL bHopping) {bFixed = !bHopping;}

		void SetScatterBuffer(MKL_Complex16 *m_fBuffer,int nSize);

		MKL_Complex16 *m_fBufferScatter;
		int m_nSizeScatter;

		bool RecordSymbol(double *pdBufferSymbol, int iSizeSymbol, int iSizeBit);
		void ChangeOutputIQ(double dI, double dQ, double& dNewI, double& dNewQ);
		//	BYTE ChangeSumbolFormat(BYTE nSymbol, int iBitNum);

	public:
		DEMOD_PARAM m_stParam;
		int			nID;//Demodulator ID in FPGA
		//calculate max
		int			nCalcMSBCounter;
		int			nCalcMSBNumberSamples;
		double		m_fMSBValue;
		double		m_fNormalizaionTSCO;
		int			m_nFrequency;
		int			nMSBPosition;
		int				nMSBLocate;
		enumModulationType	nType;
		BOOL				bFixed;
		//record
		static CString	m_strRecordRootPath;
		CString			m_strRecordPath;
//		CSoundRecord	m_Record;
		BOOL			m_bRecordIQ;
		double			m_fSnr;

		int			nTunedFrequency;
		int			nLastTunedFrequency;

		double		m_fFinalCarrier;
		double      m_fFreq2IQRecordName;


	public:
		double		m_dErrDemodTiming;
		double		m_dErrDemodPhase;



	public:
	//	FILE*		m_pFileRecord;
		CFile		*m_pFileRecord;// m_hFile
	//	SETTING_BITSTREAM *


		BYTE		*m_pnMapping;

		bool		*m_pbInverted;
		bool		*m_pbSwap;
		int			*m_piPhaseShift;
		int			*m_piSizeShift;

		bool		*m_pbWriteSymbolByte;
	};
}//namespace