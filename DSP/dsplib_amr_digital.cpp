#include "stdafx.h"
#include "dsplib_amr_digital.h"
#include "dsplib_debug.h"

namespace NS_DSP
{


	CDigitalAMRvmls::CDigitalAMRvmls()
	{
		Com_Env1=NULL;
		phasor_2=NULL;
		Phase_1 =NULL;
		C20_2=NULL;
		C21_2=NULL;
		C_2=NULL;
		C40_2=NULL;
		C42_2=NULL;
		R=NULL;
		I=NULL;
		C20_1=NULL;
		C21_1=NULL;
		C40_1=NULL;
		C42_1=NULL;
		Reset();

	}

	CDigitalAMRvmls::~CDigitalAMRvmls()
	{
		Release();
	}

	void CDigitalAMRvmls::Release()
	{

		if(Com_Env1) delete[] Com_Env1;	Com_Env1=NULL;
		if(phasor_2) delete[] phasor_2;phasor_2=NULL;
		if(Phase_1) delete[] Phase_1;Phase_1 =NULL;
		if(C20_2) delete[] C20_2;C20_2=NULL;
		if(C21_2) delete[] C21_2;C21_2=NULL;
		if(C_2) delete[] C_2;C_2=NULL;
		if(C40_2) delete[] C40_2;C40_2=NULL;
		if(C42_2) delete[] C42_2;C42_2=NULL;
		if(R) delete[] R;R=NULL;
		if(I) delete[] I;I=NULL;
		if(C20_1) delete[] C20_1;C20_1=NULL;
		if(C21_1) delete[] C21_1;C21_1=NULL;
		if(C40_1) delete[] C40_1;C40_1=NULL;
		if(C42_1) delete[] C42_1;C42_1=NULL;
	}

	void CDigitalAMRvmls::Reset()
	{

	}

	BOOL CDigitalAMRvmls::Create(BOOL bFSK,int nSize)
	{
		//convert input samples
		Com_Env1=new MKL_Complex8[nSize];
		if(!Com_Env1){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		phasor_2 =new float[nSize];
		if(!phasor_2){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		Phase_1 =new float[nSize];
		if(!Phase_1){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C20_2=new MKL_Complex8[nSize];
		if(!C20_2){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C21_2=new MKL_Complex8[nSize];
		if(!C21_2){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C_2=new MKL_Complex8[nSize];
		if(!C_2){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C42_2=new MKL_Complex8[nSize];
		if(!C42_2){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		R = new float[nSize];
		if(!R){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}	
		I = new float[nSize];
		if(!I){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C20_1=new MKL_Complex8[nSize];
		if(!C20_1){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C40_2=new MKL_Complex8[nSize];
		if(!C40_2){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C21_1=new MKL_Complex8[nSize];
		if(!C21_1){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C40_1=new MKL_Complex8[nSize];
		if(!C40_1){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		C42_1=new MKL_Complex8[nSize];
		if(!C42_1){
			Release();
			return DSP_RET_MEMORY_ALLOCATE_ERROR;
		}
		return DSP_RET_SUCCESS;
	}

	BOOL CDigitalAMRvmls::DoAMR(int *m_dwSamplesI,int *m_dwSamplesQ,int nSize,float m_fSNR,int &nModType)
	{
		int L = nSize;
		float  SNR = m_fSNR , smabs = 0 ;
		MKL_Complex8 sm;sm.real=0;sm.imag=0;
		float	sm2R = 0 , sm2I = 0 , sm2abs = 0 , count = 0;


		for(int i=0;i<nSize;i++)
		{
			Com_Env1[i].real = (float)m_dwSamplesI[i];
			Com_Env1[i].imag = (float)m_dwSamplesQ[i];
		}

		//allocate memory

		//phasor_2.clear();
		memset(phasor_2,0,nSize*sizeof(float));

		//fvec Phase_1 ;
		//Phase_1.clear();

		memset(Phase_1,0,nSize*sizeof(float));
		
		//cvec C20_2;
		//C20_2.setlength(L,1);
		//C20_2.clear();

		memset(C20_2,0,nSize*sizeof(MKL_Complex8));

		//fvec C21_2;
		//C21_2.setlength(L,1);
		//C21_2.clear();

		memset(C21_2,0,nSize*sizeof(MKL_Complex8));

		//cvec C_2 ;
		//C_2.setlength(L);
		//C_2.clear();

		memset(C_2,0,nSize*sizeof(MKL_Complex8));

		//cvec C40_2;
		//C40_2.setlength(L,1);
		//C40_2.clear();

		memset(C40_2,0,nSize*sizeof(MKL_Complex8));

		//fvec C42_2;
		//C42_2.setlength(L,1);
		//C42_2.clear();

		memset(C42_2,0,nSize*sizeof(MKL_Complex8));

		MKL_Complex8 smc20, smc40 ;
		smc20.real =0; smc20.imag=0;
		smc40.real =0; smc40.imag=0;

		float smc21 = 0 ,  smc42 = 0 , smph = 0 , sm2ph = 0;


		// AMR main algorithm
		for(int k=0 ;k<nSize;k++)
		{
			MKL_Complex8 d;
			d = Com_Env1[k];
			smabs = smabs + vmlsABS(Com_Env1[k]);
			sm2abs = sm2abs + pow(vmlsABS(Com_Env1[k]),2);
			//sm += Com_Env1[k];			
			sm.real += Com_Env1[k].real;
			sm.imag += Com_Env1[k].imag;
			sm2R = sm2R + pow(Com_Env1[k].real,2);
			sm2I = sm2I + pow(Com_Env1[k].imag,2);
		}

		//fvec R;
		//R.setlength(s);


		//fvec I;
		//long s = L;
		//I.setlength(s);

		for(int i=0;i<nSize;i++)
		{
			R[i] = Com_Env1[i].real;
			I[i] = Com_Env1[i].imag;
		}


		float m1=smabs/nSize;

		for(int i=1;i<=nSize;i++)
		{
			if (vmlsABS(Com_Env1[nSize-i]) < (0.5 * m1))
				count = count + 1;
		}

		count /= nSize;

	//	phasor_2.setlength(L);
	//	C21_2.setlength(L);
	//	C_2.setlength(L);
	//	C20_2.setlength(L);
	//	C40_2.setlength(L);
	//	C42_2.setlength(L);

		for (int k=1;k<nSize;k++)
		{
			//phasor_2[k]=angle(Com_Env1[k])-angle(Com_Env1[k-1]);
			phasor_2[k]=atan2(Com_Env1[k].imag,Com_Env1[k].real)-atan2(Com_Env1[k-1].imag,Com_Env1[k-1].real);
			C21_2[k].real=pow(vmlsABS(Com_Env1[k]),2);
			C21_2[k].imag=0;
			//C_2[k]=Com_Env1[k]*conj(Com_Env1[k-1]);
			C_2[k] = vmlsMul(Com_Env1[k],vmlsConj(Com_Env1[k-1]));
			C20_2[k]=vmlsMul(C_2[k],C_2[k]);//C_2[k] * C_2[k];
			//C40_2[k] = C_2[k] * C_2[k] * C_2[k] * C_2[k];
			C40_2[k] = vmlsMul(C_2[k],C_2[k]);
			C40_2[k] = vmlsMul(C40_2[k],C40_2[k]);
			C42_2[k].real=pow((float)vmlsABS(Com_Env1[k]),4);
			C42_2[k].imag=0;
		}

	//	cvec C20_1;
	//	C20_1.setlength(L,1);
	//	C20_1.clear();
		memset(C20_1,0,nSize*sizeof(MKL_Complex8));

		//fvec C21_1;
		//C21_1.setlength(L,1);
		//C21_1.clear();

		memset(C21_1,0,nSize*sizeof(MKL_Complex8));

		//cvec C40_1;
		//C40_1.setlength(L,1);
		//C40_1.clear();

		memset(C40_1,0,nSize*sizeof(MKL_Complex8));

		//fvec C42_1;
		//C42_1.setlength(L,1);
		//C42_1.clear();

		memset(C42_1,0,nSize*sizeof(MKL_Complex8));

		//Phase_1.setlength(L);
		//Phase_1.clear();
		memset(Phase_1,0,nSize*sizeof(float));

		/*	for(int i=0 ; i<L ; i++)
		{
		C20_1[i] = C20_2[i];
		C21_1[i] = C21_2[i];
		C40_1[i] = C40_2[i];
		C42_1[i] = C42_2[i];
		Phase_1[i] = phasor_2[i];
		}*/


		for(int i=1 ; i<nSize ; i++)
		{	
			//smc21 += C21_2[i];//C21_1[i];
			//smc20 += C20_2[i];//C20_1[i];
			//smc40 += C40_2[i];//C40_1[i];
			//smc42 += C42_2[i];//C42_1[i];
			smc21 += C21_2[i].real;//C21_1[i];
			smc20.real += C20_2[i].real;//C20_1[i];
			smc20.imag += C20_2[i].imag;//C20_1[i];
			smc40.real += C40_2[i].real;//C40_1[i];
			smc40.imag += C40_2[i].imag;//C40_1[i];
			smc42 += C42_2[i].real;//C42_1[i];
			smph  += phasor_2[i];//Phase_1[i];
			sm2ph += pow((float)phasor_2[i]/*Phase_1[i]*/,2);
		}

		double M21 = 1 + pow(10,-(0.1 * SNR));
		float Cum21 = smc21/nSize;
		MKL_Complex8 Cum20a;
		//Cum20a = smc20;
		//Cum20a.real *= (pow((double)M21,2)/L)/(Cum21*Cum21)); 
		//Cum20a.imag *= (pow((double)M21,2)/L)/(Cum21*Cum21)); 
		Cum20a.real = pow((float)M21,2)*(smc20.real/L)/(Cum21*Cum21); 
		Cum20a.imag = pow((float)M21,2)*(smc20.imag/L)/(Cum21*Cum21); 
		float Cum20 = vmlsABS(Cum20a);
		MKL_Complex8 Cum40;
		Cum40 = smc40;
		Cum40.real = pow((float)M21,4)  *  (smc40.real /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		Cum40.imag = pow((float)M21,4)  *  (smc40.imag /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		//Cum40 = pow((double)M21,4)  *  (smc40 /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		double absCum40 = vmlsABS(Cum40);
		float Cum42 = pow((float)M21,2) * (smc42/L) / (Cum21*Cum21) - (abs(Cum20)*abs(Cum20)) - 2*pow((float)M21,2);
		//float PhasVar_tst = variance(Phase_1);
		//complex PhasVar=(sm2ph-(smph * smph)/L)/(L-1);

		nModType = AMR_DIGITAL_UNKNOWN;
		//%% Decision
		if ((Cum20 > 0.5) &&  (count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum42 < -1.68) && (-2.4 < Cum42))
			nModType = AMR_DIGITAL_BPSK;

		else if ((Cum20 > 0.5) &&  (count > 0.05) && (vmlsABS(Cum40) > 0.1) && ( Cum42 < -1.2) && (-1.68 < Cum42))
			nModType = AMR_DIGITAL_4_ASK;

		else if ((vmlsABS(Cum40) < 0.1) && (count < 0.05) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_8PSK;

		else if ((Cum40.real < -0.5) && (count < 0.05) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_DQPSK;

		//else if ((Cum40.real < -0.5) && (count < 0.05) && (Cum20 < 0.1))
		//	nModType = AMR_DIGITAL_DQPSK;

		else if ((vmlsABS(Cum40) < 0.27) && (count < 0.1) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_OQPSK;

		else if ((count > 0.05) && (vmlsABS(Cum40) < 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_QAM32;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.32) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.73) && (-0.649 > Cum42))
			nModType = AMR_DIGITAL_QAM16;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.25) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.649) && (-0.611 > Cum42))
			nModType = AMR_DIGITAL_QAM64;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.2) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.611) && (-0.59 > Cum42))
			nModType = AMR_DIGITAL_QAM256;

		//else if ((count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
		//	nModType = AMR_DIGITAL_QPSK;

		else if ((count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_QPSK;


		else
			nModType = AMR_DIGITAL_UNKNOWN;

		return DSP_RET_SUCCESS;
	}

	BOOL CDigitalAMRvmls::DoAMR(double *m_fSamplesI,double *m_fSamplesQ,int nSize,float m_fSNR,int &nModType)
	{
		int L = nSize;
		float  SNR = m_fSNR , smabs = 0 ;
		MKL_Complex8 sm;sm.real=0;sm.imag=0;
		float	sm2R = 0 , sm2I = 0 , sm2abs = 0 , count = 0;


		for(int i=0;i<nSize;i++)
		{
			Com_Env1[i].real = float(m_fSamplesI[i]);
			Com_Env1[i].imag = float(m_fSamplesQ[i]);
		}

		//allocate memory

		//phasor_2.clear();
		memset(phasor_2,0,nSize*sizeof(float));

		//fvec Phase_1 ;
		//Phase_1.clear();

		memset(Phase_1,0,nSize*sizeof(float));

		//cvec C20_2;
		//C20_2.setlength(L,1);
		//C20_2.clear();

		memset(C20_2,0,nSize*sizeof(MKL_Complex8));

		//fvec C21_2;
		//C21_2.setlength(L,1);
		//C21_2.clear();

		memset(C21_2,0,nSize*sizeof(MKL_Complex8));

		//cvec C_2 ;
		//C_2.setlength(L);
		//C_2.clear();

		memset(C_2,0,nSize*sizeof(MKL_Complex8));

		//cvec C40_2;
		//C40_2.setlength(L,1);
		//C40_2.clear();

		memset(C40_2,0,nSize*sizeof(MKL_Complex8));

		//fvec C42_2;
		//C42_2.setlength(L,1);
		//C42_2.clear();

		memset(C42_2,0,nSize*sizeof(MKL_Complex8));

		MKL_Complex8 smc20, smc40 ;
		smc20.real =0; smc20.imag=0;
		smc40.real =0; smc40.imag=0;

		float smc21 = 0 ,  smc42 = 0 , smph = 0 , sm2ph = 0;


		// AMR main algorithm
		for(int k=0 ;k<nSize;k++)
		{
			MKL_Complex8 d;
			d = Com_Env1[k];
			smabs = smabs + vmlsABS(Com_Env1[k]);
			sm2abs = sm2abs + pow(vmlsABS(Com_Env1[k]),2);
			//sm += Com_Env1[k];			
			sm.real += Com_Env1[k].real;
			sm.imag += Com_Env1[k].imag;
			sm2R = sm2R + pow(Com_Env1[k].real,2);
			sm2I = sm2I + pow(Com_Env1[k].imag,2);
		}

		//fvec R;
		//R.setlength(s);


		//fvec I;
		//long s = L;
		//I.setlength(s);

		for(int i=0;i<nSize;i++)
		{
			R[i] = Com_Env1[i].real;
			I[i] = Com_Env1[i].imag;
		}


		float m1=smabs/nSize;

		for(int i=1;i<=nSize;i++)
		{
			if (vmlsABS(Com_Env1[nSize-i]) < (0.5 * m1))
				count = count + 1;
		}

		count /= nSize;

		//	phasor_2.setlength(L);
		//	C21_2.setlength(L);
		//	C_2.setlength(L);
		//	C20_2.setlength(L);
		//	C40_2.setlength(L);
		//	C42_2.setlength(L);

		for (int k=1;k<nSize;k++)
		{
			//phasor_2[k]=angle(Com_Env1[k])-angle(Com_Env1[k-1]);
			phasor_2[k]=atan2(Com_Env1[k].imag,Com_Env1[k].real)-atan2(Com_Env1[k-1].imag,Com_Env1[k-1].real);
			C21_2[k].real=pow(vmlsABS(Com_Env1[k]),2);
			C21_2[k].imag=0;
			//C_2[k]=Com_Env1[k]*conj(Com_Env1[k-1]);
			C_2[k] = vmlsMul(Com_Env1[k],vmlsConj(Com_Env1[k-1]));
			C20_2[k]=vmlsMul(C_2[k],C_2[k]);//C_2[k] * C_2[k];
			//C40_2[k] = C_2[k] * C_2[k] * C_2[k] * C_2[k];
			C40_2[k] = vmlsMul(C_2[k],C_2[k]);
			C40_2[k] = vmlsMul(C40_2[k],C40_2[k]);
			C42_2[k].real=pow((float)vmlsABS(Com_Env1[k]),4);
			C42_2[k].imag=0;
		}

		//	cvec C20_1;
		//	C20_1.setlength(L,1);
		//	C20_1.clear();
		memset(C20_1,0,nSize*sizeof(MKL_Complex8));

		//fvec C21_1;
		//C21_1.setlength(L,1);
		//C21_1.clear();

		memset(C21_1,0,nSize*sizeof(MKL_Complex8));

		//cvec C40_1;
		//C40_1.setlength(L,1);
		//C40_1.clear();

		memset(C40_1,0,nSize*sizeof(MKL_Complex8));

		//fvec C42_1;
		//C42_1.setlength(L,1);
		//C42_1.clear();

		memset(C42_1,0,nSize*sizeof(MKL_Complex8));

		//Phase_1.setlength(L);
		//Phase_1.clear();
		memset(Phase_1,0,nSize*sizeof(float));

		/*	for(int i=0 ; i<L ; i++)
		{
		C20_1[i] = C20_2[i];
		C21_1[i] = C21_2[i];
		C40_1[i] = C40_2[i];
		C42_1[i] = C42_2[i];
		Phase_1[i] = phasor_2[i];
		}*/

		for(int i=1 ; i<nSize ; i++)
		{	
			//smc21 += C21_2[i];//C21_1[i];
			//smc20 += C20_2[i];//C20_1[i];
			//smc40 += C40_2[i];//C40_1[i];
			//smc42 += C42_2[i];//C42_1[i];
			smc21 += C21_2[i].real;//C21_1[i];
			smc20.real += C20_2[i].real;//C20_1[i];
			smc20.imag += C20_2[i].imag;//C20_1[i];
			smc40.real += C40_2[i].real;//C40_1[i];
			smc40.imag += C40_2[i].imag;//C40_1[i];
			smc42 += C42_2[i].real;//C42_1[i];
			smph  += phasor_2[i];//Phase_1[i];
			sm2ph += pow((float)phasor_2[i]/*Phase_1[i]*/,2);
		}

		double M21 = 1 + pow(10,-(0.1 * SNR));
		float Cum21 = smc21/nSize;
		MKL_Complex8 Cum20a;
		//Cum20a = smc20;
		//Cum20a.real *= (pow((double)M21,2)/L)/(Cum21*Cum21)); 
		//Cum20a.imag *= (pow((double)M21,2)/L)/(Cum21*Cum21)); 
		Cum20a.real = pow((float)M21,2)*(smc20.real/L)/(Cum21*Cum21); 
		Cum20a.imag = pow((float)M21,2)*(smc20.imag/L)/(Cum21*Cum21); 
		float Cum20 = vmlsABS(Cum20a);
		MKL_Complex8 Cum40;
		Cum40 = smc40;
		Cum40.real = pow((float)M21,4)  *  (smc40.real /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		Cum40.imag = pow((float)M21,4)  *  (smc40.imag /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		//Cum40 = pow((double)M21,4)  *  (smc40 /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		double absCum40 = vmlsABS(Cum40);
		float Cum42 = pow((float)M21,2) * (smc42/L) / (Cum21*Cum21) - (abs(Cum20)*abs(Cum20)) - 2*pow((float)M21,2);
		//float PhasVar_tst = variance(Phase_1);
		//complex PhasVar=(sm2ph-(smph * smph)/L)/(L-1);

		nModType = AMR_DIGITAL_UNKNOWN;
		//%% Decision
		if ((Cum20 > 0.5) &&  (count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum42 < -1.68) && (-2.4 < Cum42))
			nModType = AMR_DIGITAL_BPSK;

		else if ((Cum20 > 0.5) &&  (count > 0.05) && (vmlsABS(Cum40) > 0.1) && ( Cum42 < -1.2) && (-1.68 < Cum42))
			nModType = AMR_DIGITAL_4_ASK;

		else if ((vmlsABS(Cum40) < 0.1) && (count < 0.05) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_8PSK;

		else if ((Cum40.real < -0.5) && (count < 0.05) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_DQPSK;

		else if ((vmlsABS(Cum40) < 0.27) && (count < 0.1) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_OQPSK;

		else if ((count > 0.05) && (vmlsABS(Cum40) < 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_QAM32;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.32) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.73) && (-0.649 > Cum42))
			nModType = AMR_DIGITAL_QAM16;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.25) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.649) && (-0.611 > Cum42))
			nModType = AMR_DIGITAL_QAM64;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.2) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.611) && (-0.59 > Cum42))
			nModType = AMR_DIGITAL_QAM256;

		else if ((count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_QPSK;

		else
			nModType = AMR_DIGITAL_UNKNOWN;

		return DSP_RET_SUCCESS;
	}
	
	BOOL CDigitalAMRvmls::DoAMR(MKL_Complex16 *m_fSamples,int nSize,float m_fSNR,int &nModType)
	{

		int L = nSize;
		float  SNR = m_fSNR , smabs = 0 ;
		MKL_Complex8 sm;sm.real=0;sm.imag=0;
		float	sm2R = 0 , sm2I = 0 , sm2abs = 0 , count = 0;

		int maxR = -100000;
		int maxI = -100000;
		int maxindex=0;
		for(int i=0;i<nSize;i++)
		{
			if(maxR < abs(m_fSamples[i].real)) 
			{
				maxR = abs(m_fSamples[i].real);
				maxindex=i;

			}
			if(maxI < abs(m_fSamples[i].imag)) maxI = abs(m_fSamples[i].imag);
		}

		//float mmax = maxR > maxI ? maxR : maxI;
		float mmax = maxR;
		for(int i=0;i<nSize;i++)
		{
			Com_Env1[i].real = float(m_fSamples[i].real) / mmax;
			Com_Env1[i].imag = float(m_fSamples[i].imag) / mmax;
		}
		//allocate memory

		//phasor_2.clear();
		memset(phasor_2,0,nSize*sizeof(float));

		//fvec Phase_1 ;
		//Phase_1.clear();

		memset(Phase_1,0,nSize*sizeof(float));

		//cvec C20_2;
		//C20_2.setlength(L,1);
		//C20_2.clear();

		memset(C20_2,0,nSize*sizeof(MKL_Complex8));

		//fvec C21_2;
		//C21_2.setlength(L,1);
		//C21_2.clear();

		memset(C21_2,0,nSize*sizeof(MKL_Complex8));

		//cvec C_2 ;
		//C_2.setlength(L);
		//C_2.clear();

		memset(C_2,0,nSize*sizeof(MKL_Complex8));

		//cvec C40_2;
		//C40_2.setlength(L,1);
		//C40_2.clear();

		memset(C40_2,0,nSize*sizeof(MKL_Complex8));

		//fvec C42_2;
		//C42_2.setlength(L,1);
		//C42_2.clear();

		memset(C42_2,0,nSize*sizeof(MKL_Complex8));

		MKL_Complex8 smc20, smc40 ;
		smc20.real =0; smc20.imag=0;
		smc40.real =0; smc40.imag=0;

		float smc21 = 0 ,  smc42 = 0 , smph = 0 , sm2ph = 0;


		// AMR main algorithm
		for(int k=0 ;k<nSize;k++)
		{
			MKL_Complex8 d;
			d = Com_Env1[k];
			smabs = smabs + vmlsABS(Com_Env1[k]);
			sm2abs = sm2abs + pow(vmlsABS(Com_Env1[k]),2);
			//sm += Com_Env1[k];			
			sm.real += Com_Env1[k].real;
			sm.imag += Com_Env1[k].imag;
			sm2R = sm2R + pow(Com_Env1[k].real,2);
			sm2I = sm2I + pow(Com_Env1[k].imag,2);
		}

		//fvec R;
		//R.setlength(s);


		//fvec I;
		//long s = L;
		//I.setlength(s);

		for(int i=0;i<nSize;i++)
		{
			R[i] = Com_Env1[i].real;
			I[i] = Com_Env1[i].imag;
		}


		float m1=smabs/nSize;

		for(int i=1;i<=nSize;i++)
		{
			if (vmlsABS(Com_Env1[nSize-i]) < (0.5 * m1))
				count = count + 1;
		}

		count /= nSize;

		//	phasor_2.setlength(L);
		//	C21_2.setlength(L);
		//	C_2.setlength(L);
		//	C20_2.setlength(L);
		//	C40_2.setlength(L);
		//	C42_2.setlength(L);

		for (int k=1;k<nSize;k++)
		{
			//phasor_2[k]=angle(Com_Env1[k])-angle(Com_Env1[k-1]);
			phasor_2[k]=atan2(Com_Env1[k].imag,Com_Env1[k].real)-atan2(Com_Env1[k-1].imag,Com_Env1[k-1].real);
			C21_2[k].real=pow(vmlsABS(Com_Env1[k]),2);
			C21_2[k].imag=0;
			//C_2[k]=Com_Env1[k]*conj(Com_Env1[k-1]);
			C_2[k] = vmlsMul(Com_Env1[k],vmlsConj(Com_Env1[k-1]));
			C20_2[k]=vmlsMul(C_2[k],C_2[k]);//C_2[k] * C_2[k];
			//C40_2[k] = C_2[k] * C_2[k] * C_2[k] * C_2[k];
			C40_2[k] = vmlsMul(C_2[k],C_2[k]);
			C40_2[k] = vmlsMul(C40_2[k],C40_2[k]);
			C42_2[k].real=pow((float)vmlsABS(Com_Env1[k]),4);
			C42_2[k].imag=0;
		}

		//	cvec C20_1;
		//	C20_1.setlength(L,1);
		//	C20_1.clear();
		memset(C20_1,0,nSize*sizeof(MKL_Complex8));

		//fvec C21_1;
		//C21_1.setlength(L,1);
		//C21_1.clear();

		memset(C21_1,0,nSize*sizeof(MKL_Complex8));

		//cvec C40_1;
		//C40_1.setlength(L,1);
		//C40_1.clear();

		memset(C40_1,0,nSize*sizeof(MKL_Complex8));

		//fvec C42_1;
		//C42_1.setlength(L,1);
		//C42_1.clear();

		memset(C42_1,0,nSize*sizeof(MKL_Complex8));

		//Phase_1.setlength(L);
		//Phase_1.clear();
		memset(Phase_1,0,nSize*sizeof(float));

		/*	for(int i=0 ; i<L ; i++)
		{
		C20_1[i] = C20_2[i];
		C21_1[i] = C21_2[i];
		C40_1[i] = C40_2[i];
		C42_1[i] = C42_2[i];
		Phase_1[i] = phasor_2[i];
		}*/

		for(int i=1 ; i<nSize ; i++)
		{	
			//smc21 += C21_2[i];//C21_1[i];
			//smc20 += C20_2[i];//C20_1[i];
			//smc40 += C40_2[i];//C40_1[i];
			//smc42 += C42_2[i];//C42_1[i];
			smc21 += C21_2[i].real;//C21_1[i];
			smc20.real += C20_2[i].real;//C20_1[i];
			smc20.imag += C20_2[i].imag;//C20_1[i];
			smc40.real += C40_2[i].real;//C40_1[i];
			smc40.imag += C40_2[i].imag;//C40_1[i];
			smc42 += C42_2[i].real;//C42_1[i];
			smph  += phasor_2[i];//Phase_1[i];
			sm2ph += pow((float)phasor_2[i]/*Phase_1[i]*/,2);
		}

		double M21 = 1 + pow(10,-(0.1 * SNR));
		float Cum21 = smc21/nSize;
		MKL_Complex8 Cum20a;
		//Cum20a = smc20;
		//Cum20a.real *= (pow((double)M21,2)/L)/(Cum21*Cum21)); 
		//Cum20a.imag *= (pow((double)M21,2)/L)/(Cum21*Cum21)); 
		Cum20a.real = pow((float)M21,2)*(smc20.real/L)/(Cum21*Cum21); 
		Cum20a.imag = pow((float)M21,2)*(smc20.imag/L)/(Cum21*Cum21); 
		float Cum20 = vmlsABS(Cum20a);
		MKL_Complex8 Cum40;
		Cum40 = smc40;
		Cum40.real = pow((float)M21,4)  *  (smc40.real /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		Cum40.imag = pow((float)M21,4)  *  (smc40.imag /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		//Cum40 = pow((double)M21,4)  *  (smc40 /L)/(Cum21*Cum21*Cum21*Cum21) - 3 * (Cum20*Cum20);
		double absCum40 = vmlsABS(Cum40);
		float Cum42 = pow((float)M21,2) * (smc42/L) / (Cum21*Cum21) - (abs(Cum20)*abs(Cum20)) - 2*pow((float)M21,2);
		//float PhasVar_tst = variance(Phase_1);
		//complex PhasVar=(sm2ph-(smph * smph)/L)/(L-1);

		//***********************************
		double sum=0;		
// 		WriteToFile("c:\\C20_2.txt",C20_2,L,0);
		for(int i=0 ; i<L ; i++)
		{
			sum += abs(C20_2[i].imag);

		}

		sum/=L;
		sum*=pow((float)M21,2);
		//***********************************


		nModType = AMR_DIGITAL_UNKNOWN;
		//%% Decision
		if ((Cum20 > 0.5) &&  (count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum42 < -1.68) && (-2.4 < Cum42))
			nModType = AMR_DIGITAL_BPSK;

		else if ((Cum20 > 0.5) &&  (count > 0.05) && (vmlsABS(Cum40) > 0.1) && ( Cum42 < -1.2) && (-1.68 < Cum42))
			nModType = AMR_DIGITAL_4_ASK;

		else if ((vmlsABS(Cum40) < 0.1) && (count < 0.05) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_8PSK;

	/*
		else if ( (Cum40.real < -.5) &&  (count < 0.05) && (Cum20 < 0.1) && sum>0.25)
			nModType = AMR_DIGITAL_DQPSK;*/

		else if ((Cum40.real < -.5) && (count < 0.05) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_DQPSK;

		else if ((vmlsABS(Cum40) < 0.27) && (count < 0.1) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_OQPSK;

		else if ((count > 0.05) && (vmlsABS(Cum40) < 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_QAM32;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.32) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.73) && (-0.649 > Cum42))
			nModType = AMR_DIGITAL_QAM16;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.25) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.649) && (-0.611 > Cum42))
			nModType = AMR_DIGITAL_QAM64;

		else if ((count > 0.05) && (vmlsABS(Cum40) > 0.2) && (Cum40.real > -0.1) && (Cum20 < 0.1) && (Cum42 > -0.611) && (-0.59 > Cum42))
			nModType = AMR_DIGITAL_QAM256;

		//else if ((count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1) && sum<0.25)
		//	nModType = AMR_DIGITAL_QPSK;


		else if ((count < 0.05) && (vmlsABS(Cum40) > 0.1) && (Cum40.real > -0.1) && (Cum20 < 0.1))
			nModType = AMR_DIGITAL_QPSK;


		else
			nModType = AMR_DIGITAL_UNKNOWN;

		return DSP_RET_SUCCESS;
	}
}//namespace