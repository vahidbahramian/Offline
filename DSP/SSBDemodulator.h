#pragma once
#include "DemodulatorBase.h"
#include "../dsp/dsplib.h"
#include "OldFilters.h"

//HF Project Demodulator
namespace NS_DSP
{

	class CSSBDemodulator : public CDemodulatorBase
	{
	public:
		CSSBDemodulator(void);
		virtual ~CSSBDemodulator(void);

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
		std::vector<__SSB_DOUBLE_VECTOR *> pVector;
		NS_DSP::CSSBHopvmld *m_pSSB_hop;
		NS_DSP::CSSBFixedvmld *m_pSSB_fixed;

		double		*m_fInputBuffer;
		int			m_nInputBufferLength;
		double		*m_fFirOutBuffer;
		int			m_nFirOutBufferLength;
		int			*m_iHopsPositionBuffer;

		double		*m_fDemodOutBuffer;
		short		*m_iRecordBuffer;
		int			m_nDemodOutBufferLength;

		int			nSSBType;//DSPLIB_LSB_DEMOD or DSPLIB_USB_DEMOD
		//NS_DSP::CFIROLAvmld m_filter;
		CFIRFilter	  m_filter;

		//filters coefficients
		static FILTER_COEFS<double>	m_stFilterCoefBuffer;
		
	};
}//namespace