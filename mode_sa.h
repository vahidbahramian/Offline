#ifndef MODE_SA_H
#define MODE_SA_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include "customplotzoom.h"
#include <QObject>
#include "qcustomplot.h"
#include "QTimer"
#include "QScrollBar"
#include "QTextBrowser"
#include "ui_mainwindow.h"
#include <vector>
#include "configuration.h"
struct SA_ALL_SETTING
{
    FFT_SETTING			stFFT;
    SPECTRUM_SETTING	stSpectrum;
    WATERFALL_SETTING	stWaterfall;
    HISTOGRAM_SETTING	stHistogram;
    //COLOR_SETTING		stColor;
    bool				bInputIQ;
};


class Mode_SA :public QWidget
{
    Q_OBJECT

public slots:
    void PlaySpectrum();

public:
    Mode_SA();
    CustomPlotZoom *signalanalyzer_Spectrum;
    QCustomPlot *signalanalyzer_WaterWall;
    QCPColorMap   *WaterFallColorMap;
    QCPColorScale *WaterFallColorScale;
    QScrollBar    *WaterFallScrollBar;
    QVector<QVector<double>> Power_Data;
    int Line;
    QList<char*> Buff;
    QMutex m;

    Ui::MainWindow *pw_ui;
    bool Initialize(Configuration *pConfig, Ui::MainWindow *ui);
    void InitGraph(Ui::MainWindow *ui);
    bool RunMode(void);
    void CloseMode(void);
    void Close(void);
    void StopSpectrum(void);
    bool StartSpectrum(void);
    bool LoadDataFile(QString path);
    bool SetParameters(SA_ALL_SETTING stSettingSA);
    void CalculateSpectrum(QVector<double> m_pdOutFFT);
    void DrawSpectrum(QVector<double> X,QVector<double> Y);
    void DrawingWaterfall(QVector<double> Power, int Scale_Angles);
    void initializePlot();

private:
    bool StartSpectrumThread(void);
    void StopSpectrumThread(void);
    bool m_bKillThreadSpectrum;
private:
    Configuration *m_pConfig;
    CCalculateFFT m_calcFFT;
    QTimer *timer;
    SA_ALL_SETTING m_stSettingSA;
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
    QVector<double> m_pdOutFFT;
    QVector<double> m_pdSpectrum_X;
    QVector<double> m_pdSpectrum_Y;
    QVector<double> m_pdMaxHoldSpectrum;
    QVector<double> m_pdMaxHoldPhaseErr;
    QVector<double> ScatterBuff;
    QVector<double> m_pdSignal;
    bool m_bInSettingMode;
public : //Spectrum
    int		m_iSizeSpectrum;
    int		m_iIndexStartSpectrum;
    int		m_iIndexStopSpectrum;
    bool	m_bUseAverageSpectrum;
    bool	m_bShowMaxHoldSpectrum;


};

#endif // MODE_SA_H
