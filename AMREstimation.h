#pragma once
#include "ipp.h"
//#include "mkl.h"
#include <QThread>
#include <QString>
#include <QWaitCondition>
#include <QMutex>
#include "QTimer"
#include "QFile"
namespace NS_FRAME_BASED
{
class CAMREstimation
{
public:
	CAMREstimation(void);
	~CAMREstimation(void);

    int Modulation,L;
	double M21,Cum21,Cum20,absCum20,absCum40,Cum42,absCum80,absCum62,absCum84,absCum63,absCum60,absCum61,absCum41,absCum81,absCum82,absCum83,absCum42;
	Ipp64fc *Com_Env1,*Temp,*Temp2;
	Ipp64fc Cum40,Cum80,Cum62,Cum84,Cum63,Cum60,Cum61,Cum41,Cum81,Cum82,Cum83;
	double *agc_i;
	double *agc_q;
	double *phasor_2,*Mag_C,std_Mag;
	Ipp64fc *C20_2,*C_2,*C40_2,*C80_2
				,*C62_2,*C41_2,*CMr_2,*C63_2
					,*C60_2,*C61_2,*C83_2,*C82_2,*C81_2;
	Ipp64fc smc20,smc40,smph,sm2ph,smc80,smc62,smc41
				,smcMr,smc63,smc60,
					smc61,smc81,smc82,smc83;

	double *C21_2,*C42_2,*C84_2,*Demod_counters,Max_counter,std_I,std_Q,var_sig,normalize_factor;
	Ipp64f smc21,smc84,smc42;
	
	int BPSK_Counter ,QPSK_Counter ,PSK8_Counter ,QAM16_Counter ,QAM32_Counter ,QAM64_Counter ,QAM128_Counter ,APSK16_Counter 
	,APSK32_Counter ,APSK64_Counter ,ASK4_Counter ,ASK2_Counter,Max_counter_Indx; 

	void initialize(int iSizeOutCIC);
	void Cumulants_caculation(double *I_consellation,double *Q_consellation);
	void SavtToFile_double(double *dbDataInp2,int nLenOut);
	void AMR(double SNR);
	
	
	int Count_Cum;
	int Amr_Out;
	int *Amr_outt;
	int Amr_outt_FH;

	
};

}
