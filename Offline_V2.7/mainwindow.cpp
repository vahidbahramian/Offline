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
CustomPlotZoom* MainWindow::GetSpectrum(int iSpec_id)
{
    switch (iSpec_id) {
    case 0:
        return demod_Spectrum;
        break;
    case 1:
        return demod_Spectrum;
        break;
    case 2:
        return demod_Spectrum;
        break;
    case 3:
        return demod_Spectrum;
        break;

    case 4:
        return demod_Spectrum;
        break;
    default:
        return demod_Spectrum;
        break;
    }

}
bool MainWindow::ModeInitialize(Configuration *pConfig)
{
    bool bResult = false;

//	bResult = m_modeSA.Initialize(pConfig);
//  bResult = m_modeFF.Initialize(pConfig);
//	bResult = m_modeFH.Initialize(pConfig);
//	bResult = m_modeDS.Initialize(pConfig);
//	bResult = m_modeOFDM.Initialize(pConfig);
//	bResult = m_modeSM.Initialize(pConfig);
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
    else*//*if(m_enLastMode == OMM_FF)*/
//        m_modeFF.CloseMode();
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
    return true;

}
void MainWindow::initGrphices()
{
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(255, 54, 59));
    axisRectGradient.setColorAt(1, QColor(255, 54, 59));


    demod_TimeDomain = new CustomPlotZoom;
    demod_TimeDomain->IsPSD=true;
    demod_TimeDomain->addGraph();
    demod_TimeDomain->addGraph();
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

    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(49, 54, 59));
    axisRectGradient.setColorAt(1, QColor(49, 54, 59));

    demod_Spectrum = new CustomPlotZoom;
    demod_Spectrum->IsPSD=true;
    demod_Spectrum->addGraph();
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
    demod_Spectrum->fftlen=4096;
    demod_Spectrum->setZoomMode(true);
    demod_Spectrum->setXZoomMode(true);
    demod_Spectrum->setZoomMode(true);
    demod_Spectrum->setSignalMode(true);
    demod_Spectrum->IsDemoduling=false;
    demod_Spectrum->xAxis->setRange(0, 83.33*1000000);
    demod_Spectrum->yAxis->setRange(-50,+50);

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

    ui->demod_Scatter->addWidget(demod_Scatter);
    ui->demod_Spectrum->addWidget(demod_Spectrum);
    ui->demod_PhaseErr->addWidget(demod_PhaseErr);
    ui->demod_TimeDomain->addWidget(demod_TimeDomain);


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

    ui->signalmanipulation_Spectrum->addWidget(signalManipulation_Spectrum);
    ui->signalmanipulation_FilterResponse->addWidget(signalManipulation_FilterResponse);
    ui->signalmanipulation_Spectrum2->addWidget(signalManipulation_Spectrum2);


}
void MainWindow::DrawSpectrum( QVector<double> *X,QVector<double> *Y)
{
    CustomPlotZoom* spectrum;
    spectrum=demod_Spectrum;
    QVector<double> x(2048);
    QVector<double> y(2048);
    for(int i=0;i< 2048;i++)
    {
        x[i]=X->value(i);
        y[i]=Y->value(i);
    }
    spectrum->graph(0)->setData(x,y);
    spectrum->graph(0)->setPen(penred);
    spectrum->graph(0)->rescaleAxes();
    spectrum->axisRect()->setBackground(axisRectGradient);
    spectrum->Rescale();
    spectrum->replot(QCustomPlot::rpImmediate);
    x.clear();
    y.clear();

}
void MainWindow::on_btn_pay_3_clicked()
{
    m_pmodeFF=new Mode_FF(this,InputFilePath);
    connect(m_pmodeFF, SIGNAL(sig_SpecdataReady(QVector<double>*,QVector<double>*)),
                this, SLOT(DrawSpectrum(QVector<double>*,QVector<double>*))); //Spectrum Data Signal

    m_pmodeFF->StartSpectrum();

}
void MainWindow::on_btn_open_11_clicked()
{
    QString filter = "(*.bin)";
    QString pathName= QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
}
void MainWindow::on_btn_stop_3_clicked()
{
    m_pmodeFF->Close();

}
void MainWindow::on_btn_open_demod_clicked()
{
    QString filter = "(*.bin)";
    InputFilePath= QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
    ui->lbl_filename_demod->setText(InputFilePath);

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


