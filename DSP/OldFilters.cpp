#include "StdAfx.h"
#include "OldFilters.h"

//NOTE : This Filters are Obsolete
//They only are used in AM/FM demodulator

namespace NS_DSP
{
	//---------------------------------------
	//----------------FIR Filter Class : BEGIN
	CFIRFilter::CFIRFilter()
	{
		numberCoeffs =0;
		coefficints=NULL;
		buffer		=	NULL;
	}
	CFIRFilter::~CFIRFilter()
	{
		if (coefficints)
			delete[] coefficints;
		if (buffer)
			delete[] buffer;

		numberCoeffs=0;
	}
	BOOL CFIRFilter::Create(double *coeffs, int numCoeffs)
	{
		int i;
		if(numCoeffs<1) return FALSE;
		coefficints = new double[numCoeffs];
		if(!coefficints) return FALSE;
		buffer = new double[numCoeffs];
		if(!buffer) return FALSE;

		numberCoeffs = numCoeffs;
		for(i=0;i<numCoeffs;i++)
			coefficints[i]=coeffs[i];
		for(i=0;i<numCoeffs;i++)
			buffer[i]=0;
		return TRUE;
	}

/*	BOOL CFIRFilter::Create(double fs,double f1,double f2,int Nfft)
	{
		CMyFFT fft;
		int i,k1 = floor(f1*Nfft/fs);
		int k2 = floor(f2*Nfft/fs);
		cvec a1(Nfft/2 -1);
		cvec freqz(Nfft);
		a1.clear();
		freqz.clear();
		freqz[0]=1;//dc component
		//	a1=[zeros(1,k1) ones(1,k2-k1) zeros(1,N/2-k2-1)];
		//	freqz=[1 a1 0 fliplr(a1)];
		//	ff=ifft(a);
		//	coefs=[real(ff(N/2+1:N)) real(ff(1:N/2))];
		for(i=k1;i<=k2-k1;i++) a1[i]=1;//f1 to f2
		for(i=0;i<Nfft/2-1;i++) freqz[i+1] = a1[i];
		freqz[Nfft/2]=0;
		for(i=0;i<Nfft/2-1;i++) 
			freqz[Nfft/2+i+1] = a1[Nfft/2-2-i];
		cvec coefs(Nfft);
		coefs = fft.ifft(freqz,Nfft);
		double *c = new double [Nfft];
		for(i=0;i<Nfft/2;i++) c[i]=coefs[i+Nfft/2].real();
		for(i=0;i<Nfft/2;i++) c[Nfft/2+i]=coefs[i].real();
		BOOL res = Create(c,Nfft);
		delete c;
		return res;
	}
*/
	void CFIRFilter::Reset()
	{
		int i;
		for(i=0;i<numberCoeffs;i++){
			buffer[i]=0;
		}
	}

	double CFIRFilter::Filter(double input)
	{
		int i;
		double sum=0;
		buffer[0]=input;

		for(i=numberCoeffs-1;i>=0;i--)
		{
			sum += (buffer[i] * coefficints[i]);
			if(i>0)
				buffer[i]=buffer[i-1];//shifts input 
		}
		return sum;
	}

	//uses symmetry proerty of coeffs.
	double CFIRFilter::Filter2(double input)
	{
		int i;
		double sum=0;
		buffer[0]=input;
#ifdef _DEBUG_AM
		double testbuf[31];
		for(i=0;i<31;i++)
			testbuf[i]=buffer[i];
#endif

		for(i=0;i<numberCoeffs/2;i++)
		{
			sum += ((buffer[i]+buffer[numberCoeffs-1-i]) * coefficints[i]);
		}
		sum += ((buffer[i]) * coefficints[i]);
		for(i=numberCoeffs-1;i>0;i--)
		{
			buffer[i]=buffer[i-1];//shifts input 
		}
		return sum;
	}

	//-----------CIC Filter--------------------------//
	//----------------CIC Filter Class
	CCIC_Filter_Fixed::CCIC_Filter_Fixed()
	{
		m_combs=NULL;
		m_integrals=NULL;
		Reset();
	}
	CCIC_Filter_Fixed::~CCIC_Filter_Fixed()
	{
		if(m_combs) {delete[] m_combs; m_combs=NULL;}
		if(m_integrals) {delete[] m_integrals; m_integrals=NULL;}

	}
	void CCIC_Filter_Fixed::Reset()
	{
		int i;
		//m_numberStages=0;
		//m_DownSample=0;
		m_smpleCntr=0;
		m_DownSampleOut=0;
		for(i=0;i<=m_numberStages;i++)
		{
			m_integrals[i]=0;
			m_combs[i]=0;
		}
		out=0;

	}
	int CCIC_Filter_Fixed::Create(int numStages, int downSample,int trunc)
	{
		if(numStages < 1) return FALSE;
		if(downSample < 1) return FALSE;
		m_integrals = new int[numStages+1];
		if(!m_integrals) return FALSE;
		m_numberStages = numStages;
		m_combs = new int[numStages+1];
		if(!m_combs) return FALSE;
		m_DownSample = downSample;
		m_trunc = trunc;
		return TRUE;
	}

	double CCIC_Filter_Fixed::Filter(int input,BOOL &oValid)
	{
		int i;
		int z;
		m_integrals[0] = input;
		for(i=m_numberStages;i>0;i--)
		{
			m_integrals[i] = m_integrals[i-1] + m_integrals[i];
		}
		if(m_smpleCntr==m_DownSample)
		{
			oValid = TRUE;
			m_DownSampleOut = m_integrals[m_numberStages];
			out = m_DownSampleOut;
			for(i=0;i<m_numberStages;i++)
			{
				z=out;
				out = out - m_combs[i];
				m_combs[i] = z;
			}
			m_smpleCntr=0;
		}else
		{
			oValid = FALSE;
			m_smpleCntr++;
		}
		return out/m_trunc;
	}
	//----------------CIC Filter Class : END
}//namespace