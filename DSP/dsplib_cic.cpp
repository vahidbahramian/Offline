#include "stdafx.h"
#include "dsplib_cic.h"

namespace NS_DSP
{
//double precision---------------------------------------------
////////////////////////////////////////////////////////////////
CCICvmld::CCICvmld()
{
	Reset();
}

CCICvmld::~CCICvmld()
{
}

void CCICvmld::Reset()
{
	m_intergral[0]=0;
	m_intergral[1]=0;
	m_comb[0] = m_comb[1]=0;
	m_nDnsampleCntr=0;

}

BOOL CCICvmld::Create(int m_nDownSample,int m_nTrunc,int m_nSize)
{

	nBulkSize = m_nSize;
	nTrunc = m_nTrunc;
	nDownSample = m_nDownSample;
	Reset();
	return DSP_RET_SUCCESS;
}

int CCICvmld::Filter(double *input,double *output,int nSize)
{
	//integral
	int i , numDnSamples=0;
	int combOut[2];
	for(i=0;i<nSize;i++)
	{
		m_intergral[0] += int(input[i] * _DSPLIB_CIC_FIXED_CONST);//must be fixed point calc
		m_intergral[1] += m_intergral[0];
		m_nDnsampleCntr ++;
		if(m_nDnsampleCntr==nDownSample)
		{
			m_nDnsampleCntr=0;
			combOut[0] = m_intergral[1] - m_comb[0];
			combOut[1] = combOut[0] - m_comb[1];
			m_comb[0] = m_intergral[1];
			m_comb[1] = combOut[0];
			output[numDnSamples] = double(combOut[1])/_DSPLIB_CIC_FIXED_CONST ;//input[i];
			output[numDnSamples] = output[numDnSamples] / nTrunc;
		}
	}
	return numDnSamples;
}	


	//single precision---------------------------------------------
	////////////////////////////////////////////////////////////////
	CCICvmls::CCICvmls()
	{
		Reset();
	}

	CCICvmls::~CCICvmls()
	{
	}

	void CCICvmls::Reset()
	{
		m_intergral[0]=0;
		m_intergral[1]=0;
		m_comb[0] = m_comb[1]=0;
		m_nDnsampleCntr=0;

	}

	BOOL CCICvmls::Create(int m_nDownSample,int m_nTrunc,int m_nSize)
	{

		nBulkSize = m_nSize;
		nTrunc = m_nTrunc;
		nDownSample = m_nDownSample;
		Reset();
		return DSP_RET_SUCCESS;
	}

	int CCICvmls::Filter(float *input,float *output,int nSize)
	{
		//integral
		int i , numDnSamples=0;
		int combOut[2];
		for(i=0;i<nSize;i++)
		{
			m_intergral[0] += int(input[i] * _DSPLIB_CIC_FIXED_CONST);//must be fixed point calc
			m_intergral[1] += m_intergral[0];
			m_nDnsampleCntr ++;
			if(m_nDnsampleCntr==nDownSample)
			{
				m_nDnsampleCntr=0;
				combOut[0] = m_intergral[1] - m_comb[0];
				combOut[1] = combOut[0] - m_comb[1];
				m_comb[0] = m_intergral[1];
				m_comb[1] = combOut[0];
				output[numDnSamples] = float(combOut[1])/_DSPLIB_CIC_FIXED_CONST ;//input[i];
				output[numDnSamples] = output[numDnSamples] / nTrunc;
			}
		}
		return numDnSamples;
	}	

	void CCICvmls::ConstrainRange(double *input,double *output,int nSize)
	{
		int i;
		double dblDPhi;
		for(i=0;i<nSize;i++)
		{
			dblDPhi=input[i];
				if (input[i]<-M_PI)
			{
				dblDPhi=input[i]+2.*M_PI;
			}else if(input[i]>M_PI)
			{
				dblDPhi=input[i]-2.*M_PI;
			}
		output[i] = (80.0e3*dblDPhi/(2.0*M_PI)-20.0e3)/5000.0*1024.;
		}
	}

	int CCICvmls::Filter(double *input,double *output,int nSize)
	{
		//integral
		int i , numDnSamples=0;
		int combOut[2];
		for(i=0;i<nSize;i++)
		{
			m_intergral[0] += int(input[i] * _DSPLIB_CIC_FIXED_CONST);//must be fixed point calc
			m_intergral[1] += m_intergral[0];
			m_nDnsampleCntr ++;
			if(m_nDnsampleCntr==nDownSample)
			{
				m_nDnsampleCntr=0;
				combOut[0] = m_intergral[1] - m_comb[0];
				combOut[1] = combOut[0] - m_comb[1];
				m_comb[0] = m_intergral[1];
				m_comb[1] = combOut[0];
				output[numDnSamples] = double(combOut[1])/_DSPLIB_CIC_FIXED_CONST ;//input[i];
				output[numDnSamples] = output[numDnSamples] / nTrunc;
				numDnSamples++;
			}
		}
		return numDnSamples;
	}	
}//namespace