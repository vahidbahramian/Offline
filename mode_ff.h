#ifndef MODE_FF_H
#define MODE_FF_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include "customplotzoom.h"


#include <vector>
#include "configuration.h"

struct FF_ALL_SETTING
{
    FFT_SETTING			stFFT;
    SPECTRUM_SETTING	stSpectrum;
};


class Mode_FF : public QWidget
{
    Q_OBJECT

    QThread thread;
public slots:
    void PlaySpectrum();

public:
    Mode_FF();
    CustomPlotZoom *pw_demod_Spectrum;
    CustomPlotZoom *pw_demod_PhaseErr;
    CustomPlotZoom *pw_demod_TimeDomain;
    CustomPlotZoom *pw_demod_Scatter;

    bool Initialize(Configuration *pConfig);
    bool RunMode(void);
    void CloseMode(void);
    void Close(void);
    void StopSpectrum(void);
    bool StartSpectrum(void);
    bool LoadDataFile(QString path);
    bool SetParameters(FF_ALL_SETTING stSettingFF);
    void CalculateSpectrum(QVector<double> m_pdOutFFT);
    void DrawSpectrum(QVector<double> X,QVector<double> Y);
    double FcEstimate(double x,double y);
    QWaitCondition qWaitSpecShowed;
    QMutex mutexSpecShowed;

private:
    bool StartSpectrumThread(void);
    void StopSpectrumThread(void);
    bool m_bKillThreadSpectrum;
public:
    Configuration *m_pConfig;
    bool m_bRunMode;
    bool m_bStartSpectrum;
    bool m_bPauseSpectrum;
    bool m_bStartSelect;
    bool IsRunMode(void){return m_bRunMode;}
    bool IsLoadedDataFile(void){return m_pConfig->m_bLoadedFileInput;}
    bool IsStartSpectrum(void){return m_bStartSpectrum;}
    bool IsPauseSpectrum(void){return m_bPauseSpectrum;}
    bool IsStartSelect(void){return m_bStartSelect;}
public:
    QString		m_strMainAddrRecord;
    QString		m_strNameFileRecord;
    double m_dEstimatedFc_New;
    double m_dEstimatedRs_New;
    double m_dEstimatedRs_uqpsk;
    double SNR_New;
    int readRegValsue;



signals:
    void sig_SpecdataReady(QVector<double> ,QVector<double> );
    void sig_ScatterdataReady(QVector<double> );
 public:
    QString name;
    QVector<double> m_pdOutFFT;
    QVector<double> m_pdSpectrum_X;
    QVector<double> m_pdSpectrum_Y;
    QVector<double> m_pdMaxHoldSpectrum;
    QVector<double> m_pdMaxHoldPhaseErr;
    QVector<double> ScatterBuff;
    QVector<double> FileReadBuff;
    double x_Freq,y_lvl;

public : //Spectrum
    int		m_iSizeSpectrum;
    int		m_iIndexStartSpectrum;
    int		m_iIndexStopSpectrum;
    bool	m_bUseAverageSpectrum;
    bool	m_bShowMaxHoldSpectrum;


    bool b_Analyzed;
    bool killthread;
    FF_ALL_SETTING m_stSettingFF;
    bool m_bInSettingMode;
     QTimer *timer;


};

#endif // MODE_FF_H

