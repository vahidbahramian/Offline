#include "mode_sm.h"
Mode_SM::Mode_SM()
{

}
bool Mode_SM::Initialize(Configuration *pConfig , Ui::MainWindow *ui)
{
    pw_ui=ui;
    m_pConfig = pConfig;
    SM_ALL_SETTING stSettingSM;
    stSettingSM.stFFTPre.iSizeFFT = 4096;
//    stSettingSM.stFFTPre.iDftiForwardDomain = DFTI_REAL;
//    stSettingSM.stFFTPre.enWindowType = FWT_HAMMING;

    stSettingSM.stSpectrumPre.dStartFrequency = 0;
    stSettingSM.stSpectrumPre.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
//    FS_Last=m_pConfig->m_stInputFile.dSamplingFrequency;
    stSettingSM.stSpectrumPre.dMaxLevel = 0;
    stSettingSM.stSpectrumPre.dMinLevel = -140;
    stSettingSM.stSpectrumPre.dOverlapRatio = 0.0;
    stSettingSM.stSpectrumPre.bUint_dBm = true;

    stSettingSM.bInputIQ = false;

    stSettingSM.stFFTNxt = stSettingSM.stFFTPre;
    stSettingSM.stSpectrumNxt = stSettingSM.stSpectrumPre;

    stSettingSM.enTypeManipulation = NONE;

    stSettingSM.dInterpRatio = 1.0;
    stSettingSM.dPhaseShift = 0;
    stSettingSM.dNewCarrier = 50;

    stSettingSM.stFilterFIR.dFc = 50;
    stSettingSM.stFilterFIR.dBW = 5;
    stSettingSM.stFilterFIR.dPW = 1;
//    stSettingSM.stFilterFIR.enPassType = BPF;
    stSettingSM.stFilterFIR.nNumTaps = 100;

    stSettingSM.stFilterIIR.dFc = 50;
    stSettingSM.stFilterIIR.dBW = 5;
//    stSettingSM.stFilterIIR.enPassType = BPF;
    stSettingSM.stFilterIIR.nOrder = 6;

    stSettingSM.stFilterBP_DownRate.dFc = 50;
    stSettingSM.stFilterBP_DownRate.dBW = 5;
    stSettingSM.stFilterBP_DownRate.dPW = stSettingSM.stFilterBP_DownRate.dBW / 2;
//    stSettingSM.stFilterBP_DownRate.enPassType = BPF;
    stSettingSM.stFilterBP_DownRate.nNumTaps = 200;

//    stSettingSM.tap_Lp=tapslen_ippLPF;
//    stSettingSM.Fc_Lp=rFreq_ippLPF;
//    stSettingSM.WinType=WinType_Index;

    InitGraph(pw_ui);



    SetParameters(stSettingSM);
    return true;
}
void Mode_SM::InitGraph(Ui::MainWindow *ui)
{
    signalManipulation_Spectrum = new CustomPlotZoom;
    signalManipulation_Spectrum->IsPSD=true;
    signalManipulation_Spectrum->addGraph();
    signalManipulation_Spectrum->addGraph();
    signalManipulation_Spectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    signalManipulation_Spectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    signalManipulation_Spectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum->xAxis->setSubTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum->yAxis->setSubTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum->xAxis->setTickLabelColor(Qt::white);
    signalManipulation_Spectrum->yAxis->setTickLabelColor(Qt::white);
    signalManipulation_Spectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalManipulation_Spectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalManipulation_Spectrum->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalManipulation_Spectrum->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalManipulation_Spectrum->xAxis->grid()->setSubGridVisible(true);
    signalManipulation_Spectrum->yAxis->grid()->setSubGridVisible(true);
    signalManipulation_Spectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalManipulation_Spectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalManipulation_Spectrum->xAxis->setUpperEnding(QCPLineEnding::esNone);
    signalManipulation_Spectrum->yAxis->setUpperEnding(QCPLineEnding::esNone);
    signalManipulation_Spectrum->IsDemoduling=false;
    signalManipulation_Spectrum->setZoomMode(true);
    signalManipulation_Spectrum->setXZoomMode(true);
    signalManipulation_Spectrum->setSignalMode(false);
    signalManipulation_Spectrum->xAxis->setRange(0, 83.33*1e6);
    signalManipulation_Spectrum->yAxis->setRange(-130,130);

    signalManipulation_FilterResponse = new CustomPlotZoom;
    signalManipulation_FilterResponse->IsPSD=true;
    signalManipulation_FilterResponse->addGraph();
    signalManipulation_FilterResponse->addGraph();
    signalManipulation_FilterResponse->xAxis->setBasePen(QPen(Qt::white, 1));
    signalManipulation_FilterResponse->yAxis->setBasePen(QPen(Qt::white, 1));
    signalManipulation_FilterResponse->xAxis->setTickPen(QPen(Qt::white, 1));
    signalManipulation_FilterResponse->yAxis->setTickPen(QPen(Qt::white, 1));
    signalManipulation_FilterResponse->xAxis->setSubTickPen(QPen(Qt::white, 1));
    signalManipulation_FilterResponse->yAxis->setSubTickPen(QPen(Qt::white, 1));
    signalManipulation_FilterResponse->xAxis->setTickLabelColor(Qt::white);
    signalManipulation_FilterResponse->yAxis->setTickLabelColor(Qt::white);
    signalManipulation_FilterResponse->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalManipulation_FilterResponse->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalManipulation_FilterResponse->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalManipulation_FilterResponse->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalManipulation_FilterResponse->xAxis->grid()->setSubGridVisible(true);
    signalManipulation_FilterResponse->yAxis->grid()->setSubGridVisible(true);
    signalManipulation_FilterResponse->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalManipulation_FilterResponse->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalManipulation_FilterResponse->xAxis->setUpperEnding(QCPLineEnding::esNone);
    signalManipulation_FilterResponse->yAxis->setUpperEnding(QCPLineEnding::esNone);
    signalManipulation_FilterResponse->IsDemoduling=false;
    signalManipulation_FilterResponse->setZoomMode(true);
    signalManipulation_FilterResponse->setXZoomMode(true);
    signalManipulation_FilterResponse->setSignalMode(false);

    signalManipulation_Spectrum2 = new CustomPlotZoom;
    signalManipulation_Spectrum2->IsPSD=true;
    signalManipulation_Spectrum2->addGraph();
    signalManipulation_Spectrum2->addGraph();
    signalManipulation_Spectrum2->xAxis->setBasePen(QPen(Qt::white, 1));
    signalManipulation_Spectrum2->yAxis->setBasePen(QPen(Qt::white, 1));
    signalManipulation_Spectrum2->xAxis->setTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum2->yAxis->setTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum2->xAxis->setSubTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum2->yAxis->setSubTickPen(QPen(Qt::white, 1));
    signalManipulation_Spectrum2->xAxis->setTickLabelColor(Qt::white);
    signalManipulation_Spectrum2->yAxis->setTickLabelColor(Qt::white);
    signalManipulation_Spectrum2->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalManipulation_Spectrum2->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalManipulation_Spectrum2->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalManipulation_Spectrum2->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalManipulation_Spectrum2->xAxis->grid()->setSubGridVisible(true);
    signalManipulation_Spectrum2->yAxis->grid()->setSubGridVisible(true);
    signalManipulation_Spectrum2->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalManipulation_Spectrum2->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalManipulation_Spectrum2->xAxis->setUpperEnding(QCPLineEnding::esNone);
    signalManipulation_Spectrum2->yAxis->setUpperEnding(QCPLineEnding::esNone);
    signalManipulation_Spectrum2->IsDemoduling=false;
    signalManipulation_Spectrum2->setZoomMode(true);
    signalManipulation_Spectrum2->setXZoomMode(true);
    signalManipulation_Spectrum2->setSignalMode(false);
    signalManipulation_Spectrum2->xAxis->setRange(0, 83.33*1e6);
    signalManipulation_Spectrum2->yAxis->setRange(-130,130);

    ui->signalmanipulation_Spectrum->addWidget(signalManipulation_Spectrum);
    ui->signalmanipulation_FilterResponse->addWidget(signalManipulation_FilterResponse);
    ui->signalmanipulation_Spectrum2->addWidget(signalManipulation_Spectrum2);

    for (int var = 0; var < n_FILTER_TYPE ;var++) {
        ui->combo_sm_FilterResponse->addItem(g_str_MANIPULATE_TYPE[var]);
    }
}
bool Mode_SM::LoadDataFile(QString path)
{
    if(m_pConfig->OpenInputFile(path))
        pw_ui->lbl_filename_sm->setText(path);
    return m_pConfig->m_bLoadedFileInput;
}
bool Mode_SM::RunMode(void)
{
    m_bRunMode = true;
    m_bStartSpectrum = false;
    m_bPauseSpectrum = false;
    m_bStartSelect = false;
    return true;
}
bool Mode_SM::StartSpectrum(void)
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
bool Mode_SM::StartSpectrumThread(void)
{
  m_bKillThreadSpectrum = false;
  timer->start(1);

}
void Mode_SM::StopSpectrumThread(void)
{
    m_bKillThreadSpectrum = true;

}
void Mode_SM::CloseMode(void)
{
    StopSpectrum();
    m_bRunMode = false;
}
void Mode_SM::StopSpectrum(void)
{
    if(timer->isActive())
        timer->stop();
}
void Mode_SM::Close(void)
{
    CloseMode();

}
bool Mode_SM::SetParameters(SM_ALL_SETTING stSettingSM)
{
    m_bInSettingMode = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlaySpectrum()));
    memcpy(&m_stSettingSM, &stSettingSM, sizeof(stSettingSM));

    if(!m_pdSpectrum_X.isEmpty())
        m_pdSpectrum_X.clear();

    if(!m_pdSignalNxt_X.isEmpty())
        m_pdSignalNxt_X.clear();

    if(!m_pdSignalNxt_Y.isEmpty())
        m_pdSignalNxt_Y.clear();

    if(!m_pdSpectrum_Y.isEmpty())
        m_pdSpectrum_Y.clear();
    if(!m_pdMaxHoldSpectrum.isEmpty())
        m_pdMaxHoldSpectrum.clear();
    if(!m_pdMaxHoldPhaseErr.isEmpty())
        m_pdMaxHoldPhaseErr.clear();
    if(!m_pdOutFFT.isEmpty())
        m_pdOutFFT.clear();


    double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;

    m_iIndexStartSpectrum =
        m_stSettingSM.stSpectrumPre.dStartFrequency / dSamplingFrequency *
        m_stSettingSM.stFFTPre.iSizeFFT;
    m_iIndexStopSpectrum =
        m_stSettingSM.stSpectrumPre.dStopFrequency / dSamplingFrequency *
        m_stSettingSM.stFFTPre.iSizeFFT;




    //--- FFT
//    FileReadBuff = new QVector<double>;
    FileReadBuff.resize(m_stSettingSM.stFFTPre.iSizeFFT);
    m_pdOutFFT.resize(m_stSettingSM.stFFTPre.iSizeFFT);
    m_pdOutFFT_Next.resize(m_stSettingSM.stFFTPre.iSizeFFT);
    if(!m_pConfig->IsFFTCreated())
        m_pConfig->CreateFFT(12,m_stSettingSM.stFFTPre.iSizeFFT);



    //--- Spectrum
    m_iSizeSpectrum = m_iIndexStopSpectrum - m_iIndexStartSpectrum + 1;
    m_pdSpectrum_X.resize(m_iSizeSpectrum);
    m_pdSpectrum_Y.resize(m_iSizeSpectrum);
    m_pdSignalNxt_X.resize(m_iSizeSpectrum);
    m_pdSignalNxt_Y.resize(m_iSizeSpectrum);
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
void Mode_SM::DrawSpectrum_Spectrum2(QVector<double> X,QVector<double> Y)
{

        signalManipulation_Spectrum2->graph(0)->setData(X,Y);
        signalManipulation_Spectrum2->replot(QCustomPlot::rpImmediate);

}
void Mode_SM::DrawSpectrum(QVector<double> X,QVector<double> Y)
{
        signalManipulation_Spectrum->graph(0)->setData(X,Y);
        signalManipulation_Spectrum->replot(QCustomPlot::rpImmediate);

}
void Mode_SM::DrawSpectrum_FilterResponse(QVector<double> X,QVector<double> Y)
{

        signalManipulation_FilterResponse->graph(0)->setData(X,Y);
        signalManipulation_FilterResponse->replot(QCustomPlot::rpImmediate);

}
void Mode_SM::CalculateSpectrum(QVector<double> m_pdOutFFT)
{
    for (int i=0; i< m_iSizeSpectrum; i++)
    {

        double dOffsetSpectrum = m_pConfig->GetOffsetSpectrum();
        int iIndex = i + m_iIndexStartSpectrum;
        if((iIndex < 0) || (iIndex >= m_stSettingSM.stFFTPre.iSizeFFT))
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
        m_pdSpectrum_X[i]=((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingSM.stFFTPre.iSizeFFT);
    }
}
void Mode_SM::PlaySpectrum()
{
        if(m_pConfig->ReadFromInputFile(FileReadBuff, m_stSettingSM.stFFTPre.iSizeFFT*2))
        {
            m_pConfig->CalcFFT(FileReadBuff,m_pdOutFFT);
            CalculateSpectrum(m_pdOutFFT);
            DrawSpectrum(m_pdSpectrum_X,m_pdSpectrum_Y);
            if(m_stSettingSM.enTypeManipulation == NONE)
            {
                FilterFIR(FileReadBuff,m_pdOutFFT_Next);
            }
            CalculateSpectrum(m_pdOutFFT_Next);
            DrawSpectrum_Spectrum2(m_pdSpectrum_X,m_pdSpectrum_Y);
        }


}
void Mode_SM::FilterFIR(QVector<double> In,QVector<double> &Out)
{
    int nSize=12;
    int iSizeFFT=4096;

    status = ippsFIRSRGetSize (nSize,  ipp64f ,  &specSize,  &bufSize );
    double a=1.0/nSize;
    dlysrc   = ippsMalloc_64f(nSize-1);
    dlydst   = ippsMalloc_64f(nSize-1);
    ippsSet_64f(0, dlysrc, nSize-1);
    taps  = ippsMalloc_64f(nSize);
    ippsSet_64f(a, taps, nSize);
    pSpec = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize);
    buf   = ippsMalloc_8u(bufSize);
    ippsFIRSRInit_64f( taps, nSize, ippAlgDirect, pSpec );

    src   = ippsMalloc_64f(iSizeFFT);
    dst   = ippsMalloc_64f(iSizeFFT);

    ippsCopy_64f(&In[0], src, iSizeFFT);
    ippsFIRSR_64f(src, dst,iSizeFFT ,  pSpec,  dlysrc, dlydst, buf);
    ippsCopy_64f(dst, &Out[0], iSizeFFT);

    ippFree(src);
    ippFree(dst);
}

void Mode_SM::FilterIIR(QVector<double> In, QVector<double> &Out)
{
   // m_iSizeSignalNxt = m_stSettingSM.stFFTPre.iSizeFFT;
//    m_filterIIR.Filter(m_pdSignal, m_iSizeSignalNxt, m_pdSignalNxt);
}
void Mode_SM::Carrier_shift()
{

}
