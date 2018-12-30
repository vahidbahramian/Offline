#ifndef MODE_FF_H
#define MODE_FF_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include "customplotzoom.h"
#include "ui_mainwindow.h"
#include <vector>
#include "configuration.h"
//#include "SRC.h"
//#include "AMREstimation.h"

//using namespace NS_FRAME_BASED;
struct FF_ALL_SETTING
{
    FFT_SETTING			stFFT;
    SPECTRUM_SETTING	stSpectrum;
};


class Mode_FF : public QWidget
{
    Q_OBJECT

public slots:
    void PlaySpectrum();

public:
    Mode_FF();
    CustomPlotZoom *demod_Spectrum;
    CustomPlotZoom *demod_PhaseErr;
    CustomPlotZoom *demod_TimeDomain;
    CustomPlotZoom *demod_Scatter;

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
    bool SetParameters(FF_ALL_SETTING stSettingFF);
    bool ReSetParameters(void);
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
    FF_ALL_SETTING m_stSettingFF;
public:
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



public:
    QString		m_strMainAddrRecord;
    QString		m_strNameFileRecord;
    double m_dEstimatedFc_New;
    double m_dEstimatedRs_New;
    double m_dEstimatedRs_uqpsk;
    double SNR_New;
    double m_DNewBandwidth;
    int iAmr_out_Index;
    CCalculateFFT m_calcFFT_FCEst;



    bool b_Analyzed;
    bool killthread;
    ESTIMATE_PARAM m_stEstimateParam;
    double unit;
    double FS_Last;
    double *pAMRinputI;
    double *pAMRinputQ;
    double dFsNonLinear;

//    CSRC  *m_pFilterToSRC;
    QString m_stModulationType;
    int amr_outt;
    int FH_ModType;


    void ParamEstimate(double d_xRuler, double d_yRuler, double &FC_Out);
    void Rs1702Param(double d_xRuler,double d_yRuler,double &Rs_Out);
    double RS_Estimate(Ipp64fc *Sig,double m_Bw_baseband, int InputSize);
    void SavtToFile_double(double *dbDataInp2,int nLenOut);

    void ChangeRsSignal_FSK(Ipp64fc *pSignal);
    void hamming(int n,double* w);
    double New_1702RS(Ipp64fc *Sig, double BW, int InputSize);

    CustomPlotZoom *Constellation_1;
    QCPCurve *ConstaltionPoint1;
    QCPCurveDataMap  *data_CH1;
    QCPScatterStyle myscatter;
    void fillcontlation(QVector<double> x,QVector<double> y);

};

#endif // MODE_FF_H

