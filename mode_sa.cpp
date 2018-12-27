#include "mode_sa.h"

Mode_SA::Mode_SA()
{

}
void Mode_SA::InitGraph(Ui::MainWindow *ui)
{
    signalanalyzer_Spectrum = new CustomPlotZoom;
    signalanalyzer_Spectrum->IsPSD=true;
    signalanalyzer_Spectrum->addGraph();
    signalanalyzer_Spectrum->addGraph();
    signalanalyzer_Spectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_Spectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_Spectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    signalanalyzer_Spectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    signalanalyzer_Spectrum->xAxis->setSubTickPen(QPen(Qt::white, 1));
    signalanalyzer_Spectrum->yAxis->setSubTickPen(QPen(Qt::white, 1));
    signalanalyzer_Spectrum->xAxis->setTickLabelColor(Qt::white);
    signalanalyzer_Spectrum->yAxis->setTickLabelColor(Qt::white);
    signalanalyzer_Spectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalanalyzer_Spectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    signalanalyzer_Spectrum->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalanalyzer_Spectrum->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    signalanalyzer_Spectrum->xAxis->grid()->setSubGridVisible(true);
    signalanalyzer_Spectrum->yAxis->grid()->setSubGridVisible(true);
    signalanalyzer_Spectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalanalyzer_Spectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    signalanalyzer_Spectrum->xAxis->setUpperEnding(QCPLineEnding::esNone);
    signalanalyzer_Spectrum->yAxis->setUpperEnding(QCPLineEnding::esNone);
    signalanalyzer_Spectrum->IsDemoduling=false;
    signalanalyzer_Spectrum->setZoomMode(true);
    signalanalyzer_Spectrum->setXZoomMode(true);
    signalanalyzer_Spectrum->setSignalMode(false);
    signalanalyzer_Spectrum->xAxis->setRange(0, 83.33*1e6);
    signalanalyzer_Spectrum->yAxis->setRange(-130,130);


    signalanalyzer_WaterWall=new QCustomPlot;
    signalanalyzer_WaterWall->setBackground(QBrush(QColor(49, 54, 59)));
    signalanalyzer_WaterWall->xAxis->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->yAxis->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->xAxis->setTickPen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->yAxis->setTickPen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->xAxis->setSubTickPen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->yAxis->setSubTickPen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->xAxis->setTickLabelColor(Qt::white);
    signalanalyzer_WaterWall->xAxis2->setTickLabelColor(Qt::white);
    signalanalyzer_WaterWall->yAxis->setTickLabelColor(Qt::white);
    signalanalyzer_WaterWall->xAxis->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->xAxis2->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->yAxis2->setBasePen(QPen(Qt::white, 1));
    signalanalyzer_WaterWall->xAxis->setLabelColor(Qt::white);
    signalanalyzer_WaterWall->xAxis2->setLabelColor(Qt::white);
    signalanalyzer_WaterWall->yAxis->setLabelColor(Qt::white);
    signalanalyzer_WaterWall->xAxis->setRange(0, 2048);
    signalanalyzer_WaterWall->yAxis->setRange(0, 180);
    signalanalyzer_WaterWall->setMaximumHeight(1000);

    //initialize custom plot
    initializePlot();
    //initialize color scale
    WaterFallColorScale = new QCPColorScale(signalanalyzer_WaterWall);
    //initialize color map
    WaterFallColorMap = new QCPColorMap(signalanalyzer_WaterWall->xAxis, signalanalyzer_WaterWall->yAxis);
    WaterFallColorMap->data()->setSize(1024, 500);
    WaterFallColorMap->data()->setRange(QCPRange(0, 1024), QCPRange(0, 180));
    WaterFallColorMap->setColorScale(WaterFallColorScale); // associate the color map with the color scale
    WaterFallColorMap->setGradient(QCPColorGradient::gpPolar);
   // signalanalyzer_WaterWall->plotLayout()->addElement(0, 1, WaterFallColorScale); // add it to the right of the main axis rect
    WaterFallColorScale->setType(QCPAxis::atBottom); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    WaterFallColorScale->axis()->setTicks(true);
    WaterFallColorScale->axis()->setBasePen(QPen(Qt::white, 1));
    WaterFallColorScale->axis()->setTickPen(QPen(Qt::white, 1));
    WaterFallColorScale->axis()->setSubTickPen(QPen(Qt::white));
    WaterFallColorScale->axis()->setTickLabelColor(Qt::white);



    ui->signalanalyzer_Spectrum->addWidget(signalanalyzer_Spectrum);
    ui->signalanalyzer_WaterWall->addWidget(signalanalyzer_WaterWall);

}
void Mode_SA::initializePlot()
{
    signalanalyzer_WaterWall->setInteractions(QCP::iRangeDrag);
    signalanalyzer_WaterWall->axisRect()->setupFullAxesBox(true);
    signalanalyzer_WaterWall->xAxis->setRange(0,2048);
    signalanalyzer_WaterWall->yAxis->setRange(0,180);
    //signalanalyzer_WaterWall->yAxis->setRange(.write, ViewHeightEnd);
    signalanalyzer_WaterWall->xAxis->setLabel("Angle");
    signalanalyzer_WaterWall->yAxis->setLabel("Time");
}
bool Mode_SA::LoadDataFile(QString path)
{
    if(m_pConfig->OpenInputFile(path))
        pw_ui->lbl_filename_demod->setText(path);
    return m_pConfig->m_bLoadedFileInput;
}
bool Mode_SA::RunMode(void)
{
    m_bRunMode = true;
    m_bStartSpectrum = false;
    m_bPauseSpectrum = false;
    m_bStartSelect = false;
    return true;
}
bool Mode_SA::StartSpectrum(void)
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
bool Mode_SA::StartSpectrumThread(void)
{
  m_bKillThreadSpectrum = false;
//  startthread();

    timer->start(1);

}
void Mode_SA::StopSpectrumThread(void)
{
    m_bKillThreadSpectrum = true;

}
void Mode_SA::CloseMode(void)
{
    StopSpectrum();
    m_bRunMode = false;
}
void Mode_SA::StopSpectrum(void)
{

    if(m_bStartSpectrum)
    {
        if(timer->isActive())
            timer->stop();
        m_bStartSpectrum = false;
        m_bPauseSpectrum = false;
    }
}
void Mode_SA::Close(void)
{
    CloseMode();

}
bool Mode_SA::SetParameters(SA_ALL_SETTING stSettingSA)
{
    m_bInSettingMode = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlaySpectrum()));
    memcpy(&m_stSettingSA, &stSettingSA, sizeof(stSettingSA));

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
        m_stSettingSA.stSpectrum.dStartFrequency / dSamplingFrequency *
        m_stSettingSA.stFFT.iSizeFFT;
    m_iIndexStopSpectrum =
        m_stSettingSA.stSpectrum.dStopFrequency / dSamplingFrequency *
        m_stSettingSA.stFFT.iSizeFFT;




    //--- FFT
    m_pdSignal.resize(m_stSettingSA.stFFT.iSizeFFT);
    m_pdOutFFT.resize(m_stSettingSA.stFFT.iSizeFFT);
    m_calcFFT.SetParameters(m_stSettingSA.stFFT);



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
void Mode_SA::DrawSpectrum(QVector<double> X,QVector<double> Y)
{
        signalanalyzer_Spectrum->graph(0)->setData(X,Y);
        signalanalyzer_Spectrum->replot(QCustomPlot::rpImmediate);


}
void Mode_SA::DrawingWaterfall(QVector<double> Power, int Scale_Angles)
{
    m.lock();
    double MaxValue = 0.0;
    QVector<double> WaterFallSingleLine;
    MaxValue = Power[0];
    for(int i = 1;i < Scale_Angles;i++)
        MaxValue = qMax(MaxValue,Power[i]);
    for (int i = 0; i < Scale_Angles; ++i)
        WaterFallSingleLine.append(pow(Power[i]/MaxValue,10)*100000);
    Power_Data.append(WaterFallSingleLine);
    Line = 0;
    for (int xIndex = Power_Data.length()-1024 < 0 ? 0 : Power_Data.length()-1024; xIndex < Power_Data.length(); ++xIndex)
    {
        for (int yIndex = 0; yIndex < Scale_Angles; ++yIndex)
        {
            WaterFallColorMap->data()->setCell(Line, yIndex, Power_Data[xIndex][yIndex]*Power_Data[xIndex][yIndex]*100000);
        }
        Line++;
    }
    WaterFallColorMap->rescaleDataRange();
    signalanalyzer_WaterWall->rescaleAxes();
    signalanalyzer_WaterWall->replot();
    m.unlock();
}

void Mode_SA::CalculateSpectrum(QVector<double> m_pdOutFFT)
{
    for (int i=0; i< m_iSizeSpectrum; i++)
    {

        double dOffsetSpectrum = m_pConfig->GetOffsetSpectrum();
        int iIndex = i + m_iIndexStartSpectrum;
        if((iIndex < 0) || (iIndex >= m_stSettingSA.stFFT.iSizeFFT))
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
        m_pdSpectrum_X[i]=((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingSA.stFFT.iSizeFFT);
    }
}
bool Mode_SA::Initialize(Configuration *pConfig , Ui::MainWindow *ui)
{
    pw_ui=ui;
    m_pConfig = pConfig;
    SA_ALL_SETTING stSettingSA;
    stSettingSA.stFFT.iSizeFFT = 4096;
    stSettingSA.stFFT.FFT_order=log2(stSettingSA.stFFT.iSizeFFT);
    stSettingSA.stSpectrum.dStartFrequency = 0;
    stSettingSA.stSpectrum.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
    stSettingSA.stSpectrum.dMaxLevel = 0;
    stSettingSA.stSpectrum.dMinLevel = -150;
    stSettingSA.stSpectrum.dOverlapRatio = 0.0;
    stSettingSA.stSpectrum.bUint_dBm = true;

    InitGraph(pw_ui);



    SetParameters(stSettingSA);
    return true;
}
void Mode_SA::PlaySpectrum()
{
        if(m_pConfig->ReadFromInputFile(m_pdSignal, m_stSettingSA.stFFT.iSizeFFT*2))
        {
            m_calcFFT.CalcFFT(m_pdSignal,m_pdOutFFT);
            CalculateSpectrum(m_pdOutFFT);
            DrawSpectrum(m_pdSpectrum_X,m_pdSpectrum_Y);
            DrawingWaterfall(m_pdSpectrum_Y,2048);
        }


}
