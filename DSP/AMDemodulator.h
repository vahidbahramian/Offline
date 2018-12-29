#pragma once

#include "DemodulatorBase.h"
#include "OldFilters.h"
#include "../dsp/MorseCode.h"
#include "../dsp/MorseCode_gen.h"
#include "../dsp/MorseTree.h"
#include "../dsp/dsplib.h"
//HF Project Demodulator
namespace NS_DSP
{


	class CAMDemodulator : public CDemodulatorBase
	{
	public:
		CAMDemodulator(void);
		virtual ~CAMDemodulator(void);
	
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
		static FILTER_COEFS<double>	m_stFilterCoefHilbert;
		
		//Demodulate Func
		CFIRFilter	m_InputFilter;
		CFIRFilter	m_HilbertFilter;
		int			m_nDelayLineLength;
		double		*m_fDelayLine;
	};

}//namespace