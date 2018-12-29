#include "../StdAfx.h"
#include "DspBase.h"

namespace NS_DSP
{


	CDspBase::CDspBase(void)
	{
		m_nLastError =0;
	}

	CDspBase::~CDspBase(void)
	{
	}

	int CDspBase::GetLastError()
	{
		int nRet = m_nLastError;
		m_nLastError=0;
		return nRet;
	}

	LPCTSTR CDspBase::GetLastErrorString(int nError)
	{
		if (nError) return _T("DSP Library Error");
		else return _T("");
	}


}//namespace