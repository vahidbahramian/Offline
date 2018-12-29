#pragma once

#include "DemodulatorBase.h"
#include "OldFilters.h"
#include "../dsp/MorseCode.h"
#include "../dsp/MorseCode_gen.h"
#include "../dsp/MorseTree.h"

//HF Project Demodulator
namespace NS_DSP
{

	class CMorseDemodulator : public CDemodulatorBase
	{
	public:
		CMorseDemodulator(void);
		virtual ~CMorseDemodulator(void);
	
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
		NS_DSP::CMC_gen	m_cMC;
		NS_DSP::CMorseCode m_cMorseCode;
		double		*m_fInputBufferI;
		double		*m_fInputBufferQ;
		int			m_nInputBufferLength;

		double		m_fThreshold;//morse threshold
		int			m_iLenAdd;
		int			m_iLenDataNeed;
		double     *m_pStoreData;

		//filters coefficients
		static FILTER_COEFS<double>	m_stFilterCoefBuffer;

		CFIRFilter	m_filterI;
		CFIRFilter	m_filterQ;

	};

}//namespace