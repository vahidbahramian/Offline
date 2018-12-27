#include "mode_dsss.h"

Mode_DSSS::Mode_DSSS()
{

}
void Mode_DSSS::InitGraph(Ui::MainWindow *ui)
{
    dsss_Spectrum = new CustomPlotZoom;
    dsss_Spectrum->IsPSD=true;
    dsss_Spectrum->addGraph();
    dsss_Spectrum->addGraph();
    dsss_Spectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    dsss_Spectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    dsss_Spectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    dsss_Spectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    dsss_Spectrum->xAxis->setSubTickPen(QPen(Qt::white, 1));
    dsss_Spectrum->yAxis->setSubTickPen(QPen(Qt::white, 1));
    dsss_Spectrum->xAxis->setTickLabelColor(Qt::white);
    dsss_Spectrum->yAxis->setTickLabelColor(Qt::white);
    dsss_Spectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    dsss_Spectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    dsss_Spectrum->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    dsss_Spectrum->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    dsss_Spectrum->xAxis->grid()->setSubGridVisible(true);
    dsss_Spectrum->yAxis->grid()->setSubGridVisible(true);
    dsss_Spectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    dsss_Spectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    dsss_Spectrum->xAxis->setUpperEnding(QCPLineEnding::esNone);
    dsss_Spectrum->yAxis->setUpperEnding(QCPLineEnding::esNone);
    dsss_Spectrum->IsDemoduling=false;
    dsss_Spectrum->setZoomMode(true);
    dsss_Spectrum->setXZoomMode(true);
    dsss_Spectrum->setSignalMode(false);

    dsss_AutoCorr=new CustomPlotZoom;
    dsss_AutoCorr->IsPSD=true;
    dsss_AutoCorr->addGraph();
    dsss_AutoCorr->addGraph();
    dsss_AutoCorr->xAxis->setBasePen(QPen(Qt::white, 1));
    dsss_AutoCorr->yAxis->setBasePen(QPen(Qt::white, 1));
    dsss_AutoCorr->xAxis->setTickPen(QPen(Qt::white, 1));
    dsss_AutoCorr->yAxis->setTickPen(QPen(Qt::white, 1));
    dsss_AutoCorr->xAxis->setSubTickPen(QPen(Qt::white, 1));
    dsss_AutoCorr->yAxis->setSubTickPen(QPen(Qt::white, 1));
    dsss_AutoCorr->xAxis->setTickLabelColor(Qt::white);
    dsss_AutoCorr->yAxis->setTickLabelColor(Qt::white);
    dsss_AutoCorr->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    dsss_AutoCorr->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    dsss_AutoCorr->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    dsss_AutoCorr->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    dsss_AutoCorr->xAxis->grid()->setSubGridVisible(true);
    dsss_AutoCorr->yAxis->grid()->setSubGridVisible(true);
    dsss_AutoCorr->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    dsss_AutoCorr->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    dsss_AutoCorr->xAxis->setUpperEnding(QCPLineEnding::esNone);
    dsss_AutoCorr->yAxis->setUpperEnding(QCPLineEnding::esNone);
    dsss_AutoCorr->IsDemoduling=false;
    dsss_AutoCorr->setZoomMode(true);
    dsss_AutoCorr->setXZoomMode(true);
    dsss_AutoCorr->setSignalMode(false);

    dsss_Welch=new CustomPlotZoom;
    dsss_Welch->IsPSD=true;
    dsss_Welch->addGraph();
    dsss_Welch->addGraph();
    dsss_Welch->xAxis->setBasePen(QPen(Qt::white, 1));
    dsss_Welch->yAxis->setBasePen(QPen(Qt::white, 1));
    dsss_Welch->xAxis->setTickPen(QPen(Qt::white, 1));
    dsss_Welch->yAxis->setTickPen(QPen(Qt::white, 1));
    dsss_Welch->xAxis->setSubTickPen(QPen(Qt::white, 1));
    dsss_Welch->yAxis->setSubTickPen(QPen(Qt::white, 1));
    dsss_Welch->xAxis->setTickLabelColor(Qt::white);
    dsss_Welch->yAxis->setTickLabelColor(Qt::white);
    dsss_Welch->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    dsss_Welch->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    dsss_Welch->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    dsss_Welch->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    dsss_Welch->xAxis->grid()->setSubGridVisible(true);
    dsss_Welch->yAxis->grid()->setSubGridVisible(true);
    dsss_Welch->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    dsss_Welch->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    dsss_Welch->xAxis->setUpperEnding(QCPLineEnding::esNone);
    dsss_Welch->yAxis->setUpperEnding(QCPLineEnding::esNone);
    dsss_Welch->IsDemoduling=false;
    dsss_Welch->setZoomMode(true);
    dsss_Welch->setXZoomMode(true);
    dsss_Welch->setSignalMode(false);

//-----
    dsss_Scatter=new CustomPlotZoom;
    dsss_Scatter->legend->setVisible(true);
    dsss_Scatter->legend->setFont(QFont("Helvetica", 9));
    dsss_Scatter->legend->setRowSpacing(-3);
    dsss_Scatter->addGraph();
    dsss_Scatter->graph()->rescaleAxes(true);
    dsss_Scatter->graph()->setPen(QPen(Qt::white, 1));
    //dsss_Scatter->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
    dsss_Scatter->graph()->setLineStyle(QCPGraph::lsLine);
    dsss_Scatter->rescaleAxes();
    dsss_Scatter->xAxis->setTicks(false);
    dsss_Scatter->yAxis->setTicks(false);
    dsss_Scatter->xAxis->setTickLabels(false);
    dsss_Scatter->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    dsss_Scatter->axisRect()->setupFullAxesBox();

    ui->dsss_Spectrum->addWidget(dsss_Spectrum);
    ui->dsss_Scatter->addWidget(dsss_Scatter);
    ui->dsss_Welch->addWidget(dsss_Welch);
    ui->dsss_AutoCorrelation->addWidget(dsss_AutoCorr);

}
bool Mode_DSSS::LoadDataFile(QString path)
{
    if(m_pConfig->OpenInputFile(path))
        pw_ui->lbl_filename_demod->setText(path);
    return m_pConfig->m_bLoadedFileInput;
}
bool Mode_DSSS::RunMode(void)
{
    m_bRunMode = true;
    m_bStartSpectrum = false;
    m_bPauseSpectrum = false;
    m_bStartSelect = false;
    return true;
}
bool Mode_DSSS::StartSpectrum(void)
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
bool Mode_DSSS::StartSpectrumThread(void)
{
  m_bKillThreadSpectrum = false;
//  startthread();

    timer->start(1);

}
void Mode_DSSS::StopSpectrumThread(void)
{
    m_bKillThreadSpectrum = true;

}
void Mode_DSSS::CloseMode(void)
{
    StopSpectrum();
    m_bRunMode = false;
}
void Mode_DSSS::StopSpectrum(void)
{

    if(m_bStartSpectrum)
    {
        if(timer->isActive())
            timer->stop();
        m_bStartSpectrum = false;
        m_bPauseSpectrum = false;
    }
}
void Mode_DSSS::Close(void)
{
    CloseMode();

}
bool Mode_DSSS::SetParameters(DS_ALL_SETTING stSettingDS)
{
    m_bInSettingMode = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlaySpectrum()));
    memcpy(&m_stSettingDS, &stSettingDS, sizeof(stSettingDS));

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
        m_stSettingDS.stSpectrum.dStartFrequency / dSamplingFrequency *
        m_stSettingDS.stFFT.iSizeFFT;
    m_iIndexStopSpectrum =
        m_stSettingDS.stSpectrum.dStopFrequency / dSamplingFrequency *
        m_stSettingDS.stFFT.iSizeFFT;




    //--- FFT

    m_pdSignal.resize(m_stSettingDS.stFFT.iSizeFFT);
    m_pdOutFFT.resize(m_stSettingDS.stFFT.iSizeFFT);
    if(!m_calcFFT.IsFFTCreated())
        m_calcFFT.SetParameters(m_stSettingDS.stFFT);



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
void Mode_DSSS::DrawSpectrum(QVector<double> X,QVector<double> Y)
{
        dsss_Spectrum->graph(0)->setData(X,Y);
        dsss_Spectrum->replot(QCustomPlot::rpImmediate);

}
void Mode_DSSS::CalculateSpectrum(QVector<double> m_pdOutFFT)
{
    for (int i=0; i< m_iSizeSpectrum; i++)
    {

        double dOffsetSpectrum = m_pConfig->GetOffsetSpectrum();
        int iIndex = i + m_iIndexStartSpectrum;
        if((iIndex < 0) || (iIndex >= m_stSettingDS.stFFT.iSizeFFT))
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
        m_pdSpectrum_X[i]=((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingDS.stFFT.iSizeFFT);
    }
}
bool Mode_DSSS::Initialize(Configuration *pConfig , Ui::MainWindow *ui)
{
    pw_ui=ui;
    m_pConfig = pConfig;
    DS_ALL_SETTING stSettingDS;
    stSettingDS.stFFT.iSizeFFT = 4096;
    stSettingDS.stFFT.FFT_order=log2(stSettingDS.stFFT.iSizeFFT);
    stSettingDS.stSpectrum.dStartFrequency = 0;
    stSettingDS.stSpectrum.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
    stSettingDS.stSpectrum.dMaxLevel = 0;
    stSettingDS.stSpectrum.dMinLevel = -150;
    stSettingDS.stSpectrum.dOverlapRatio = 0.0;
    stSettingDS.stSpectrum.bUint_dBm = true;

    InitGraph(pw_ui);



    SetParameters(stSettingDS);
    return true;
}
void Mode_DSSS::PlaySpectrum()
{
        if(m_pConfig->ReadFromInputFile(m_pdSignal, m_stSettingDS.stFFT.iSizeFFT*2))
        {
            m_calcFFT.CalcFFT(m_pdSignal,m_pdOutFFT);
            CalculateSpectrum(m_pdOutFFT);
            DrawSpectrum(m_pdSpectrum_X,m_pdSpectrum_Y);
        }


}
