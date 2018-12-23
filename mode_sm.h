#ifndef MODE_SM_H
#define MODE_SM_H
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include "customplotzoom.h"
#include "ui_mainwindow.h"
#include <vector>
#include "configuration.h"

#define n_FILTER_TYPE   10
enum OSA_MANIPULATE_TYPE { OMT_FILTER_FIR, OMT_FILTER_IIR, OMT_CARRIER_SHIFT, OMT_PHASE_SHIFT, OMT_CHANGE_RATE,
                           OMT_DOWN_RATE, OMT_DC_REMOVAL, OMT_BaseBandConv,OMT_LoadFilter ,NONE};
const QString g_str_MANIPULATE_TYPE[n_FILTER_TYPE] = {"FILTER_FIR" ,"FILTER_IIR"
                                                      ,"CARRIER_SHIFT" ,"PHASE_SHIFT" ,"CHANGE_RATE"
                                                      ,"DOWN_RATE","DC_REMOVAL" ,"BaseBandConv","LoadFilter","NONE"};

struct FIR_FILTER
{
    double		dFc;			// MHz
    double		dBW;			// MHz
    double		dPW;			// MHz
//    TPassType	enPassType;		// LPF, HPF, BPF, NOTCH
    int			nNumTaps;		// max -> 256
};

struct IIR_FILTER
{
    double		dFc;			// MHz
    double		dBW;			// MHz
//    TPassType	enPassType;		// LPF, HPF, BPF, NOTCH
    int			nOrder;			// max -> 30
};

struct SM_ALL_SETTING
{
    INPUT_FILE_SETTING  *pstInputFile;
    bool				bInputIQ;

    FFT_SETTING			stFFTPre;
    SPECTRUM_SETTING	stSpectrumPre;

    FFT_SETTING			stFFTNxt;
    SPECTRUM_SETTING	stSpectrumNxt;


    OSA_MANIPULATE_TYPE enTypeManipulation;

    double				dInterpRatio;		// Next to Pre
    double				dPhaseShift;		//radian
    double				dNewCarrier;		// MHz
    double				ChangeRatio;		//iAdded
    int					WinType;		//iAdded
    double				Fc_Lp;			//iAdded
    int					tap_Lp;			//iAdded
    double				*FilteBuff;
    int					FilteBuff_Size;
    FIR_FILTER			stFilterFIR;
    IIR_FILTER			stFilterIIR;

    FIR_FILTER			stFilterBP_DownRate;
    int unit ;

};

class Mode_SM : public QWidget
{
    Q_OBJECT
public:
    Mode_SM();

public slots:
    void PlaySpectrum();

public:
    CustomPlotZoom *signalManipulation_Spectrum;
    CustomPlotZoom *signalManipulation_FilterResponse;
    CustomPlotZoom *signalManipulation_Spectrum2;

    Ui::MainWindow *pw_ui;
    bool Initialize(Configuration *pConfig, Ui::MainWindow *ui);
    void InitGraph(Ui::MainWindow *ui);
    bool RunMode(void);
    void CloseMode(void);
    void Close(void);
    void StopSpectrum(void);
    bool StartSpectrum(void);
    bool LoadDataFile(QString path);
    bool SetParameters(SM_ALL_SETTING stSettingSM);
    void CalculateSpectrum(QVector<double> m_pdOutFFT);
    void DrawSpectrum(QVector<double> X,QVector<double> Y);
    void DrawSpectrum_Spectrum2(QVector<double> X,QVector<double> Y);
    void DrawSpectrum_FilterResponse(QVector<double> X,QVector<double> Y);

    void FilterFIR(QVector<double> In, QVector<double> &Out);
    void FilterIIR(QVector<double> In,QVector<double> &Out);
private:
    bool StartSpectrumThread(void);
    void StopSpectrumThread(void);
    bool m_bKillThreadSpectrum;
public:
    QString name;
    QVector<double> m_pdOutFFT;
    QVector<double> m_pdOutFFT_Next;
    QVector<double>	m_pdSignalNxt_X;
    QVector<double>	m_pdSignalNxt_Y;
    double	*m_pdSignalNxt_I;
    double	*m_pdSignalNxt_Q;
    double	*m_pdOutFFTNxt;
    QVector<double> m_pdSpectrum_X;
    QVector<double> m_pdSpectrum_Y;
    QVector<double> m_pdMaxHoldSpectrum;
    QVector<double> m_pdMaxHoldPhaseErr;
    QVector<double> ScatterBuff;
    QVector<double> FileReadBuff;
public : //Spectrum
    int		m_iSizeSpectrum;
    int		m_iIndexStartSpectrum;
    int		m_iIndexStopSpectrum;
    bool	m_bUseAverageSpectrum;
    bool	m_bShowMaxHoldSpectrum;


    int		m_iSizeSignalNxt;
    double	*m_pdSignalNxt;
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
    bool m_bInSettingMode;
    QTimer *timer;
    SM_ALL_SETTING m_stSettingSM;
    QString  InputFilePath;

public:
    IppsFIRSpec_64f *pSpec;
    double           *src, *dst, *dlysrc, *dlydst, *taps, *DATA;
    Ipp8u           *buf;
    int             specSize, bufSize;
    IppStatus status;
};

#endif // MODE_SM_H
