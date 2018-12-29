#pragma once
//#ifndef _DSPLIB_BSP_BASE
//#define _DSPLIB_BSP_BASE

#include "dsplib_general_func.h"

namespace NS_DSP
{

	class CDspBase
	{
	public:
		CDspBase(void);
		~CDspBase(void);
		virtual int GetLastError();
		virtual LPCTSTR GetLastErrorString(int nError);

	public:
		int		m_nLastError;
	};

}//namespace
//#endif  //_DSPLIB_BSP_BASE
