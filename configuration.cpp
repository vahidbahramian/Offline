#include "configuration.h"

Configuration::Configuration()
{
    m_bLoadedFileInput=0;
    m_strAddrFileInput="";
    wavefile=false;
    iSizeFileRecord=0;
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

bool Configuration::OpenSaveFile(QString strAddrFile)
{
//    CFileException ex;
    m_bLoadedFileSave = false;
    m_fileSave.setFileName(strAddrFile);
    if (! m_fileSave.open(QFile::WriteOnly))
    {
        qDebug("Cannot open file");
        return false;
    }

    m_bLoadedFileSave = true;
    m_strAddrFileSave = strAddrFile;

    return true;
}

void Configuration::CloseSaveFile(void)
{
    if(m_bLoadedFileSave)
    {
        m_bLoadedFileSave = false;
        m_fileSave.close();
    }
}

bool Configuration::WriteToSaveFileIQ(double *pdDataI, double *pdDataQ, int iSize)
{
//    if(!m_bLoadedFileSave)
//        return false;

//    int iMinValueA2D = m_stInputFile.iMinValueA2D;
//    int iMaxValueA2D = m_stInputFile.iMaxValueA2D;

//    qint16 *piData16 = new qint16 [iSize*2];

//    if(m_stInputFile.enByteNum == IFB_2B)
//    {
//        qint16 *pnData = new qint16 [iSize*4];

//        for(int i=0; i<iSize; i++)
//        {
//            piData16[2*i+0]=pdDataI[i];
//            piData16[2*i+1]=pdDataQ[i];
//        }

//            if(!m_stInputFile.bFirstLSB)
//            {
//                memcpy(pnData, piData16, iSize*4);
//                q16 nData;
//                for(int i=0; i<iSize*2; i++)
//                {
//                    nData = pnData[2*i+0];
//                    pnData[2*i+0] = pnData[2*i+1];
//                    pnData[2*i+1] = nData;
//                }
//                memcpy(piData16, pnData, iSize*2);
//            }
//        m_fileSave.write(piData16, iSize*4);


//            SAFERELEASE_ARRAY(piData16);
//            SAFERELEASE_ARRAY(pnData);

//    }


    return true;
}
bool Configuration::WriteToSaveFile(double *pdData, int iSize)
{
    if(!m_bLoadedFileSave)
        return false;

    int iMinValueA2D = m_stInputFile.iMinValueA2D;
    int iMaxValueA2D = m_stInputFile.iMaxValueA2D;


    if(m_stInputFile.enByteNum == IFB_1B)
    {
//       char nData;

//        fscanf_s(pFile, "%c", &nData);

//        fData = nData;
//        if(m_stInputFile.bSignedValue)
//        fData = (signed)nData;

    }
    else if(m_stInputFile.enByteNum == IFB_2B)
    {
        if(iSize == 1)
        {
            char pnData[2];
            qint16 iData16;

            for(int i=0; i<iSize; i++)
            {
                iData16 = pdData[i];//(pdData[i] - iMinValueA2D) * (pow(2.0,16) - 1) / (iMaxValueA2D - iMinValueA2D);

                memcpy(pnData, &iData16, 2);

                if(m_stInputFile.bFirstLSB)
                {
                    m_fileSave.write(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
                    m_fileSave.write(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
                }
                else
                {
                    m_fileSave.write(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
                    m_fileSave.write(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
                }
            }
        }
        else
        {
            qint16 *piData16 = new qint16 [iSize];
            char *pnData=new char[iSize*2];

            if(m_stInputFile.bSignedValue)
            {
                for(int i=0; i<iSize; i++)
                    piData16[i] = pdData[i];
            }
            else
            {
                for(int i=0; i<iSize; i++)
                    piData16[i] = pdData[i];
            }

            memcpy(pnData, piData16, iSize*2);
            if(!m_stInputFile.bFirstLSB)
            {
                memcpy(pnData, piData16, iSize*2);
                char nData;
                for(int i=0; i<iSize; i++)
                {
                    nData = pnData[2*i+0];
                    pnData[2*i+0] = pnData[2*i+1];
                    pnData[2*i+1] = nData;
                }
                //memcpy(piData16, pnData, iSize*1);
            }

            m_fileSave.write(pnData, iSize*2);
            iSizeFileRecord+=iSize*2;



            delete piData16;
            delete pnData;
        }
    }
//    else if(m_stInputFile.enByteNum == IFB_4B)
//    {
//        /*			BYTE pnData[4];
//        INT32 iData32;

//        fscanf_s(pFile, "%c", &pnData[0]);
//        fscanf_s(pFile, "%c", &pnData[1]);
//        fscanf_s(pFile, "%c", &pnData[2]);
//        fscanf_s(pFile, "%c", &pnData[3]);

//        memcpy(&iData32, pnData, 4);

//        fData = iData32;
//        if(!m_stInputFile.bSignedValue)
//        fData = (unsigned)iData32;
//        */
//    }

    return true;
}

void Configuration::CheckSpectrumLimit(double& dStart, double& dStop)
{
    double dFs = m_stInputFile.dSamplingFrequency;
    double dFs_2 = dFs / 2;
    //dStop = dFs_2;
    //dStop*=2;
    if(dStart < 0)
        dStart = 0;
    if(dStop <= 0)
        dStop = dFs_2;

    if(dStop > dFs_2)
        dStop = dFs_2;
    if(dStart > dFs_2)
        dStart = 0;

    if(dStart >= dStop)
    {
        double dChange = dStart;
        dStart = dStop;
        dStop = dChange;
    }
}
bool Configuration::ReadFromInputFileIQ_wave(double *pdDataI, double *pdDataQ, int iSize)
{
//	if(m_ulPosInputFile >= m_ulSizeInputFile)
//		return false;

//	if(m_stInputFile.enByteNum == IFB_1B)
//	{
//		short *pnData = new short [iSize*2];
//		sf_read_short(m_pSndFile,pnData,m_stFileInfo.format);
//		if(!m_stInputFile.bSignedValue)
//		{
//			for(int i=0; i<iSize; i++)
//			{
//				pdDataI[i] = (INT8)pnData[2*i+0];
//				pdDataQ[i] = (INT8)pnData[2*i+1];
//			}
//		}
//		else
//		{
//			for(int i=0; i<iSize; i++)
//			{
//				pdDataI[i] = pnData[2*i+0];
//				pdDataQ[i] = pnData[2*i+1];
//			}
//		}
//		SAFERELEASE_ARRAY(pnData);

//	}
//	else if(m_stInputFile.enByteNum == IFB_2B)
//	{
//		INT16 *piData = new INT16 [iSize*2];
//		sf_read_short(m_pSndFile,piData,iSize*2);
//		//m_fileInput.Read(piData, iSize*4);

//		if(m_stInputFile.bSignedValue)
//		{
//			for(int i=0; i<iSize; i++)
//			{
//				pdDataI[i] = piData[2*i+0];
//				pdDataQ[i] = piData[2*i+1];
//			}
//		}
//		else
//		{
//			for(int i=0; i<iSize; i++)
//			{
//				pdDataI[i] = (unsigned)piData[2*i+0];
//				pdDataQ[i] = (unsigned)piData[2*i+1];
//			}
//		}
//		SAFERELEASE_ARRAY(piData);
//	}

//	//if (Count==0)
//	//{
//	//	m_ulPosInputFile += (4*iSize)+20;
//	//}
//	//else
//	//{
//		m_ulPosInputFile += (4*iSize);
//	//}

//	//Count++;
    return true;
}

bool Configuration::ReadFromInputFile(double *pdDataInput, int iSize)
{
    if(m_ulPosInputFile >= m_ulSizeInputFile)
            return false;

        if(m_stInputFile.enByteNum == IFB_1B)
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
                        pdDataInput[i] = (qint8)pnData[i];
                }
                else
                {
                    for(int i=0; i<iSize; i++)
                        pdDataInput[i] = pnData[i];
                }
            }
        }
        else if(m_stInputFile.enByteNum == IFB_2B)
        {
            if(iSize == 1)
            {
                char pnData[2];
                qint16 iData16;

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

                pdDataInput[0] = iData16;
                if(!m_stInputFile.bSignedValue)
                    pdDataInput[0] = (unsigned)iData16;
            }
            else
            {
                qint16 *piData16 = new qint16 [iSize];
                char *pnData = new char [(iSize*2)+1];

                m_fileInput.read(pnData, iSize*2);
                short tmp;
                if(m_stInputFile.bSignedValue)
                {
                    for(int i=0; i<iSize*2; i+=2)
                    {
                        memcpy(&tmp,pnData+i,2);
                        pdDataInput[i/2] = tmp;
                    }
                }
                else
                {
                    for(int i=0; i<iSize*2; i+=2)
                    {
                        memcmp(&tmp,pnData+i,2);
                        pdDataInput[i/2] =tmp;
                    }
                }

                delete [] piData16;
                delete [] pnData;
            }


        }
        else if(m_stInputFile.enByteNum == IFB_4B)
        {
            char pnData[4];
            qint32 iData32;

            if(m_stInputFile.bFirstLSB)
            {
                m_fileInput.read(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
                m_fileInput.read(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
                m_fileInput.read(&pnData[2], 1);		// fscanf_s(pFile, "%c", &pnData[2]);
                m_fileInput.read(&pnData[3], 1);		// fscanf_s(pFile, "%c", &pnData[3]);
            }
            else
            {
                m_fileInput.read(&pnData[3], 1);		// fscanf_s(pFile, "%c", &pnData[3]);
                m_fileInput.read(&pnData[2], 1);		// fscanf_s(pFile, "%c", &pnData[2]);
                m_fileInput.read(&pnData[1], 1);		// fscanf_s(pFile, "%c", &pnData[1]);
                m_fileInput.read(&pnData[0], 1);		// fscanf_s(pFile, "%c", &pnData[0]);
            }

            memcpy(&iData32, pnData, 4);

            pdDataInput[0] = iData32;
            if(!m_stInputFile.bSignedValue)
                pdDataInput[0] = (unsigned)iData32;
        }

        m_ulPosInputFile = m_fileInput.pos();

        return true;
}
void readTest()
{
//    if(m_ulPosInputFile >= m_ulSizeInputFile)
//        return false;

//     m_fileInput.seek(m_ulPosInputFile);
//    char *pnData = new char [iSize];
//    m_fileInput.read(pnData, iSize);
//    short tmp;
//    if(m_stInputFile.bSignedValue)
//    {
//        for(int i=0; i<iSize; i+=2)
//        {
//            memcpy(&tmp,pnData+i,2);
//            pdDataInput[i/2] = tmp;
//        }
//    }
//    else
//    {
//        for(int i=0; i<iSize; i+=2)
//        {
//            memcmp(&tmp,pnData+i,2);
//            pdDataInput[i/2] =tmp;
//        }
//    }
//    //delete piData16;
//    delete pnData;
}

CCalculateFFT::CCalculateFFT(void)
{
    m_pdWindow = NULL;
    m_pdInputBlock = NULL;
    m_stSettingFFT.iSizeFFT = 0;
    m_stSettingFFT.enWindowType = FWT_HANNING;
//    SetParameters(m_stSettingFFT);
}
CCalculateFFT::~CCalculateFFT(void)
{
//    CloseParamFFT();
    if(m_pdWindow)
        delete [] m_pdWindow;
    if(m_pdInputBlock)
        delete [] m_pdInputBlock;
}
bool CCalculateFFT::SetParameters(FFT_SETTING stSettingFFT)
{
    if(	(m_stSettingFFT.iSizeFFT == stSettingFFT.iSizeFFT) &&
        (m_stSettingFFT.FFT_order == stSettingFFT.FFT_order) &&
        (m_stSettingFFT.enWindowType == stSettingFFT.enWindowType))
        return false;

//    CloseParamFFT();
    if(m_pdWindow)
    {
        delete [] m_pdWindow;
        m_pdWindow = NULL;
    }
    if(m_pdInputBlock)
    {
        delete [] m_pdInputBlock;
        m_pdInputBlock = NULL;
    }


    m_stSettingFFT = stSettingFFT;
    m_pdWindow = new double [m_stSettingFFT.iSizeFFT];
    m_pdInputBlock = new double [m_stSettingFFT.iSizeFFT];

    for(int i=0; i<m_stSettingFFT.iSizeFFT; i++)
        m_pdWindow[i] = 1;

    switch (m_stSettingFFT.enWindowType)
    {
    case FWT_BOXCAR:
        boxcar(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    case FWT_TRIANG:
        triang(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    case FWT_HAMMING:
        hamming(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    case FWT_HANNING:
        hanning(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    case FWT_BLACKMAN:
        blackman(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    case FWT_FLATTOP:
        flattop(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    case FWT_KAISER:
        flattop(m_stSettingFFT.iSizeFFT, m_pdWindow);
        break;
    }

    InitParamFFT();

    return true;
}
bool CCalculateFFT::CalcFFT(double *In,double *Out)
{
    ippsRealToCplx_64f((Ipp64f*)In,(Ipp64f*)img,FFT_in_out,m_stSettingFFT.iSizeFFT);
    ippsFFTFwd_CToC_64fc_I(FFT_in_out,(IppsFFTSpec_C_64fc*) pSpec,workBuffer);
    ippsCplxToReal_64fc(FFT_in_out,Out,img,m_stSettingFFT.iSizeFFT);
}


void CCalculateFFT::InitParamFFT()
{
    img=ippsMalloc_64f(m_stSettingFFT.iSizeFFT);
    ippsZero_64f(img, m_stSettingFFT.iSizeFFT);
    FFT_in_out=ippsMalloc_64fc(m_stSettingFFT.iSizeFFT);
    eps=2.220446049250313e-16;
    ippsFFTGetSize_C_64fc(m_stSettingFFT.FFT_order,IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,&pSpecSize,&pSpecBufferSize,&pBufferSize);
    pSpec = ippsMalloc_8u(pSpecSize);
    workBuffer=ippsMalloc_8u(pBufferSize);
    ppFFTSpec=(IppsFFTSpec_C_64fc**)ippsMalloc_8u(pSpecSize);
    pSpecBuffer=ippsMalloc_8u(pSpecSize);
    ippsFFTInit_C_64fc(ppFFTSpec, m_stSettingFFT.FFT_order,IPP_FFT_DIV_FWD_BY_N, ippAlgHintNone,pSpec,pSpecBuffer);

    //-----------
    bFFTCreated=true;

}
void CCalculateFFT::CloseParamFFT(void)
{


    if(pSpec)
          delete pSpec;

    if(workBuffer)
    {
        ippsFree(workBuffer);
    }
    if(ppFFTSpec)
    {
         ippsFree(ppFFTSpec);
    }
    if(pSpecBuffer)
    {
        ippsFree(pSpecBuffer);;
    }
    if(m_pdWindow)
    {
        delete [] m_pdWindow;
        m_pdWindow = NULL;
    }
    if(m_pdInputBlock)
    {
        delete [] m_pdInputBlock;
        m_pdInputBlock = NULL;
    }
     bFFTCreated=false;

}
/*
* Boxcar
*
* n window length
* w buffer for the window parameters
*/
void CCalculateFFT::boxcar(int n, double* w)
{
    int i;
    /* Calculate window coefficients */
    for (i=0 ; i<n ; i++)
        w[i] = 1.0;
}
/*
* Triang a.k.a Bartlett
*
*               |    (N-1)|
*           2 * |k - -----|
*               |      2  |
* w = 1.0 - ---------------
*                    N+1
* n window length
* w buffer for the window parameters
*/
void CCalculateFFT::triang(int n, double* w)
{
    double k1  = (double)(n & 1);
    double k2  = 1/((double)n + k1);
    int      end = (n + 1) >> 1;
    int	   i;

    /* Calculate window coefficients */
    for (i=0 ; i<end ; i++)
        w[i] = w[n-i-1] = (2.0*((double)(i+1))-(1.0-k1))*k2;
}
/*
* Hanning
*                   2*pi*k
* w = 0.5 - 0.5*cos(------), where 0 < k <= N
*                    N+1
* n window length
* w buffer for the window parameters
*/
void CCalculateFFT::hanning(int n, double* w)
{
    int	   i;
    double k = 2*PI/((double)(n+1)); /* 2*pi/(N+1) */

    /* Calculate window coefficients */
    for (i=0; i<n; i++)
        *w++ = 0.5*(1.0 - cos(k*(double)(i+1)));
}
/*
* Hamming
*                        2*pi*k
* w(k) = 0.54 - 0.46*cos(------), where 0 <= k < N
*                         N-1
*
* n window length
* w buffer for the window parameters
*/
void CCalculateFFT::hamming(int n,double* w)
{
    int      i;
    double k = 2*PI/((double)(n-1)); /* 2*pi/(N-1) */

    /* Calculate window coefficients */
    for (i=0; i<n; i++)
        *w++ = 0.54 - 0.46*cos(k*(double)i);
}
/*
* Blackman
*                       2*pi*k             4*pi*k
* w(k) = 0.42 - 0.5*cos(------) + 0.08*cos(------), where 0 <= k < N
*                        N-1                 N-1
*
* n window length
* w buffer for the window parameters
*/
void CCalculateFFT::blackman(int n,double* w)
{
    int      i;
    double k1 = 2*PI/((double)(n-1)); /* 2*pi/(N-1) */
    double k2 = 2*k1; /* 4*pi/(N-1) */

    /* Calculate window coefficients */
    for (i=0; i<n; i++)
        *w++ = 0.42 - 0.50*cos(k1*(double)i) + 0.08*cos(k2*(double)i);
}
/*
* Flattop
*                                        2*pi*k                     4*pi*k
* w(k) = 0.2810638602 - 0.5208971735*cos(------) + 0.1980389663*cos(------), where 0 <= k < N
*                                          N-1                        N-1
*
* n window length
* w buffer for the window parameters
*/
void CCalculateFFT::flattop(int n,double* w)
{
    int      i;
    double k1 = 2*PI/((double)(n-1)); /* 2*pi/(N-1) */
    double k2 = 2*k1;                   /* 4*pi/(N-1) */

    /* Calculate window coefficients */
    for (i=0; i<n; i++)
        *w++ = 0.2810638602 - 0.5208971735*cos(k1*(double)i) + 0.1980389663*cos(k2*(double)i);
}

