#include "../StdAfx.h"
#include "dsplib_fir_design.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "mkl_cblas.h"

/******************************************************************************
*  FIR filter design
******************************************************************************/

/* Design FIR filter using the Window method

n     filter length must be odd for HP and BS filters
w     buffer for the filter taps (must be n long)
fc    cutoff frequencies (1 for LP and HP, 2 for BP and BS) 
0 < fc < 1 where 1 <=> Fs/2
flags window and filter type as defined in filter.h
variables are ored together: i.e. LP|HAMMING will give a 
low pass filter designed using a hamming window  
opt   beta constant used only when designing using kaiser windows

returns 0 if OK, -1 if fail
*/
int design_fir(unsigned int n,double* w, double* fc, 
			   unsigned int flags ,double opt)
{
	unsigned int	o   = n & 1;          	/* Indicator for odd filter length */
	unsigned int	end = ((n + 1) >> 1) - o;  /* Loop end */
	unsigned int	i;			/* Loop index */

	double k1 = 2 * M_PI;		/* 2*pi*fc1 */
	double k2 = 0.5 * (double)(1 - o);/* Constant used if the filter has even length */
	double k3;				/* 2*pi*fc2 Constant used in BP and BS design */
	double g  = 0.0;     		/* Gain */
	double t1,t2,t3;     		/* Temporary variables */
	double fc1,fc2;			/* Cutoff frequencies */

	/* Sanity check */
	if(!w || (n == 0)) return -1;

	/* Get window coefficients */
	switch(flags & WINDOW_MASK){
  case(BOXCAR):
	  boxcar(n,w); break;
  case(TRIANG):
	  triang(n,w); break;
  case(HAMMING):
	  hamming(n,w); break;
  case(HANNING):
	  hanning(n,w); break;
  case(BLACKMAN):
	  blackman(n,w); break;
  case(FLATTOP):
	  flattop(n,w); break;
  case(KAISER):
	  kaiser(n,w,opt); break;
  default:
	  return -1;	
	}

	if(flags & (LP | HP)){ 
		fc1=*fc;
		/* Cutoff frequency must be < 0.5 where 0.5 <=> Fs/2 */
		fc1 = ((fc1 <= 1.0) && (fc1 > 0.0)) ? fc1/2 : 0.25;
		k1 *= fc1;

		if(flags & LP){ /* Low pass filter */

			/*
			* If the filter length is odd, there is one point which is exactly
			* in the middle. The value at this point is 2*fCutoff*sin(x)/x, 
			* where x is zero. To make sure nothing strange happens, we set this
			* value separately.
			*/
			if (o){
				w[end] = fc1 * w[end] * 2.0;
				g=w[end];
			}

			/* Create filter */
			for (i=0 ; i<end ; i++){
				t1 = (double)(i+1) - k2;
				w[end-i-1] = w[n-end+i] = w[end-i-1] * sin(k1 * t1)/(M_PI * t1); /* Sinc */
				g += 2*w[end-i-1]; /* Total gain in filter */
			}
		}
		else{ /* High pass filter */
			if (!o) /* High pass filters must have odd length */
				return -1;
			w[end] = 1.0 - (fc1 * w[end] * 2.0);
			g= w[end];

			/* Create filter */
			for (i=0 ; i<end ; i++){
				t1 = (double)(i+1);
				w[end-i-1] = w[n-end+i] = -1 * w[end-i-1] * sin(k1 * t1)/(M_PI * t1); /* Sinc */
				g += ((i&1) ? (2*w[end-i-1]) : (-2*w[end-i-1])); /* Total gain in filter */
			}
		}
	}

	if(flags & (BP | BS)){
		fc1=fc[0];
		fc2=fc[1];
		/* Cutoff frequencies must be < 1.0 where 1.0 <=> Fs/2 */
		fc1 = ((fc1 <= 1.0) && (fc1 > 0.0)) ? fc1/2 : 0.25;
		fc2 = ((fc2 <= 1.0) && (fc2 > 0.0)) ? fc2/2 : 0.25;
		k3  = k1 * fc2; /* 2*pi*fc2 */
		k1 *= fc1;      /* 2*pi*fc1 */

		if(flags & BP){ /* Band pass */
			/* Calculate center tap */
			if (o){
				g=w[end]*(fc1+fc2);
				w[end] = (fc2 - fc1) * w[end] * 2.0;
			}

			/* Create filter */
			for (i=0 ; i<end ; i++){
				t1 = (double)(i+1) - k2;
				t2 = sin(k3 * t1)/(M_PI * t1); /* Sinc fc2 */
				t3 = sin(k1 * t1)/(M_PI * t1); /* Sinc fc1 */
				g += w[end-i-1] * (t3 + t2);   /* Total gain in filter */
				w[end-i-1] = w[n-end+i] = w[end-i-1] * (t2 - t3); 
			}
		}      
		else{ /* Band stop */
			if (!o) /* Band stop filters must have odd length */
				return -1;
			w[end] = 1.0 - (fc2 - fc1) * w[end] * 2.0;
			g= w[end];

			/* Create filter */
			for (i=0 ; i<end ; i++){
				t1 = (double)(i+1);
				t2 = sin(k1 * t1)/(M_PI * t1); /* Sinc fc1 */
				t3 = sin(k3 * t1)/(M_PI * t1); /* Sinc fc2 */
				w[end-i-1] = w[n-end+i] = w[end-i-1] * (t2 - t3); 
				g += 2*w[end-i-1]; /* Total gain in filter */
			}
		}
	}

	/* Normalize gain */
	g=1/g;
	for (i=0; i<n; i++) 
		w[i] *= g;

	return 0;

}

/*=============================================================================
*	
*  This software has been released under the terms of the GNU Public
*  license. See http://www.gnu.org/copyleft/gpl.html for details.
*
*  Copyright 2001 Anders Johansson ajh@atri.curtin.edu.au
*
*=============================================================================
*/

/* Calculates a number of window functions. The following window
* functions are currently implemented: Boxcar, Triang, Hanning,
* Hamming, Blackman, Flattop and Kaiser. In the function call n is
* the number of filter taps and w the buffer in which the filter
* coefficients will be stored.
*/

/*
* Boxcar
*
* n window length
* w buffer for the window parameters
*/
void boxcar(int n, double* w)
{
	int i;
	/* Calculate window coefficients */
	for (i=0 ; i<n ; i++)
		w[i] = 1.0;
}


/*
* Triang a.k.a Bartlett
*
*               |    (N-1)| 
*           2 * |k - -----|
*               |      2  |
* w = 1.0 - ---------------
*                    N+1
* n window length
* w buffer for the window parameters
*/
void triang(int n, double* w)
{
	double k1  = (double)(n & 1);
	double k2  = 1/((double)n + k1);
	int      end = (n + 1) >> 1;
	int	   i;

	/* Calculate window coefficients */
	for (i=0 ; i<end ; i++)
		w[i] = w[n-i-1] = (2.0*((double)(i+1))-(1.0-k1))*k2;
}


/*
* Hanning
*                   2*pi*k
* w = 0.5 - 0.5*cos(------), where 0 < k <= N
*                    N+1
* n window length
* w buffer for the window parameters
*/
void hanning(int n, double* w)
{
	int	   i;
	double k = 2*M_PI/((double)(n+1)); /* 2*pi/(N+1) */

	/* Calculate window coefficients */
	for (i=0; i<n; i++)
		*w++ = 0.5*(1.0 - cos(k*(double)(i+1)));
}

/*
* Hamming
*                        2*pi*k
* w(k) = 0.54 - 0.46*cos(------), where 0 <= k < N
*                         N-1
*
* n window length
* w buffer for the window parameters
*/
void hamming(int n,double* w)
{
	int      i;
	double k = 2*M_PI/((double)(n-1)); /* 2*pi/(N-1) */

	/* Calculate window coefficients */
	for (i=0; i<n; i++)
		*w++ = 0.54 - 0.46*cos(k*(double)i);
}

/*
* Blackman
*                       2*pi*k             4*pi*k
* w(k) = 0.42 - 0.5*cos(------) + 0.08*cos(------), where 0 <= k < N
*                        N-1                 N-1
*
* n window length
* w buffer for the window parameters
*/
void blackman(int n,double* w)
{
	int      i;
	double k1 = 2*M_PI/((double)(n-1)); /* 2*pi/(N-1) */
	double k2 = 2*k1; /* 4*pi/(N-1) */

	/* Calculate window coefficients */
	for (i=0; i<n; i++)
		*w++ = 0.42 - 0.50*cos(k1*(double)i) + 0.08*cos(k2*(double)i);
}

/*
* Flattop
*                                        2*pi*k                     4*pi*k
* w(k) = 0.2810638602 - 0.5208971735*cos(------) + 0.1980389663*cos(------), where 0 <= k < N
*                                          N-1                        N-1
*
* n window length
* w buffer for the window parameters
*/
void flattop(int n,double* w)
{
	int      i;
	double k1 = 2*M_PI/((double)(n-1)); /* 2*pi/(N-1) */
	double k2 = 2*k1;                   /* 4*pi/(N-1) */

	/* Calculate window coefficients */
	for (i=0; i<n; i++)
		*w++ = 0.2810638602 - 0.5208971735*cos(k1*(double)i) + 0.1980389663*cos(k2*(double)i);
}

/* Computes the 0th order modified Bessel function of the first kind.  
* (Needed to compute Kaiser window) 
*   
* y = sum( (x/(2*n))^2 )
*      n
*/
#define BIZ_EPSILON 1E-21 /* Max error acceptable */

static double besselizero(double x)
{ 
	double temp;
	double sum   = 1.0;
	double u     = 1.0;
	double halfx = x/2.0;
	int      n     = 1;

	do {
		temp = halfx/(double)n;
		u *=temp * temp;
		sum += u;
		n++;
	} while (u >= BIZ_EPSILON * sum);
	return(sum);
}

/*
* Kaiser
*
* n window length
* w buffer for the window parameters
* b beta parameter of Kaiser window, Beta >= 1
*
* Beta trades the rejection of the low pass filter against the
* transition width from passband to stop band.  Larger Beta means a
* slower transition and greater stop band rejection.  See Rabiner and
* Gold (Theory and Application of DSP) under Kaiser windows for more
* about Beta.  The following table from Rabiner and Gold gives some
* feel for the effect of Beta:
* 
* All ripples in dB, width of transition band = D*N where N = window
* length
* 
* BETA    D       PB RIP   SB RIP
* 2.120   1.50  +-0.27      -30
* 3.384   2.23    0.0864    -40
* 4.538   2.93    0.0274    -50
* 5.658   3.62    0.00868   -60
* 6.764   4.32    0.00275   -70
* 7.865   5.0     0.000868  -80
* 8.960   5.7     0.000275  -90
* 10.056  6.4     0.000087  -100
*/
void kaiser(int n, double* w, double b)
{
	double tmp;
	double k1  = 1.0/besselizero(b);
	int	   k2  = 1 - (n & 1);
	int      end = (n + 1) >> 1;
	int      i; 

	/* Calculate window coefficients */
	for (i=0 ; i<end ; i++){
		tmp = (double)(2*i + k2) / ((double)n - 1.0);
		w[end-(1&(!k2))+i] = w[end-1-i] = k1 * besselizero(b*sqrt(1.0 - tmp*tmp));
	}
}
void Median(int blksz,int filterSize,double *m_fInputBuff,double *m_fMedBuffer)
{
	int k;
	int i;
	int j;
	double dblMax=0;
	int	   nIndxMax=2049;
	int nMinRange=0;
	int nMaxRange=0;
	int nLen=0;
	double *m_fSFrame = new double[16384];
	memset(m_fSFrame,0,16384*sizeof(double));

	if (blksz==1)
	{
		cblas_dcopy(filterSize,m_fInputBuff,1,m_fSFrame,1);

		nLen = filterSize;

		double fTemp;
		for(i=0; i < ceil(double(nLen+1)/2); i++)
		{
			for(j=i; j<nLen; j++)
			{
				if( m_fSFrame[i]>m_fSFrame[j] )
				{
					fTemp = m_fSFrame[i];
					m_fSFrame[i] = m_fSFrame[j];
					m_fSFrame[j] = fTemp;
				}
			}
		}

		if( nLen%2 )
			m_fMedBuffer[0] = m_fSFrame[ (nLen-1)/2 ];
		else
			m_fMedBuffer[0] = ( m_fSFrame[ nLen/2 ]+m_fSFrame[ nLen/2-1 ] )/2;
	}
	else
	{
		for (k=0; k<filterSize; k++)
		{
			nLen = blksz;
			if (nLen%2)
			{
				nLen = (nLen-1)/2;
			}else
			{
				nLen=nLen/2-1;
			}

			nMinRange = k - nLen;
			nMaxRange = k + nLen;

			if (nMinRange < 0)
				nMinRange = 0;

			if (nMaxRange > filterSize)
				nMaxRange = filterSize;

			nLen = nMaxRange-nMinRange+1;

			//if (blksz==1)
			//	cblas_dcopy(filterSize,m_fInputBuff,1,m_fSFrame+1,1);
			//else
			//	cblas_dcopy(filterSize,m_fInputBuff,1,m_fSFrame,1);

			cblas_dcopy(nLen, m_fInputBuff+nMinRange, 1, m_fSFrame, 1);

			/*for (j=0;j<=nLen;j++)
			{
			for (i=nMinRange;i<=nMaxRange;i++)
			{
			if (m_fSFrame[i]>dblMax)
			{
			dblMax = m_fSFrame[i];
			nIndxMax = i;
			}
			}
			if (j!=(nLen))
			{
			m_fSFrame[nIndxMax]=0;
			dblMax=0;
			nIndxMax=0;
			}
			else if (j==(nLen))
			m_fMedBuffer[k]=dblMax;
			}*/

			double fTemp;
			for(i=0; i<(nLen+1)/2; i++)
			{
				for(j=i; j<nLen; j++)
				{
					if( m_fSFrame[i]>m_fSFrame[j] )
					{
						fTemp = m_fSFrame[i];
						m_fSFrame[i] = m_fSFrame[j];
						m_fSFrame[j] = fTemp;
					}
				}
			}

			m_fMedBuffer[k] = m_fSFrame[(nLen-1)/2];
		}
	}
	delete[] m_fSFrame;
	m_fSFrame = NULL;
}
void movingavg(double *fInputBuffer,int nInputLen,int nFiltLen,double *fOutBuffer)
{
	nFiltLen = nFiltLen + int((nFiltLen+1)%2);
	memset(fOutBuffer,0,nInputLen*sizeof(double));

	double fSum = 0;
	int nLim1 = int((nFiltLen-1)/2);
	int nLim2 = int((nFiltLen+1)/2);

	for (int i=0;i<nLim1;i++)
	{
		fSum = fSum + fInputBuffer[i];
	}
	for (int i=0;i<nLim2;i++)
	{
		fSum = fSum + fInputBuffer[i+nLim1];
		fOutBuffer[i] = fSum/nFiltLen;
	}
	for (int i=nLim2;i<(nInputLen - nLim1);i++)
	{
		fSum = fSum + fInputBuffer[i+nLim1] - fInputBuffer[i-nLim1-1];
		fOutBuffer[i] = fSum/nFiltLen;
	}
	for (int i = (nInputLen-nLim1);i<nInputLen;i++)
	{
		fSum = fSum - fInputBuffer[i-nLim1-1];
		fOutBuffer[i] = fSum/nFiltLen;
	}
}



