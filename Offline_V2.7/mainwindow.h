#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "customplotzoom.h"
#include "zmq.h"
#include <QMainWindow>
#include "configuration.h"
#include "mode_ff.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initGrphices();
    QString  InputFilePath;
    QFile *Inputfile;
    int iSizeFFT;
    int iSizeRead;
    QVector<double> data;
    double *data_FFT;
    bool init();

public slots:
     void DrawSpectrum(QVector<double> *X,QVector<double> *Y);
signals:
    void Sig();

private slots:
     void on_btn_pay_3_clicked();
     void on_btn_open_11_clicked();
     void on_btn_stop_3_clicked();
     void on_btn_open_demod_clicked();
     void on_btn_pause_3_clicked();
     void on_MainWindow_destroyed();
     void on_tabWidget_currentChanged(int index);
     CustomPlotZoom * GetSpectrum(int ID);
public:
    Ui::MainWindow *ui;

    CustomPlotZoom *fhss_Spectrum;
    CustomPlotZoom *fhss_WaterWall;
    CustomPlotZoom *fhss_PhaseErr;
    CustomPlotZoom *fhss_Scatter;

    CustomPlotZoom *dsss_Spectrum;
    CustomPlotZoom *dsss_Scatter;
    CustomPlotZoom *dsss_AutoCorr;
    CustomPlotZoom *dsss_Welch;

    CustomPlotZoom *signalanalyzer_Spectrum;
    CustomPlotZoom *signalanalyzer_WaterWall;


    CustomPlotZoom *demod_Spectrum;
    CustomPlotZoom *demod_PhaseErr;
    CustomPlotZoom *demod_TimeDomain;
    CustomPlotZoom *demod_Scatter;

    CustomPlotZoom *ofdm_Scatter;
    CustomPlotZoom *ofdm_Spectrum;
    CustomPlotZoom *ofdm_PhaseError;

    CustomPlotZoom *signalManipulation_Spectrum;
    CustomPlotZoom *signalManipulation_FilterResponse;
    CustomPlotZoom *signalManipulation_Spectrum2;

    double* SpectrumBuffTemp;

    QPen penyellow;
    QPen penGreeen;
    QPen penWhite;
    QPen penred;
    QLinearGradient axisRectGradient;
    int start;
    int stop;
public:
    bool ModeInitialize(Configuration *pConfig);
    void Close(void);
    bool ChangeMode(OSA_MAIN_MODES enMode);
    OSA_MAIN_MODES GetLastMode(void){return m_enLastMode;}
    OSA_MAIN_MODES m_enLastMode;
//    Mode_SA			*pm_modeSA;
    Mode_FF			*m_pmodeFF;
//    Mode_FH			*m_pmodeFH;
//    Mode_DSSS		*m_pmodeDS;
//    Mode_OFDM		*m_pmodeOFDM;
//    Mode_SM			*m_pmodeSM;

//z    //    Mode_SA		m_modeSA;
//        Mode_FF			m_modeFF;
//    //    Mode_FH		m_modeFH;
//    //    Mode_DSSS		m_modeDS;
//    //    Mode_OFDM		m_modeOFDM;
//    //    Mode_SM		m_modeSM;

    QString m_strAddressMain;

    INPUT_FILE_SETTING S_ModeSetiing;
    Configuration m_config;

};

#endif // MAINWINDOW_H
