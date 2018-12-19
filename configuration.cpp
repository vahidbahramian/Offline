#include "configuration.h"

Configuration::Configuration()
{
    m_bLoadedFileInput=0;
    m_strAddrFileInput="";
}
bool Configuration::Initialize(void)
{
    if(1)
    {
        m_stInputFile.enFreqUnit = IFU_M;
        m_stInputFile.dSamplingFrequency = 166.66666;
        m_stInputFile.dMinVoltageA2D = -1.5;
        m_stInputFile.dMaxVoltageA2D = 1.5;
        m_stInputFile.iMinValueA2D = 0;
        m_stInputFile.iMaxValueA2D = pow(2.0, 15);

        m_stInputFile.enByteNum = IFB_2B;
        m_stInputFile.bSignedValue = true;
        m_stInputFile.bFirstLSB = true;

//        InitDemodBitmaps();

    }

    ArrangeInputFile(&m_stInputFile);

    return true;
}
bool Configuration::OpenInputFile(QString strAddrFile)
{
//    if(1)
//    {
//        if(m_strAddrFileInput.compare(strAddrFile) != 0)
//            CloseInputFile();
//        else
//            return true;
//    }

    m_bLoadedFileInput = false;

    // open the source file for reading
    m_fileInput.setFileName(strAddrFile);
    if (! m_fileInput.open(QFile::ReadOnly))
    {
        qDebug("Cannot open file");
        return false;
    }

    int m_ulSizeInput = m_fileInput.size();
    m_bLoadedFileInput = true;
    m_strAddrFileInput = strAddrFile;
//    int Lenght=m_strAddrFileInput.size();
//    QString Str=m_strAddrFileInput;
//    unsigned found = Str.find_last_of("\\");
//    m_strAddrFileInput=m_strAddrFileInput.Right(Lenght-found);

    m_ulSizeInputFile = m_fileInput.size();
    m_ulPosInputFile = m_fileInput.pos();

    return true;
}
void Configuration::CloseInputFile(void)
{
    if(m_bLoadedFileInput)
        m_fileInput.close();
        m_strAddrFileInput = "";
        m_bLoadedFileInput = false;
}
bool Configuration::IsEndOfFile(void)
{
    if(m_ulPosInputFile >= m_ulSizeInputFile)
        return true;
    else
        return false;
}
long Configuration::GetInputFileSampleSize(void)
{
    int ulSampleSize = 0;
    if(m_bLoadedFileInput)
    {
        int icharNum = m_stInputFile.enByteNum;
        int ulSize = m_fileInput.size();
        //ulSampleSize = ulSize / icharNum;
        ulSampleSize = ulSize / pow(2,icharNum);
    }
    return ulSampleSize;
}
void Configuration::GotoFileStartPos(void)
{
    if(m_bLoadedFileInput)
        m_fileInput.seek(20);

    m_ulPosInputFile = 0;


}
void Configuration::GotoNextSample(int iNumSample)
{
    if(m_bLoadedFileInput)
    {
        int icharNum = m_stInputFile.enByteNum;
        int ulSize = iNumSample * g_iINPUT_FILE_char[icharNum];
        m_fileInput.seek(ulSize);
    }
}
void Configuration::ArrangeInputFile(INPUT_FILE_SETTING *pInputFile)
{
    int iBitNum = 8 * g_iINPUT_FILE_char[(int)pInputFile->enByteNum];
    if(pInputFile->bSignedValue)
    {
        pInputFile->iMinValueA2D = -1 * pow(2.0, iBitNum - 1);
        pInputFile->iMaxValueA2D = pow(2.0, iBitNum - 1);
    }
    else
    {
        pInputFile->iMinValueA2D = 0;
        pInputFile->iMaxValueA2D = pow(2.0, iBitNum);
    }
}
double Configuration::GetOffsetSpectrum(void)
{
    return -30.0;
}
bool Configuration::ReadFromInputFile(QVector<double> &pdDataInput, int iSize)
{
    if(m_ulPosInputFile >= m_ulSizeInputFile)
        return false;

     m_fileInput.seek(m_ulPosInputFile);


/*    if(m_stInputFile.encharNum == IFB_1B)
    {
        if(iSize == 1)
        {
            char nData;
            m_fileInput.read(&nData, 1);

            pdDataInput[0] = nData;
            if(m_stInputFile.bSignedValue)
                pdDataInput[0] = (signed)nData;
        }
        else
        {
            char *pnData = new char [iSize];
            m_fileInput.read(pnData, iSize);

            if(m_stInputFile.bSignedValue)
            {
                for(int i=0; i<iSize; i++)
                    pdDataInput[i] = (int8_t)pnData[i];
            }
            else
            {
                for(int i=0; i<iSize; i++)
                    pdDataInput[i] = pnData[i];
            }
        }
    }
    else */
    if(m_stInputFile.enByteNum == IFB_2B)
    {
        if(iSize == 1)
        {
            char pnData[2];
            int16_t iData16;

            if(m_stInputFile.bFirstLSB)
            {
                m_fileInput.read(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
                m_fileInput.read(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
            }
            else
            {
                m_fileInput.read(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
                m_fileInput.read(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
            }

            memcpy(&iData16, pnData, 2);

        }
        else
        {
            char *pnData = new char [iSize];
            m_fileInput.read(pnData, iSize);
            short tmp;
            if(m_stInputFile.bSignedValue)
            {
                for(int i=0; i<iSize; i+=2)
                {
                    memcpy(&tmp,pnData+i,2);
                    pdDataInput[i/2] = tmp;
                }
            }
            else
            {
                for(int i=0; i<iSize; i+=2)
                {
                    memcmp(&tmp,pnData+i,2);
                    pdDataInput[i/2] =tmp;
                }
            }
            //delete piData16;
            delete pnData;
        }


    }
//    else if(m_stInputFile.encharNum == IFB_4B)
//    {
//        char pnData[4];
//        int32_t iData32;

//        if(m_stInputFile.bFirstLSB)
//        {
//            m_fileInput.read(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
//            m_fileInput.read(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
//            m_fileInput.read(&pnData[2], 1);		// fscanf_s(pFile, "%c", &pnData[2]);
//            m_fileInput.read(&pnData[3], 1);		// fscanf_s(pFile, "%c", &pnData[3]);
//        }
//        else
//        {
//            m_fileInput.read(&pnData[3], 1);		// fscanf_s(pFile, "%c", &pnData[3]);
//            m_fileInput.read(&pnData[2], 1);		// fscanf_s(pFile, "%c", &pnData[2]);
//            m_fileInput.read(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
//            m_fileInput.read(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
//        }

//        memcpy(&iData32, pnData, 4);

//        pdDataInput[0] = iData32;
//        if(!m_stInputFile.bSignedValue)
//            pdDataInput[0] = (unsigned)iData32;
//    }

    m_ulPosInputFile = m_fileInput.pos();

    return true;
}
void Configuration::CreateFFT(int inum,int iSizefft)
{

    iSizeFFT= iSizefft;

    FFT_in_out=ippsMalloc_64fc(iSizeFFT);
    eps=2.220446049250313e-16;
    ippsFFTGetSize_C_64fc(inum,IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,&pSpecSize,&pSpecBufferSize,&pBufferSize);
    pSpec = ippsMalloc_8u(pSpecSize);
    workBuffer=ippsMalloc_8u(pBufferSize);
    ppFFTSpec=(IppsFFTSpec_C_64fc**)ippsMalloc_8u(pSpecSize);
    pSpecBuffer=ippsMalloc_8u(pSpecSize);
    ippsFFTInit_C_64fc(ppFFTSpec, inum,IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,pSpec,pSpecBuffer);

    //-----------


    src   = ippsMalloc_64f(iSizeFFT);
    dst   = ippsMalloc_64f(iSizeFFT);
    bFFTCreated=true;
}
void Configuration::DestroyFFT()
{
    ippsFree(pSpec);
    ippsFree(workBuffer);
    ippsFree(ppFFTSpec);
    ippsFree(pSpecBuffer);

    delete src;
    delete dst;
    bFFTCreated=false;

}
void Configuration::CalcFFT(QVector<double> In,QVector<double> &Out)
{
    Ipp64f *img;
    img=ippsMalloc_64f(iSizeFFT);
    ippsZero_64f(img, iSizeFFT);
    ippsCopy_64f(&In[0], src, iSizeFFT);
    ippsRealToCplx_64f((Ipp64f*)src,(Ipp64f*)img,FFT_in_out,iSizeFFT);
    ippsFFTFwd_CToC_64fc_I(FFT_in_out,(IppsFFTSpec_C_64fc*) pSpec,workBuffer);
    ippsCplxToReal_64fc(FFT_in_out,dst,img,iSizeFFT);
    ippsCopy_64f(dst, &Out[0], iSizeFFT);
    ippsFree(img);

}
