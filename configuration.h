#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <iostream>
#include <QString>
#include <vector>
#include <QVector>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <qfiledialog.h>
#include <QDir>
#include <ipp.h>
#include <math.h>
#include <QColor>


using namespace std;
#define PI 3.1415926535897932384626433832795
//--- FFT Window types
enum FFT_WINDOW_TYPE {FWT_NONE, FWT_BOXCAR, FWT_TRIANG, FWT_HAMMING, FWT_HANNING, FWT_BLACKMAN, FWT_FLATTOP, FWT_KAISER};
#define nOSD_DEMOD_TYPE 37
#define nINPUT_FILE_char 4
#define nINPUT_FILE_char 4
#define nINPUT_FREQ_UNIT 3
enum OSA_MAIN_MODES {OMM_SA, OMM_SM, OMM_FF, OMM_DS, OMM_FH, OMM_OFDM};
enum INPUT_FREQ_UNIT {IFU_H, IFU_K, IFU_M};
const QString g_strINPUT_FREQ_UNIT[nINPUT_FREQ_UNIT] = {"Hz", "KHz", "MHz"};
const int g_iINPUT_FREQ_UNIT[nINPUT_FREQ_UNIT] = {1, 1000, 1000000};

enum INPUT_FILE_char {IFB_1B, IFB_2B, IFB_4B};
const QString g_strINPUT_FILE_char[nINPUT_FILE_char] = {"1 char", "2 char", "4 char"};
const int g_iINPUT_FILE_char[nINPUT_FILE_char] = {1,2,4};
const int g_iINPUT_FILE_BYTE[nINPUT_FILE_char] = {1,2,4};


struct DEMOD_BITMAP
{
    bool bInverted;
    bool bSwap;
    bool bGrayCode;
    int iPhaseShift;

    char pnMapping[512];
    int iSizeMap;
    int iSizeShift;

    //--- for graphic show
    double pdPosMapI[512];
    double pdPosMapQ[512];
};
struct ALL_DEMOD_BITMAP
{
    DEMOD_BITMAP pstDemod[nOSD_DEMOD_TYPE];
    bool bWriteSymbolchar;	// if true, write any symbol per one char

};
struct SPECTRUM_SETTING
{
    double	dStartFrequency;
    double	dStopFrequency;

    double	dMaxLevel;
    double	dMinLevel;

    double	dOverlapRatio;
    bool	bUint_dBm;
};

struct TIMESIG_SETTING
{
    double	dMaxLevel;
    double	dMinLevel;
    int		iSampleShow;
};

struct TIME_CORR_SETTING
{
    double	dMaxLevel;
    double	dMinLevel;
    int		iSampleShow;
    int		iCorrSample1;
    int		iCorrSample2;
};

struct WATERFALL_SETTING
{
    int		iLinePerSecond;
    bool	bShowUpperLevelThr;
};

struct HISTOGRAM_SETTING
{
    bool	bShowUpperLevelThr;
};
struct FFT_SETTING
{
    int		iSizeFFT;
//	int		iDftiForwardDomain;
//	int		iSampleOverlap;
    int FFT_order;
    FFT_WINDOW_TYPE enWindowType;
};
struct INPUT_FILE_SETTING
{
    INPUT_FREQ_UNIT enFreqUnit;
    double	dSamplingFrequency;		//in MHz
    bool bInputIQ;
    double	dMinVoltageA2D;
    double	dMaxVoltageA2D;			//Voltage

    int		iMinValueA2D;			//
    int		iMaxValueA2D;

//	INPUT_FILE_TYPE		enFileType;
    INPUT_FILE_char		enByteNum;

    bool	bSignedValue;
    bool	bFirstLSB;
};




enum OSD_DEMOD_TYPE {ODT_NONE, ODT_BPSK, ODT_QPSK, ODT_8PSK, ODT_2FSK,
                     ODT_CP_4FSK, ODT_CP_8FSK, ODT_DQPSK ,ODT_OQPSK ,
                     ODT_QAM16 ,ODT_QAM32, ODT_QAM64, ODT_D8PSK, ODT_APSK16,
                     ODT_APSK32, ODT_APSK64,ODT_QAM128,ODT_QPR9,ODT_QPR25,
                     ODT_QPR49 , ODT_pi4DQPSK, ODT_UAQPSK,
                     ODT_4FSK, ODT_DC8FSK, ODT_ASK4, ODT_ASK2,
                     ODT_SQPSK, ODT_CP_2FSK, ODT_MSK, ODT_GMSK, ODT_LGMSK,
                     ODT_SOQPSK, ODT_AM, ODT_FM, ODT_CW, ODT_SQPN,ODT_DSB};
const QString g_strOSD_DEMOD_TYPE[nOSD_DEMOD_TYPE] = {"None" ,"BPSK" ,"QPSK" ,"8PSK" ,"2FSK" ,"CP4FSK" ,
                                                     "CP8FSK" , "DQPSK" , "OQPSK" , "QAM16" , "QAM32" ,
                                                     "QAM64", "D8PSK", "APSK16", "APSK32","64APSK" ,"QAM128",
                                                     "QPR9", "QPR25","QPR49", "pi/4DQPSK", "UAQPSK", "4FSK",
                                                     "8FSK", "ASK4", "ASK2", "SQPSK", "CP2FSK", "MSK", "GMSK", "GMSK",
                                                     "SOQPSK", "AM", "FM", "SSB", "SQPN","DSB"};
struct ESTIMATE_PARAM
{
    int iAMR;
    double dFC;
    double dSNR;
    double sRS;

};

class Configuration
{
public:
    Configuration();

    INPUT_FILE_SETTING  m_stInputFile;
    ALL_DEMOD_BITMAP	m_stDemodBitmaps;




    QString				m_strMainAddr;
//    VIEW_SETTING		m_stSettingView;

    QFile m_fileInput;
    bool m_bLoadedFileInput;
    int aa;
    QString m_strAddrFileInput;
    long m_ulSizeInputFile;
    long m_ulPosInputFile;

    QFile m_fileInput2;
    bool m_bLoadedFileInput2;
    QString m_strAddrFileInput2;
    long m_ulSizeInputFile2;
    long m_ulPosInputFile2;

    QFile m_fileSave;
    bool m_bLoadedFileSave;
    QString m_strAddrFileSave;

public:
    bool Chk_Equalizer,Chk_LSB_First,m_ChkUserChoose;

    bool *Get_Chk_Lsb(void) {return  &Chk_LSB_First;}
    bool *Get_Chk_Equ(void) {return  &Chk_Equalizer;}

    int Demod_Idx;
     QVector<double> *auto_Corrolation;
    int B_SizeOfRecord;
    QVector<char> DATA;

    // i added
    bool m_pFlagFHInit;
    long StartPoseInit;


    bool Initialize(void);
    void ArrangeInputFile(INPUT_FILE_SETTING *pInputFile);
    void InitDemodBitmaps(void);
    void Close(void);
    bool LoadConfigFromFile(void);
    bool SaveConfigToFile(void);
    bool OpenInputFile(QString strAddrFile);
    void CloseInputFile(void);
    bool IsEndOfFile(void);
    long GetInputFileSampleSize(void);
    void GotoFileStartPos(void);
    void GotoNextSample(int iNumSample);


    //--- File 2
    bool OpenInputFile2(QString strAddrFile);
    void CloseInputFile2(void);
    bool IsEndOfFile2(void);
    long GetInputFileSampleSize2(void);
    void GotoFileStartPos2(void);
    void GotoNextSample2(int iNumSample);


    bool OpenSaveFile(QString strAddrFile);
    void CloseSaveFile(void);


    bool ReadFromInputFile(double *pdDataInput, int iSize);
    bool ReadFromInputFile2( QVector<double> *pdDataInput, int iSize);
    bool ReadFromInputFileIQ( QVector<double> *pdDataI,  QVector<double> *pdDataQ, int iSize);
    bool ReadFromInputFileIQ_wave(double *pdDataI, double *pdDataQ, int iSize);

    bool ScaleTimingSignal( QVector<double> *pdDataIn, int iSize,  QVector<double> *pdTimingSignal);
    double GetOffsetSpectrum(void);
    double CalcAmplitude(double dDBm);

    bool WriteToSaveFile( double *pdData, int iSize);
    bool WriteToSaveFileIQ( double *pdDataI, double *pdDataQ, int iSize);
    void CheckSpectrumLimit(double& dStart, double& dStop);


    void CreateDemodBitmaps(ALL_DEMOD_BITMAP *pstDemodBitmaps);
    bool wavefile;
    int iSizeFileRecord;


};
void readTest();
class CCalculateFFT
{
public:
    CCalculateFFT();
    ~CCalculateFFT();
public:
    bool SetParameters(FFT_SETTING stSettingFFT);
    bool CalcFFT(double *In, double *Out);
    bool bFFTCreated;
    bool IsFFTCreated(void){return bFFTCreated;}

private:
    void boxcar(int n, double* w);
    void triang(int n, double* w);
    void hamming(int n,double* w);
    void hanning(int n, double* w);
    void blackman(int n,double* w);
    void flattop(int n,double* w);
    void kaiser(int n, double* w,double b);
    void blackmanHarris(int n,double* w);

    void InitParamFFT(void);
    void CloseParamFFT(void);

    //--- FFT Parameters
    FFT_SETTING				m_stSettingFFT;
    double					*m_pdWindow;
    double					*m_pdInputBlock;
    Ipp64f *img;


private:
    double           *src, *dst, *dlysrc, *dlydst, *taps, *DATA;
    Ipp64fc *Temp2,*Zeros,*Temp_Sum,*Max_FFT,*RS_Array_Total,*Final_FFT_Buff;
    Ipp8u *pSpec,* workBuffer;
    Ipp64f eps;
    IppsFFTSpec_C_64fc** ppFFTSpec;
    Ipp8u* pSpec1;
    Ipp8u* pSpecBuffer;
    Ipp64fc *Sig,*Delayed_Sig,*FFT_in_out,*Temp,*Temp3;

    Ipp64f pMax;
    int pIndx,*Indexes;
    int pSpecSize,pSpecBufferSize,pBufferSize;


};

#endif // CONFIGURATION_H
