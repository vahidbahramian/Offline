#include "processor.h"


Processor::Processor()
{

}
void Processor::CreateFFT(int inum,int iFFTSize )
{
    iSizeFFT=iFFTSize;

    FFT_in_out=ippsMalloc_64fc(4096);
    eps=2.220446049250313e-16;
    ippsFFTGetSize_C_64fc(inum,IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,&pSpecSize,&pSpecBufferSize,&pBufferSize);
    pSpec = ippsMalloc_8u(pSpecSize);
    workBuffer=ippsMalloc_8u(pBufferSize);
    ppFFTSpec=(IppsFFTSpec_C_64fc**)ippsMalloc_8u(pSpecSize);
    pSpecBuffer=ippsMalloc_8u(pSpecSize);
    ippsFFTInit_C_64fc(ppFFTSpec, inum,IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,pSpec,pSpecBuffer);

    //-----------


    src   = ippsMalloc_64f(4096);
    dst   = ippsMalloc_64f(4096);
}
void Processor::DestroyFFT()
{
//    ippsFree(pSpec3);
//    ippsFree(workBuffer3);
//    ippsFree(ppFFTSpec3);
//    ippsFree(pSpecBuffer3);
//    ippsFree(FFT_in_out3);

}
void Processor::CalcFFT(QVector<double> *In,QVector<double> *Out)
{
    Out->clear();
    Ipp64f *img;
    img=ippsMalloc_64f(4096);
    ippsZero_64f(img, 4096);
    ippsCopy_64f(In[0].data(), src, 4096);
    ippsRealToCplx_64f((Ipp64f*)src,(Ipp64f*)img,FFT_in_out,4096);
    ippsFFTFwd_CToC_64fc_I(FFT_in_out,(IppsFFTSpec_C_64fc*) pSpec,workBuffer);
    ippsCplxToReal_64fc(FFT_in_out,dst,img,4096);
    for(int i=0;i<4096;i++)
        Out->append(dst[i]);
    ippsFree(img);

}
void Filter()
{

}
