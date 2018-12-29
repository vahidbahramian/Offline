#pragma once

#include "../BCGP_HF/SoundRecord.h"

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
		void			SetRecordRootPath(CString path){m_strRecordRootPath=path;} 
		CString			CreateRecordFileName();
		inline BOOL		IsRecording(){return m_Record.IsOpen();}
		inline char*	GetLastRecordError() {return m_Record.GetLastError();}
		static BOOL		CreateRecordRootDirectory();

		//overrides
		virtual BOOL IsLoadedFIRCoef() =0;
		virtual BOOL	Create(int nID=-1);
		virtual void	Reset();
		virtual void	Release()=0;
		virtual void	InterpretCardBuffer (DWORD *m_pInBuffer,int nSize)=0;
		virtual void	CalculateMSB()=0;
		virtual BOOL	Demodulate()=0;
		virtual BOOL	Record(BOOL bStart=TRUE)=0;
		virtual BOOL	PlayOut(LPARAM lParam=NULL)=0;

		inline int GetID() {return nID;}
		inline void SetID(int n){nID = n;}
		inline enumModulationType GetType() {return nType;}
		inline void SetType(enumModulationType t){nType = t;}
		inline void SetHopping(BOOL bHopping) {bFixed = !bHopping;}

	public:
		int			nID;//Demodulator ID in FPGA
		//calculate max
		int			nCalcMSBCounter;
		int			nCalcMSBNumberSamples;
		int			nMSBValue;
		int			nMSBPosition;
		enumModulationType	nType;
		BOOL				bFixed;
		//record
		static CString	m_strRecordRootPath;
		CSoundRecord	m_Record;
	};
}//namespace