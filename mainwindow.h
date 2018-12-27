#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "customplotzoom.h"
#include "zmq.h"
#include <QMainWindow>
#include "configuration.h"
#include "mode_ff.h"
#include "mode_sa.h"
#include "mode_sm.h"
#include "mode_dsss.h"

#if defined(FCESTIMATOR_LIBRARY)
#  define FCESTIMATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FCESTIMATORSHARED_EXPORT Q_DECL_IMPORT
#endif


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


private slots:
     void on_btn_pay_3_clicked();
     void on_btn_open_11_clicked();
     void on_btn_stop_3_clicked();
     void on_btn_open_demod_clicked();
     void on_btn_pause_3_clicked();
     void on_MainWindow_destroyed();
     void on_tabWidget_currentChanged(int index);
     void on_check_signed_sm_clicked();
     void on_Btn_FF_FCEstimate_clicked();
     void on_Btn_Setting_Apply_clicked();
     void on_Btn_FF_AutoEst_clicked();

     void on_Btn_SM_FIleLoad_clicked();

     void on_Btn_SM_PlaySPectrum_clicked();




     void on_combo_sm_FilterResponse_activated(int index);

     void on_Btn_SM_Stop_clicked();

     void on_Btn_SA_FileLoad_clicked();

     void on_Btn_SA_PlaySPectrum_clicked();

     void on_btn_SA_stopSpectrum_clicked();

public:
    Ui::MainWindow *ui;

    CustomPlotZoom *fhss_Spectrum;
    CustomPlotZoom *fhss_WaterWall;
    CustomPlotZoom *fhss_PhaseErr;
    CustomPlotZoom *fhss_Scatter;


    CustomPlotZoom *ofdm_Scatter;
    CustomPlotZoom *ofdm_Spectrum;
    CustomPlotZoom *ofdm_PhaseError;





    QPen penyellow;
    QPen penGreeen;
    QPen penWhite;
    QPen penred;
    QLinearGradient axisRectGradient;
    int start;
    int stop;
private:
    void ThreadSpectrum_Show();
    bool ModeInitialize(Configuration *pConfig);
    void Close(void);
    bool ChangeMode(OSA_MAIN_MODES enMode);
    OSA_MAIN_MODES GetLastMode(void){return m_enLastMode;}
    OSA_MAIN_MODES m_enLastMode;
    Mode_SA			m_modeSA;
    Mode_FF			m_modeFF;
//    Mode_FH		m_modeFH;
    Mode_DSSS		m_modeDS;
//    Mode_OFDM		m_modeOFDM;
    Mode_SM			m_modeSM;



    QString m_strAddressMain;

    INPUT_FILE_SETTING S_ModeSetiing;
    Configuration *m_pConfig;

};

#endif // MAINWINDOW_H
