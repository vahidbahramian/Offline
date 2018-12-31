#include "mode_sm.h"
Mode_SM::Mode_SM()
{
    m_pdSignal = NULL;
    m_pdOutFFT = NULL;
    m_pdOutFFTNxt=NULL;
    m_pdMaxHoldSpectrum = NULL;
    m_pdMaxHoldPhaseErr = NULL;
    ScatterBuff=NULL;
    tapslen_ippLPF=200; //iAdded
    rFreq_ippLPF=1; //iAdded
    WinType_Index=0;
    m_bRunConvert = false;
    m_bFirstFrame = true;
    m_bStartSpectrum = false;

}
Mode_SM::~Mode_SM()
{
}

bool Mode_SM::Initialize(Configuration *pConfig , Ui::MainWindow *ui)
{
    pw_ui=ui;
    m_pConfig = pConfig;
    SM_ALL_SETTING stSettingSM;
    stSettingSM.stFFTPre.iSizeFFT = 4096;
    stSettingSM.stFFTPre.FFT_order=log2(stSettingSM.stFFTPre.iSizeFFT);
    stSettingSM.stSpectrumPre.dStartFrequency = 0;
    stSettingSM.stSpectrumPre.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
    //FS_Last=m_pConfig->m_stInputFile.dSamplingFrequency;
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
    stSettingSM.stFilterFIR.enPassType = BPF;
    stSettingSM.stFilterFIR.nNumTaps = 100;

    stSettingSM.stFilterIIR.dFc = 50;
    stSettingSM.stFilterIIR.dBW = 5;
    stSettingSM.stFilterIIR.enPassType = BPF;
    stSettingSM.stFilterIIR.nOrder = 6;

    stSettingSM.stFilterBP_DownRate.dFc = 50;
    stSettingSM.stFilterBP_DownRate.dBW = 5;
    stSettingSM.stFilterBP_DownRate.dPW = stSettingSM.stFilterBP_DownRate.dBW / 2;
    stSettingSM.stFilterBP_DownRate.enPassType = BPF;
    stSettingSM.stFilterBP_DownRate.nNumTaps = 200;

    stSettingSM.tap_Lp=tapslen_ippLPF;
    stSettingSM.Fc_Lp=rFreq_ippLPF;
    stSettingSM.WinType=WinType_Index;

    InitGraph(pw_ui);



    SetParameters(stSettingSM);
    m_dThresholdLevel = -50;
    m_dCurFrequency = 21.4;
    m_dPosTime = 50;
    m_bUseAverageSpectrum = false;
    m_iCounterCarrierShift = 0;
    return true;
}
void Mode_SM::InitGraph(Ui::MainWindow *ui)
{
    signalManipulation_Spectrum = new CustomPlotZoom;
    signalManipulation_Spectrum->IsPSD=true;
    signalManipulation_Spectrum->addGraph();
    signalManipulation_Spectrum->graph(0)->setPen(QPen(Qt::green)); // line color blue for first graph
    signalManipulation_Spectrum->graph(0)->setBrush(QBrush(QColor(0, 255, 0, 20)));
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
    signalManipulation_FilterResponse->xAxis->setRange(0, 83.33*1e6);
    signalManipulation_FilterResponse->yAxis->setRange(-150,130);

    signalManipulation_Spectrum2 = new CustomPlotZoom;
    signalManipulation_Spectrum2->IsPSD=true;
    signalManipulation_Spectrum2->addGraph();
    signalManipulation_Spectrum2->graph(0)->setPen(QPen(Qt::green)); // line color blue for first graph
    signalManipulation_Spectrum2->graph(0)->setBrush(QBrush(QColor(0, 255, 0, 20)));
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
bool Mode_SM::ReadDataFromFile(void)
{
    //	CreateVirtualSignal();

        bool bReadResult = false;
        if(m_pConfig->m_bLoadedFileInput)
        {
            int iSizeFFT = m_stSettingSM.stFFTPre.iSizeFFT;
            double dOverlapRatio = m_stSettingSM.stSpectrumPre.dOverlapRatio;

            int iStartFile = 0;
            if((dOverlapRatio > 0) && (dOverlapRatio < 1))
            {
                iStartFile = int(dOverlapRatio * iSizeFFT);

                for(int i=0; i<iStartFile; i++)
                    m_pdSignal[i] = m_pdSignal[iSizeFFT - iStartFile - 1 + i] ;
            }


            //----------------------------------------------------------
            //--- Read Data from File ----------------------------------
            //----------------------------------------------------------
            if((m_bKillThreadSpectrum && !m_bRunConvert) || m_pConfig->IsEndOfFile())
                return false;

            if(m_pConfig->m_stInputFile.bInputIQ)
            {
//                if ( m_pConfig->wavefile)
//                    bReadResult=m_pConfig->ReadFromInputFileIQ_wave(m_pdDataI, m_pdDataQ, iSizeFFT);
//                else
//                    bReadResult = m_pConfig->ReadFromInputFileIQ(m_pdDataI, m_pdDataQ, iSizeFFT);
                for(int i=0; i<iSizeFFT; i++)
                    m_pdSignal[i] = m_pdDataI[i];//sqrt(m_pdDataI[i]*m_pdDataI[i] + m_pdDataQ[i]*m_pdDataQ[i]);
                memcpy(&m_pdDataIQ->re,m_pdDataI,sizeof (double)*iSizeFFT);
                memcpy(&m_pdDataIQ->im,m_pdDataQ,sizeof (double)*iSizeFFT);
            }
            else if(!m_pConfig->wavefile)
                    bReadResult = m_pConfig->ReadFromInputFile(m_pdSignal, iSizeFFT);
        }

        return bReadResult;
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
    if(m_bStartSpectrum)
    {
        timer->stop();
        m_bStartSpectrum = false;
        m_bPauseSpectrum = false;
    }

}
void Mode_SM::Close(void)
{
    CloseMode();

}

bool Mode_SM::SetManipulationType(OSA_MANIPULATE_TYPE enType)
{
    SM_ALL_SETTING stSettingSM;
    memcpy(&stSettingSM, &m_stSettingSM, sizeof(stSettingSM));
    stSettingSM.enTypeManipulation = enType;
    return SetParameters(stSettingSM);
}
bool Mode_SM::SetParameters(SM_ALL_SETTING stSettingSM)
{
    m_bInSettingMode = true;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlaySpectrum()));
    memcpy(&m_stSettingSM, &stSettingSM, sizeof(stSettingSM));

    //------------------- i changed --------------//
        stSettingSM.stFFTPre.iSizeFFT = stSettingSM.stFFTPre.iSizeFFT;
        m_stSettingSM.stFFTNxt.iSizeFFT = stSettingSM.stFFTNxt.iSizeFFT;
        //--------------------------------------------//

//        m_iChangeRatio=GetModeSetting()->ChangeRatio;//iAdded
//        m_iChangeRatio=m_stSettingSM.ChangeRatio;//iAdded
//        m_iWinType=GetModeSetting()->WinType;//iAdded
        //--------------------------------------------------------
        //--- Pre. Signal Setting --------------------------------
        //--------------------------------------------------------
        if(!m_pdSpectrum_X.isEmpty())
            m_pdSpectrum_X.clear();
        if(!m_pdSpectrum_Y.isEmpty())
            m_pdSpectrum_Y.clear();
        if(!m_pdSpectrumNxt_X.isEmpty())
            m_pdSpectrumNxt_X.clear();
        if(!m_pdSpectrumNxt_Y.isEmpty())
            m_pdSpectrumNxt_Y.clear();
        if(m_pdMaxHoldSpectrum)
            delete [] m_pdMaxHoldSpectrum;
        if(m_pdMaxHoldPhaseErr)
            delete [] m_pdMaxHoldPhaseErr;
        if(m_pdSignal)
            delete [] m_pdSignal;
        if(m_pdOutFFT)
            delete [] m_pdOutFFT;
        if(m_pdOutFFTNxt)
            delete [] m_pdOutFFTNxt;

        double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;

        if (m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
        {
            stSettingSM.stSpectrumNxt.dStopFrequency=m_pConfig->m_stInputFile.dSamplingFrequency / 2.0 /stSettingSM.dInterpRatio;
        }

        m_iIndexStartSpectrum =
            m_stSettingSM.stSpectrumPre.dStartFrequency / dSamplingFrequency *
            m_stSettingSM.stFFTPre.iSizeFFT;
        m_iIndexStopSpectrum =
            m_stSettingSM.stSpectrumPre.dStopFrequency / dSamplingFrequency *
            m_stSettingSM.stFFTPre.iSizeFFT;



        //--- FFT
        m_pdSignal=new double[m_stSettingSM.stFFTPre.iSizeFFT];
        m_pdOutFFT=new double[m_stSettingSM.stFFTPre.iSizeFFT];
        m_pdDataI = new double [m_stSettingSM.stFFTPre.iSizeFFT];
        m_pdDataQ = new double [m_stSettingSM.stFFTPre.iSizeFFT];
        m_pdDataIQ =ippsMalloc_32fc(m_stSettingSM.stFFTPre.iSizeFFT);
        m_calcFFT.SetParameters(m_stSettingSM.stFFTNxt);


        //--- FFT Filter
        FFT_SETTING stFFTFilter = m_stSettingSM.stFFTPre;
        stFFTFilter.enWindowType = FWT_NONE;
        m_calcFFT_Filter.SetParameters(stFFTFilter);

        //--- Signal Next (Filtered Signal)
        m_pdSignalNxt =new double[m_stSettingSM.stFFTNxt.iSizeFFT];
        m_pdSignalNxt_Complx-ippsMalloc_64fc(m_stSettingSM.stFFTNxt.iSizeFFT);
        m_pdSignalNxt_I = new double [m_stSettingSM.stFFTNxt.iSizeFFT];
        m_pdSignalNxt_Q = new double [m_stSettingSM.stFFTNxt.iSizeFFT];


        //--- Spectrum
        m_iSizeSpectrum = m_iIndexStopSpectrum - m_iIndexStartSpectrum + 1;
        m_pdSpectrum_X.resize(m_iSizeSpectrum);
        m_pdSpectrum_Y.resize(m_iSizeSpectrum);
        for(int i=0; i<m_iSizeSpectrum; i++)
            m_pdSpectrum_Y[i] = -150;

        //--- Max Hold Buffer
        m_pdMaxHoldSpectrum=new double[m_iSizeSpectrum];
        for(int i=0; i<m_iSizeSpectrum; i++)
            m_pdMaxHoldSpectrum[i] = -1e6;



        //--- Spectrum Filter
        m_pdSpectrumFilter_X.resize(m_iSizeSpectrum);
        m_pdSpectrumFilter_Y.resize(m_iSizeSpectrum);
        for(int i=0; i<m_iSizeSpectrum; i++)
            m_pdSpectrumFilter_Y[i] = -150;

        if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
        {
            m_stSettingSM.stFFTNxt.iSizeFFT = m_stSettingSM.stFFTPre.iSizeFFT * m_stSettingSM.dInterpRatio;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
        {
            m_stSettingSM.stFFTNxt.iSizeFFT = m_stSettingSM.stFFTPre.iSizeFFT/* / m_stSettingSM.dInterpRatio*/;
        }

        m_dCurrentDC = 0;




//        //--------------------------------------------------------
//        //--- Next. Signal Setting -------------------------------
//        //--------------------------------------------------------
//        SAFERELEASE_ARRAY(m_pdInterpFrame);
//        SAFERELEASE_ARRAY(m_pdSignalInterp);
//        SAFERELEASE_ARRAY(m_pdSignalInterp_I);
//        SAFERELEASE_ARRAY(m_pdSignalInterp_Q);

        double dSamplingFrequencyNxt;
        if(m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR)
        {
            //	m_pdSignalInterp  = new double[m_stSettingSM.stFFTPre.iSizeFFT];
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_FILTER_IIR)
        {
            //	m_pdSignalInterp  = new double[m_stSettingSM.stFFTPre.iSizeFFT];
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_LoadFilter)
        {
            //	m_pdSignalInterp  = new double[m_stSettingSM.stFFTPre.iSizeFFT];
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_CARRIER_SHIFT)
        {
            //	m_pdSignalInterp  = new double[m_stSettingSM.stFFTPre.iSizeFFT];
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_BaseBandConv)
        {
            //	m_pdSignalInterp  = new double[m_stSettingSM.stFFTPre.iSizeFFT];
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_PHASE_SHIFT)
        {
            //	m_pdSignalInterp  = new double[m_stSettingSM.stFFTPre.iSizeFFT];
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
        {
            m_pdInterpFrame = new double[2+2*m_stSettingSM.stFFTPre.iSizeFFT];
            int iSize = sizeof(double) * (2+2*m_stSettingSM.stFFTPre.iSizeFFT);
            memset(m_pdInterpFrame,0,iSize);

            int iSizeInterp = m_stSettingSM.stFFTPre.iSizeFFT * m_stSettingSM.dInterpRatio ; //+2 // iAdded
            m_pdSignalInterp  = new double[iSizeInterp];
            m_pdSignalInterp_I  = new double[iSizeInterp];
            m_pdSignalInterp_Q  = new double[iSizeInterp];

            dSamplingFrequencyNxt = dSamplingFrequency * m_stSettingSM.dInterpRatio;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
        {
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_DC_REMOVAL)
        {
            dSamplingFrequencyNxt = dSamplingFrequency;
        }
        else if(m_stSettingSM.enTypeManipulation == NONE)
        {
            dSamplingFrequencyNxt = dSamplingFrequency;
        }



        if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
        {
            m_iIndexStartSpectrumNxt =
                m_stSettingSM.stSpectrumNxt.dStartFrequency / dSamplingFrequencyNxt *
                m_stSettingSM.stFFTNxt.iSizeFFT;
            m_iIndexStopSpectrumNxt =
                (m_stSettingSM.stSpectrumNxt.dStopFrequency *m_stSettingSM.dInterpRatio)/ dSamplingFrequencyNxt *
                m_stSettingSM.stFFTNxt.iSizeFFT;
        }
        else
        {
            m_iIndexStartSpectrumNxt =
                m_stSettingSM.stSpectrumNxt.dStartFrequency / dSamplingFrequencyNxt *
                m_stSettingSM.stFFTNxt.iSizeFFT;
            m_iIndexStopSpectrumNxt =
                m_stSettingSM.stSpectrumNxt.dStopFrequency / dSamplingFrequencyNxt *
                m_stSettingSM.stFFTNxt.iSizeFFT;
        }


        //--- FFT
        m_pdOutFFTNxt=new double[m_stSettingSM.stFFTNxt.iSizeFFT];
        m_calcFFTNxt.SetParameters(m_stSettingSM.stFFTNxt);


        //--- Spectrum
        m_iSizeSpectrumNxt = m_iIndexStopSpectrumNxt - m_iIndexStartSpectrumNxt + 1;
        m_pdSpectrumNxt_X.resize(m_iSizeSpectrumNxt);
        m_pdSpectrumNxt_Y.resize(m_iSizeSpectrumNxt);
        for(int i=0; i<m_iSizeSpectrumNxt; i++)
            m_pdSpectrumNxt_Y[i] = -150;


        //--- Max Hold Buffer
        m_pdMaxHoldSpectrumNxt = new double [m_iSizeSpectrumNxt];
        for(int i=0; i<m_iSizeSpectrumNxt; i++)
            m_pdMaxHoldSpectrumNxt[i] = -150;



        //--------------------------------------------------------
        //--- FIR FIlter -----------------------------------------
        //--------------------------------------------------------
        SetFilterParamFIR(stSettingSM.stFilterFIR);


        //--------------------------------------------------------
        //--- IIR FIlter -----------------------------------------
        //--------------------------------------------------------
        SetFilterParamIIR(stSettingSM.stFilterIIR);


        //--------------------------------------------------------
        //--- FIR FIlter -----------------------------------------
        //--------------------------------------------------------
        SetFilterParamLP_DownRate(stSettingSM.stFilterBP_DownRate);


        CalcFilterResponse();


//        //--------------------------------------------------------
//        //--- I/Q Signal Setting ---------------------------------
//        //--------------------------------------------------------
        int nFrameSize = m_stSettingSM.stFFTNxt.iSizeFFT;
//        m_pdSignalReal = new double[nFrameSize];
//        m_pdSignalImag = new double[nFrameSize];
        m_iCounterCarrierShift = 0;

//        LeaveCriticalSection(&m_csAccessParam);
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
void Mode_SM::CalculateSpectrum(double *m_pdOutFFT)
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
bool Mode_SM::CalculateSpectrumInterp(void)
{
    if(m_bKillThreadSpectrum)
        return false;


    if((m_pConfig->m_stInputFile.bInputIQ && m_stSettingSM.enTypeManipulation != OMT_CARRIER_SHIFT) || (m_stSettingSM.enTypeManipulation == OMT_BaseBandConv))
{
//    if(m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR)
//        m_calcFFTNxt.CalcFFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_FILTER_IIR)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_CARRIER_SHIFT)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_BaseBandConv)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_PHASE_SHIFT)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);
//    else if(m_stSettingSM.enTypeManipulation == OMT_DC_REMOVAL)
//        m_calcFFTNxt.FFT(m_pdSignalNxt_Complx, m_pdOutFFTNxt);

}
else
{

    if(m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
    else if(m_stSettingSM.enTypeManipulation == OMT_FILTER_IIR)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
    else if(m_stSettingSM.enTypeManipulation == OMT_LoadFilter)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
    else if(m_stSettingSM.enTypeManipulation == OMT_CARRIER_SHIFT)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
    else if(m_stSettingSM.enTypeManipulation == OMT_PHASE_SHIFT)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
    else if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
    {
        /*m_pdSignalInterp[0] = 0; // iCommented
        m_pdSignalInterp[1] = 0;*/ // iCommented
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt); // iAddedNew
    }
    else if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
    else if(m_stSettingSM.enTypeManipulation == OMT_DC_REMOVAL)
        m_calcFFTNxt.CalcFFT(m_pdSignalNxt, m_pdOutFFTNxt);
}
    double dOffsetSpectrum = m_pConfig->GetOffsetSpectrum();
    for(int i=0; i<m_iSizeSpectrumNxt; i++)
    {
        int iIndex = i + m_iIndexStartSpectrumNxt;
        if((iIndex < 0) || (iIndex >= m_stSettingSM.stFFTNxt.iSizeFFT))
            continue;

        double dValue = 20*log10(m_pdOutFFTNxt[i + m_iIndexStartSpectrumNxt]) + dOffsetSpectrum;

        if(m_bUseAverageSpectrumNxt)
            m_pdSpectrumNxt_Y[i] = 0.9*m_pdSpectrumNxt_Y[i] + 0.1*dValue;
        else
            m_pdSpectrumNxt_Y[i] = dValue;

        if(m_bShowMaxHoldSpectrumNxt)
        {
            if(m_pdSpectrumNxt_Y[i] > m_pdMaxHoldSpectrumNxt[i])
                m_pdMaxHoldSpectrumNxt[i] = m_pdSpectrumNxt_Y[i];
        }
        m_pdSpectrumNxt_X[i]=((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingSM.stFFTNxt.iSizeFFT);
    }

    return true;
}

void Mode_SM::PlaySpectrum()
{
        if(ReadDataFromFile())
        {
            m_calcFFT.CalcFFT(m_pdSignal,m_pdOutFFT);
            CalculateSpectrum(m_pdOutFFT);
            DrawSpectrum(m_pdSpectrum_X,m_pdSpectrum_Y);

            if(m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR)
            {
                FilterFIR();
            //	SaveSignalNxt();
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_FILTER_IIR)
            {
                FilterIIR();
            //	SaveSignalNxt();
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_LoadFilter)
            {
                LoadFilter();
            //	SaveSignalNxt();
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_CARRIER_SHIFT)
            {
                CarierShift();
                //	SaveSignalNxt();
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_BaseBandConv)
            {
                BaseBandConverter();
            }

            else if(m_stSettingSM.enTypeManipulation == OMT_PHASE_SHIFT)
            {
                PhaseShift();
                //	SaveSignalNxt();
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
            {
                 Upsampling(); //Interpolate(); // iAdded
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
            {
                DownRate();
            }
            else if(m_stSettingSM.enTypeManipulation == OMT_DC_REMOVAL)
            {
                RemoveDC();
            }
            CalculateSpectrumInterp();
            DrawSpectrum_Spectrum2(m_pdSpectrumNxt_X,m_pdSpectrumNxt_Y);
        }


}
bool Mode_SM::SetFilterParamFIR(FIR_FILTER stFilterParam)
{
    m_stSettingSM.stFilterFIR = stFilterParam;


    //================== Check Parameters =================//
    double dBW;
    double dfc;
    if (m_stSettingSM.stFilterFIR.dFc > m_pConfig->m_stInputFile.dSamplingFrequency/2 || m_stSettingSM.stFilterFIR.dBW > m_pConfig->m_stInputFile.dSamplingFrequency/2)
    {
        dBW = m_pConfig->m_stInputFile.dSamplingFrequency/4;
        dfc = m_pConfig->m_stInputFile.dSamplingFrequency/4;
    }
    else
    {
        dBW = m_stSettingSM.stFilterFIR.dBW ;
        dfc = m_stSettingSM.stFilterFIR.dFc;
    }

    //=====================================================//



    m_stSettingSM.stFilterFIR.dPW = m_stSettingSM.stFilterFIR.dBW / 2;
    //-------------------------------------------------------
    //--- Filter Design -------------------------------------
    //-------------------------------------------------------


//    status_FIR = ippsFIRSRGetSize (m_stSettingSM.stFilterFIR.nNumTaps,  ipp64f ,  &specSize_FIR,  &bufSize_FIR );
//    nSize=m_stSettingSM.stFilterFIR.nNumTaps;
//    dlysrc_FIR   = ippsMalloc_64f(nSize-1);
//    dlydst_FIR   = ippsMalloc_64f(nSize-1);
//    taps_FIR=new double[nSize];
//    //ippsSet_64f(0, dlysrc_FIR, nSize-1);
//    pSpec_FIR = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize_FIR); //specSize
//    buf_FIR   = ippsMalloc_8u(bufSize_FIR); //bufSize
//    bufLPF=ippsMalloc_8u(tapslen_ippLPF*sizeof(Ipp64f));
//    ippsFIRGenLowpass_64f(rFreq_ippLPF, taps_ippLPF, tapslen_ippLPF, WinType_new, ippTrue,bufLPF); // iAdded
//    status = ippsFIRSRGetSize (tapslen_ippLPF,  ipp64f ,  &specSize,  &bufSize );
    //ippsFIRSRInit_64f(taps_FIR, m_stSettingSM.stFilterFIR.nNumTaps, ippAlgDirect, pSpec_FIR );


    return true;
}
bool Mode_SM::SetFilterParamIIR(IIR_FILTER stFilterParam)
{
    m_stSettingSM.stFilterIIR = stFilterParam;


    //================== Check Parameters =================//
    double dBW;
    double dfc;
    if (m_stSettingSM.stFilterFIR.dFc > m_pConfig->m_stInputFile.dSamplingFrequency/2 || m_stSettingSM.stFilterFIR.dBW > m_pConfig->m_stInputFile.dSamplingFrequency/2)
    {
        dBW = m_pConfig->m_stInputFile.dSamplingFrequency/4;
        dfc = m_pConfig->m_stInputFile.dSamplingFrequency/4;
    }
    else
    {
        dBW = m_stSettingSM.stFilterFIR.dBW ;
        dfc = m_stSettingSM.stFilterFIR.dFc;
    }

    //=====================================================//



    //-------------------------------------------------------
    //--- Filter Design -------------------------------------
    //-------------------------------------------------------
    status_IIR = ippsFIRSRGetSize (m_stSettingSM.FilteBuff_Size,  ipp64f ,  &specSize_IIR,  &bufSize_IIR );
    nSize=m_stSettingSM.FilteBuff_Size;
    dlysrc_IIR   = ippsMalloc_64f(nSize-1);
    dlydst_IIR   = ippsMalloc_64f(nSize-1);
    ippsSet_64f(0, dlysrc_IIR, nSize-1);
    pSpec_IIR = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize_IIR); //specSize
    buf_IIR   = ippsMalloc_8u(bufSize_IIR); //bufSize
    ippsFIRSRInit_64f( m_stSettingSM.FilteBuff, nSize, ippAlgDirect, pSpec_IIR );

    return true;
}

void Mode_SM::CreatLPFilter(int SIZEE) // i Added
{
    double FS=m_pConfig->m_stInputFile.dSamplingFrequency;
    IppWinType WinType_new;
    tapslen_ippLPF=m_stSettingSM.tap_Lp;
    rFreq_ippLPF=m_stSettingSM.Fc_Lp/(FS/2)/2;

    //------ For Upsampling ----//
    if (m_stSettingSM.enTypeManipulation==4)
        rFreq_ippLPF = rFreq_ippLPF/(m_stSettingSM.dInterpRatio);

    //--------------------------//
    if (m_stSettingSM.WinType==0)     WinType_new=ippWinBartlett;
    else if(m_stSettingSM.WinType==1) WinType_new=ippWinBlackman;
    else if(m_stSettingSM.WinType==2) WinType_new=ippWinHamming;
    else if(m_stSettingSM.WinType==3) WinType_new=ippWinHann;
    else if(m_stSettingSM.WinType==4) WinType_new=ippWinRect;

    int *pBuffer;
    pBuffer=new int[tapslen_ippLPF];
    taps_ippLPF = ippsMalloc_64f(tapslen_ippLPF*sizeof(Ipp64f)); // iAdded
    ippsFIRGenGetBufferSize(tapslen_ippLPF,pBuffer);
    ippsFIRGenLowpass_64f(rFreq_ippLPF, taps_ippLPF, tapslen_ippLPF,
                          WinType_new, ippTrue
                          ,(Ipp8u*)pBuffer); // iAdded

    status = ippsFIRSRGetSize (tapslen_ippLPF,  ipp64f ,  &specSize,  &bufSize );

    nSize=tapslen_ippLPF;
    double a=1.0/nSize;
    dlysrc   = ippsMalloc_64f(nSize-1);
    dlydst   = ippsMalloc_64f(nSize-1);
    ippsSet_64f(0, dlysrc, nSize-1);
   // taps_ippLPF  = ippsMalloc_64f(nSize);
    //ippsSet_64f(a, taps_ippLPF, nSize);
    pSpec = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize); //specSize
    //buf   = ippsMalloc_8u(bufSize); //bufSize
    ippsFIRSRInit_64f( taps_ippLPF, nSize, ippAlgDirect, pSpec );

    //dst   = ippsMalloc_64f(SIZEE/8);
}
void Mode_SM::CreatLPFilter_I(int SIZEE) // i Added
{
    double FS=m_pConfig->m_stInputFile.dSamplingFrequency;
    IppWinType WinType_new;
    tapslen_ippLPF=m_stSettingSM.tap_Lp;
    rFreq_ippLPF=m_stSettingSM.Fc_Lp/(FS/2)/2;

    //------ For Upsampling ----//
    if (m_stSettingSM.enTypeManipulation==4)
        rFreq_ippLPF = rFreq_ippLPF/(m_stSettingSM.dInterpRatio);


    if (m_stSettingSM.WinType==0)     WinType_new=ippWinBartlett;
    else if(m_stSettingSM.WinType==1) WinType_new=ippWinBlackman;
    else if(m_stSettingSM.WinType==2) WinType_new=ippWinHamming;
    else if(m_stSettingSM.WinType==3) WinType_new=ippWinHann;
    else if(m_stSettingSM.WinType==4) WinType_new=ippWinRect;

    int *pBuffer;
    taps_ippLPF = ippsMalloc_64f(tapslen_ippLPF*sizeof(Ipp64f)); // iAdded
    pBuffer=new int[tapslen_ippLPF];
    ippsFIRGenGetBufferSize(tapslen_ippLPF,pBuffer);
    ippsFIRGenLowpass_64f(rFreq_ippLPF, taps_ippLPF, tapslen_ippLPF, WinType_new, ippTrue,(Ipp8u*)pBuffer); // iAdded

    statusi = ippsFIRSRGetSize (tapslen_ippLPF,  ipp64f ,  &specSizei,  &bufSizei);

    nSize=tapslen_ippLPF;
    double a=1.0/nSize;
    dlysrci   = ippsMalloc_64f(nSize-1);
    dlydsti   = ippsMalloc_64f(nSize-1);
    ippsSet_64f(0, dlysrci, nSize-1);
   // taps_ippLPF  = ippsMalloc_64f(nSize);
    //ippsSet_64f(a, taps_ippLPF, nSize);
    pSpeci = (IppsFIRSpec_64f*)ippsMalloc_8u(specSizei); //specSize
    bufi   = ippsMalloc_8u(bufSizei); //bufSize
    ippsFIRSRInit_64f( taps_ippLPF, nSize, ippAlgDirect, pSpeci );

    //dst   = ippsMalloc_64f(SIZEE/8);
}
void Mode_SM::CreatLPFilter_Q(int SIZEE) // i Added
{

    double FS=m_pConfig->m_stInputFile.dSamplingFrequency;
    IppWinType WinType_new;
    tapslen_ippLPF=m_stSettingSM.tap_Lp;
    rFreq_ippLPF=m_stSettingSM.Fc_Lp/(FS/2)/2;

    //------ For Upsampling ----//
    if (m_stSettingSM.enTypeManipulation==4)
        rFreq_ippLPF = rFreq_ippLPF/(m_stSettingSM.dInterpRatio);

    if (m_stSettingSM.WinType==0)     WinType_new=ippWinBartlett;
    else if(m_stSettingSM.WinType==1) WinType_new=ippWinBlackman;
    else if(m_stSettingSM.WinType==2) WinType_new=ippWinHamming;
    else if(m_stSettingSM.WinType==3) WinType_new=ippWinHann;
    else if(m_stSettingSM.WinType==4) WinType_new=ippWinRect;
    int *pBuffer;
    taps_ippLPF = ippsMalloc_64f(tapslen_ippLPF*sizeof(Ipp64f)); // iAdded
    pBuffer=new int[tapslen_ippLPF];
    ippsFIRGenGetBufferSize(tapslen_ippLPF,pBuffer);
    ippsFIRGenLowpass_64f(rFreq_ippLPF, taps_ippLPF, tapslen_ippLPF, WinType_new , ippTrue,(Ipp8u*)pBuffer); // iAdded


    statusq = ippsFIRSRGetSize (tapslen_ippLPF,  ipp64f ,  &specSizeq,  &bufSizeq );

    nSize=tapslen_ippLPF;
    double a=1.0/nSize;
    dlysrcq   = ippsMalloc_64f(nSize-1);
    dlydstq   = ippsMalloc_64f(nSize-1);
    ippsSet_64f(0, dlysrcq, nSize-1);
   // taps_ippLPF  = ippsMalloc_64f(nSize);
    //ippsSet_64f(a, taps_ippLPF, nSize);
    pSpecq = (IppsFIRSpec_64f*)ippsMalloc_8u(specSizeq); //specSize
    bufq   = ippsMalloc_8u(bufSizeq); //bufSize
    ippsFIRSRInit_64f( taps_ippLPF, nSize, ippAlgDirect, pSpecq );

    //dst   = ippsMalloc_64f(SIZEE/8);
}
void Mode_SM::CreatLoadFilter(int SIZEE) // i Added
{
    double FS=m_pConfig->m_stInputFile.dSamplingFrequency;


    status_LF = ippsFIRSRGetSize (m_stSettingSM.FilteBuff_Size,  ipp64f ,  &specSize_LF,  &bufSize_LF );

    nSize=m_stSettingSM.FilteBuff_Size;
    dlysrc_LF   = ippsMalloc_64f(nSize-1);
    dlydst_LF   = ippsMalloc_64f(nSize-1);
    ippsSet_64f(0, dlysrc_LF, nSize-1);

    pSpec_LF = (IppsFIRSpec_64f*)ippsMalloc_8u(specSize_LF); //specSize
    buf_LF   = ippsMalloc_8u(bufSize_LF); //bufSize
    ippsFIRSRInit_64f( m_stSettingSM.FilteBuff, nSize, ippAlgDirect, pSpec_LF );

}
bool Mode_SM::SetFilterParamLP_DownRate(FIR_FILTER stFilterParam)
{

    CreatLPFilter(m_stSettingSM.stFFTPre.iSizeFFT);
    CreatLPFilter_I(m_stSettingSM.stFFTPre.iSizeFFT); // iAdded
    CreatLPFilter_Q(m_stSettingSM.stFFTPre.iSizeFFT); // iAdded
    CreatLoadFilter(m_stSettingSM.stFFTPre.iSizeFFT);

    m_stSettingSM.stFilterBP_DownRate = stFilterParam;


    //================== Check Parameters =================//
    double dBW;
    double dfc;
    if (m_stSettingSM.stFilterFIR.dFc > m_pConfig->m_stInputFile.dSamplingFrequency/2 || m_stSettingSM.stFilterFIR.dBW > m_pConfig->m_stInputFile.dSamplingFrequency/2)
    {
        dBW = m_pConfig->m_stInputFile.dSamplingFrequency/4;
        dfc = m_pConfig->m_stInputFile.dSamplingFrequency/4;
    }
    else
    {
        dBW = m_stSettingSM.stFilterFIR.dBW ;
        dfc = m_stSettingSM.stFilterFIR.dFc;
    }

    //=====================================================//




    m_stSettingSM.stFilterBP_DownRate.dPW = m_stSettingSM.stFilterBP_DownRate.dBW / 2;

    double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;
    double dDownSample =m_stSettingSM.dInterpRatio;//2 // dSamplingFrequency / 2 / m_stSettingSM.stFilterBP_DownRate.dBW / 2; // iAdded
    m_iDownRate_DownSample = dDownSample; // pow(2.0, floor(log10(dDownSample) / log10(2.0)));;	//4; // iAdded

    double dUpSample =m_stSettingSM.dInterpRatio;//2; // iAdded
    m_iDownRate_UpSample = dUpSample; // iAdded

    //-------------------------------------------------------
    //--- Filter Design -------------------------------------
    //-------------------------------------------------------
//    m_filterBP_DownRate.DesignFirFilter_PMC(
//        m_stSettingSM.stFilterBP_DownRate.enPassType,
//        m_pConfig->m_stInputFile.dSamplingFrequency,
//        m_stSettingSM.stFilterBP_DownRate.dFc,
//        m_stSettingSM.stFilterBP_DownRate.dBW,
//        m_stSettingSM.stFilterBP_DownRate.dPW,
//        m_stSettingSM.stFilterBP_DownRate.nNumTaps);

    return true;
}

void Mode_SM::FilterFIR()
{
     ippsFIRSR_64f(&m_pdSignal[0], &m_pdSignalNxt[0], m_stSettingSM.stFFTPre.iSizeFFT,  pSpec_FIR,  dlysrc_FIR, dlydst_FIR,  buf_FIR);
}
void Mode_SM::FilterIIR()
{
//    ippsIIR_64f(&In[0],&Out[0],In.size(),pState);
}
void Mode_SM::CarierShift()
{
    int iSizeSignal = m_stSettingSM.stFFTPre.iSizeFFT;

        m_iSizeSignalNxt = iSizeSignal;

        double dDeltaf = abs(m_stSettingSM.dNewCarrier - m_dCurFrequency);
        double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;

        if(m_pConfig->m_stInputFile.bInputIQ)
        {
            double dNewCarrier = m_stSettingSM.dNewCarrier;
            double dNewI, dNewQ, teta, dOutput;
            for (int i=0; i<iSizeSignal; i++)
            {
                teta = 2*PI * dNewCarrier*(m_iCounterCarrierShift+i)/dSamplingFrequency;
                dNewI = m_pdDataI[i] * cos(teta) - m_pdDataQ[i] * sin(teta);
                dNewQ = m_pdDataI[i] * sin(teta) + m_pdDataQ[i] * cos(teta);

                //dOutput = 2*(dNewI -dNewQ);
                dOutput = 1*(dNewI);


                m_pdSignalNxt[i] = dOutput;//dOutput;
            }
            m_iCounterCarrierShift += iSizeSignal;
        }
        else
        {
            //--------------- i Added
            double dNewCarrier = m_dCurFrequency;
            double *dNewI, *dNewQ, teta;
            dNewI = new double[iSizeSignal];
            dNewQ = new double[iSizeSignal];


            for (int i=0; i<iSizeSignal; i++)
            {
                teta = 2*PI * dNewCarrier*(m_iCounterCarrierShift+i)/dSamplingFrequency;
                dNewI[i] = m_pdSignal[i] * cos(teta) ;
                dNewQ[i] = m_pdSignal[i] * sin(teta) ;
            }


          //------------------------------------------------------------
          //-------------------   LPF (ipp) // iAdded   ----------------
          //------------------------------------------------------------

         double *dst_I   =new double[iSizeSignal];
         double *dst_Q   =new double[iSizeSignal];

         ippsFIRSR_64f(dNewI, dst_I, iSizeSignal,  pSpeci,  dlysrci, dlydsti,  bufi);
         ippsCopy_64f(dlydsti,dlysrci,tapslen_ippLPF);

         ippsFIRSR_64f(dNewQ, dst_Q, iSizeSignal,  pSpecq,  dlysrcq, dlydstq,  bufq);
         ippsCopy_64f(dlydstq,dlysrcq,tapslen_ippLPF);

          //---------------- Shift to New Carrier ------------------ // iAdded
         dNewCarrier = m_stSettingSM.dNewCarrier;
         for (int i=0; i<iSizeSignal; i++)
            {
                teta = 2*PI * dNewCarrier*(m_iCounterCarrierShift+i)/dSamplingFrequency;
                m_pdSignalNxt[i] = 2* (dst_I[i] * cos(teta) - dst_Q[i] * sin(teta) );
            }

          m_iCounterCarrierShift += iSizeSignal;


          delete [] dst_I;
          delete [] dst_Q;
          delete [] dNewI;
          delete [] dNewQ;

            //--------------- i commented
            //for (int i=0; i<iSizeSignal; i++)
            //	m_pdSignalNxt[i] = 2 * m_pdSignal[i] * cos(2*PI*dDeltaf* (m_iCounterCarrierShift + i) /dSamplingFrequency);
            //m_iCounterCarrierShift += iSizeSignal;
        }
}
void Mode_SM::PhaseShift(void)
{
    int iSizeSignal = m_stSettingSM.stFFTPre.iSizeFFT;

    m_iSizeSignalNxt = iSizeSignal;

    if(m_pConfig->m_stInputFile.bInputIQ)
    {
        double dNewI, dNewQ, teta;
        for (int i=0; i<iSizeSignal; i++)
        {
            teta = PI * m_stSettingSM.dPhaseShift;
            dNewI = m_pdDataI[i] * cos(teta) - m_pdDataQ[i] * sin(teta);
            dNewQ = m_pdDataI[i] * sin(teta) + m_pdDataQ[i] * cos(teta);

            m_pdSignalNxt[i] = dNewI;
        }
    }
    else
    {
        for (int i=0; i<iSizeSignal; i++)
            m_pdSignalNxt[i] = m_pdSignal[i];
    }
}
void Mode_SM::LoadFilter(void)
{
    int iSizeSignal = m_stSettingSM.stFFTPre.iSizeFFT;

    m_iSizeSignalNxt = iSizeSignal ;

    //------------------------------------------------------------
    //-------------------   LPF (ipp) // iAdded   ----------------
    //------------------------------------------------------------

    ippsFIRSR_64f(&m_pdSignal[0], &m_pdSignalNxt[0], iSizeSignal,  pSpec_LF,  dlysrc_LF, dlydst_LF,  buf_LF);

    ippsCopy_64f(dlydst_LF,dlysrc_LF,m_stSettingSM.FilteBuff_Size);


}
void Mode_SM::BaseBandConverter(void)
{
    int iSizeSignal = m_stSettingSM.stFFTPre.iSizeFFT;

    m_iSizeSignalNxt = iSizeSignal;

    double dDeltaf = abs(m_stSettingSM.dNewCarrier - m_dCurFrequency);
    double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;

        //--------------- i Added
        double dNewCarrier = m_dCurFrequency;
        double *dNewI, *dNewQ, teta;
        dNewI = new double[iSizeSignal];
        dNewQ = new double[iSizeSignal];


        for (int i=0; i<iSizeSignal; i++)
        {
            teta = 2*PI * dNewCarrier*(m_iCounterCarrierShift+i)/dSamplingFrequency;
            dNewI[i] = 2*m_pdSignal[i] * cos(teta) ;
            dNewQ[i] = 2*m_pdSignal[i] * sin(teta) ;
        }

        m_iCounterCarrierShift += iSizeSignal;
      //------------------------------------------------------------
      //-------------------   LPF (ipp) // iAdded   ----------------
      //------------------------------------------------------------

     ippsFIRSR_64f(dNewI, m_pdSignalNxt_I, iSizeSignal,  pSpeci,  dlysrci, dlydsti,  bufi);
     ippsCopy_64f(dlydsti,dlysrci,tapslen_ippLPF);

     ippsFIRSR_64f(dNewQ, m_pdSignalNxt_Q, iSizeSignal,  pSpecq,  dlysrcq, dlydstq,  bufq);
     ippsCopy_64f(dlydstq,dlysrcq,tapslen_ippLPF);


    memcpy(&m_pdSignalNxt_Complx->re,m_pdSignalNxt_I,sizeof (double)*m_iSizeSignalNxt);
    memcpy(&m_pdSignalNxt_Complx->im,m_pdSignalNxt_Q,sizeof (double)*m_iSizeSignalNxt);



     delete [] dNewI;
     delete [] dNewQ;
}
void Mode_SM::SaveSignalNxt(void)
{


    if ((m_pConfig->m_stInputFile.bInputIQ && m_stSettingSM.enTypeManipulation != OMT_CARRIER_SHIFT) || (m_stSettingSM.enTypeManipulation == OMT_BaseBandConv))
    {
        m_pConfig->WriteToSaveFileIQ(&m_pdSignalNxt_I[0],m_pdSignalNxt_Q, m_iSizeSignalNxt);
    }
    else
    {
        m_pConfig->WriteToSaveFile(&m_pdSignalNxt[0], m_iSizeSignalNxt);

    }

}
bool Mode_SM::Interpolate(void)
{
    double piTime[4] = {0,0,0,0};
    double pdSignal[4] = {0,0,0,0};
    int piIndex[4] = {0,0,0,0};
    double *dOutput;
    int iSizeFFT = m_stSettingSM.stFFTPre.iSizeFFT;
    double iIndex = 0;

    int iDoubleLen = sizeof(double);

    memcpy(m_pdInterpFrame, m_pdInterpFrame+iSizeFFT, /*iSizeFFT*/(2+iSizeFFT)*iDoubleLen);
    memcpy(m_pdInterpFrame+iSizeFFT+2, &m_pdSignal[0], iSizeFFT*iDoubleLen);


    //----------------------------------------------------
    double dInterpRatio = m_stSettingSM.dInterpRatio;
    int iOutFrameSize = iSizeFFT * dInterpRatio;
    double dInputSamplingFreq = m_pConfig->m_stInputFile.dSamplingFrequency * g_iINPUT_FREQ_UNIT[(int)m_pConfig->m_stInputFile.enFreqUnit];
    double dOutputSamplingFreq = dInputSamplingFreq * dInterpRatio;


    for (int i=2; i<iOutFrameSize+2; i++)
    {
        iIndex = (int)(i / dInterpRatio);
        piTime[1] = iIndex;
        if(iIndex == 0)
        {
            piTime[0] = 0;
            piIndex[0] = 0;
        }
        else
        {
            piTime[0] = (iIndex - 1) * 10e9 / dInputSamplingFreq;
            piIndex[0] = (int)((iIndex - 1));
        }
        piTime[1] = iIndex * 10e9 / dInputSamplingFreq;
        piTime[2] = (iIndex + 1) * 10e9 / dInputSamplingFreq;
        piTime[3] = (iIndex + 2) * 10e9 / dInputSamplingFreq;

        piIndex[1] = (int)(iIndex );
        piIndex[2] = (int)((iIndex + 1));
        piIndex[3] = (int)((iIndex + 2));

        pdSignal[0] = m_pdInterpFrame[piIndex[0]];
        pdSignal[1] = m_pdInterpFrame[piIndex[1]];
        pdSignal[2] = m_pdInterpFrame[piIndex[2]];
        pdSignal[3] = m_pdInterpFrame[piIndex[3]];

        m_pdSignalInterp[i] =  CalcInterpolation( piTime, pdSignal, 4, (i * 10e9 /dOutputSamplingFreq));
    }

     return true;
}
void Mode_SM::DownRate(void)
{
    int iSizeSignal = m_stSettingSM.stFFTPre.iSizeFFT;

    double dCenter = m_stSettingSM.stFilterFIR.dFc;
    double dBW = m_stSettingSM.stFilterBP_DownRate.dBW;
    double dDeltaf = abs(dCenter - dBW);
    double dSamplingFrequency = m_pConfig->m_stInputFile.dSamplingFrequency;

    m_iSizeSignalNxt = iSizeSignal / m_iDownRate_DownSample;



if(m_pConfig->m_stInputFile.bInputIQ)
{

    double *dst_I =new double[iSizeSignal];
    double *dst_Q =new double[iSizeSignal];
     ippsFIRSR_64f(m_pdDataI, dst_I, iSizeSignal,  pSpeci,  dlysrci, dlydsti,  bufi);
     ippsCopy_64f(dlydsti,dlysrci,tapslen_ippLPF);

     ippsFIRSR_64f(m_pdDataQ, dst_Q, iSizeSignal,  pSpecq,  dlysrcq, dlydstq,  bufq);
     ippsCopy_64f(dlydstq,dlysrcq,tapslen_ippLPF);


        for (int i=0; i<iSizeSignal; i++)
        {
                m_pdSignalNxt_I[i] = 0;
                m_pdSignalNxt_Q[i] = 0;
        }

     for (int i=0; i<m_iSizeSignalNxt; i++)
        m_pdSignalNxt_I[i] = dst_I[i*m_iDownRate_DownSample];


     for (int i=0; i<m_iSizeSignalNxt; i++)
        m_pdSignalNxt_Q[i] = dst_Q[i*m_iDownRate_DownSample];


    memcpy(&m_pdSignalNxt_Complx->re,m_pdSignalNxt_I,sizeof (double)*iSizeSignal);
    memcpy(&m_pdSignalNxt_Complx->im,m_pdSignalNxt_Q,sizeof (double)*iSizeSignal);

    delete dst_I;
    delete dst_Q;
}
else
{
//======================== iAdded (i commented) ===========================//

    //------------------------------------------------------------
    //-------------------   LPF (ipp) // iAdded   ----------------
    //------------------------------------------------------------

     double *dst   =new double[iSizeSignal];

     ippsFIRSR_64f(m_pdSignal, dst, iSizeSignal,  pSpec,  dlysrc, dlydst,  buf);

     ippsCopy_64f(dlydst,dlysrc,tapslen_ippLPF);


    //------------------------------------------------------------
    //--- Down Sample --------------------------------------------
    //------------------------------------------------------------
    double *pdInt = new double [iSizeSignal];
    double dSum = 0;

    for (int i=0; i<iSizeSignal; i++)
    {
        m_pdSignalNxt[i] = 0;
        dSum += dst[i];
        pdInt[i] = dSum;
    }

    for (int i=0; i<m_iSizeSignalNxt; i++)
        m_pdSignalNxt[i] = dst[i*m_iDownRate_DownSample];  //pdInt[i*m_iDownRate_DownSample+1] - pdInt[i*m_iDownRate_DownSample];


            //			if(1)
                //{
                //	FILE *pFile;
                //	fopen_s(&pFile, "d:\\DataDown5.txt", "at");
                //	if(pFile)
                //	{
                //		for(int i=0; i<m_iSizeSignalNxt; i++)
                //			fprintf_s(pFile, "%.6f\n", m_pdSignalNxt[i]);
                //		fclose(pFile);
                //	}
                //}

//	ippsMulC_64f_I(m_iDownRate_DownSample,m_pdSignalNxt,m_iSizeSignalNxt);


    delete pdInt;
    delete dst;

}
}
void Mode_SM::Upsampling(void) // iAdded
{
    int iSizeSignal = m_stSettingSM.stFFTPre.iSizeFFT;
    m_iSizeSignalNxt = iSizeSignal * m_iDownRate_UpSample;
    int phase=0;


if(m_pConfig->m_stInputFile.bInputIQ)
{
     ippsSampleUp_64f (m_pdDataI, iSizeSignal, m_pdSignalInterp_I,&m_iSizeSignalNxt, m_iDownRate_UpSample, &phase);
     ippsSampleUp_64f (m_pdDataQ, iSizeSignal, m_pdSignalInterp_Q,&m_iSizeSignalNxt, m_iDownRate_UpSample, &phase);


     ippsFIRSR_64f(m_pdSignalInterp_I, m_pdSignalNxt_I, m_iSizeSignalNxt,  pSpeci,  dlysrci, dlydsti,  bufi);
     ippsCopy_64f(dlydsti,dlysrci,tapslen_ippLPF);
     ippsMulC_64f_I(m_iDownRate_UpSample,m_pdSignalNxt_I,m_iSizeSignalNxt);

     ippsFIRSR_64f(m_pdSignalInterp_Q, m_pdSignalNxt_Q, m_iSizeSignalNxt,  pSpecq,  dlysrcq, dlydstq,  bufq);
     ippsCopy_64f(dlydstq,dlysrcq,tapslen_ippLPF);
     ippsMulC_64f_I(m_iDownRate_UpSample,m_pdSignalNxt_Q,m_iSizeSignalNxt);


    memcpy(&m_pdSignalNxt_Complx->re,m_pdSignalNxt_I,sizeof (double)*m_iSizeSignalNxt);
    memcpy(&m_pdSignalNxt_Complx->im,m_pdSignalNxt_Q,sizeof (double)*m_iSizeSignalNxt);

}
else
{
    //------------------------------------------------------------
    //--- Zero Padding --------------------------------------------
    //------------------------------------------------------------

     ippsSampleUp_64f (&m_pdSignal[0], iSizeSignal, m_pdSignalInterp,&m_iSizeSignalNxt, m_iDownRate_UpSample, &phase);

    //------------------------------------------------------------
    //-------------------   LPF (ipp) // iAdded   ----------------
    //------------------------------------------------------------

     ippsFIRSR_64f(m_pdSignalInterp, m_pdSignalNxt, m_iSizeSignalNxt,  pSpec,  dlysrc, dlydst,  buf);
     ippsMulC_64f_I(m_iDownRate_UpSample,m_pdSignalNxt,m_iSizeSignalNxt);

     ippsCopy_64f(dlydst,dlysrc,tapslen_ippLPF);

}


}

void Mode_SM::RemoveDC(void)
{
    m_iSizeSignalNxt = m_stSettingSM.stFFTPre.iSizeFFT;

    double dDC = 0;
    for(int i=0; i<m_iSizeSignalNxt; i++)
        dDC += m_pdSignal[i];
    dDC /= m_iSizeSignalNxt;

    m_dCurrentDC = 0.9 * m_dCurrentDC + 0.1 * dDC;

    for(int i=0; i<m_iSizeSignalNxt; i++)
        m_pdSignalNxt[i] = m_pdSignal[i] - m_dCurrentDC;
}
double Mode_SM::CalcInterpolation(double *piTime, double *pdDataIn, int iNumPoint, double dCurrTime)
{
    if(iNumPoint <= 1)
        return 0;

    double a[60];
    memset(a, 0, sizeof(a));
    double h[60];
    memset(h, 0, sizeof(h));
    h[0] = 1000;

    for (int i=1; i<iNumPoint; i++)
    {
        if(i == 59)
            int a=1;
        double a1 = piTime[i];
        double a2 = piTime[i-1];
        double a3 = (piTime[i] - piTime[i - 1]);

        h[i] = (piTime[i] - piTime[i - 1]);
        if(h[i] == 0)
            h[i] = 1;
    }

    double sub[60];		sub[0] = 0;
    double diag[60];	diag[0] = 0;
    double sup[60];		sup[0] = 0;

    for (int i=1; i<(iNumPoint-1); i++)
    {
        diag[i] = (h[i] + h[i+1])/3;
        sup[i] = h[i+1]/6;
        sub[i] = h[i]/6;
        a[i] = (pdDataIn[i+1] - pdDataIn[i])/h[i+1] -
            (pdDataIn[i] - pdDataIn[i - 1])/h[i];
    }


    solveTridiag(sub, diag, sup, a, iNumPoint-2);
    //return;


    double x1,x2,x,y;
    for (int i=2; i<3; i++)
    {
        if(piTime[i] == 0)
            continue;

        // loop over intervals between nodes
        int iStart = piTime[i - 1];
        int iEnd = piTime[i];
        int counter = iStart;

        x1 = dCurrTime - iStart;
        x2 = h[i] - x1;
        y = ((-a[i-1]/6*(x2+h[i])*x1 + pdDataIn[i-1])*x2 +
            (-a[i]/6*(x1+h[i])*x2 + pdDataIn[i])*x1)/h[i];


        counter++;
    }
    return y;
}
void Mode_SM::solveTridiag(double *sub, double *diag, double *sup, double *b, int n)
{
    /*                  solve linear system with tridiagonal n by n matrix a
    using Gaussian elimination *without* pivoting
    where   a(i,i-1) = sub[i]  for 2<=i<=n
    a(i,i)   = diag[i] for 1<=i<=n
    a(i,i+1) = sup[i]  for 1<=i<=n-1
    (the values sub[1], sup[n] are ignored)
    right hand side vector b[1:n] is overwritten with solution
    NOTE: 1...n is used in all arrays, 0 is unused */
    int i;
    /*                  factorization and forward substitution */
    for(i=2; i<=n; i++)
    {
        sub[i] = sub[i]/diag[i-1];
        diag[i] = diag[i] - sub[i]*sup[i-1];
        b[i] = b[i] - sub[i]*b[i-1];
    }
    b[n] = b[n]/diag[n];
    for(i=n-1;i>=1;i--)
    {
        b[i] = (b[i] - sup[i]*b[i+1])/diag[i];
    }
}
bool Mode_SM::CalcFilterResponse(void)
{
    //-------------------------------------------------------
    //--- Filter Spectrum -----------------------------------
    //-------------------------------------------------------
    int iSizeFFT = m_stSettingSM.stFFTPre.iSizeFFT;
    int nNumTaps = m_stSettingSM.stFilterFIR.nNumTaps;

    double *pdSignalStep=new double[iSizeFFT];
    double *pdSignalFilter=new double[iSizeFFT];
    double *pdOutFFTFilter=new double[iSizeFFT];



    if(m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR)
    {

        nNumTaps = m_stSettingSM.stFilterFIR.nNumTaps;
        for(int i=0; i<nNumTaps; i++)
        pdSignalFilter[i] =taps_ippLPF[i];
        for(int i=nNumTaps; i<iSizeFFT; i++)
        pdSignalFilter[i] = 0;
    }
    else if(m_stSettingSM.enTypeManipulation == OMT_FILTER_IIR)
    {
        //m_filterIIR.ResetFilter();
        for(int i=0; i<iSizeFFT; i++)
            pdSignalStep[i] = 0;
        pdSignalStep[0] = 1;
        //m_filterIIR.Filter(pdSignalStep, iSizeFFT, pdSignalFilter);
        //m_filterIIR.ResetFilter();
    }
    else if(m_stSettingSM.enTypeManipulation == OMT_LoadFilter)
    {
        nNumTaps = m_stSettingSM.FilteBuff_Size ;
        for(int i=0; i<nNumTaps; i++)
            pdSignalFilter[i] = m_stSettingSM.FilteBuff[i];
        for(int i=nNumTaps; i<iSizeFFT; i++)
            pdSignalFilter[i] = 0;
    }
    if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
    {
        nNumTaps =  tapslen_ippLPF; // m_stSettingSM.stFilterBP_DownRate.nNumTaps; // iAdded
        for(int i=0; i<nNumTaps; i++)
            pdSignalFilter[i] = taps_ippLPF[i]; //m_filterBP_DownRate.m_pdFilterCoeff[i]; // iAdded
        for(int i=nNumTaps; i<iSizeFFT; i++)
            pdSignalFilter[i] = 0;
    }
    if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
    {
        nNumTaps =  tapslen_ippLPF; // m_stSettingSM.stFilterBP_DownRate.nNumTaps; // iAdded
        for(int i=0; i<nNumTaps; i++)
            pdSignalFilter[i] = taps_ippLPF[i]; //m_filterBP_DownRate.m_pdFilterCoeff[i]; // iAdded
        for(int i=nNumTaps; i<iSizeFFT; i++)
            pdSignalFilter[i] = 0;
    }
    if(m_stSettingSM.enTypeManipulation == OMT_CARRIER_SHIFT) // iAdded
    {
        nNumTaps =  tapslen_ippLPF; // m_stSettingSM.stFilterBP_DownRate.nNumTaps; // iAdded
        for(int i=0; i<nNumTaps; i++)
            pdSignalFilter[i] = taps_ippLPF[i]; //m_filterBP_DownRate.m_pdFilterCoeff[i]; // iAdded
        for(int i=nNumTaps; i<iSizeFFT; i++)
            pdSignalFilter[i] = 0;
    }

    if(m_stSettingSM.enTypeManipulation == OMT_BaseBandConv) // iAdded
    {
        nNumTaps =  tapslen_ippLPF; // m_stSettingSM.stFilterBP_DownRate.nNumTaps; // iAdded
        for(int i=0; i<nNumTaps; i++)
            pdSignalFilter[i] = taps_ippLPF[i]; //m_filterBP_DownRate.m_pdFilterCoeff[i]; // iAdded
        for(int i=nNumTaps; i<iSizeFFT; i++)
            pdSignalFilter[i] = 0;
    }


    m_calcFFT_Filter.CalcFFT(pdSignalFilter,pdOutFFTFilter);

    for(int i=0; i<m_iSizeSpectrum; i++)
    {
        int iIndex = i + m_iIndexStartSpectrum;
        if((iIndex < 0) || (iIndex >= iSizeFFT))
            continue;


        double dOffsetSpectrum = m_pConfig->GetOffsetSpectrum();
        double dValue = 20*log10(pdOutFFTFilter[i + m_iIndexStartSpectrum]) + 67;

        if(i == 0)
        {
            m_dMaxLevelSpectrumFilter = dValue;
            m_dMinLevelSpectrumFilter = dValue;
        }

        if(dValue > m_dMaxLevelSpectrumFilter)
            m_dMaxLevelSpectrumFilter = dValue;
        if(dValue < m_dMinLevelSpectrumFilter)
            m_dMinLevelSpectrumFilter = dValue;

        m_pdSpectrumFilter_Y[i] = dValue;
        m_pdSpectrumFilter_X[i]=((float)i*((float)(m_pConfig->m_stInputFile.dSamplingFrequency*1e6))/(float)m_stSettingSM.stFFTPre.iSizeFFT);
    }
    DrawSpectrum_FilterResponse(m_pdSpectrumFilter_X,m_pdSpectrumFilter_Y);

    delete []pdSignalFilter;
    delete []pdOutFFTFilter;
    delete []pdSignalStep;
    return true;
}
void Mode_SM::RunConvert(bool bRun)
{
    if(bRun)
    {
        if(!m_pConfig->m_bLoadedFileSave)
            return;

        m_pConfig->GotoFileStartPos();
        m_iSizeConvert = m_pConfig->GetInputFileSampleSize();
        m_iIndexConvert = 0;
        m_iCounterCarrierShift = 0;
        StopSpectrum();
        int size=m_pConfig->m_ulSizeInputFile;
        progressBarsize=size/100;
        pw_ui->pBar_SM_SaveFile->setRange(0,100);
        StartConvertThread();
    }
    else
    {
        StopConvertThread();
        m_pConfig->CloseSaveFile();
    }
}


void Mode_SM::ThreadConvert()
{

    m_bRunConvert = true;

    while (!m_bKillThreadConvert)
    {
       // EnterCriticalSection(&m_csAccessParam);

        if(!ConvertBuffer())
        {
            m_bKillThreadConvert = true;
            //AfxMessageBox(_T("Finish Record!"));
        }

       // LeaveCriticalSection(&m_csAccessParam);
    }

    m_pConfig->CloseSaveFile();

    m_bRunConvert = false;
}
void Mode_SM::OpenSaveFile(QString strAddr)
{
    m_pConfig->OpenSaveFile(strAddr);
}
bool Mode_SM::ConvertBuffer(void)
{
    if(ReadDataFromFile())
    {

        if(m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR)
        {
            FilterFIR();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_FILTER_IIR)
        {
            FilterIIR();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_LoadFilter)
        {
            LoadFilter();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_CARRIER_SHIFT)
        {
            CarierShift();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_BaseBandConv)
        {
            BaseBandConverter();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_PHASE_SHIFT)
        {
            PhaseShift();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_CHANGE_RATE)
        {
            Upsampling();   //Interpolate(); //iAddedNew
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_DOWN_RATE)
        {
            DownRate();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == OMT_DC_REMOVAL)
        {
            RemoveDC();
            SaveSignalNxt();
        }
        else if(m_stSettingSM.enTypeManipulation == NONE)
        {
            m_pConfig->WriteToSaveFile(m_pdSignal,4096);
            if(m_pConfig->iSizeFileRecord > progressBarsize )
            {
                pw_ui->pBar_SM_SaveFile->setValue(progressBarStep);
                progressBarStep++;
                m_pConfig->iSizeFileRecord=0;
            }

            pw_ui->lbl_SM_SavefileSize->setNum((progressBarStep*progressBarsize)/1e6);

        }
        else
            return false;




//        if(!m_pConfig->wavefile)
//            m_iIndexConvert = m_pConfig->m_fileInput.GetPosition();
//        else
//            m_iIndexConvert+=2*4096;


        return true;
    }
    return false;
}

