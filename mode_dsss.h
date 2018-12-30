#ifndef MODE_DSSS_H
#define MODE_DSSS_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include "customplotzoom.h"
#include "ui_mainwindow.h"
#include <vector>
#include "configuration.h"

struct DS_ALL_SETTING
{
    FFT_SETTING			stFFT;
    SPECTRUM_SETTING	stSpectrum;
    INPUT_FILE_SETTING stInputFile;
    bool				bInputIQ;
};
class Mode_DSSS : public QWidget
{
     Q_OBJECT

public slots:
    void PlaySpectrum();
public:
    Mode_DSSS();
public:
    CustomPlotZoom *dsss_Spectrum;
    CustomPlotZoom *dsss_Scatter;
    CustomPlotZoom *dsss_AutoCorr;
    CustomPlotZoom *dsss_Welch;


    Ui::MainWindow *pw_ui;
    bool Initialize(Configuration *pConfig, Ui::MainWindow *ui);
    void InitGraph(Ui::MainWindow *ui);
    bool RunMode(void);
    void CloseMode(void);
    void Close(void);
    void StopSpectrum(void);
    bool StartSpectrum(void);
    bool LoadDataFile(QString path);
    bool ReadDataFromFile(void);
    bool SetParameters(DS_ALL_SETTING stSettingSA);
    void CalculateSpectrum(double *m_pdOutFFT);
    void DrawSpectrum(QVector<double> X,QVector<double> Y);

private:
    bool StartSpectrumThread(void);
    void StopSpectrumThread(void);
    bool m_bKillThreadSpectrum;
private:
    Configuration *m_pConfig;
    CCalculateFFT m_calcFFT;
    QTimer *timer;
    DS_ALL_SETTING m_stSettingDS;
    bool m_bRunMode;
    bool m_bStartSpectrum;
    bool m_bPauseSpectrum;
    bool m_bStartSelect;
public:
    bool IsRunMode(void){return m_bRunMode;}
    bool IsLoadedDataFile(void){return m_pConfig->m_bLoadedFileInput;}
    bool IsStartSpectrum(void){return m_bStartSpectrum;}
    bool IsPauseSpectrum(void){return m_bPauseSpectrum;}
    bool IsStartSelect(void){return m_bStartSelect;}
public:
    QString name;
    QVector<double> m_pdSpectrum_X;
    QVector<double> m_pdSpectrum_Y;
    double *m_pdOutFFT;
    double *m_pdMaxHoldSpectrum;
    double *m_pdMaxHoldPhaseErr;
    double *ScatterBuff;
    double *m_pdSignal;
    bool m_bInSettingMode;
public : //Spectrum
    int		m_iSizeSpectrum;
    int		m_iIndexStartSpectrum;
    int		m_iIndexStopSpectrum;
    bool	m_bUseAverageSpectrum;
    bool	m_bShowMaxHoldSpectrum;

};

#endif // MODE_DSSS_H
