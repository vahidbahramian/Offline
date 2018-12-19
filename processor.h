#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <ipp.h>
#include "configuration.h"
class Processor
{
public:
    Processor();
int iSizeFFT;
public:
    double           *src, *dst, *dlysrc, *dlydst, *taps, *DATA;
    Ipp64fc *Temp2,*Zeros,*Temp_Sum,*Max_FFT,*RS_Array_Total,*Final_FFT_Buff;
    Ipp8u *pSpec,* workBuffer;
    Ipp64f eps;
    IppsFFTSpec_C_64fc** ppFFTSpec;
    Ipp8u* pSpec1;
    Ipp8u* pSpecBuffer;
    Ipp64fc *Sig,*Delayed_Sig,*FFT_in_out,*Temp,*Temp3;

    Ipp64f pMax;
    int pIndx,*Indexes;
    int pSpecSize,pSpecBufferSize,pBufferSize;

    void CreateFFT(int inum,int iFFTSize );
    void DestroyFFT();
    void CalcFFT(QVector<double> *In,QVector<double> *Out);

};
void Filter();
#endif // PROCESSOR_H
