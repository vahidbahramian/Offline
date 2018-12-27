#include "mode_ff.h"

Mode_FF::Mode_FF()
{

}
void Mode_FF::InitGraph(Ui::MainWindow *ui)
{
    demod_TimeDomain = new CustomPlotZoom;
    demod_TimeDomain->IsPSD=true;
    demod_TimeDomain->addGraph();
    demod_TimeDomain->graph(0)->setPen(QPen(Qt::green)); // line color blue for first graph
    demod_TimeDomain->graph(0)->setBrush(QBrush(QColor(0, 255, 0, 20)));
    demod_TimeDomain->xAxis->setBasePen(QPen(Qt::white, 1));
    demod_TimeDomain->yAxis->setBasePen(QPen(Qt::white, 1));
    demod_TimeDomain->xAxis->setTickPen(QPen(Qt::white, 1));
    demod_TimeDomain->yAxis->setTickPen(QPen(Qt::white, 1));
    demod_TimeDomain->xAxis->setSubTickPen(QPen(Qt::white, 1));
    demod_TimeDomain->yAxis->setSubTickPen(QPen(Qt::white, 1));
    demod_TimeDomain->xAxis->setTickLabelColor(Qt::white);
    demod_TimeDomain->yAxis->setTickLabelColor(Qt::white);
    demod_TimeDomain->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    demod_TimeDomain->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    demod_TimeDomain->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    demod_TimeDomain->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    demod_TimeDomain->xAxis->grid()->setSubGridVisible(true);
    demod_TimeDomain->yAxis->grid()->setSubGridVisible(true);
    demod_TimeDomain->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    demod_TimeDomain->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    demod_TimeDomain->xAxis->setUpperEnding(QCPLineEnding::esNone);
    demod_TimeDomain->yAxis->setUpperEnding(QCPLineEnding::esNone);
    demod_TimeDomain->IsDemoduling=false;
    demod_TimeDomain->setZoomMode(true);
    demod_TimeDomain->setXZoomMode(true);
    demod_TimeDomain->setSignalMode(false);
    demod_TimeDomain->xAxis->setRange(0, 83.33*1e6);
    demod_TimeDomain->yAxis->setRange(-130,130);



    demod_PhaseErr = new CustomPlotZoom;
    demod_PhaseErr->IsPSD=true;
    demod_PhaseErr->addGraph();
    demod_PhaseErr->addGraph();
    demod_PhaseErr->xAxis->setBasePen(QPen(Qt::white, 1));
    demod_PhaseErr->yAxis->setBasePen(QPen(Qt::white, 1));
    demod_PhaseErr->xAxis->setTickPen(QPen(Qt::white, 1));
    demod_PhaseErr->yAxis->setTickPen(QPen(Qt::white, 1));
    demod_PhaseErr->xAxis->setSubTickPen(QPen(Qt::white, 1));
    demod_PhaseErr->yAxis->setSubTickPen(QPen(Qt::white, 1));
    demod_PhaseErr->xAxis->setTickLabelColor(Qt::white);
    demod_PhaseErr->yAxis->setTickLabelColor(Qt::white);
    demod_PhaseErr->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    demod_PhaseErr->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    demod_PhaseErr->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    demod_PhaseErr->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    demod_PhaseErr->xAxis->grid()->setSubGridVisible(true);
    demod_PhaseErr->yAxis->grid()->setSubGridVisible(true);
    demod_PhaseErr->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    demod_PhaseErr->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    demod_PhaseErr->xAxis->setUpperEnding(QCPLineEnding::esNone);
    demod_PhaseErr->yAxis->setUpperEnding(QCPLineEnding::esNone);
    demod_PhaseErr->IsDemoduling=false;
    demod_PhaseErr->setZoomMode(true);
    demod_PhaseErr->setXZoomMode(true);
    demod_PhaseErr->setSignalMode(false);
    demod_PhaseErr->xAxis->setRange(0, 83.33*1e6);
    demod_PhaseErr->yAxis->setRange(-130,130);


    demod_Spectrum = new CustomPlotZoom;
    demod_Spectrum->IsPSD=true;
    demod_Spectrum->addGraph();
    demod_Spectrum->graph(0)->setPen(QPen(Qt::green)); // line color blue for first graph
    demod_Spectrum->graph(0)->setBrush(QBrush(QColor(0, 255, 0, 20)));
    demod_Spectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    demod_Spectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    demod_Spectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    demod_Spectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    demod_Spectrum->xAxis->setSubTickPen(QPen(Qt::white, 1));
    demod_Spectrum->yAxis->setSubTickPen(QPen(Qt::white, 1));
    demod_Spectrum->xAxis->setTickLabelColor(Qt::white);
    demod_Spectrum->yAxis->setTickLabelColor(Qt::white);
    demod_Spectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    demod_Spectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    demod_Spectrum->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    demod_Spectrum->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    demod_Spectrum->xAxis->grid()->setSubGridVisible(true);
    demod_Spectrum->yAxis->grid()->setSubGridVisible(true);
    demod_Spectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    demod_Spectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    demod_Spectrum->xAxis->setUpperEnding(QCPLineEnding::esNone);
    demod_Spectrum->yAxis->setUpperEnding(QCPLineEnding::esNone);
    demod_Spectrum->IsDemoduling=false;
    demod_Spectrum->setZoomMode(true);
    demod_Spectrum->setXZoomMode(true);
    demod_Spectrum->setSignalMode(false);
    demod_Spectrum->xAxis->setRange(0, 83.33*1e6);
    demod_Spectrum->yAxis->setRange(-130,130);

    demod_Scatter= new CustomPlotZoom;
//    demod_Scatter->legend->setVisible(true);
    demod_Scatter->legend->setFont(QFont("Helvetica", 9));
    demod_Scatter->legend->setRowSpacing(-3);
    demod_Scatter->addGraph();
    demod_Scatter->graph()->rescaleAxes(true);
    demod_Scatter->graph()->setPen(QPen(Qt::white, 1));
    //demod_Scatter->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
    demod_Scatter->graph()->setLineStyle(QCPGraph::lsLine);
    demod_Scatter->rescaleAxes();
    demod_Scatter->xAxis->setTicks(false);
    demod_Scatter->yAxis->setTicks(false);
    demod_Scatter->xAxis->setTickLabels(false);
    demod_Scatter->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    demod_Scatter->axisRect()->setupFullAxesBox();
    demod_Scatter->xAxis->setRange(0, 83.33*1e6);
    demod_Scatter->yAxis->setRange(-130,130);
    ui->demod_Scatter->addWidget(demod_Scatter);
    ui->demod_Spectrum->addWidget(demod_Spectrum);
    ui->demod_PhaseErr->addWidget(demod_PhaseErr);
    ui->demod_TimeDomain->addWidget(demod_TimeDomain);

    for (int var = 0; var < nOSD_DEMOD_TYPE ;var++) {
        ui->combo_FF_Demodename->addItem(g_strOSD_DEMOD_TYPE[var]);
    }
}
bool Mode_FF::LoadDataFile(QString path)
{
    if(m_pConfig->OpenInputFile(path))
        pw_ui->lbl_filename_demod->setText(path);
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
    if(!m_bStartSpectrum)
    {
        m_pConfig->GotoFileStartPos();
        StartSpectrumThread();
        m_bStartSpectrum = true;
        m_bPauseSpectrum = false;
        m_bStartSelect = false;
    }

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

    if(m_bStartSpectrum)
    {
        if(timer->isActive())
            timer->stop();
        m_bStartSpectrum = false;
        m_bPauseSpectrum = false;
    }
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
    if(!m_pdSignal.isEmpty())
        m_pdSignal.clear();
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

    m_pdSignal.resize(m_stSettingFF.stFFT.iSizeFFT);
    m_pdOutFFT.resize(m_stSettingFF.stFFT.iSizeFFT);
    if(!m_calcFFT.IsFFTCreated())
        m_calcFFT.SetParameters(m_stSettingFF.stFFT);



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
void Mode_FF::DrawSpectrum(QVector<double> X,QVector<double> Y)
{
        demod_Spectrum->graph(0)->setData(X,Y);
        demod_Spectrum->replot(QCustomPlot::rpImmediate);

        demod_PhaseErr->graph(0)->setData(X,Y);
        demod_PhaseErr->replot(QCustomPlot::rpImmediate);


//        demod_TimeDomain->graph(0)->setData(X,Y);
//        demod_TimeDomain->replot(QCustomPlot::rpImmediate);

//        demod_Scatter->graph(0)->setData(X,Y);
//        demod_Scatter->replot(QCustomPlot::rpImmediate);



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
        m_pdSpectrum_X[i]=((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingFF.stFFT.iSizeFFT);
    }
}
bool Mode_FF::Initialize(Configuration *pConfig , Ui::MainWindow *ui)
{
    pw_ui=ui;
    m_pConfig = pConfig;
    FF_ALL_SETTING stSettingFF;
    stSettingFF.stFFT.iSizeFFT = 4096;
    stSettingFF.stFFT.FFT_order=log2(stSettingFF.stFFT.iSizeFFT);
    stSettingFF.stSpectrum.dStartFrequency = 0;
    stSettingFF.stSpectrum.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
    stSettingFF.stSpectrum.dMaxLevel = 0;
    stSettingFF.stSpectrum.dMinLevel = -150;
    stSettingFF.stSpectrum.dOverlapRatio = 0.0;
    stSettingFF.stSpectrum.bUint_dBm = true;

    InitGraph(pw_ui);



    SetParameters(stSettingFF);
    return true;
}
void Mode_FF::PlaySpectrum()
{
        if(m_pConfig->ReadFromInputFile(m_pdSignal, m_stSettingFF.stFFT.iSizeFFT*2))
        {
            m_calcFFT.CalcFFT(m_pdSignal,m_pdOutFFT);
            CalculateSpectrum(m_pdOutFFT);
            DrawSpectrum(m_pdSpectrum_X,m_pdSpectrum_Y);
        }


}
void  Mode_FF::ParamEstimate(double d_xRuler,double d_yRuler,double &FC_Out)
{
    ESTIMATE_PARAM stParamEstimate;
    if(!m_pConfig->m_bLoadedFileInput)
        return ;

    if(m_pConfig->IsEndOfFile())
        return ;

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

         int Order_FFT=log2(m_stSettingFF.stFFT.iSizeFFT);
         m_calcFFT_FCEst.SetParameters(m_stSettingFF.stFFT);

        for (int k = 0; k < 10; k++)
        {

            for(int i=0; i<iSizeFFT; i++)
            {
                FFT_in[i]=(double)(ReadBuff[(k*iSizeFFT)+i] * pow(2.0,Order_FFT));
            }
            m_calcFFT_FCEst.CalcFFT(FFT_in,FFT_out);


//            ippsMagnitude_64f(&FFT_out[0],Amp_Sig,iSizeFFT);
            ippsSqr_64f_I(&FFT_out[0],iSizeFFT);
            ippsAdd_64f_I(&FFT_out[0],&Sum_Amp_Sig[0],iSizeFFT);

        }
        ippsCopy_64f(&Sum_Amp_Sig[0],&Amp_Sig[0],iSizeFFT);
        ippsCopy_64f(&Amp_Sig[0],NF,iSizeFFT/2);
        double FS=Fs;
        double RBW=FS/iSizeFFT;
        int Start_Index=d_xRuler/RBW;
        int Stop_Index=d_yRuler/RBW;
        int *Buffer_EST;double *Temp_Ma;
        Buffer_EST=new int[Stop_Index-Start_Index+1];
//================================-  Fc medain Filtering   =================================
        int maskSize = (Stop_Index-Start_Index)/10;
//        ippsFilterMedian_64f_I(Amp_Sig,iSizeFFT, maskSize,pDlySrc,pDlyDst,pBuffer);

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
        stParamEstimate.dFC=((newStop+newStart)*RBW/2) / 1e6; // Fc medain Filtering
        if(p < 0.5 * (Stop_Index - Start_Index))
        {
            summ = 0;
            p = 0;

            for (int k = Start_Index; k < Stop_Index+1; k++)
            {
                summ = summ+Amp_Sig[k];
                p = p + 1;
            }

            m_DNewBandwidth = (d_yRuler - d_xRuler) * g_iINPUT_FREQ_UNIT[(int)m_pConfig->m_stInputFile.enFreqUnit];
            m_Fc_indx=(Stop_Index + Start_Index)/2;
        }
        else
            m_DNewBandwidth = (newStop-newStart+1)* RBW * 1.3; // New BW with Medain Filtering

        double Psig=0;
        Psig = summ / p;

        double Pnoise=Amp_Sig[Start_Index-1]+Amp_Sig[Start_Index-2]+Amp_Sig[Start_Index-3]+Amp_Sig[Start_Index-4]+Amp_Sig[Start_Index-5]+Amp_Sig[Stop_Index+1]+Amp_Sig[Stop_Index+2]+Amp_Sig[Stop_Index+3]+Amp_Sig[Stop_Index+4]+Amp_Sig[Stop_Index+5];
        Pnoise=Pnoise/10;

//        delete [] Amp_Sig;
//        delete [] Buffer_EST;
//        delete [] Sum_Amp_Sig;
        double Noise_floor;
        ippsSortAscend_64f_I(NF,iSizeFFT/2);
        ippsMean_64f(NF,iSizeFFT/2,&Noise_floor);
        int res=iSizeFFT/2; int cnt;
        for (int j = 0; j < 100; j++)
        {
            cnt=0;

            for (int k =0; k < iSizeFFT/2; k++)
            {
                if (NF[k]>2*Noise_floor)
                {
                    NF[k]=0;
                    cnt++;
                }

            }
            res=res-cnt;
            ippsSum_64f(NF,iSizeFFT/2, &Noise_floor);
            Noise_floor=Noise_floor/res;
            if (cnt==0)
            {
                break;
            }
        }

        delete [] NF;


        SNR_New=10*log10(Psig/Pnoise);
        stParamEstimate.dSNR=SNR_New;
        FC_Out = stParamEstimate.dFC;

}

