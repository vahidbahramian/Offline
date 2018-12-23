#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QString>
#include "ippcore.h"
#include "QMessageBox"
#include "QDebug"
#include "QFuture"
#include "QtConcurrent/QtConcurrent"
#include "QThread"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

}
MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::ModeInitialize(Configuration *pConfig)
{
    bool bResult = false;

//	bResult = m_modeSA.Initialize(pConfig);
  bResult = m_modeFF.Initialize(pConfig,ui);
//	bResult = m_modeFH.Initialize(pConfig);
//	bResult = m_modeDS.Initialize(pConfig);
//	bResult = m_modeOFDM.Initialize(pConfig);
  bResult = m_modeSM.Initialize(pConfig,ui);
//	bResult = m_modeSSA.Initialize(pConfig);


//	ChangeMode(OMM_SA);

    return true;
}
void MainWindow::Close(void)
{
    ChangeMode((OSA_MAIN_MODES)(-1));

//	m_modeSA.Close();
//	m_modeFF.Close();
//	m_modeFH.Close();
//	m_modeDS.Close();
//	m_modeOFDM.Close();
//	m_modeSM.Close();
//	m_modeSSA.Close();
}
bool MainWindow::ChangeMode(OSA_MAIN_MODES enMode)
{
    //------------------------------------------------------------------
    //--- Stop Last Mode -----------------------------------------------
    //------------------------------------------------------------------
    /*if(m_enLastMode == OMM_SA)
        m_modeSA.CloseMode();
    else*//*if(m_enLastMode == OMM_FF)
        m_modeFF.CloseMode();*/
//	else if(m_enLastMode == OMM_FH)
//		m_modeFH.CloseMode();
//	else if(m_enLastMode == OMM_DS)
//		m_modeDS.CloseMode();
//	else if(m_enLastMode == OMM_OFDM)
//		m_modeOFDM.CloseMode();
//	else if(m_enLastMode == OMM_SM)
//		m_modeSM.CloseMode();

    //------------------------------------------------------------------
    //--- Start New Mode -----------------------------------------------
    //------------------------------------------------------------------
    bool bResult = true;
    /*if(enMode == OMM_SA)
        bResult = m_modeSA.RunMode();
    else*/// if(enMode == OMM_FF)
    //	bResult = m_modeFF.RunMode();
//	else if(enMode == OMM_FH)
//		bResult = m_modeFH.RunMode();
//	else if(enMode == OMM_DS)
//		bResult = m_modeDS.RunMode();
//	else if(enMode == OMM_OFDM)
//		bResult = m_modeOFDM.RunMode();
//	else if(enMode == OMM_SM)
//		bResult = m_modeSM.RunMode();


    if(bResult)
        m_enLastMode = enMode;
    else
        qDebug("Error in selected mode");

    return bResult;
}
bool MainWindow::init()
{

    initGrphices();
    m_pConfig=new Configuration();
    m_pConfig->Initialize();
    if(!ModeInitialize(m_pConfig))
    {
        qDebug("Error in mode control initializing ...!");
        return false;
    }

    return true;

}
void MainWindow::initGrphices()
{
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(255, 54, 59));
    axisRectGradient.setColorAt(1, QColor(255, 54, 59));



    //---------------------------------------------------
    ofdm_Spectrum = new CustomPlotZoom;
    ofdm_Spectrum->IsPSD=true;
    ofdm_Spectrum->addGraph();
    ofdm_Spectrum->addGraph();
    ofdm_Spectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    ofdm_Spectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    ofdm_Spectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    ofdm_Spectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    ofdm_Spectrum->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ofdm_Spectrum->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ofdm_Spectrum->xAxis->setTickLabelColor(Qt::white);
    ofdm_Spectrum->yAxis->setTickLabelColor(Qt::white);
    ofdm_Spectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ofdm_Spectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ofdm_Spectrum->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ofdm_Spectrum->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ofdm_Spectrum->xAxis->grid()->setSubGridVisible(true);
    ofdm_Spectrum->yAxis->grid()->setSubGridVisible(true);
    ofdm_Spectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ofdm_Spectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ofdm_Spectrum->xAxis->setUpperEnding(QCPLineEnding::esNone);
    ofdm_Spectrum->yAxis->setUpperEnding(QCPLineEnding::esNone);
    ofdm_Spectrum->IsDemoduling=false;
    ofdm_Spectrum->setZoomMode(true);
    ofdm_Spectrum->setXZoomMode(true);
    ofdm_Spectrum->setSignalMode(false);

    ofdm_Scatter=new CustomPlotZoom;
    ofdm_Scatter->legend->setVisible(true);
    ofdm_Scatter->legend->setFont(QFont("Helvetica", 9));
    ofdm_Scatter->legend->setRowSpacing(-3);
    ofdm_Scatter->addGraph();
    ofdm_Scatter->graph()->rescaleAxes(true);
    ofdm_Scatter->graph()->setPen(QPen(Qt::white, 1));
    //ofdm_Scatter->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
    ofdm_Scatter->graph()->setLineStyle(QCPGraph::lsLine);
    ofdm_Scatter->rescaleAxes();
    ofdm_Scatter->xAxis->setTicks(false);
    ofdm_Scatter->yAxis->setTicks(false);
    ofdm_Scatter->xAxis->setTickLabels(false);
    ofdm_Scatter->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    ofdm_Scatter->axisRect()->setupFullAxesBox();

    ofdm_PhaseError=new CustomPlotZoom;
    ofdm_PhaseError->IsPSD=true;
    ofdm_PhaseError->addGraph();
    ofdm_PhaseError->addGraph();
    ofdm_PhaseError->xAxis->setBasePen(QPen(Qt::white, 1));
    ofdm_PhaseError->yAxis->setBasePen(QPen(Qt::white, 1));
    ofdm_PhaseError->xAxis->setTickPen(QPen(Qt::white, 1));
    ofdm_PhaseError->yAxis->setTickPen(QPen(Qt::white, 1));
    ofdm_PhaseError->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ofdm_PhaseError->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ofdm_PhaseError->xAxis->setTickLabelColor(Qt::white);
    ofdm_PhaseError->yAxis->setTickLabelColor(Qt::white);
    ofdm_PhaseError->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ofdm_PhaseError->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ofdm_PhaseError->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ofdm_PhaseError->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ofdm_PhaseError->xAxis->grid()->setSubGridVisible(true);
    ofdm_PhaseError->yAxis->grid()->setSubGridVisible(true);
    ofdm_PhaseError->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ofdm_PhaseError->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ofdm_PhaseError->xAxis->setUpperEnding(QCPLineEnding::esNone);
    ofdm_PhaseError->yAxis->setUpperEnding(QCPLineEnding::esNone);
    ofdm_PhaseError->IsDemoduling=false;
    ofdm_PhaseError->setZoomMode(true);
    ofdm_PhaseError->setXZoomMode(true);
    ofdm_PhaseError->setSignalMode(false);


    //---------------------------------------------------

    fhss_Spectrum = new CustomPlotZoom;
    fhss_Spectrum->IsPSD=true;
    fhss_Spectrum->addGraph();
    fhss_Spectrum->addGraph();
    fhss_Spectrum->xAxis->setBasePen(QPen(Qt::white, 1));
    fhss_Spectrum->yAxis->setBasePen(QPen(Qt::white, 1));
    fhss_Spectrum->xAxis->setTickPen(QPen(Qt::white, 1));
    fhss_Spectrum->yAxis->setTickPen(QPen(Qt::white, 1));
    fhss_Spectrum->xAxis->setSubTickPen(QPen(Qt::white, 1));
    fhss_Spectrum->yAxis->setSubTickPen(QPen(Qt::white, 1));
    fhss_Spectrum->xAxis->setTickLabelColor(Qt::white);
    fhss_Spectrum->yAxis->setTickLabelColor(Qt::white);
    fhss_Spectrum->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    fhss_Spectrum->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    fhss_Spectrum->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    fhss_Spectrum->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    fhss_Spectrum->xAxis->grid()->setSubGridVisible(true);
    fhss_Spectrum->yAxis->grid()->setSubGridVisible(true);
    fhss_Spectrum->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    fhss_Spectrum->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    fhss_Spectrum->xAxis->setUpperEnding(QCPLineEnding::esNone);
    fhss_Spectrum->yAxis->setUpperEnding(QCPLineEnding::esNone);
    fhss_Spectrum->IsDemoduling=false;
    fhss_Spectrum->setZoomMode(true);
    fhss_Spectrum->setXZoomMode(true);
    fhss_Spectrum->setSignalMode(false);

    fhss_WaterWall=new CustomPlotZoom;
    fhss_WaterWall=new CustomPlotZoom;
    QCPColorMap *colorMap = new QCPColorMap(fhss_WaterWall->xAxis, fhss_WaterWall->yAxis);
    fhss_WaterWall->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    fhss_WaterWall->axisRect()->setupFullAxesBox(true);
    fhss_WaterWall->xAxis->setLabel("x");
    fhss_WaterWall->yAxis->setLabel("y");
    QCPColorScale *colorScale = new QCPColorScale(fhss_WaterWall);
    fhss_WaterWall->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("Magnetic Field Strength");
    colorMap->setGradient(QCPColorGradient::gpPolar);
    colorMap->rescaleDataRange();
    QCPMarginGroup *marginGroup = new QCPMarginGroup(fhss_WaterWall);
    fhss_WaterWall->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    fhss_PhaseErr=new CustomPlotZoom;
    fhss_PhaseErr->IsPSD=true;
    fhss_PhaseErr->addGraph();
    fhss_PhaseErr->addGraph();
    fhss_PhaseErr->xAxis->setBasePen(QPen(Qt::white, 1));
    fhss_PhaseErr->yAxis->setBasePen(QPen(Qt::white, 1));
    fhss_PhaseErr->xAxis->setTickPen(QPen(Qt::white, 1));
    fhss_PhaseErr->yAxis->setTickPen(QPen(Qt::white, 1));
    fhss_PhaseErr->xAxis->setSubTickPen(QPen(Qt::white, 1));
    fhss_PhaseErr->yAxis->setSubTickPen(QPen(Qt::white, 1));
    fhss_PhaseErr->xAxis->setTickLabelColor(Qt::white);
    fhss_PhaseErr->yAxis->setTickLabelColor(Qt::white);
    fhss_PhaseErr->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    fhss_PhaseErr->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    fhss_PhaseErr->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    fhss_PhaseErr->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    fhss_PhaseErr->xAxis->grid()->setSubGridVisible(true);
    fhss_PhaseErr->yAxis->grid()->setSubGridVisible(true);
    fhss_PhaseErr->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    fhss_PhaseErr->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    fhss_PhaseErr->xAxis->setUpperEnding(QCPLineEnding::esNone);
    fhss_PhaseErr->yAxis->setUpperEnding(QCPLineEnding::esNone);
    fhss_PhaseErr->IsDemoduling=false;
    fhss_PhaseErr->setZoomMode(true);
    fhss_PhaseErr->setXZoomMode(true);
    fhss_PhaseErr->setSignalMode(false);

    fhss_Scatter=new CustomPlotZoom;
    fhss_Scatter->legend->setVisible(true);
    fhss_Scatter->legend->setFont(QFont("Helvetica", 9));
    fhss_Scatter->legend->setRowSpacing(-3);
    fhss_Scatter->addGraph();
    fhss_Scatter->graph()->rescaleAxes(true);
    fhss_Scatter->graph()->setPen(QPen(Qt::white, 1));
    //fhss_Scatter->graph()->setName(QCPScatterStyle::staticMetaObject.enumerator(QCPScatterStyle::staticMetaObject.indexOfEnumerator("ScatterShape")).valueToKey(shapes.at(i)));
    fhss_Scatter->graph()->setLineStyle(QCPGraph::lsLine);
    fhss_Scatter->rescaleAxes();
    fhss_Scatter->xAxis->setTicks(false);
    fhss_Scatter->yAxis->setTicks(false);
    fhss_Scatter->xAxis->setTickLabels(false);
    fhss_Scatter->yAxis->setTickLabels(false);
    // make top right axes clones of bottom left axes:
    fhss_Scatter->axisRect()->setupFullAxesBox();
    //---------------------------------------------------

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
    //---------------------------------------------------

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

    signalanalyzer_WaterWall=new CustomPlotZoom;
    QCPColorMap *colorMap1 = new QCPColorMap(signalanalyzer_WaterWall->xAxis, signalanalyzer_WaterWall->yAxis);
    signalanalyzer_WaterWall->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    signalanalyzer_WaterWall->axisRect()->setupFullAxesBox(true);
    signalanalyzer_WaterWall->xAxis->setLabel("x");
    signalanalyzer_WaterWall->yAxis->setLabel("y");
//    QCPColorScale *colorScale = new QCPColorScale(signalanalyzer_WaterWall);
    signalanalyzer_WaterWall->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap1->setColorScale(colorScale);
    colorScale->axis()->setLabel("Magnetic Field Strength");
    colorMap1->setGradient(QCPColorGradient::gpPolar);
    colorMap1->rescaleDataRange();
//    QCPMarginGroup *marginGroup = new QCPMarginGroup(signalanalyzer_WaterWall);
    signalanalyzer_WaterWall->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    //---------------------------------------------------




    ui->ofdm_Spectrum->addWidget(ofdm_Spectrum);
    ui->ofdm_Scatter->addWidget(ofdm_Scatter);
    ui->ofdm_PhaseError->addWidget(ofdm_PhaseError);

    ui->fhss_Spectrum->addWidget(fhss_Spectrum);
    ui->fhss_Scatter->addWidget(fhss_Scatter);
    ui->fhss_PhaseErr->addWidget(fhss_PhaseErr);
    ui->fhss_WaterWall->addWidget(fhss_WaterWall);

    ui->dsss_Spectrum->addWidget(dsss_Spectrum);
    ui->dsss_Scatter->addWidget(dsss_Scatter);
    ui->dsss_Welch->addWidget(dsss_Welch);
    ui->dsss_AutoCorrelation->addWidget(dsss_AutoCorr);


    ui->signalanalyzer_Spectrum->addWidget(signalanalyzer_Spectrum);
    ui->signalanalyzer_WaterWall->addWidget(signalanalyzer_WaterWall);



}



void MainWindow::on_btn_pay_3_clicked()
{
    if(!m_modeFF.m_bStartSpectrum && m_pConfig->m_bLoadedFileInput)
        m_modeFF.StartSpectrum();

}
void MainWindow::on_btn_open_11_clicked()
{

}
void MainWindow::on_btn_stop_3_clicked()
{
    m_modeFF.Close();
}
void MainWindow::on_btn_open_demod_clicked()
{
    QString filter = "(*.bin)";
    InputFilePath = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
    m_modeFF.LoadDataFile(InputFilePath);
}
void MainWindow::on_btn_pause_3_clicked()
{

}
void MainWindow::on_MainWindow_destroyed()
{
}
void MainWindow::on_tabWidget_currentChanged(int index)
{
    ChangeMode(OMM_FF);
}
void MainWindow::on_check_signed_sm_clicked()
{

//            double tempInstantaneousGenSize = ui->check_signed_sm->checkState() == Qt::Checked ? 10.0 : 11.0;
//            tempInstantaneousGenSize=tempInstantaneousGenSize+1;
}


void MainWindow::on_Btn_FF_FCEstimate_clicked()
{


}


void MainWindow::on_Btn_Setting_Apply_clicked()
{
    QString dSamplingFrequency=ui->txt_setting_FS->text();
    QString dMinVoltageA2D=ui->txt_setting_min_vol_set->text();
    QString dMaxVoltageA2D=ui->txt_setting_max_vol_set->text();
    QString iMinValueA2D=ui->txt_setting_min_set->text();
    QString iMaxValueA2D=ui->txt_setting_max_set->text();


    m_pConfig->m_stInputFile.dSamplingFrequency = dSamplingFrequency.toDouble();
    m_pConfig->m_stInputFile.enFreqUnit = (INPUT_FREQ_UNIT)ui->Combo_Setting_ByteNum->currentIndex();
    m_pConfig->m_stInputFile.dMinVoltageA2D = dMinVoltageA2D.toDouble();
    m_pConfig->m_stInputFile.dMaxVoltageA2D = dMaxVoltageA2D.toDouble();
    m_pConfig->m_stInputFile.iMinValueA2D = iMinValueA2D.toDouble();
    m_pConfig->m_stInputFile.iMaxValueA2D = iMaxValueA2D.toDouble();

    m_pConfig->m_stInputFile.enByteNum = (INPUT_FILE_char)ui->Combo_Setting_ByteNum->currentIndex();
    m_pConfig->m_stInputFile.bSignedValue = ui->check_signed_sm->checkState();
    m_pConfig->m_stInputFile.bFirstLSB = ui->check_first_lsb->checkState();


    m_pConfig->ArrangeInputFile(&m_pConfig->m_stInputFile);
//    m_iMinValue = m_pConfig->m_stInputFile.iMinValueA2D;
//    m_iMaxValue = m_pConfig->m_stInputFile.iMaxValueA2D;
    m_pConfig->m_stInputFile.IQ_Input= ui->check_IQ->checkState();
//    OSA_MAIN_MODES m_enCurMode = m_enLastMode;
//    if(m_enCurMode == OMM_SA)
//        theApp.m_modeControl.m_modeSA.ReSetParameters();
//    else if(m_enCurMode == OMM_FF)
//       m_modeFF.SetParameters();
//    else if(m_enCurMode == OMM_FH)
//        theApp.m_modeControl.m_modeFH.ReSetParameters();
//    else if(m_enCurMode == OMM_DS)
//        theApp.m_modeControl.m_modeDS.ReSetParameters();
//    else if(m_enCurMode == OMM_OFDM)
//        theApp.m_modeControl.m_modeOFDM.ReSetParameters();
//    else if(m_enCurMode == OMM_SM)
//        theApp.m_modeControl.m_modeSM.ReSetParameters();

}

void MainWindow::on_Btn_FF_AutoEst_clicked()
{
    double FC;
    m_modeFF.ParamEstimate(m_modeFF.demod_Spectrum->x1out/1e6,m_modeFF.demod_Spectrum->x2out/1e6,FC);
    ui->txt_FcEstimate->setText(QString::number(FC));
    ui->txt_RSEstimate->setText(QString::number(0));
    ui->txt_AMREstimate->setText(g_strOSD_DEMOD_TYPE[1]);
}

void MainWindow::on_Btn_SM_FIleLoad_clicked()
{
    QString filter = "(*.bin)";
    InputFilePath = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
    m_modeSM.LoadDataFile(InputFilePath);
}

void MainWindow::on_Btn_SM_PlaySPectrum_clicked()
{
    if(!m_modeSM.m_bStartSpectrum && m_pConfig->m_bLoadedFileInput)
        m_modeSM.StartSpectrum();

}


void MainWindow::on_combo_sm_FilterResponse_currentIndexChanged(int index)
{
    m_modeSM.m_stSettingSM.enTypeManipulation == OMT_FILTER_FIR;
}
