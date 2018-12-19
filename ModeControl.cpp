#include "ModeControl.h"

CModeControl::CModeControl(void)
{
    m_enLastMode = (OSA_MAIN_MODES)(-1);
}

CModeControl::~CModeControl(void)
{
}

bool CModeControl::Initialize(Configuration *pConfig)
{
	bool bResult = false;
    m_mode_ff=new Mode_FF();

//	bResult = m_modeSA.Initialize(pConfig);
  bResult = m_mode_ff->Initialize(pConfig);
//	bResult = m_modeFH.Initialize(pConfig);
//	bResult = m_modeDS.Initialize(pConfig);
//	bResult = m_modeOFDM.Initialize(pConfig);
//	bResult = m_modeSM.Initialize(pConfig);
//	bResult = m_modeSSA.Initialize(pConfig);


//	ChangeMode(OMM_SA);

	return true;
}

void CModeControl::Close(void)
{
	ChangeMode((OSA_MAIN_MODES)(-1));

//	m_modeSA.Close();
//	m_modeFF.Close();
//	m_modeFH.Close();
//	m_modeDS.Close();
//	m_modeOFDM.Close();
//	m_modeSM.Close();
//	m_modeSSA.Close();
}

bool CModeControl::ChangeMode(OSA_MAIN_MODES enMode)
{
	//------------------------------------------------------------------
	//--- Stop Last Mode -----------------------------------------------
	//------------------------------------------------------------------
    /*if(m_enLastMode == OMM_SA)
		m_modeSA.CloseMode();
    else*/// if(m_enLastMode == OMM_FF)
//		m_modeFF.CloseMode();
//	else if(m_enLastMode == OMM_FH)
//		m_modeFH.CloseMode();
//	else if(m_enLastMode == OMM_DS)
//		m_modeDS.CloseMode();
//	else if(m_enLastMode == OMM_OFDM)
//		m_modeOFDM.CloseMode();
//	else if(m_enLastMode == OMM_SM)
//		m_modeSM.CloseMode();

	//------------------------------------------------------------------
	//--- Start New Mode -----------------------------------------------
	//------------------------------------------------------------------
	bool bResult = true;
    /*if(enMode == OMM_SA)
		bResult = m_modeSA.RunMode();
    else*/// if(enMode == OMM_FF)
    //	bResult = m_modeFF.RunMode();
//	else if(enMode == OMM_FH)
//		bResult = m_modeFH.RunMode();
//	else if(enMode == OMM_DS)
//		bResult = m_modeDS.RunMode();
//	else if(enMode == OMM_OFDM)
//		bResult = m_modeOFDM.RunMode();
//	else if(enMode == OMM_SM)
//		bResult = m_modeSM.RunMode();


	if(bResult)
		m_enLastMode = enMode;
	else
        qDebug("Error in selected mode");

	return bResult;
}
