#include "mode_ff.h"

Mode_FF::Mode_FF(QObject *parent,QString FilePath):
      QThread(parent),name(FilePath)
{
    m_pConfig=new Configuration();
    bool yes=LoadDataFile(FilePath);
    killthread=true;
    b_Analyzed=false;
    m_process=new Processor();
}
bool Mode_FF::LoadDataFile(QString path)
{
    m_pConfig->OpenInputFile(path);
    return m_pConfig->m_bLoadedFileInput;
}
bool Mode_FF::RunMode(void)
{
    m_bRunMode = true;
    m_bStartSpectrum = false;
    m_bPauseSpectrum = false;
    m_bStartSelect = false;
    return true;
}
bool Mode_FF::StartSpectrum(void)
{
        m_pConfig->GotoFileStartPos();
        StartSpectrumThread();
        m_bStartSpectrum = true;
        m_bPauseSpectrum = false;
        m_bStartSelect = false;
  return m_bStartSpectrum;
}
bool Mode_FF::StartSpectrumThread(void)
{
  m_bKillThreadSpectrum = false;
  this->start(QThread::HighestPriority);
}
void Mode_FF::StopSpectrumThread(void)
{
    m_bKillThreadSpectrum = true;
    this->destroyed();

}
void Mode_FF::CloseMode(void)
{
    StopSpectrum();
}
void Mode_FF::StopSpectrum(void)
{
     this->destroyed();
}
void Mode_FF::Close(void)
{
    CloseMode();
}
void Mode_FF::CalculateSpectrum(QVector<double> *Input,QVector<double> *outx,QVector<double> *outy)
{
    outx->clear();
    outy->clear();
    for (int i=0; i< Input->size()/2; i++)
    {
//        outx->at(i)=((float)i*((float)(166.66*1000000))/(float)4096);
//        outy->at(i)=10*log10(pow(abs(Input->value(i)),2)+0.00001);
        outx->append(((float)i*((float)(166.66*1000000))/(float)4096));
        outy->append(10*log10(pow(abs(Input->value(i)),2)+0.00001));
    }
}
bool Mode_FF::Initialize(Configuration *pConfig)
{
    m_pConfig = pConfig;
    FF_ALL_SETTING stSettingFF;
    stSettingFF.stFFT.iSizeFFT = 1*4096;
    stSettingFF.stSpectrum.dStartFrequency = 0;
    stSettingFF.stSpectrum.dStopFrequency = m_pConfig->m_stInputFile.dSamplingFrequency / 2.0;
    stSettingFF.stSpectrum.dMaxLevel = 0;
    stSettingFF.stSpectrum.dMinLevel = -150;
    stSettingFF.stSpectrum.dOverlapRatio = 0.0;
    stSettingFF.stSpectrum.bUint_dBm = true;
    return true;
}
void Mode_FF::run()
{

    FileReadBuff = new QVector<double>;
    SpecBuff = new QVector<double>;
    SpecY = new QVector<double>;
    SpecX = new QVector<double>;
    SpecBuff->reserve(4096);
    FileReadBuff->reserve(4096);
    SpecX->reserve(2048);
    SpecY->reserve(2048);
    m_process->CreateFFT(12,8192);
    while (m_pConfig->ReadFromInputFile(FileReadBuff,8192))
    {
        m_process->CalcFFT(FileReadBuff,SpecBuff);
        CalculateSpectrum(SpecBuff,SpecX,SpecY);
        if(SpecX->size()==2048 && SpecY->size()==2048){
            emit sig_SpecdataReady(SpecX,SpecY);
        }
        else {
            qDebug("not  full buffer");

        }
        if(b_Analyzed)
           emit sig_ScatterdataReady(ScatterBuff);

        sleep(0.5);
    }
}
