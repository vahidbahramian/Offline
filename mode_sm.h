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

#define PI   3.1416
#define n_FILTER_TYPE   10
enum OSA_MANIPULATE_TYPE { OMT_FILTER_FIR, OMT_FILTER_IIR, OMT_CARRIER_SHIFT, OMT_PHASE_SHIFT, OMT_CHANGE_RATE,
                           OMT_DOWN_RATE, OMT_DC_REMOVAL, OMT_BaseBandConv,OMT_LoadFilter ,NONE};
const QString g_str_MANIPULATE_TYPE[n_FILTER_TYPE] = {"FIR FILTER" ,"IIR FILTER"
                                                      ,"CARRIER SHIFT" ,"PHASE SHIFT" ,"CHANGE RATE"
                                                      ,"DOWN RATE","DC REMOVAL" ,"BaseBandConv","LoadFilter","NONE"};

enum TPassType {LPF, HPF, BPF, NOTCH };

struct FIR_FILTER
{
    double		dFc;			// MHz
    double		dBW;			// MHz
    double		dPW;			// MHz
    TPassType	enPassType;		// LPF, HPF, BPF, NOTCH
    int			nNumTaps;		// max -> 256
};

struct IIR_FILTER
{
    double		dFc;			// MHz
    double		dBW;			// MHz
    TPassType	enPassType;		// LPF, HPF, BPF, NOTCH
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
    ~Mode_SM();
QThread thread;
public slots:
    void PlaySpectrum();
    void ThreadConvert();

public:
    void StartConvertThread()
    {
        moveToThread(&thread);
        m_bKillThreadConvert = false;
        connect(&thread,SIGNAL(started()),this,SLOT(ThreadConvert()));
        thread.start(QThread::HighestPriority);
    }
    void StopConvertThread()
    {
        thread.destroyed();
    }

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
    bool ReadDataFromFile(void);
    bool LoadDataFile(QString path);
    bool SetParameters(SM_ALL_SETTING stSettingSM);
    void CalculateSpectrum(double *m_pdOutFFT);
    bool CalculateSpectrumInterp();
    void DrawSpectrum(QVector<double> X,QVector<double> Y);

    void DrawSpectrum_Spectrum2(QVector<double> X,QVector<double> Y);
    void DrawSpectrum_FilterResponse(QVector<double> X,QVector<double> Y);


    void FilterFIR();
    void FilterIIR();
    void SaveSignalNxt(void);
    bool Interpolate(void);
    void DownRate(void);
    void Upsampling(void); // iAdded
    void RemoveDC(void);
    bool RealImageSeprate(void);
    void BaseBandConverter(void);
    void PhaseShift(void);
    void CarierShift(void);
    void LoadFilter(void);
    bool SetManipulationType(OSA_MANIPULATE_TYPE enType);
    bool SetFilterParamFIR(FIR_FILTER stFilterParam);
    bool SetFilterParamIIR(IIR_FILTER stFilterParam);
    bool SetFilterParamLP_DownRate(FIR_FILTER stFilterParam);
    bool CalcFilterResponse(void);

    double CalcInterpolation(double *piTime, double *pdDataIn, int iNumPoint, double dCurrTime);
    void solveTridiag(double *sub, double *diag, double *sup, double *b, int n);
private:
    bool StartSpectrumThread(void);
    void StopSpectrumThread(void);
    bool m_bKillThreadSpectrum;
public:
    QString name;

    double *m_pdOutFFTNxt;
    QVector<double>	m_pdSpectrumNxt_X;
    QVector<double>	m_pdSpectrumNxt_Y;

    double	*m_pdSignalNxt_I;
    double	*m_pdSignalNxt_Q;
    Ipp64fc *m_pdSignalNxt_Complx;

    double	*m_pdDataI;
    double	*m_pdDataQ;
    Ipp32fc *m_pdDataIQ;

   double *m_pdOutFFT;
   QVector<double> m_pdSpectrum_X;
   QVector<double> m_pdSpectrum_Y;
   double *m_pdMaxHoldSpectrum;
   double *m_pdMaxHoldPhaseErr;

    double *ScatterBuff;
    double  *m_pdSignal;
    bool m_bInSettingMode;
public : //Spectrum
    int		m_iSizeSpectrum;
    int		m_iIndexStartSpectrum;
    int		m_iIndexStopSpectrum;
    bool	m_bUseAverageSpectrum;
    bool	m_bShowMaxHoldSpectrum;
    int m_iCounterCarrierShift;

    double	m_dThresholdLevel;
    double	m_dCurFrequency;
    double	m_dNextFrequency;
    double	m_dPosTime;
public:
    bool bRun;
    int m_iIndexConvert;
    qulonglong m_iSizeConvert;
    int		m_iSizeSignalNxt;
    double	*m_pdSignalNxt;
private:
    Configuration *m_pConfig;
    CCalculateFFT m_calcFFT;
    QTimer *timer;
    SM_ALL_SETTING m_stSettingSM;
public:
    void OpenSaveFile(QString strAddr);
    void RunConvert(bool bRun);
    bool IsRunConvert(void){return m_bRunConvert;}
    bool m_bStartSpectrum;
    bool m_bPauseSpectrum;
    bool m_bStartSelect;
    bool IsRunMode(void){return m_bRunMode;}
    bool IsLoadedDataFile(void){return m_pConfig->m_bLoadedFileInput;}
    bool IsStartSpectrum(void){return m_bStartSpectrum;}
    bool IsPauseSpectrum(void){return m_bPauseSpectrum;}
    bool IsStartSelect(void){return m_bStartSelect;}
private:
    bool m_bRunConvert;
    bool ConvertBuffer(void);
    bool m_bKillThreadConvert;
    bool m_bRunMode;
    double m_dCurrentDC;
    QString  InputFilePath;
    bool m_bFirstFrame;
    double *m_pdInterpFrame;
    //--- I Q
    int m_iFrameCount;
    QVector<double>	m_pdSpectrumFilter_X;
    QVector<double>	m_pdSpectrumFilter_Y;
    double	m_dMaxLevelSpectrumFilter;
    double	m_dMinLevelSpectrumFilter;
    double *m_pdSignalInterp;
    double *m_pdSignalInterp_I;
    double *m_pdSignalInterp_Q;
    //--- Spectrum Next data
    int		m_iSizeSpectrumNxt;
    int		m_iIndexStartSpectrumNxt;
    int		m_iIndexStopSpectrumNxt;
    bool	m_bUseAverageSpectrumNxt;
    bool	m_bShowMaxHoldSpectrumNxt;
    double	*m_pdMaxHoldSpectrumNxt;

    CCalculateFFT m_calcFFT_Filter;
    CCalculateFFT m_calcFFTNxt;

    int		m_iDownRate_DownSample;
    int		m_iDownRate_UpSample; // iAdded
    int		m_iChangeRatio ;// iAdded
    int		m_iWinType ;// iAdded

    Ipp8u  *bufLPF;
    Ipp64f *taps_ippLPF ; // iAdded
    int tapslen_ippLPF; // iAdded
    double rFreq_ippLPF; // iAdded
    int WinType_Index;
    int nSize;


public: // IIR Var
    IppsFIRSpec_64f *pSpec_IIR;
    double           *src_IIR, *dst_IIR, *dlysrc_IIR, *dlydst_IIR, *taps_IIR, *DATA_IIR;
    Ipp8u           *buf_IIR;
    int             specSize_IIR, bufSize_IIR;
    IppStatus status_IIR;

public: // FIR var
    IppsFIRSpec_64f *pSpec_FIR;
    double           *src_FIR, *dst_FIR, *dlysrc_FIR, *dlydst_FIR,*taps_FIR,*DATA_FIR;

    Ipp8u           *buf_FIR;
    int             specSize_FIR, bufSize_FIR;
    IppStatus status_FIR;

public: // iAdded
    IppsFIRSpec_64f *pSpec;
    double           *src, *dst, *dlysrc, *dlydst, *taps, *DATA;
    Ipp8u           *buf;
    int             specSize, bufSize;
    IppStatus status;

    void CreatLPFilter(int SIZE);

public: // iAdded
    IppsFIRSpec_64f *pSpeci;
    double           *srci, *dsti, *dlysrci, *dlydsti, *tapsi, *DATAi;
    Ipp8u           *bufi;
    int             specSizei, bufSizei;
    IppStatus statusi;
    void CreatLPFilter_I(int SIZEE);

public: // iAdded
    IppsFIRSpec_64f *pSpecq;
    double           *srcq, *dstq, *dlysrcq, *dlydstq, *tapsq, *DATAq;
    Ipp8u           *bufq;
    int             specSizeq, bufSizeq;
    IppStatus statusq;
    void CreatLPFilter_Q(int SIZEE);

    double *dst1;

public: // iAdded
    IppsFIRSpec_64f *pSpec_LF;
    double           *src_LF, *dst_LF, *dlysrc_LF, *dlydst_LF, *taps_LF, *DATA_LF;
    Ipp8u           *buf_LF;
    int             specSize_LF, bufSize_LF;
    IppStatus status_LF;

    void CreatLoadFilter(int SIZEE);

    void SavtToFile_double(double *dbDataInp2,int nLenOut);
    int unit;
    double FS_Last;
};

#endif // MODE_SM_H
