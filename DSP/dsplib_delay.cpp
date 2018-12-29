#include "stdafx.h"
#include "dsplib_delay.h"

namespace NS_DSP
{
///////////////////////////////////////////////////////////////////////
	//Double Precision
//-----------------------------------------
CDelayLined::CDelayLined()
{
	m_nDelay = 0;
	m_nPos=0;//read position / write pos = (read pos - 1) % m_nDelay
	m_fBuffer = NULL;
}

CDelayLined::~CDelayLined()
{
	if(m_fBuffer ) delete[] m_fBuffer;
	m_nDelay = 0;
	m_fBuffer = NULL;
}

BOOL CDelayLined::Create(int nDelay)
{
	m_fBuffer = new double[nDelay];
	if(!m_fBuffer) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	m_nDelay = nDelay;
	Reset();
	return DSP_RET_SUCCESS;
}

void CDelayLined::Reset()
{
	if(m_fBuffer ) memset(m_fBuffer,0,m_nDelay * sizeof(double));
	m_nPos=0;
}

double CDelayLined::Delay(double m_fInput)
{
	double ret = m_fBuffer[m_nPos];
	if(m_nPos==0) m_fBuffer[m_nDelay-1] = m_fInput;
	else m_fBuffer[m_nPos-1] = m_fInput;
	m_nPos = (m_nPos+1)%m_nDelay;
	return ret;
}

///////////////////////////////////////////////////////////////////////
//Single Precision
//-----------------------------------------
CDelayLines::CDelayLines()
{
	m_nDelay = 0;
	m_nPos=0;//read position / write pos = (read pos - 1) % m_nDelay
	m_fBuffer = NULL;
}

CDelayLines::~CDelayLines()
{
	if(m_fBuffer ) delete[] m_fBuffer;
	m_nDelay = 0;
	m_fBuffer = NULL;
}

BOOL CDelayLines::Create(int nDelay)
{
	m_fBuffer = new float[nDelay];
	if(!m_fBuffer) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	m_nDelay = nDelay;
	Reset();
	return DSP_RET_SUCCESS;
}

void CDelayLines::Reset()
{
	if(m_fBuffer ) memset(m_fBuffer,0,m_nDelay * sizeof(double));
	m_nPos=0;
}

float CDelayLines::Delay(float m_fInput)
{
	float ret = m_fBuffer[m_nPos];
	if(m_nPos==0) m_fBuffer[m_nDelay-1] = m_fInput;
	else m_fBuffer[m_nPos-1] = m_fInput;
	m_nPos = (m_nPos+1)%m_nDelay;
	return ret;
}

}//namespace