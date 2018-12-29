#pragma once
#include "DemodulatorBase.h"

using namespace NS_DSP;
namespace NS_FRAME_BASED
{
class CSRC
{
public:
	CSRC(void);
	~CSRC(void);
	bool Create(CString strMainAddr);
	bool LoadFilterCoef(CString strMainAddr);
	void ReleaseBuffers(void);
	void FilterToSRC(double *pOutLPF_I,double *pOutLPF_Q,int SizeofOutLPF,double R);
	void SRCnew(double T_ErrorInput,int iPreCounterInput,double pre_mu_inInput,double dMaxcountervalueINInput,double &EnableOutput,int &iCounterOutput,double &muOutput,double &dMaxcountervalueOutput);

//	double *m_pdCoefFilterLPF;
//	int m_nSizeCoefFilterLPF;
	double *m_pdZf_I;
	double *m_pdZf_Q;
	double  m_dLPF_Filt_Out_I;
	double	m_dLPF_Filt_Out_Q;
	double m_dI_SRCin;
	double m_dQ_SRCin;
	double m_dI_SRCinHistory[4];
	double m_dQ_SRCinHistory[4];
	int m_iCounter;
	int m_iPreCounter;
	double m_dMaxcountervalue;
	double m_dMaxcountervalueIN;
	double m_dR;
	double m_dSRCEnable;
	double m_dI_SRCout;
	double m_dQ_SRCout;
	double **m_pMatrixI;
	double m_pdX1[2];
	double m_pdX2[2];
	double m_pdX3[2];
	double m_pdOut[2];
	int m_iSPS_Counter;
	int m_nSizeCoefMatchFilter;
	double  m_dMatch_Filt_Out_I;
	double	m_dMatch_Filt_Out_Q;
	double *m_pdCoefMatchFilter;
	double mu;
	double m_dgain;
	double *m_pMatchfilterOutI;
	double *m_pMatchfilterOutQ;
	int m_iSRCBufferCounter;

};
}
