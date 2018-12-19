#include "mode_ff.h"

Mode_FF::Mode_FF()
{
    x_Freq=0.0;y_lvl=-50.0;

}
bool Mode_FF::LoadDataFile(QString path)
{
    m_pConfig->OpenInputFile(path);
    return m_pConfig->m_bLoadedFileInput;
}
bool Mode_FF::RunMode(void)
{
    m_bRunMode = true;
    m_bStartSpectrum = false;
    m_bPauseSpectrum = false;
    m_bStartSelect = false;
    return true;
}
bool Mode_FF::StartSpectrum(void)
{
        m_pConfig->GotoFileStartPos();
        StartSpectrumThread();
        m_bStartSpectrum = true;
        m_bPauseSpectrum = false;
        m_bStartSelect = false;
  return m_bStartSpectrum;
}
bool Mode_FF::StartSpectrumThread(void)
{
  m_bKillThreadSpectrum = false;
//  startthread();

    timer->start(1);

}
void Mode_FF::StopSpectrumThread(void)
{
    m_bKillThreadSpectrum = true;

}
void Mode_FF::CloseMode(void)
{
    StopSpectrum();
    m_bRunMode = false;
}
void Mode_FF::StopSpectrum(void)
{

    if(timer->isActive())
        timer->stop();
//    if(m_pConfig->IsFFTCreated())
//        m_pConfig->DestroyFFT();
}
void Mode_FF::Close(void)
{
    CloseMode();

}
bool Mode_FF::SetParameters(FF_ALL_SETTING stSettingFF)
{
    m_bInSettingMode = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlaySpectrum()));
    memcpy(&m_stSettingFF, &stSettingFF, sizeof(stSettingFF));

    if(!m_pdSpectrum_X.isEmpty())
        m_pdSpectrum_X.clear();
    if(!m_pdSpectrum_Y.isEmpty())
        m_pdSpectrum_Y.clear();
    if(!m_pdMaxHoldSpectrum.isEmpty())
        m_pdMaxHoldSpectrum.clear();
    if(!m_pdMaxHoldPhaseErr.isEmpty())
        m_pdMaxHoldPhaseErr.clear();
//    if(!FileReadBuff->isEmpty())
//        FileReadBuff->clear();
    if(!m_pdOutFFT.isEmpty())
        m_pdOutFFT.clear();


    double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;

    m_iIndexStartSpectrum =
        m_stSettingFF.stSpectrum.dStartFrequency / dSamplingFrequency *
        m_stSettingFF.stFFT.iSizeFFT;
    m_iIndexStopSpectrum =
        m_stSettingFF.stSpectrum.dStopFrequency / dSamplingFrequency *
        m_stSettingFF.stFFT.iSizeFFT;




    //--- FFT
//    FileReadBuff = new QVector<double>;
    FileReadBuff.resize(m_stSettingFF.stFFT.iSizeFFT);
    m_pdOutFFT.resize(m_stSettingFF.stFFT.iSizeFFT);
    if(!m_pConfig->IsFFTCreated())
        m_pConfig->CreateFFT(12,m_stSettingFF.stFFT.iSizeFFT);



    //--- Spectrum
    m_iSizeSpectrum = m_iIndexStopSpectrum - m_iIndexStartSpectrum + 1;
    m_pdSpectrum_X.resize(m_iSizeSpectrum);
    m_pdSpectrum_Y.resize(m_iSizeSpectrum);
    for(int i=0; i<m_iSizeSpectrum; i++)
        m_pdSpectrum_Y[i] = -150;


    //--- Max Hold Buffer
    m_pdMaxHoldSpectrum.resize(m_iSizeSpectrum);
    for(int i=0; i<m_iSizeSpectrum; i++)
        m_pdMaxHoldSpectrum[i] = -1e6;

    m_pdMaxHoldPhaseErr.resize(300);
    for(int i=0; i<300; i++)
        m_pdMaxHoldPhaseErr[i] = -15000;

    m_bInSettingMode = false;

    return true;
}
double  Mode_FF::FcEstimate(double x,double y)
{

    if(!m_pConfig->m_bLoadedFileInput)
        return -1;

    if(m_pConfig->IsEndOfFile())
        return -1;

    m_pConfig->GotoFileStartPos();
    QVector<double> ReadBuff;
    ReadBuff.resize(10*m_stSettingFF.stFFT.iSizeFFT*2);

    double Fs = m_pConfig->m_stInputFile.dSamplingFrequency * g_iINPUT_FREQ_UNIT[(int)m_pConfig->m_stInputFile.enFreqUnit];
    double Fc = Fc * g_iINPUT_FREQ_UNIT[(int)m_pConfig->m_stInputFile.enFreqUnit];

    int iSizeFFT = m_stSettingFF.stFFT.iSizeFFT;

        qlonglong iSizeFile = m_pConfig->GetInputFileSampleSize();
        if((10*iSizeFFT) > iSizeFile)
            qDebug("Error in File Size");

        m_pConfig->ReadFromInputFile(ReadBuff, 10*iSizeFFT);


        QVector<double> FFT_in(iSizeFFT);
        QVector<double> FFT_out(iSizeFFT);
        QVector<double> Amp_Sig(iSizeFFT);
        QVector<double> Sum_Amp_Sig(iSizeFFT);
        double *NF,*Amp_Sig_fil,*mvaverage;

        NF=new double[iSizeFFT/2];
        ippsZero_64f(&Sum_Amp_Sig[0],iSizeFFT);

        int Order_FFT=log10(iSizeFFT)/log10(2);
//        if(!m_pConfig->IsFFTCreated())
            m_pConfig->CreateFFT(Order_FFT,m_stSettingFF.stFFT.iSizeFFT);

        for (int k = 0; k < 10; k++)
        {

            for(int i=0; i<iSizeFFT; i++)
            {
                FFT_in[i]=(double)(ReadBuff[(k*iSizeFFT)+i] * pow(2.0,Order_FFT));
            }
            m_pConfig->CalcFFT(FFT_in,FFT_out);


//            ippsMagnitude_64f(&FFT_out[0],Amp_Sig,iSizeFFT);
            ippsSqr_64f_I(&FFT_out[0],iSizeFFT);
            ippsAdd_64f_I(&FFT_out[0],&Sum_Amp_Sig[0],iSizeFFT);

        }
        ippsCopy_64f(&Sum_Amp_Sig[0],&Amp_Sig[0],iSizeFFT);
        ippsCopy_64f(&Amp_Sig[0],NF,iSizeFFT/2);
        double FS=Fs;
        double RBW=FS/iSizeFFT;
        int Start_Index=x/RBW;
        int Stop_Index=y/RBW;
        int *Buffer_EST;double *Temp_Ma;
        Buffer_EST=new int[Stop_Index-Start_Index+1];

//        Ipp64f *pDlySrc,*pDlyDst;
//        Ipp8u *pBuffer;
//        pBuffer = ippsMalloc_8u(iSizeFFT);
//        pDlySrc = ippsMalloc_64f(iSizeFFT);
//        pDlyDst = ippsMalloc_64f(iSizeFFT);
//        ippsZero_8u(pBuffer,iSizeFFT);
//        ippsZero_64f(pDlySrc,iSizeFFT);
//        ippsZero_64f(pDlyDst,iSizeFFT);
//        //================================-  Fc medain Filtering   =================================
        int maskSize = (Stop_Index-Start_Index)/10;
//        ippsFilterMedian_64f_I(Amp_Sig,iSizeFFT, maskSize,pDlySrc,pDlyDst,pBuffer);

//        //-----------------------------------------------------

        double dMax;
        double dMin;
        int MinInd,MaxInd;
        int newStop=Start_Index,newStart=Stop_Index;

        ippsMinIndx_64f(&Amp_Sig[Start_Index],Stop_Index-Start_Index+1,&dMin,&MinInd);
        ippsMaxIndx_64f(&Amp_Sig[Start_Index],Stop_Index-Start_Index+1,&dMax,&MaxInd);

        for (int i = MaxInd+Start_Index/*(Start_Index+Stop_Index)/2*/; i < Stop_Index; i++)
        {
            if (Amp_Sig[i]<(dMin+dMax)/2)
            {
                newStop=i;
                break;
            }
        }

        for (int i = MaxInd+Start_Index/*(Start_Index+Stop_Index)/2*/; i > Start_Index; i--)
        {
            if (Amp_Sig[i]<(dMin+dMax)/2)
            {
                newStart=i;
                break;
            }
        }

//        //==========================- End of Medain Filtering -===================================

//        //========================= Fc with Markaze Jerm ===================================
        ippsSortIndexDescend_64f_I(&Amp_Sig[Start_Index], Buffer_EST,Stop_Index-Start_Index+1);
        double weithed_sum=0,summ=0;
        int p=0;
        for (int k = Start_Index; k < Stop_Index+1; k++)
        {
            weithed_sum=weithed_sum+(Buffer_EST[k-Start_Index]+Start_Index)*Amp_Sig[k];
            summ=summ+Amp_Sig[k];
            p=p+1;
            if (2*Amp_Sig[k]<summ/(k-Start_Index+1))
                break;
        }


        int m_Fc_indx;
        m_dEstimatedFc_New = ((newStop+newStart)*RBW/2) / 1e6; // Fc medain Filtering
        return m_dEstimatedFc_New;


}
void Mode_FF::DrawSpectrum(QVector<double> X,QVector<double> Y)
{
        pw_demod_Spectrum->graph(0)->setData(X,Y);
        pw_demod_Spectrum->replot(QCustomPlot::rpImmediate);

}
void Mode_FF::CalculateSpectrum(QVector<double> m_pdOutFFT)
{
    for (int i=0; i< m_iSizeSpectrum; i++)
    {

        double dOffsetSpectrum = m_pConfig->GetOffsetSpectrum();
        int iIndex = i + m_iIndexStartSpectrum;
        if((iIndex < 0) || (iIndex >= m_stSettingFF.stFFT.iSizeFFT))
            continue;
        double dValue =10*log10(pow(abs(m_pdOutFFT[i + m_iIndexStartSpectrum]),2))+dOffsetSpectrum;

        m_bUseAverageSpectrum=true;
        if(m_bUseAverageSpectrum)
            m_pdSpectrum_Y[i] = 0.9*m_pdSpectrum_Y[i] + 0.1*dValue;
        else
            m_pdSpectrum_Y[i] = dValue;

        if(m_bShowMaxHoldSpectrum)
        {
            if(m_pdSpectrum_Y[i] > m_pdMaxHoldSpectrum[i])
                m_pdMaxHoldSpectrum[i] = m_pdSpectrum_Y[i];
        }

        x_Freq = ((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingFF.stFFT.iSizeFFT);
        m_pdSpectrum_X[i]=x_Freq;
    }
     qWaitSpecShowed.wakeOne();
}
bool Mode_FF::Initialize(Configuration *pConfig)
{

    m_pConfig = pConfig;
    FF_ALL_SETTING stSettingFF;
    stSettingFF.stFFT.iSizeFFT = 1*4096;
    stSettingFF.stSpectrum.dStartFrequency = 0;
    stSettingFF.stSpectrum.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
    stSettingFF.stSpectrum.dMaxLevel = 0;
    stSettingFF.stSpectrum.dMinLevel = -150;
    stSettingFF.stSpectrum.dOverlapRatio = 0.0;
    stSettingFF.stSpectrum.bUint_dBm = true;

    SetParameters(stSettingFF);
    return true;
}
void Mode_FF::PlaySpectrum()
{
        if(m_pConfig->ReadFromInputFile(FileReadBuff, m_stSettingFF.stFFT.iSizeFFT*2))
        {
            m_pConfig->CalcFFT(FileReadBuff,m_pdOutFFT);
            CalculateSpectrum(m_pdOutFFT);
            DrawSpectrum(m_pdSpectrum_X,m_pdSpectrum_Y);
            if(b_Analyzed)
               emit sig_ScatterdataReady(ScatterBuff);
        }


}
