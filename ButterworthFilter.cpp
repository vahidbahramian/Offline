//#include "StdAfx.h"
#include "ButterworthFilter.h"

CButterworthFilter::CButterworthFilter(void)
{
	ResetFilter();
}

CButterworthFilter::~CButterworthFilter(void)
{
}
bool CButterworthFilter::DesignFilter(TPassType PassType, double dFs, double dFc, double dBW, int nOrder)
{
	ResetFilter();

	int n;            // filter order
	int sff;          // scale flag: 1 to scale, 0 to not scale ccof
	int i;            // loop variables
	double fcf;       // cutoff frequency (fraction of pi)
	double f1f;       // lower cutoff frequency (fraction of pi)
	double f2f;       // upper cutoff frequency (fraction of pi)
	double sf;        // scaling factor
	double *dcof;     // d coefficients
	int *ccof;        // c coefficients

	if(PassType == LPF)
	{
		m_nSize = nOrder;
		n = nOrder;
		fcf = 2 * dFc / dFs;
		sff = 1;

		/* calculate the d coefficients */
		dcof = dcof_bwlp( n, fcf );
		if( dcof == NULL )
			return false;

		/* calculate the c coefficients */
		ccof = ccof_bwlp( n );
		if( ccof == NULL )
			return false;

		sf = sf_bwlp( n, fcf ); /* scaling factor for the c coefficients */

		/* Output the c coefficients */
		if( sff == 0 )
			for( i = 0; i <= n; ++i)
				m_pdCoefB[i] = ccof[i];
		else
			for( i = 0; i <= n; ++i)
				m_pdCoefB[i] = ccof[i]*sf;

		/* Output the d coefficients */
		for( i = 0; i <= n; ++i )
			m_pdCoefA[i] = dcof[i];

		free( dcof );
		free( ccof );
	}


	else if(PassType == HPF)
	{
		m_nSize = nOrder;
		n = nOrder;
		fcf = 2 * dFc / dFs;
		sff = 1;

		/* calculate the d coefficients */
		dcof = dcof_bwhp( n, fcf );
		if( dcof == NULL )
			return false;

		/* calculate the c coefficients */
		ccof = ccof_bwhp( n );
		if( ccof == NULL )
			return false;

		sf = sf_bwhp( n, fcf ); /* scaling factor for the c coefficients */

		/* Output the c coefficients */
		if( sff == 0 )
			for( i = 0; i <= n; ++i)
				m_pdCoefB[i] = ccof[i];
		else
			for( i = 0; i <= n; ++i)
				m_pdCoefB[i] = ccof[i]*sf;

		/* Output the d coefficients */
		for( i = 0; i <= n; ++i )
			m_pdCoefA[i] = dcof[i];

		free( dcof );
		free( ccof );
	}


	else if(PassType == BPF)
	{
		m_nSize = 2*nOrder;
		n = nOrder;
		f1f = 2 * (dFc - dBW/2.0) / dFs;
		f2f = 2 * (dFc + dBW/2.0) / dFs;
		sff = 1;

		/* calculate the d coefficients */
		dcof = dcof_bwbp( n, f1f, f2f );
		if( dcof == NULL )
			return false;

		/* calculate the c coefficients */
		ccof = ccof_bwbp( n );
		if( ccof == NULL )
			return false;

		sf = sf_bwbp( n, f1f, f2f ); /* scaling factor for the c coefficients */

		/* Output the c coefficients */
		if( sff == 0 )
			for( i = 0; i <= 2*n; ++i)
				m_pdCoefB[i] = ccof[i];
		else
			for( i = 0; i <= 2*n; ++i)
				m_pdCoefB[i] = ccof[i]*sf;

		/* Output the d coefficients */
		for( i = 0; i <= 2*n; ++i )
			m_pdCoefA[i] = dcof[i];

		free( dcof );
		free( ccof );
	}

	
	else if(PassType == NOTCH)
	{
		m_nSize = 2*nOrder;
		n = nOrder;
		f1f = 2 * (dFc - dBW/2.0) / dFs;
		f2f = 2 * (dFc + dBW/2.0) / dFs;
		sff = 1;

		/* calculate the d coefficients */
		dcof = dcof_bwbs( n, f1f, f2f );
		if( dcof == NULL )
			return false;

		/* calculate the c coefficients */
		double *ccof;        // c coefficients
		ccof = ccof_bwbs( n, f1f, f2f );
		if( ccof == NULL )
			return false;

		sf = sf_bwbs( n, f1f, f2f ); /* scaling factor for the c coefficients */

		/* Output the c coefficients */
		if( sff == 0 )
			for( i = 0; i <= 2*n; ++i)
				m_pdCoefB[i] = ccof[i];
		else
			for( i = 0; i <= 2*n; ++i)
				m_pdCoefB[i] = ccof[i]*sf;

		/* Output the d coefficients */
		for( i = 0; i <= 2*n; ++i )
			m_pdCoefA[i] = dcof[i];

		free( dcof );
		free( ccof );
	}

	return true;
}
bool CButterworthFilter::Filter(double *pdInput, int iSize, double *pdOutput)
{
	double dOut;
	int n = m_nSize;

	for(int i=0; i<iSize; i++)
	{
		for(int j=n; j>0; j--)
			m_pdHisX[j] = m_pdHisX[j - 1];
		m_pdHisX[0] = pdInput[i];

		dOut = 0;
		for(int j=0; j<=n; j++)
			dOut = dOut + m_pdHisX[j] * m_pdCoefB[j];

		for(int j=1; j<=n; j++)
			dOut = dOut - m_pdHisY[j-1] * m_pdCoefA[j];

		pdOutput[i] = dOut;

		for(int j=n; j>0; j--)
			m_pdHisY[j] = m_pdHisY[j - 1];
		m_pdHisY[0] = dOut;
	}

	return true;
}

void CButterworthFilter::ResetFilter(void)
{
	for(int j=0; j<256; j++)
	{
		m_pdHisX[j] = 0;
		m_pdHisY[j] = 0;
	}
}
