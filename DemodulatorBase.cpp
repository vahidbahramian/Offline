#include "StdAfx.h"
#include "DemodulatorBase.h"

//#include "SSBDemodulator.h"
//#include "../BCGP_HF/SharedVariables.h"
//#include "../BCGP_HF/ChangeDate.h"
//HF Project Demodulator
namespace NS_DSP
{
	CString	CDemodulatorBase::m_strRecordRootPath = "d:\\HF_Audios";

	CDemodulatorBase::CDemodulatorBase(void)
	{
		//calculate max
		nCalcMSBCounter			=	0;
		nCalcMSBNumberSamples	=	1000000;
		m_fMSBValue				=	0;
		nMSBPosition			=	0;
		nType					=	eModulationUnknown;
		bFixed					=	TRUE;
		m_strRecordPath			=	_T("");
		nTunedFrequency			=	0;
		nLastTunedFrequency		=	0;
		m_nFrequency			=	0;
		m_fSnr					=	0;
		m_fBufferScatter		=	NULL;

		m_dErrDemodTiming		=	0;
		m_dErrDemodPhase		=	0;

		m_pFileRecord			=	NULL;
	}

	CDemodulatorBase::~CDemodulatorBase(void)
	{
//		m_Record.Close();
		if(m_fBufferScatter)
			delete [] m_fBufferScatter;
	}

	BOOL CDemodulatorBase::Create(DEMOD_PARAM stParam)
	{
		m_stParam = stParam;
	//	m_pFileRecord = NULL;
		m_fBufferScatter = new MKL_Complex16 [2000];
		return TRUE;
	}

	void CDemodulatorBase::Reset()
	{
		ResetCalcMSB();
		nTunedFrequency			=	0;
		nLastTunedFrequency		=	0;

	}

	//Re-Calculate MSB bit from CIC output
	void CDemodulatorBase::ResetCalcMSB()
	{
		nCalcMSBCounter			=	0;
		nMSBLocate				=	0;
		nMSBPosition			=	0;
		// 		nMSBValue				=	0;
	}


	BOOL CDemodulatorBase::CreateRecordRootDirectory()
	{
		CString strRoot = GetRecordRootPath();
//--- chech osa
//		strRoot = strRoot + _T("//")+CSharedVariables::m_Session.m_stCurrentUser.m_chUName;
		CString strSubDir , strTotal;
		BOOL bRet = TRUE;
		int i;
		DWORD dwRet = GetFileAttributes(strRoot);
		if (dwRet !=FILE_ATTRIBUTE_DIRECTORY)
		{//if directory doesn't exist
			bRet = CreateDirectory(strRoot,NULL);
			//create sub-directories
			for (i=0;i<eNumberModulation;i++)
			{
				if (i == eModulationUnknown ) continue;
				strSubDir = CGlobalTypes::GetEnumModulationStr((enumModulationType)i);
				strTotal = strRoot + "\\" + strSubDir;
				bRet = CreateDirectory(strTotal,NULL);
			}
			return bRet;
		}

		//check sub-directories
		for (i=0;i<eNumberModulation;i++)
		{
			if (i == eModulationUnknown ) continue;
			strSubDir = CGlobalTypes::GetEnumModulationStr((enumModulationType)i);
			strTotal = strRoot + "\\" + strSubDir;
			dwRet = GetFileAttributes(strTotal);
			if (dwRet != FILE_ATTRIBUTE_DIRECTORY)
				bRet = CreateDirectory(strTotal,NULL);
		}
		return bRet;
	}

	CString CDemodulatorBase::CreateRecordDate()
	{
		COleDateTime m_Now = COleDateTime::GetCurrentTime();
//		CChangeDate chngDte;
//		ALL_DATE GeorgianDate;
//		GeorgianDate.wYear = m_Now.GetYear();
//		GeorgianDate.wMonth = m_Now.GetMonth();
//		GeorgianDate.wDay = m_Now.GetDay();
//		ALL_DATE ShamsiDate;
//		chngDte.ConvertMiladi2Shamsi(&GeorgianDate,&ShamsiDate);

		//CString strNow = m_Now.Format("%Y_%m_%d_%H_%M_%S");
		CString strNow;
//		strNow.Format("%d_%d_%d_%d_%d_%d",(int)ShamsiDate.wYear,(int)ShamsiDate.wMonth,(int)ShamsiDate.wDay,
//			m_Now.GetHour(),m_Now.GetMinute(),m_Now.GetSecond());
		return strNow;


	}
	BOOL CDemodulatorBase::Record(BOOL bStart,double fFreq)
	{
		int nWrite=0;
		if (!bStart)
		{//stop recording 
//--- chech osa
//			m_Record.Close();
		}else
		{//start recording

//--- chech osa
//			if (!IsRecording())
			{//if Begins Recording ,Create File First

				if (m_strRecordPath.IsEmpty())
				{
//--- chech osa
//					if (CSharedVariables::m_nModType == eModulationLSB || CSharedVariables::m_nModType == eModulationUSB)
					{
						m_nFrequency = (int)fFreq;
					}
					m_strRecordPath = CreateRecordFileName(m_nFrequency);
				}
//--- chech osa
//				return m_Record.Open(m_strRecordPath,1,
//					HF_AUDIO_RATE,SF_FORMAT_PCM_16,SF_FORMAT_WAV);
			}
			//Record Samples
			return RecordSamples();
		}//if
		return TRUE;
	}//func

	CString CDemodulatorBase::CreateRecordFileName(double fFreq)
	{
		//if (fFreq>30000)
		//{
		//	m_nFrequency = ceil(fFreq/1000);
		//}
		//m_nFrequency = fFreq;
		CString strNow = CreateRecordDate();
		CString strPath = GetRecordRootPath();
		if (nType == eModulationSSB)
		{
//			nType = CSharedVariables::m_nModType;
		}
/*		if (nType == eModulationCW || 
			CGlobalTypes::GetModulationCategory(nType)==eModulationCatDigital)
			strPath.Format ("%s\\%s\\%s\\%s_%d_%f.txt",GetRecordRootPath(),CSharedVariables::m_Session.m_stCurrentUser.m_chUName,
			CGlobalTypes::GetEnumModulationStr(nType),strNow,nID,fFreq);
		else
			strPath.Format ("%s\\%s\\%s\\%s_%d_%f.wav",GetRecordRootPath(),CSharedVariables::m_Session.m_stCurrentUser.m_chUName,
			CGlobalTypes::GetEnumModulationStr(nType),strNow,nID,fFreq);
			*/
		return strPath;
	}//func

	BOOL CDemodulatorBase::RecordSamples()
	{
		return TRUE;
	}

	void CDemodulatorBase::DigitStat(BOOL bIsRecording)//,BOOL bIsPlaying,double fRs,double fFineTune,double fDevFreq,double fFreq)
	{
	}

	double CDemodulatorBase::CarrierEstimate()
	{
		return 1;
	}

	void CDemodulatorBase::SelectFilter(enumFilterBandwidth nFilter,double fFineTune,enumModulationType nModulation,double fNewInpFrequency,BOOL bIsSetCarrier)
	{
		//return 1;
	}

	void CDemodulatorBase::SetScatterBuffer(MKL_Complex16 *m_fBuffer, int nSize)
	{
		if(nSize > OFFLINE_MAX_BIT_BUFFER_LENGTH)
			AfxMessageBox("too large bit");

		if(nSize > 2000)
			nSize = 2000;

		memcpy(m_fBufferScatter, m_fBuffer, sizeof(MKL_Complex16)*nSize);
		m_nSizeScatter = nSize;
	}

	bool CDemodulatorBase::RecordSymbol(double *pdBufferSymbol, int iSizeSymbol, int iSizeBit)
	{
		if(!m_pFileRecord)
			return false;


		if(*m_pbWriteSymbolByte)
		{
			BYTE *pnData = new BYTE [iSizeSymbol];
			for(int i=0; i<iSizeSymbol; i++)
				pnData[i] = (BYTE)pdBufferSymbol[i];
			m_pFileRecord->Write(pnData, iSizeSymbol);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}

		
		if(iSizeBit == 1) //BPSK,ASK2,
		{
			BYTE nData;
			int iSizeByte = iSizeSymbol/8;
			BYTE *pnData = new BYTE [iSizeByte];
			BYTE nSymbol;

			for (int i=0; i<iSizeByte; i++)
			{
				nData = 0;
				for(int j=0; j<8; j++)
				{
					nData <<= 1;
					nSymbol = pdBufferSymbol[8*i + j];

					//--- Change Format
					//	nSymbol = ChangeSumbolFormat(nSymbol, 2);

					nData |= nSymbol;
				}
				pnData[i] = nData;
			}

			if(m_pFileRecord != NULL)
				m_pFileRecord->Write(pnData, iSizeByte);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}


		else if(iSizeBit == 2) //QPSK-OQPSK-DQPSK
		{
			BYTE nData;
			int iSizeByte = iSizeSymbol / 4;
			BYTE *pnData = new BYTE [iSizeByte];
			BYTE nSymbol;

			for (int i=0; i<iSizeByte; i++)
			{
				nData = 0;
				for(int j=0; j<4; j++)
				{
					nData <<= 2;
					nSymbol = pdBufferSymbol[4*i + j];

					//--- Change Format
					//	nSymbol = ChangeSumbolFormat(nSymbol, 2);

					nData |= nSymbol;
				}
				pnData[i] = nData;
			}

			if(m_pFileRecord != NULL)
				m_pFileRecord->Write(pnData, iSizeByte);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}


		else if(iSizeBit == 3)
		{
			UINT32 dwBuffer;
			int iSizeByte = iSizeSymbol*iSizeBit / 8;
			BYTE *pnData = new BYTE [iSizeByte];
			DWORD dwSymbol;

			int iSizeByte_3 = iSizeByte / 3;

			for (int i=0; i<iSizeByte_3; i++)
			{
				dwBuffer = 0;
				for(int j=0; j<8; j++)
				{
					dwBuffer <<= 3;
					dwSymbol = pdBufferSymbol[8*i + j];
					dwBuffer |= dwSymbol;
				}

				pnData[3*i + 2] = BYTE((dwBuffer & 0x000000FF));
				pnData[3*i + 1] = BYTE((dwBuffer & 0x0000FF00) >> 8);
				pnData[3*i + 0] = BYTE((dwBuffer & 0x00FF0000) >> 16);
			}

			if(m_pFileRecord != NULL)
				m_pFileRecord->Write(pnData, iSizeByte);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}


		else if(iSizeBit == 4)
		{
			BYTE nData;
			int iSizeByte = iSizeSymbol / 2;
			BYTE *pnData = new BYTE [iSizeByte];
			BYTE nSymbol;

			for (int i=0; i<iSizeByte; i++)
			{
				nData = 0;
				for(int j=0; j<2; j++)
				{
					nData <<= 4;
					nSymbol = pdBufferSymbol[2*i + j];

					//--- Change Format
					//	nSymbol = ChangeSumbolFormat(nSymbol, 2);

					nData |= nSymbol;
				}
				pnData[i] = nData;
			}

			if(m_pFileRecord != NULL)
				m_pFileRecord->Write(pnData, iSizeByte);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}

		
		else if(iSizeBit == 5)
		{
			UINT64 uiBuffer;
			int iSizeByte = iSizeSymbol*iSizeBit / 8;
			BYTE *pnData = new BYTE [iSizeByte];
			UINT64 uiSymbol;

			int iSizeByte_5 = iSizeByte / 5;

			for (int i=0; i<iSizeByte_5; i++)
			{
				uiBuffer = 0;
				for(int j=0; j<8; j++)
				{
					uiBuffer <<= 5;
					uiSymbol = pdBufferSymbol[8*i + j];
					uiBuffer |= uiSymbol;
				}

				pnData[5*i + 4] = BYTE((uiBuffer & 0x0000000000FF) >> 0);
				pnData[5*i + 3] = BYTE((uiBuffer & 0x00000000FF00) >> 8);
				pnData[5*i + 2] = BYTE((uiBuffer & 0x000000FF0000) >> 16);
				pnData[5*i + 1] = BYTE((uiBuffer & 0x0000FF000000) >> 24);
				pnData[5*i + 0] = BYTE((uiBuffer & 0x00FF00000000) >> 32);
			}

			if(m_pFileRecord != NULL)
				m_pFileRecord->Write(pnData, iSizeByte);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}

		
		else if(iSizeBit == 6)
		{
			UINT64 uiBuffer;
			int iSizeByte = iSizeSymbol*iSizeBit / 8;
			BYTE *pnData = new BYTE [iSizeByte];
			UINT64 uiSymbol;

			int iSizeByte_6 = iSizeByte / 6;

			for (int i=0; i<iSizeByte_6; i++)
			{
				uiBuffer = 0;
				for(int j=0; j<8; j++)
				{
					uiBuffer <<= 6;
					uiSymbol = pdBufferSymbol[8*i + j];
					uiBuffer |= uiSymbol;
				}

				pnData[6*i + 5] = BYTE((uiBuffer & 0x0000000000FF) >> 0);
				pnData[6*i + 4] = BYTE((uiBuffer & 0x00000000FF00) >> 8);
				pnData[6*i + 3] = BYTE((uiBuffer & 0x000000FF0000) >> 16);
				pnData[6*i + 2] = BYTE((uiBuffer & 0x0000FF000000) >> 24);
				pnData[6*i + 1] = BYTE((uiBuffer & 0x00FF00000000) >> 32);
				pnData[6*i + 0] = BYTE((uiBuffer & 0xFF0000000000) >> 40);
			}

			if(m_pFileRecord != NULL)
				m_pFileRecord->Write(pnData, iSizeByte);

			SAFERELEASE_ARRAY(pnData);
			return true;
		}
		
	}//func

	void CDemodulatorBase::ChangeOutputIQ(double dI, double dQ, double& dNewI, double& dNewQ)
	{
		double dTempI = dI;
		double dTempQ = dQ;
		double dChange;


		bool	bInverted = *m_pbInverted;
		bool	bSwap = *m_pbSwap;
		int		iPhaseShift = *m_piPhaseShift;
		int		iSizeShift = *m_piSizeShift;


		if(bInverted)
		{
			dTempQ = -dTempQ;
		}

		if(bSwap)
		{
			dChange = dTempI;
			dTempI = dTempQ;
			dTempQ = dChange;
		}

		if(iPhaseShift > 0)
		{
			double dShiftTeta,dShiftI,dShiftQ;
			dShiftTeta = 2.0 * PI * iPhaseShift / iSizeShift;

			dShiftI = cos(dShiftTeta) * dTempI - sin(dShiftTeta) * dTempQ;
			dShiftQ = sin(dShiftTeta) * dTempI + cos(dShiftTeta) * dTempQ;

			dTempI = dShiftI;
			dTempQ = dShiftQ;
		}

		dNewI = dTempI;
		dNewQ = dTempQ;
	}

/*	BYTE CDemodulatorBase::ChangeSumbolFormat(BYTE nSymbol, int iBitNum)
	{
		

		if(iBitNum == 1)
		{
			if(m_pstBitStream->bGrayCode == 1)
			{
				nSymbol = nSymbol & 0x1;
				nSymbol = nSymbol ^ (nSymbol >> 1);
			}
			if(m_pstBitStream->bInverted == 1)
			{
				nSymbol = ~nSymbol;
				nSymbol = nSymbol & 0x1;
			}
			if(m_pstBitStream->bSwap == 1)
			{
				nSymbol = nSymbol & 0x1;
				BYTE RevSymbol = 0;
				for(int i=0;i<iBitNum;i++)
				{
					RevSymbol = RevSymbol<<1;
					RevSymbol |= (nSymbol & 0x1);
					nSymbol = nSymbol>>1;

				}
				nSymbol = RevSymbol;
			}
		}

		if(iBitNum == 2)
		{
			if(m_pstBitStream->bGrayCode == 1)
			{
				nSymbol = nSymbol & 0x3;
				nSymbol = nSymbol ^ (nSymbol >> 1);
			}
			if(m_pstBitStream->bInverted == 1)
			{
				nSymbol = ~nSymbol;
				nSymbol = nSymbol & 0x3;
			}
			if(m_pstBitStream->bSwap == 1)
			{
				nSymbol = nSymbol & 0x3;
				BYTE RevSymbol = 0;
				for(int i=0;i<iBitNum;i++)
				{
					RevSymbol = RevSymbol<<1;
					RevSymbol |= (nSymbol & 0x1);
					nSymbol = nSymbol>>1;

				}
				nSymbol = RevSymbol;
			}
		}
		
		else if(iBitNum == 3)
		{
			if(m_pstBitStream->bGrayCode == 1)
			{
				nSymbol = nSymbol & 0x7;
				nSymbol = nSymbol ^ (nSymbol >> 1);
			}
			if(m_pstBitStream->bInverted == 1)
			{
				nSymbol = ~nSymbol;
				nSymbol = nSymbol & 0x7;
			}
			if(m_pstBitStream->bSwap == 1)
			{
				nSymbol = nSymbol & 0x7;
				BYTE RevSymbol = 0;
				for(int i=0;i<iBitNum;i++)
				{
					RevSymbol = RevSymbol<<1;
					RevSymbol |= (nSymbol & 0x1);
					nSymbol = nSymbol>>1;

				}
				nSymbol = RevSymbol;
			}

		}

		else if(iBitNum == 4)
		{
			if(m_pstBitStream->bGrayCode == 1)
			{
				nSymbol = nSymbol & 0xf;
				nSymbol = nSymbol ^ (nSymbol >> 1);
			}
			if(m_pstBitStream->bInverted == 1)
			{
				nSymbol = ~nSymbol;
				nSymbol = nSymbol & 0xf;
			}
			if(m_pstBitStream->bSwap == 1)
			{
				nSymbol = nSymbol & 0xf;
				BYTE RevSymbol = 0;
				for(int i=0;i<iBitNum;i++)
				{
					RevSymbol = RevSymbol<<1;
					RevSymbol |= (nSymbol & 0x1);
					nSymbol = nSymbol>>1;

				}
				nSymbol = RevSymbol;
			}

		}

		else if(iBitNum == 5)
		{
			if(m_pstBitStream->bGrayCode == 1)
			{
				nSymbol = nSymbol & 0x1f;
				nSymbol = nSymbol ^ (nSymbol >> 1);
			}
			if(m_pstBitStream->bInverted == 1)
			{
				nSymbol = ~nSymbol;
				nSymbol = nSymbol & 0x1f;
			}
			if(m_pstBitStream->bSwap == 1)
			{
				nSymbol = nSymbol & 0x1f;
				BYTE RevSymbol = 0;
				for(int i=0;i<iBitNum;i++)
				{
					RevSymbol = RevSymbol<<1;
					RevSymbol |= (nSymbol & 0x1);
					nSymbol = nSymbol>>1;

				}
				nSymbol = RevSymbol;
			}

		}

		else if(iBitNum == 6)
		{
			if(m_pstBitStream->bGrayCode == 1)
			{
				nSymbol = nSymbol & 0x3f;
				nSymbol = nSymbol ^ (nSymbol >> 1);
			}
			if(m_pstBitStream->bInverted == 1)
			{
				nSymbol = ~nSymbol;
				nSymbol = nSymbol & 0x3f;
			}
			if(m_pstBitStream->bSwap == 1)
			{
				nSymbol = nSymbol & 0x3f;
				BYTE RevSymbol = 0;
				for(int i=0;i<iBitNum;i++)
				{
					RevSymbol = RevSymbol<<1;
					RevSymbol |= (nSymbol & 0x1);
					nSymbol = nSymbol>>1;

				}
				nSymbol = RevSymbol;
			}

		}
		return nSymbol;
	}
	*/

}//namespace
