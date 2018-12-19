#pragma once

#include "configuration.h"
#include "mode_ff.h"
#include "mode_dsss.h"
#include "mode_ofdm.h"
#include "mode_sa.h"
#include "mode_sm.h"

//--- OSA Main Modes

class CModeControl
{
public:
	CModeControl(void);
	~CModeControl(void);

public:
    bool Initialize(Configuration *pConfig);
	void Close(void);

	bool ChangeMode(OSA_MAIN_MODES enMode);
	OSA_MAIN_MODES GetLastMode(void){return m_enLastMode;}

public:
	OSA_MAIN_MODES m_enLastMode;

//	CModeSpectrumAnalyse			m_modeSA;
    Mode_FF				*m_mode_ff;
//	CModeFrequencyHopping			m_modeFH;
//	CModeDirectSequence				m_modeDS;
//	CModeOFDM						m_modeOFDM;
//	CModeSignalManipulation			m_modeSM;
//	CModeStatisticalSignalAnalysis	m_modeSSA;

    QString m_strAddressMain;
};
