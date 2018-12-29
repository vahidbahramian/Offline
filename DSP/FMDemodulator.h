#pragma once
#include "DemodulatorBase.h"
#include "OldFilters.h"

namespace NS_DSP
{


	class CFMDemodulator :	public CDemodulatorBase
	{
	public:
		CFMDemodulator(void);
		virtual ~CFMDemodulator(void);

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

	public:
		double		*m_fInputBufferI;
		double		*m_fInputBufferQ;
		int			m_nInputBufferLength;

		double		*m_fDemodOutBuffer;
		short		*m_iRecordBuffer;
		int			m_nDemodOutBufferLength;

		//filters coefficients
		static FILTER_COEFS<double>	m_stFilterCoefBuffer;
		static FILTER_COEFS<double>	m_stFilterCoefOutput;

		//Demodulate
		CFIRFilter			m_InputFilterI;
		CFIRFilter			m_InputFilterQ;
		CCIC_Filter_Fixed	m_CicFilter;
		CFIRFilter			m_OutputFilter;//a filter after CIC
		double				m_fFirOutI,m_fFirOutQ;
		double				m_fOldFirOutI,m_fOldFirOutQ;
	};
}//namespace