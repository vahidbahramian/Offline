#pragma once

/*
	FIR Filter Design Package
*/

#define LP          0x00010000 /* Low pass */
#define HP          0x00020000 /* High pass */
#define BP          0x00040000 /* Band pass */
#define BS          0x00080000 /* Band stop */
#define TYPE_MASK   0x000F0000

/* Window types */
#define BOXCAR      0x00000001
#define TRIANG      0x00000002
#define HAMMING     0x00000004
#define HANNING     0x00000008
#define BLACKMAN    0x00000010
#define FLATTOP     0x00000011
#define KAISER      0x00000012
#define WINDOW_MASK 0x0000001F

/* Parallel filter design */
#define	FWD   	    0x00000001 /* Forward indexing of polyphase filter */
#define REW         0x00000002 /* Reverse indexing of polyphase filter */
#define ODD         0x00000010 /* Make filter HP */

void boxcar(int n, double* w);
void triang(int n, double* w);
void hanning(int n, double* w);
void hamming(int n,double* w);
void blackman(int n,double* w);
void flattop(int n,double* w);
void kaiser(int n, double* w,double b);
int design_fir(unsigned int n,double* w, double* fc, unsigned int flags
			   ,double opt);
void Median(int blksz,int filterSize,double *m_fInputBuff,double *m_fMedBuffer);
void movingavg(double *fInputBuffer,int nInputLen,int nFiltLen,double *fOutBuffer);


