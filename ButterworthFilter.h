#pragma once
#include "iir.h"
#include "NewParksMcClellan.h"

//enum TPassType {LPF, HPF, BPF, NOTCH };

class CButterworthFilter
{
public:
	CButterworthFilter(void);
	~CButterworthFilter(void);

public:
	bool DesignFilter(TPassType PassType, double dFs, double dFc, double dBW, int nOrder);
	bool Filter(double *pdInput, int iSize, double *pdOutput);
	void ResetFilter(void);

public:
	int		m_nSize;
	double	m_pdCoefA[256];
	double	m_pdCoefB[256];

	double	m_pdHisX[256];
	double	m_pdHisY[256];
};
