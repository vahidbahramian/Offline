#pragma once

//NOTE : This Filters are Obsolete
//They only are used in AM/FM demodulator

//HF Project Demodulator
namespace NS_DSP
{
	class CFIRFilter
	{
	public:
		int numberCoeffs;
		double *coefficints;
		double *buffer; //delays of input

	public:
		CFIRFilter();
		~CFIRFilter();
		BOOL Create(double *coeffs,int numCoeffs);
		//BOOL Create(double fs,double f1,double f2,int Nfft);
		void Reset();
		double Filter(double input);
		double Filter2(double input);
	};

	class CCIC_Filter_Fixed
	{
	public:
		CCIC_Filter_Fixed();
		~CCIC_Filter_Fixed();
		void Reset();
		int Create(int numStages,int downSample,int trunc);
		double Filter(int input,BOOL &oValid);

	public:
		int m_numberStages;
		int m_DownSample;
		int m_smpleCntr;
		int *m_combs;
		int *m_integrals;
		int m_DownSampleOut;
		int out;
		int m_trunc;

	};
}//namespace