#include "StdAfx.h"
#include "DemodulatorBase.h"

#include "SSBDemodulator.h"
//HF Project Demodulator
namespace NS_DSP
{
	CString	CDemodulatorBase::m_strRecordRootPath = "c:\\HF_Audios";

	CDemodulatorBase::CDemodulatorBase(void)
	{
		nID						=	-1;
		//calculate max
		nCalcMSBCounter			=	0;
		nCalcMSBNumberSamples	=	10000;
		nMSBValue				=	0;
		nMSBPosition			=	0;
		nType					=	eModulationUnknown;
		bFixed					=	TRUE;
	}

	CDemodulatorBase::~CDemodulatorBase(void)
	{
		m_Record.Close();
	}

	BOOL CDemodulatorBase::Create(int m_nID)
	{
		nID = m_nID;
		return TRUE;
	}

	void CDemodulatorBase::Reset()
	{
		ResetCalcMSB();
	}

	//Re-Calculate MSB bit from CIC output
	void CDemodulatorBase::ResetCalcMSB()
	{
		nCalcMSBCounter			=	0;
		nMSBValue				=	0;
		nMSBPosition			=	0;
	}

	BOOL CDemodulatorBase::CreateRecordRootDirectory()
	{
		CString strRoot = GetRecordRootPath();
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

	CString CDemodulatorBase::CreateRecordFileName()
	{
		COleDateTime m_Now = COleDateTime::GetCurrentTime();
		CString strNow = m_Now.Format("%Y_%m_%d_%H_%M_%S");
		CString strPath = GetRecordRootPath();
		strPath.Format ("%s\\%s\\%s_%d.wav",GetRecordRootPath(),
			CGlobalTypes::GetEnumModulationStr(nType),strNow,nID);
		return strPath;
	}//func

}//namespace