#ifndef MODE_FF_H
#define MODE_FF_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include <vector>
#include "configuration.h"
#include "processor.h"

struct FF_ALL_SETTING
{
    FFT_SETTING			stFFT;
    SPECTRUM_SETTING	stSpectrum;
};


class Mode_FF : public QThread
{
    Q_OBJECT
public:
    Mode_FF(QObject *parent,QString FilePath);
    void run();
    bool Initialize(Configuration *pConfig);
    bool RunMode(void);
    void CloseMode(void);
    void Close(void);
    void StopSpectrum(void);
    bool StartSpectrum(void);
    bool LoadDataFile(QString path);
    void CalculateSpectrum( QVector<double> *Input,QVector<double> *outx,QVector<double> *outy);

    QWaitCondition qWaitSpecShowed;
    QMutex mutexSpecShowed;

private:
    bool StartSpectrumThread(void);
    void StopSpectrumThread(void);
//    static DWORD WINAPI ThreadSpectrum(LPVOID lpParam);
//    HANDLE m_hThreadSpectrum;
    bool m_bKillThreadSpectrum;
public:
    Configuration *m_pConfig;
    bool m_bRunMode;
    bool m_bStartSpectrum;
    bool m_bPauseSpectrum;
    bool m_bStartSelect;
public:
    QString		m_strMainAddrRecord;
    QString		m_strNameFileRecord;
    double m_dEstimatedFc_New;
    double m_dEstimatedRs_New;
    double m_dEstimatedRs_uqpsk;
    double SNR_New;
    int readRegValue;
    QWaitCondition qWaitConditionRegRead;
    QMutex mutexRegRead;
    int readConfValue;
    QWaitCondition qWaitConditionConfRead;
    QMutex mutexConfRead;


signals:
    void sig_SpecdataReady(QVector<double> *,QVector<double> *);
    void sig_ScatterdataReady(QVector<double> *);
 public:
    QString name;
    QVector<double> *SpecBuff;
    QVector<double> *SpecX;
    QVector<double> *SpecY;
    QVector<double> *ScatterBuff;
    QVector<double> *FileReadBuff;

    Processor *m_process;
    bool b_Analyzed;
    bool killthread;


};

#endif // MODE_FF_H
