#pragma once

#include "DemodulatorBase.h"

//HF Project Demodulator
namespace NS_DSP
{


	class CAFDemodulator : public CDemodulatorBase
	{
	public:
		CAFDemodulator(void);
		~CAFDemodulator(void);
	
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
		double		*m_fInputBuffer;
		int			m_nInputBufferLength;
		double		*m_fDemodOutBuffer;
		short		*m_iRecordBuffer;
		int			m_nDemodOutBufferLength;

		//filters coefficients
		static FILTER_COEFS<double>	m_stFilterCoefBuffer;

	};

}//namespace