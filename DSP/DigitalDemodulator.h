#pragma once

#include "DemodulatorBase.h"

class CDigitalDlgBar;
//HF Project Demodulator
namespace NS_DSP
{


	class CDigitalDemodulator : public CDemodulatorBase
	{
	public:
		CDigitalDemodulator(void);
		~CDigitalDemodulator(void);
	
		//overrides
		virtual BOOL	IsLoadedFIRCoef() ;
		virtual BOOL	Create(int nID=-1);
		virtual void	Reset();
		virtual void	Release();
		virtual void	InterpretCardBuffer (DWORD *m_pInBuffer,int nSize);
		virtual void	CalculateMSB();
		virtual BOOL	Demodulate();
		virtual BOOL	Record(BOOL bStart=TRUE);
		virtual BOOL	PlayOut(LPARAM lParam=NULL);
		//statics
		static BOOL LoadFiltersCoefficients();
		static void UnLoadFiltersCoefficients();

		inline void SetDigitalWindow (CDigitalDlgBar *pWnd) {m_pWndDigital=pWnd;}
	public:
		double		*m_fInputBufferI;
		double		*m_fInputBufferQ;
		int			m_nInputBufferLength;
		double		*m_fDemodOutBuffer;
		short		*m_iRecordBuffer;
		int			m_nDemodOutBufferLength;

		CDigitalDlgBar *m_pWndDigital;

	};

}//namespace