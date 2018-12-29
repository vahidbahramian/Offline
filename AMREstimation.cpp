//#include "stdafx.h"
#include "AMREstimation.h"
#include "math.h"

namespace NS_FRAME_BASED
{

CAMREstimation::CAMREstimation(void)
{

}


CAMREstimation::~CAMREstimation(void)
{
}

void CAMREstimation::initialize(int iSizeOutCIC)
{

	L = iSizeOutCIC;//2000;
	agc_i=new double[L];
	agc_q=new double[L];
	phasor_2=new double[L];
	C84_2=new double[L];
	C42_2=new double[L];
	C21_2=new double[L];

	C_2=ippsMalloc_64fc(L);
	C40_2=ippsMalloc_64fc(L);
	
	C80_2=ippsMalloc_64fc(L);
	C20_2=ippsMalloc_64fc(L);
	
	C62_2=ippsMalloc_64fc(L);
	C41_2=ippsMalloc_64fc(L);
	CMr_2=ippsMalloc_64fc(L);
	C63_2=ippsMalloc_64fc(L);
	C60_2=ippsMalloc_64fc(L);
	C61_2=ippsMalloc_64fc(L);
	C83_2=ippsMalloc_64fc(L);
	C82_2=ippsMalloc_64fc(L);
	C81_2=ippsMalloc_64fc(L);
	Mag_C=ippsMalloc_64f(L);

	Com_Env1=ippsMalloc_64fc(L);
	Temp =ippsMalloc_64fc(L);
	Temp2=ippsMalloc_64fc(L);
	ippsZero_64fc(Temp,L);
	Demod_counters=new double[12];
	M21=1+1e-6;
	Count_Cum=0;
	//agc_i =(I_consellation);
    //agc_q =(Q_consellation);
	
	memset(phasor_2,0.0,L);

	memset(C20_2,0.0,L);
	memset(C21_2,0.0,L);
	memset(C_2,0.0,L);
	memset(C40_2,0.0,L);
	memset(C42_2,0.0,L);
	memset(C80_2,0.0,L);
	memset(C84_2,0.0,L);
	memset(C62_2,0.0,L);
	memset(C41_2,0.0,L);
	memset(CMr_2,0.0,L);
	memset(C63_2,0.0,L);
	memset(C60_2,0.0,L);
	
	//smc21=0;smc20=0;smc40=0;smc42=0;smph=0;sm2ph=0;smc80=0;smc84=0;smc62=0;smc41=0;smcMr=0;smc63=0;smc60=0;smc61=0;smc81=0;smc82=0;smc83=0;
}

void CAMREstimation::Cumulants_caculation(double *I_consellation,double *Q_consellation)
	
{
	ippsCopy_64f(I_consellation,agc_i,L/*2000*/);
	ippsCopy_64f(Q_consellation,agc_q,L/*2000*/);

	//m_PCIcard.SavtToFile_double(agc_i,2500);
	ippsStdDev_64f(agc_i, L, &std_I);
	ippsStdDev_64f(agc_q, L, &std_Q);
	normalize_factor=1/(pow(std_I,2)+pow(std_Q,2));

	ippsMulC_64f_I(normalize_factor,agc_i,L);
	ippsMulC_64f_I(normalize_factor,agc_q,L);

	ippsRealToCplx_64f(agc_i,agc_q,Com_Env1,L);
	ippsMagnitude_64fc(Com_Env1,Mag_C,L);
	
	ippsCopy_64fc(Com_Env1,Temp+1,(L-1));

	ippsMagnitude_64fc(Com_Env1,C21_2,L);
	ippsSqr_64f_I(C21_2,L);
	ippsConj_64fc_I(Temp,L);
	ippsMul_64fc(Com_Env1,Temp,C_2,L);
	ippsSqr_64fc(C_2,C20_2,L);
	ippsSqr_64fc(C20_2,C40_2,L);
	/*ippsSqr_64fc(Com_Env1,Temp,L);
	ippsSqr_64fc_I(Temp,L);*/
	ippsMagnitude_64fc(Com_Env1,C42_2,L);
	ippsSqr_64f_I(C42_2,L);
	ippsSqr_64f_I(C42_2,L);
	
	ippsSqr_64fc(C40_2,C80_2,L);
	ippsMagnitude_64fc(C_2,C84_2,L);
	ippsSqr_64f_I(C84_2,L);
	ippsSqr_64f_I(C84_2,L);
	ippsSqr_64f_I(C84_2,L);

	ippsSqr_64fc(Com_Env1,Temp,L);
	ippsSqr_64fc_I(Temp,L);
	ippsConj_64fc(Com_Env1,C62_2,L);//C62_2[k]=Com_Env1[k].^4 .*conj(Com_Env1[k]).^2;
	ippsSqr_64fc_I(C62_2,L);
	ippsMul_64fc_I(Temp,C62_2,L);

	ippsSqr_64fc(Com_Env1,Temp,L);
	ippsMul_64fc_I(Com_Env1,Temp,L);//C41_2[k]=Com_Env1[k].^3 .*conj(Com_Env1[k]);
	ippsConj_64fc(Com_Env1,C41_2,L);
	ippsMul_64fc_I(Temp,C41_2,L);

	ippsSqr_64fc(Com_Env1,Temp,L);
	ippsMul_64fc_I(Com_Env1,Temp,L);
	ippsConj_64fc(Com_Env1,C63_2,L);//C63_2[k]=Com_Env1[k].^3 .*conj(Com_Env1[k]).^3;
	ippsSqr_64fc(C63_2,Temp2,L);
	ippsMul_64fc_I(Temp2,C63_2,L);
	ippsMul_64fc_I(Temp,C63_2,L);

	ippsSqr_64fc(Temp,C60_2,L);  //C60_2[k]=Com_Env1[k].^6;

	ippsConj_64fc(Com_Env1,C61_2,L);
	ippsMul_64fc_I(Com_Env1,Temp,L);
	ippsMul_64fc_I(Com_Env1,Temp,L);//C61_2[k]=Com_Env1[k].^5.*conj(Com_Env1[k]);
	ippsMul_64fc_I(Temp,C61_2,L);

	ippsConj_64fc(Com_Env1,C83_2,L);
	ippsSqr_64fc(C83_2,Temp2,L);
	ippsMul_64fc_I(Temp2,C83_2,L);//C83_2[k]= Com_Env1[k].^5.*conj(Com_Env1[k]).^3;
	ippsMul_64fc_I(Temp,C83_2,L);

	ippsConj_64fc(Com_Env1,C82_2,L);
	ippsSqr_64fc_I(C82_2,L);
	ippsMul_64fc_I(Com_Env1,Temp,L);//		C82_2[k]=Com_Env1[k].^6.*conj(Com_Env1[k]).^2;
	ippsMul_64fc_I(Temp,C82_2,L);

	ippsConj_64fc(Com_Env1,C81_2,L);
	ippsMul_64fc_I(Com_Env1,Temp,L);//C81_2[k]=Com_Env1[k].^7.*conj(Com_Env1[k]);
	ippsMul_64fc_I(Temp,C81_2,L);


	ippsMean_64f(C21_2+1,(L-1),&smc21);
	ippsMean_64f(C84_2+1,(L-1),&smc84);
	ippsMean_64f(C42_2+1,(L-1),&smc42);

    ippsMean_64fc(C20_2+1,(L-1),&smc20);
    ippsMean_64fc(C40_2+1,(L-1),&smc40);
  

    
    ippsMean_64fc(C80_2+1,(L-1),&smc80);
   
    ippsMean_64fc(C62_2+1,(L-1),&smc62);
    ippsMean_64fc(C41_2+1,(L-1),&smc41);
    ippsMean_64fc(C63_2+1,(L-1),&smc63);
    ippsMean_64fc(C60_2+1,(L-1),&smc60);

    ippsMean_64fc(C61_2+1,(L-1),&smc61);
    ippsMean_64fc(C81_2+1,(L-1),&smc81);
    ippsMean_64fc(C82_2+1,(L-1),&smc82);
    ippsMean_64fc(C83_2+1,(L-1),&smc83);



	//ippsZero_64fc(Temp,L);
	//ippsCopy_64fc(Com_Env1,Temp+1,(L-1));

	//Ipp64f *Test;
	Ipp64fc Tmp,Tmp2,Tmp3,Tmp4,Tmp5,Tmp6,Tmp7;
	//Test=ippsMalloc_64f(1);
	//ippsDivC_64f(&smc21,L,Test,1);
	//ippsMul_64fc(pow(M21,2), const Ipp64fc* pSrc2, Ipp64fc* pDst, int len);
	Cum21 = pow(M21,2)*smc21;
	

	Tmp.re=pow(Cum21,2);
	Tmp.im=0;
	ippsDiv_64fc(&Tmp,&smc20,&Tmp2,1);
	ippsMagnitude_64fc(&Tmp2,&Cum20,1);  //Cum20


	absCum20=abs(Cum20);


	Tmp3.re=pow(Cum21,4)- 3*pow(Cum20,2);
	Tmp3.im=0;
	Tmp.re=pow(M21,4);
	Tmp.im=0;
	ippsMul_64fc(&Tmp,&smc40,&Tmp2,1);  // Cum40
	ippsDiv_64fc(&Tmp3,&Tmp2,&Cum40,1);



	ippsMagnitude_64fc(&Cum40,&absCum40,1);  //absCum40



	Cum42=pow(M21,2)*smc42/pow(Cum21,2)-pow(Cum20,2)-2*pow(Cum21,2);  // Cum42 & absCum42
	absCum42=abs(Cum42);


	Tmp.re=pow(M21,8)/pow(Cum21,8);
	Tmp.im=0;
	ippsMul_64fc_I(&smc80,&Tmp,1);
	Tmp2.re=pow(Cum21,4);
	Tmp2.im=0;
	ippsDiv_64fc(&Tmp2,&smc40,&Tmp3,1);
	ippsSqr_64fc_I(&Tmp3,1);
	Tmp2.re=-35.0;
	Tmp2.im=0;
	ippsMulC_64fc_I(Tmp2,&Tmp3,1);
	Tmp2.re=-28.0*Cum20/pow(Cum21,6);
	Tmp2.im=0;
	ippsMul_64fc(&Tmp2,&smc60,&Tmp4,1);  //Cum80
	Tmp2.re=420.0*pow(Cum20,2)/pow(Cum21,4);
	Tmp2.im=0;
	ippsMul_64fc_I(&smc40,&Tmp2,1);
	Tmp5.re=-630*pow(Cum20,4);
	Tmp5.im=0;
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp5,&Cum80,1);


	ippsMagnitude_64fc(&Cum80,&absCum80,1);  // absCum80


	Tmp.re=pow(M21,4)/pow(Cum21,4);
	Tmp.im=0;
	ippsMul_64fc_I(&smc62,&Tmp,1);
	Tmp2.re=-1*pow(Cum20,2)*pow(M21,4)/pow(Cum21,4);
	Tmp2.im=0;
	ippsMul_64fc(&Tmp2,&smc40,&Tmp3,1);
	Tmp2.re=-8*Cum21*pow(M21,3)/pow(Cum21,3);
	Tmp2.im=0;
	ippsMul_64fc(&Tmp2,&smc41,&Tmp4,1);  //Cum62
	Tmp5.re=(-6*pow(M21,2)*smc42*pow(Cum20,2)/pow(Cum21,2))+6*pow(Cum20,3)+24*Cum20*pow(Cum21,2);
	Tmp5.im=0;
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp5,&Cum62,1);


	ippsMagnitude_64fc(&Cum62,&absCum62,1);  // absCum62


	Tmp.re=smc84*pow(M21,4)/pow(Cum21,4);
	Tmp.im=0;
	Tmp2.re=-16*Cum21;
	Tmp2.im=0;
	ippsMul_64fc(&Tmp2,&Cum62,&Tmp3,1);
	Tmp4.re=pow(absCum40,2)-18*pow(Cum42,2)-72*pow(Cum20,2)	-24*pow(Cum21,4);   //Cum84
	Tmp4.im=0;
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp4,&Cum84,1);


	ippsMagnitude_64fc(&Cum84,&absCum84,1);  // absCum84


	Tmp.re=pow(M21,3)/pow(Cum21,3);
	Tmp.im=0;
	ippsMul_64fc_I(&smc63,&Tmp,1);	
	Tmp2.re=-6*Cum20*pow(M21,3)/pow(Cum21,3);
	Tmp2.im=0;
	ippsMul_64fc(&smc41,&Tmp2,&Tmp3,1);	
	Tmp4.re=-9*Cum21*pow(M21,3)*smc42/pow(Cum21,2);			//Cum63
	Tmp4.im=0;
	Tmp5.re=18*pow(Cum20,2)*Cum21+12*pow(Cum21,3);
	Tmp5.im=0;
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp5,&Cum63,1);



	ippsMagnitude_64fc(&Cum63,&absCum63,1);  // absCum63



	Tmp.re=pow(M21,6)/pow(Cum21,6);
	Tmp.im=0;
	ippsMul_64fc_I(&smc60,&Tmp,1);	
	Tmp2.re=-15*Cum20*pow(M21,4)/pow(Cum21,4);
	Tmp2.im=0;
	ippsMul_64fc(&smc40,&Tmp2,&Tmp3,1);     //Cum60
	Tmp4.re=30*pow(Cum20,3);
	Tmp4.im=0;
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp4,&Cum60,1);



	ippsMagnitude_64fc(&Cum60,&absCum60,1);  // absCum60



	Tmp.re=pow(M21,5)/pow(Cum21,5);
	Tmp.im=0;
	ippsMul_64fc_I(&smc61,&Tmp,1);
	Tmp2.re=-10*Cum20*pow(M21,3)/pow(Cum21,3);
	Tmp2.im=0;		
	ippsMul_64fc(&smc41,&Tmp2,&Tmp3,1);
	Tmp2.re=-5*Cum21*pow(M21,4)/pow(Cum21,4);
	ippsMul_64fc(&smc40,&Tmp2,&Tmp4,1);   // Cum61
	Tmp5.re=30*pow(Cum20,2)*Cum21;
	Tmp5.im=0;
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp5,&Cum61,1);



	ippsMagnitude_64fc(&Cum61,&absCum61,1);  // absCum61



	Tmp.re=pow(M21,3)/pow(Cum21,3);
	Tmp.im=0;
	ippsMul_64fc_I(&smc41,&Tmp,1);   //Cum41
	Tmp2.re=-3*Cum20;
	Tmp2.im=0;
	ippsAdd_64fc(&Tmp,&Tmp2,&Cum41,1);



	ippsMagnitude_64fc(&Cum41,&absCum41,1);  // absCum41



	Tmp.re=pow(M21,7)/pow(Cum21,7);
	Tmp.im=0;
	ippsMul_64fc_I(&smc81,&Tmp,1);
	Tmp2.re=-35*pow(M21,7)/pow(Cum21,7);
	Tmp2.im=0;		
	ippsMul_64fc(&smc41,&Tmp2,&Tmp3,1);
	ippsMul_64fc_I(&smc40,&Tmp3,1);
	Tmp4.re=-630*pow(Cum20,4);
	Tmp4.im=0;
	Tmp2.re=210*pow(M21,4)*Cum20/pow(Cum21,3);   //Cum81
	Tmp2.im=0;
	ippsMul_64fc(&smc40,&Tmp2,&Tmp5,1);
	Tmp2.re=210*pow(M21,3)*pow(Cum20,2)/pow(Cum21,3);
	Tmp2.im=0;
	ippsMul_64fc(&smc41,&Tmp2,&Tmp6,1);
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc_I(&Tmp5,&Tmp,1);
	ippsAdd_64fc(&Tmp,&Tmp6,&Cum81,1);



	ippsMagnitude_64fc(&Cum81,&absCum81,1);  // absCum81



	Tmp.re=pow(M21,6)/pow(Cum21,6);
	Tmp.im=0;
	ippsMul_64fc_I(&smc82,&Tmp,1);
	Tmp2.re=-15*pow(M21,6)*smc42/pow(Cum21,6);
	Tmp2.im=0;		
	ippsMul_64fc(&smc40,&Tmp2,&Tmp3,1);
	Tmp2.re=30*pow(M21,4)*pow(Cum20,2)/pow(Cum21,4);
	Tmp2.im=0;	
	ippsMul_64fc(&smc40,&Tmp2,&Tmp4,1);
	Tmp2.re=60*pow(M21,4)/pow(Cum21,2);    //Cum82
	Tmp2.im=0;	
	ippsMul_64fc(&smc40,&Tmp2,&Tmp5,1);
	Tmp2.re=240*pow(M21,3)*Cum20/pow(Cum21,2);
	Tmp2.im=0;	
	ippsMul_64fc(&smc41,&Tmp2,&Tmp6,1);
	Tmp2.re=-90*pow(M21,2)*pow(Cum20,2)*smc42/pow(Cum21,2)-90*pow(Cum20,4)-560*pow(Cum20,2)*pow(Cum21,2);
	Tmp2.im=0;	
	ippsAdd_64fc_I(&Tmp2,&Tmp,1);
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc_I(&Tmp5,&Tmp,1);
	ippsAdd_64fc(&Tmp6,&Tmp,&Cum82,1);



	ippsMagnitude_64fc(&Cum82,&absCum82,1);  // absCum82


	Tmp.re=pow(M21,5)/pow(Cum21,5);
	Tmp.im=0;
	ippsMul_64fc_I(&smc83,&Tmp,1);
	Tmp2.re=-5*pow(M21,6)*smc42/pow(Cum21,6);
	Tmp2.im=0;	
	ippsMul_64fc(&smc40,&Tmp2,&Tmp3,1);
	Tmp2.re=-30*pow(M21,5)*smc42/pow(Cum21,5);
	Tmp2.im=0;	
	ippsMul_64fc(&smc41,&Tmp2,&Tmp4,1);
	Tmp2.re=90*pow(M21,6)*pow(Cum20,2)/pow(Cum21,6);
	Tmp2.im=0;	
	ippsMul_64fc_I(&smc41,&Tmp2,1);
	ippsMul_64fc(&smc41,&Tmp2,&Tmp5,1);   //Cum83
	Tmp2.re=120*pow(M21,3)/Cum21;
	Tmp2.im=0;	
	ippsMul_64fc(&smc41,&Tmp2,&Tmp6,1);
	Tmp2.re=30*pow(M21,4)*Cum20/pow(Cum21,3);
	Tmp2.im=0;	
	ippsMul_64fc(&smc40,&Tmp2,&Tmp7,1);
	Tmp2.re=-270*pow(Cum20,3)*Cum21+180*pow(M21,2)*smc42*Cum20/Cum21-360*Cum20*pow(Cum21,3);
	Tmp2.im=0;
	ippsAdd_64fc_I(&Tmp2,&Tmp,1);
	ippsAdd_64fc_I(&Tmp3,&Tmp,1);
	ippsAdd_64fc_I(&Tmp4,&Tmp,1);
	ippsAdd_64fc_I(&Tmp5,&Tmp,1);
	ippsAdd_64fc_I(&Tmp6,&Tmp,1);
	ippsAdd_64fc(&Tmp7,&Tmp,&Cum83,1);

	ippsMagnitude_64fc(&Cum83,&absCum83,1);  // absCum83
	
	ippsStdDev_64f(Mag_C, L, &std_Mag);

}
void CAMREstimation::SavtToFile_double(double *dbDataInp2,int nLenOut)
{
//    QFile BufferFile;
	
//    InputFilePath = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
//	if(fd.DoModal() != IDOK )
//	{
//		//return;
//	}
//	else
//	{
//		BufferFile.Open(/*_T("C:\\SavedBuffer.bin")*/ fd.GetPathName(),CFile::modeCreate | CFile::modeReadWrite);
		
//		BufferFile.Write(dbDataInp2,nLenOut);
//		BufferFile.Flush();
//		BufferFile.Close();
//	}

}
void CAMREstimation::AMR(double SNR)
	
{

	BPSK_Counter = 0;
	QPSK_Counter = 0;
	PSK8_Counter = 0;
	QAM16_Counter = 0;
	QAM32_Counter = 0;
	QAM64_Counter = 0;
	QAM128_Counter = 0;
	APSK16_Counter = 0;
	APSK32_Counter = 0;
	APSK64_Counter = 0;
	ASK4_Counter = 0;
	ASK2_Counter = 0;

 if (absCum20 > 0.8)
	
	{
		BPSK_Counter = BPSK_Counter+10;
		ASK2_Counter = ASK2_Counter+10;
	}
 else if (absCum20<0.3)
	{
		QPSK_Counter = QPSK_Counter+10;
	}


if (absCum20<0.07)
	{
		QAM32_Counter = QAM32_Counter+10; 
		QAM128_Counter = QAM128_Counter+10;
	}

if (absCum20<0.02)
	{
		QAM64_Counter = QAM64_Counter+10;
		QAM16_Counter = QAM16_Counter+10;
		PSK8_Counter = PSK8_Counter+10;
	}
else if ((0.02<absCum20)&&(absCum20<0.03) )
{
    QAM64_Counter = QAM64_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
     PSK8_Counter = PSK8_Counter+10;
}
else if ((0.03<absCum20)&&(absCum20<0.06) )
{
    QAM64_Counter = QAM64_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
    PSK8_Counter = PSK8_Counter+10;
}

// FOR APSK Group, cumulants will be calculated separately.
if (absCum20<0.05)
{
APSK16_Counter = APSK16_Counter+10;
APSK32_Counter = APSK32_Counter+10;
APSK64_Counter = APSK64_Counter+10;
}
else if ((0.05<absCum20)&&(absCum20<0.1))
APSK64_Counter = APSK64_Counter+10;


if (absCum20>0.9)
 ASK4_Counter = ASK4_Counter + 10;
    
    
if (absCum40<0.1)
{
    QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    PSK8_Counter = PSK8_Counter+10;
}
else if ((0.1<absCum40)&&(absCum40<0.2) )
{
    QAM128_Counter = QAM128_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
}
else if ((0.2<absCum40)&&(absCum40<0.5))
{
     QAM128_Counter = QAM128_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
}
else if ((0.5<absCum40)&&(absCum40<0.6))
{
    QAM64_Counter = QAM64_Counter+10;
    QPSK_Counter = QPSK_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
}
else if ((0.6<absCum40)&&(absCum40<1.2))
{
    QPSK_Counter = QPSK_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
}
else if ((1.2<absCum40)&&(absCum40<2))
{
    BPSK_Counter = BPSK_Counter+10;
}


if (absCum40<0.1)
{
APSK16_Counter = APSK16_Counter+10;
APSK32_Counter = APSK32_Counter+10;
APSK64_Counter = APSK64_Counter+10;
}
else if ((0.1<absCum40)&&(absCum40<0.2))
{
APSK32_Counter = APSK32_Counter+10;
APSK64_Counter = APSK64_Counter+10;
}

if ((0.1<absCum40)&&(absCum40<0.7))
 ASK4_Counter = ASK4_Counter + 10;

if ((0.8<absCum40)&&(absCum40<1.5))
 ASK2_Counter = ASK2_Counter + 10;


if (absCum41<0.2)
{
	QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
}
else if ((0.2<absCum41)&&(absCum41<0.5))
{
    QAM128_Counter = QAM128_Counter+10;
}

if  (absCum41<0.2)
{
   QPSK_Counter = QPSK_Counter+10;
   QAM64_Counter = QAM64_Counter+10;
   QAM16_Counter = QAM16_Counter+10;  
}
else if ((absCum41>0.2)&&(absCum41<0.4))
{
    QAM64_Counter = QAM64_Counter+10;
}

if  ((0.01<absCum41)&&(absCum41<0.2))
   PSK8_Counter = PSK8_Counter+10;

if  ((2<absCum41)&&(absCum41<3))
   BPSK_Counter = BPSK_Counter+10;

if (absCum41<0.3)
{
   APSK16_Counter = APSK16_Counter+10;
   APSK32_Counter = APSK32_Counter+10;
   APSK64_Counter = APSK64_Counter+10;
}

if  ((2.4<absCum41)&&(absCum41<3))
   ASK4_Counter = ASK4_Counter+10;

if  ((2<absCum41)&&(absCum41<4))
   ASK2_Counter = ASK2_Counter+10;


if (absCum42<0.78)
   QAM32_Counter = QAM32_Counter+10;
else if ((0.78<absCum42)&&(absCum42<0.87))
{
    QAM32_Counter = QAM32_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
}
else if ((0.87<absCum42)&&(absCum42<0.9))
    QAM128_Counter = QAM128_Counter+10;

if ((0.7<absCum42)&&(absCum42<0.86))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
}
else if ((0.86<absCum42)&&(absCum42<1))
    QAM64_Counter = QAM64_Counter+10;

if ((0.5<absCum42)&&(absCum42<0.6))
{
  PSK8_Counter = PSK8_Counter+10;
  QPSK_Counter = QPSK_Counter+10;
}
if  ((0.1<absCum42)&&(absCum42<0.5))
  BPSK_Counter = BPSK_Counter+10;

if (absCum42<=0.79)
   APSK16_Counter = APSK16_Counter+10;
else if ((0.79<absCum42)&&(absCum42<=0.8))
{
   APSK16_Counter = APSK16_Counter+10;
   APSK64_Counter = APSK64_Counter+10;
}
else if ((0.8<absCum42)&&(absCum42<0.9))
   APSK64_Counter = APSK64_Counter+10;
else if (0.9<absCum42)
   APSK32_Counter = APSK32_Counter+10; 

if  ((0.1<absCum42)&&(absCum42<0.35))
  ASK4_Counter = ASK4_Counter+10;

if  ((0.3<absCum42)&&(absCum42<0.8))
  ASK2_Counter = ASK2_Counter+10;

if (absCum60<0.01)
   QAM16_Counter = QAM16_Counter+10;
else if ((0.01<absCum60)&&(absCum60<0.02))
{
   QAM16_Counter = QAM16_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
}
else if ((0.02<absCum60)&&(absCum60<0.05))
{
	  QPSK_Counter = QPSK_Counter+10;
      QAM128_Counter = QAM128_Counter+10;
}
else if ((0.05<absCum60)&&(absCum60<0.1))
{
   PSK8_Counter = PSK8_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
     QAM16_Counter = QAM16_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
}
else if ((0.1<absCum60)&&(absCum60<0.2))
{
   PSK8_Counter = PSK8_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
    QAM32_Counter = QAM32_Counter+10; 
   QAM64_Counter = QAM64_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
   QAM128_Counter = QAM128_Counter+10;
}
else if ((0.2<absCum60)&&(absCum60<0.3))
{
   PSK8_Counter = PSK8_Counter+10;
   QAM16_Counter = QAM16_Counter+10;
   QAM32_Counter = QAM32_Counter+10; 
   QAM64_Counter = QAM64_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
   QAM128_Counter = QAM128_Counter+10;
}
else if ((0.3<absCum60)&&(absCum60<0.5))
{
   PSK8_Counter = PSK8_Counter+10;
   QAM16_Counter = QAM16_Counter+10;
   QAM32_Counter = QAM32_Counter+10; 
   QAM64_Counter = QAM64_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
   QAM128_Counter = QAM128_Counter+10;
}
else if ((0.5<absCum60)&&(absCum60<0.7))
{
   QAM128_Counter = QAM128_Counter+10;
   QAM16_Counter = QAM16_Counter+10;
   QAM32_Counter = QAM32_Counter+10; 
   QAM64_Counter = QAM64_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
}
else if ((0.7<absCum60)&&(absCum60<1))
{
   QAM128_Counter = QAM128_Counter+10;
   QAM16_Counter = QAM16_Counter+10;
   QAM32_Counter = QAM32_Counter+10; 
   QAM64_Counter = QAM64_Counter+10;
   QPSK_Counter = QPSK_Counter+10;
}
else if ((1<absCum60)&&(absCum60<1.3))
{
    QAM32_Counter = QAM32_Counter+10; 
  QAM128_Counter = QAM128_Counter+10;
  QAM16_Counter = QAM16_Counter+10;
   QAM64_Counter = QAM64_Counter+10;
}
else if ((1.3<absCum60)&&(absCum60<1.5))
{
  QAM16_Counter = QAM16_Counter+10;
   QAM64_Counter = QAM64_Counter+10;
}
else if (7<absCum60)
	 BPSK_Counter = BPSK_Counter+10;

if (absCum60<1)
{
   APSK16_Counter = APSK16_Counter+10;
   APSK32_Counter = APSK32_Counter+10; 
   APSK64_Counter = APSK64_Counter+10;
}
else if ((1<absCum60)&&(absCum60<2))
   APSK32_Counter = APSK32_Counter+10;  


if ((9<absCum60)&&(absCum60<12))
   ASK4_Counter = ASK4_Counter+10;    

if ((15<absCum60)&&(absCum60<40))
   ASK2_Counter = ASK2_Counter+10;    


if (absCum61<0.1)
{
   QAM32_Counter = QAM32_Counter+10;
     QAM128_Counter = QAM128_Counter+10;
   PSK8_Counter = PSK8_Counter+10;
}
else if ((0.1<absCum61)&&(absCum61<0.2))
{
   QAM32_Counter = QAM32_Counter+10;
   PSK8_Counter = PSK8_Counter+10;
   QAM128_Counter = QAM128_Counter+10;
}
else if ((0.2<absCum61)&&(absCum61<0.4))
{
   QAM32_Counter = QAM32_Counter+10;
   PSK8_Counter = PSK8_Counter+10;
   QAM128_Counter = QAM128_Counter+10;
}
else if ((0.4<absCum61)&&(absCum61<0.7))
{
   QAM32_Counter = QAM32_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
   QAM128_Counter = QAM128_Counter+10;
}
else if ((0.7<absCum61)&&(absCum61<0.9))
{
  QAM32_Counter = QAM32_Counter+10;
  QAM64_Counter = QAM64_Counter+10; 
  QAM16_Counter = QAM16_Counter+10;
  QAM128_Counter = QAM128_Counter+10;
}
  else if ((0.9<absCum61)&&(absCum61<1))
{
  QAM64_Counter = QAM64_Counter+10; 
  QAM16_Counter = QAM16_Counter+10;
}
else if ((1<absCum61)&&(absCum61<2))
{
  QPSK_Counter = QPSK_Counter + 10;
  QAM64_Counter = QAM64_Counter+10;  
  QAM16_Counter = QAM16_Counter+10;
}
else if ((2<absCum61)&&(absCum61<3))
  QPSK_Counter = QPSK_Counter + 10;
else if (8<absCum61)
  BPSK_Counter = BPSK_Counter + 10; 


if (absCum61<0.5)
    APSK16_Counter = APSK16_Counter+10;

if ((0.04<absCum61)&&(absCum61<0.7))
  APSK32_Counter = APSK32_Counter+10;

if ((0.03<absCum61)&&(absCum61<0.5))
   APSK64_Counter = APSK64_Counter+10;


if ((5<absCum61)&&(absCum61<10))
   ASK4_Counter = ASK4_Counter+10;    

if ((2<absCum61)&&(absCum61<8))
   ASK2_Counter = ASK2_Counter+10;    

 
if (absCum62<0.1)
{
    QAM32_Counter = QAM32_Counter+10;
  QAM128_Counter = QAM128_Counter+10; 
  QAM16_Counter = QAM16_Counter+10;
  QAM64_Counter = QAM64_Counter+10;
  PSK8_Counter = PSK8_Counter+10;
  QPSK_Counter = QPSK_Counter+10;
}
else if((0.1<absCum62)&&(absCum62<0.2))
{
  PSK8_Counter = PSK8_Counter+10;
  QAM16_Counter = QAM16_Counter+10;
  QAM128_Counter = QAM128_Counter+10; 
  QAM32_Counter = QAM32_Counter+10; 
  QAM64_Counter = QAM64_Counter+10;
  QPSK_Counter = QPSK_Counter+10;
}
else if(( 0.2<absCum62)&&(absCum62<0.3))
{
  PSK8_Counter = PSK8_Counter+10;
  QAM16_Counter = QAM16_Counter+10;
  QAM128_Counter = QAM128_Counter+10; 
  QAM32_Counter = QAM32_Counter+10; 
  QAM64_Counter = QAM64_Counter+10; 
  QPSK_Counter = QPSK_Counter+10;
}
else if((0.3<absCum62)&&(absCum62<0.5))
{
  QAM16_Counter = QAM16_Counter+10;
  QAM128_Counter = QAM128_Counter+10; 
  QAM32_Counter = QAM32_Counter+10; 
  QAM64_Counter = QAM64_Counter+10; 
  QPSK_Counter = QPSK_Counter+10;
  PSK8_Counter = PSK8_Counter+10;
}
else if((0.5<absCum62)&&(absCum62<0.7))
{
  QAM64_Counter = QAM64_Counter+10; 
}
else if((2<absCum62)&&(absCum62<6))
    BPSK_Counter = BPSK_Counter+10;

if(absCum62<0.5)
{
  APSK16_Counter = APSK16_Counter+10;
  APSK32_Counter = APSK32_Counter+10;
  APSK64_Counter = APSK64_Counter+10;
}

if((0.1<absCum62)&&(absCum62<0.7))
  ASK4_Counter = ASK4_Counter+10;  

if((1<absCum62)&&(absCum62<5))
  ASK2_Counter = ASK2_Counter+10;  

if((1.9<absCum63)&&(absCum63<2.3))
{
  PSK8_Counter = PSK8_Counter+10;
  QPSK_Counter = QPSK_Counter+10;
  QAM32_Counter = QAM32_Counter+10; 
  QAM128_Counter = QAM128_Counter+10;
}
else if((2.3<absCum63)&&(absCum63<2.7))   
{
  QAM16_Counter = QAM16_Counter+10; 
  QAM32_Counter = QAM32_Counter+10;
  QAM64_Counter = QAM64_Counter+10; 
  QAM128_Counter = QAM128_Counter+10;
  QPSK_Counter = QPSK_Counter+10;
}
else if((2.7<absCum63)&&(absCum63<4))
{
  QAM32_Counter = QAM32_Counter+10; 
  QAM128_Counter = QAM128_Counter+10;
}
else if((4<absCum63)&&(absCum63<4.5))
	BPSK_Counter = BPSK_Counter+10;
else if((4.5<absCum63)&&(absCum63<7))
   BPSK_Counter = BPSK_Counter + 10;


if((2.5<absCum63)&&(absCum63<2.7))
    APSK16_Counter = APSK16_Counter+10;

if((2.6<absCum63)&&(absCum63<2.7))
  APSK32_Counter = APSK32_Counter+10;

if((2.4<absCum63)&&(absCum63<2.6))
   APSK64_Counter = APSK64_Counter+10;

if((4<absCum63)&&(absCum63<7))
   ASK4_Counter = ASK4_Counter+10;

if((2<absCum63)&&(absCum63<9))
   ASK2_Counter = ASK2_Counter+10;
if (absCum80<0.1)
     QAM32_Counter = QAM32_Counter+10;
else if((0.1<absCum80)&&(absCum80<0.3))
{
     QAM128_Counter = QAM128_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
}
else if((0.3<absCum80)&&(absCum80<0.6))
{
	 QAM128_Counter = QAM128_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
     QAM64_Counter = QAM64_Counter+10;
}
else if((0.6<absCum80)&&(absCum80<0.7))
{
     QAM128_Counter = QAM128_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
     QAM64_Counter = QAM64_Counter+10;
     PSK8_Counter = PSK8_Counter+10;
}
else if((0.7<absCum80)&&(absCum80<1))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    PSK8_Counter = PSK8_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
}
else if((1<absCum80)&&(absCum80<1.4))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
}
else if((1.4<absCum80)&&(absCum80<1.7))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
}
else if((1.7<absCum80)&&(absCum80<1.8))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
}
else if((1.8<absCum80)&&(absCum80<2))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
}
else if((2<absCum80)&&(absCum80<4))
{
    QAM16_Counter = QAM16_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
}
else if((4<absCum80)&&(absCum80<7))
    QAM64_Counter = QAM64_Counter+10;
else if((15<absCum80)&&(absCum80<35))
    QPSK_Counter = QPSK_Counter+10;
else if(100<absCum80)
    BPSK_Counter = BPSK_Counter + 10;


if (absCum80<0.8)
    APSK16_Counter = APSK16_Counter+10;
if (absCum80<2)
  APSK32_Counter = APSK32_Counter+10;
if (absCum80<1.5)
   APSK64_Counter = APSK64_Counter+10;
if((200<absCum80)&&(absCum80<300))
   ASK4_Counter = ASK4_Counter+10;

if((200<absCum80)&&(absCum80<700))
   ASK2_Counter = ASK2_Counter+10;

if(absCum81<0.1)
PSK8_Counter = PSK8_Counter+10;   
else if((0.1<absCum81)&&(absCum81<0.15))
{
	PSK8_Counter = PSK8_Counter+10; 
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
}
else if((0.15<absCum81)&&(absCum81<0.3))
{
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	PSK8_Counter = PSK8_Counter+10; 
	QAM64_Counter = QAM64_Counter+10;
}
else if((0.3<absCum81)&&(absCum81<0.4))
{
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	PSK8_Counter = PSK8_Counter+10; 
	QAM64_Counter = QAM64_Counter+10;
}
else if((0.4<absCum81)&&(absCum81<0.6))
{
	PSK8_Counter = PSK8_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
	QAM64_Counter = QAM64_Counter+10;
}
else if((0.6<absCum81)&&(absCum81<1))
{
	QPSK_Counter = QPSK_Counter+10;
	PSK8_Counter = PSK8_Counter+10; 
	QAM64_Counter = QAM64_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
}
else if((1<absCum81)&&(absCum81<1.2))
{
	QPSK_Counter = QPSK_Counter+10;
	QAM64_Counter = QAM64_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
}
else if((1.2<absCum81)&&(absCum81<3))
{
	QPSK_Counter = QPSK_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	QAM64_Counter = QAM64_Counter+10;
	QAM32_Counter = QAM32_Counter+10;
	QAM128_Counter = QAM128_Counter+10;
}
else if((3<absCum81)&&(absCum81<4))
{
	QAM32_Counter = QAM32_Counter+10;
	QPSK_Counter = QPSK_Counter+10;
	QAM16_Counter = QAM16_Counter+10;
	QAM64_Counter = QAM64_Counter+10;
}
else if(100<absCum81)
BPSK_Counter = BPSK_Counter + 10;


if(absCum81<1)
{
	APSK16_Counter = APSK16_Counter+10;
	APSK32_Counter = APSK32_Counter+10;
	APSK64_Counter = APSK64_Counter+10;
}
else if((1<absCum81)&&(absCum81<2))
{
	APSK16_Counter = APSK16_Counter+10;
	APSK32_Counter = APSK32_Counter+10;
	APSK64_Counter = APSK64_Counter+10;
}
else if((2<absCum81)&&(absCum81<2.5))
APSK32_Counter = APSK32_Counter+10;


if(200<absCum81)
   ASK4_Counter = ASK4_Counter+10;
if ((16<absCum81)&&(absCum81<260))
   ASK2_Counter = ASK2_Counter+10;

if(absCum82<0.2)
{
	PSK8_Counter = PSK8_Counter+10;
    QPSK_Counter = QPSK_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
    QAM128_Counter = QAM128_Counter+10;
}
else if ((0.2<absCum82)&&(absCum82<0.3))
{
     PSK8_Counter = PSK8_Counter+10; 
     QPSK_Counter = QPSK_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
     QAM128_Counter = QAM128_Counter+10;
}
else if ((0.3<absCum82)&&(absCum82<0.5))
{
     PSK8_Counter = PSK8_Counter+10;
     QPSK_Counter = QPSK_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
     QAM128_Counter = QAM128_Counter+10;
}
     
else if ((0.5<absCum82)&&(absCum82<0.7))
{
    QAM32_Counter = QAM32_Counter+10;  
    QAM128_Counter = QAM128_Counter+10; 
}
    else if((0.7<absCum82)&&(absCum82<0.8))
{
    QAM32_Counter = QAM32_Counter+10;  
    QAM128_Counter = QAM128_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
}
else if ((0.8<absCum82)&&(absCum82<0.9))
{
    QAM32_Counter = QAM32_Counter+10;  
    QAM128_Counter = QAM128_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
}
else if ((0.9<absCum82)&&(absCum82<1))
{
    QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;  
    QAM64_Counter = QAM64_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
}
else if((1<absCum82)&&(absCum82<1.5))
{
     QAM128_Counter = QAM128_Counter+10;
       QAM32_Counter = QAM32_Counter+10; 
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
}
else if ((1.5<absCum82)&&(absCum82<1.8))
{
     QAM128_Counter = QAM128_Counter+10;
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
}
else if ((1.8<absCum82)&&(absCum82<3))
{
     QAM128_Counter = QAM128_Counter+10;
     QAM32_Counter = QAM32_Counter+10; 
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10; 
}
else if ((3<absCum82)&&(absCum82<4))
    QAM64_Counter = QAM64_Counter+10; 
else if  (50<absCum82)
BPSK_Counter = BPSK_Counter + 10;

if (absCum82<1)
{
APSK16_Counter = APSK16_Counter+10;
APSK32_Counter = APSK32_Counter+10;
APSK64_Counter = APSK64_Counter+10;
}
else if ((1<absCum82)&&(absCum82<2.5))
APSK32_Counter = APSK32_Counter+10;

if ((15<absCum82)&&(absCum82<60))
   ASK4_Counter = ASK4_Counter+10;

if ((4<absCum82)&&(absCum82<25))
   ASK2_Counter = ASK2_Counter+10;

if (absCum83<0.1)
{
QAM128_Counter = QAM128_Counter+10;  
QAM32_Counter = QAM32_Counter+10;
}
else if ((0.1<absCum83)&&(absCum83<0.2))
{
QAM128_Counter = QAM128_Counter+10; 
QAM64_Counter = QAM64_Counter+10;  
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
}
else if ((0.2<absCum83)&&(absCum83<0.3)  )
{
QAM128_Counter = QAM128_Counter+10; 
QAM16_Counter = QAM16_Counter+10;
QAM64_Counter = QAM64_Counter+10;  
QAM32_Counter = QAM32_Counter+10;
}
else if ((0.3<absCum83)&&(absCum83<0.6))
{
PSK8_Counter = PSK8_Counter+10;   
QAM128_Counter = QAM128_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
QAM64_Counter = QAM64_Counter+10;  
}
else if ((0.6<absCum83)&&(absCum83<0.7))
{
 PSK8_Counter = PSK8_Counter+10;   
QAM128_Counter = QAM128_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
QAM64_Counter = QAM64_Counter+10;  
}
else if ((0.7<absCum83)&&(absCum83<0.9))
{
 PSK8_Counter = PSK8_Counter+10; 
QAM128_Counter = QAM128_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
QAM64_Counter = QAM64_Counter+10;  
}
else if ((0.9<absCum83)&&(absCum83<1))
{
PSK8_Counter = PSK8_Counter+10; 
QAM64_Counter = QAM64_Counter+10;  
QAM128_Counter = QAM128_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
}
else if ((1<absCum83)&&(absCum83<1.4))
{
PSK8_Counter = PSK8_Counter+10; 
QAM128_Counter = QAM128_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
QAM64_Counter = QAM64_Counter+10;  
}
else if ((1.4<absCum83)&&(absCum83<1.5))
{
PSK8_Counter = PSK8_Counter+10; 
QAM128_Counter = QAM128_Counter+10; 
QAM32_Counter = QAM32_Counter+10;
QAM16_Counter = QAM16_Counter+10;
QAM64_Counter = QAM64_Counter+10;  
}
else if ((1.5<absCum83)&&(absCum83<1.6))
{
QAM128_Counter = QAM128_Counter+10; 
PSK8_Counter = PSK8_Counter+10; 
QAM16_Counter = QAM16_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM64_Counter = QAM64_Counter+10;
}
else if ((1.6<absCum83)&&(absCum83<3))
{
PSK8_Counter = PSK8_Counter+10; 
QAM128_Counter = QAM128_Counter+10;
QAM16_Counter = QAM16_Counter+10;
 QAM32_Counter = QAM32_Counter+10;
QAM64_Counter = QAM64_Counter+10;
}
else if ((3<absCum83)&&(absCum83<3.5))
{
QAM64_Counter = QAM64_Counter+10;
QAM32_Counter = QAM32_Counter+10;
QAM128_Counter = QAM128_Counter+10;
QPSK_Counter = QPSK_Counter+10;
}
else if ((3.5<absCum83)&&(absCum83<4))
{
QAM32_Counter = QAM32_Counter+10;
QPSK_Counter = QPSK_Counter+10;
QAM64_Counter = QAM64_Counter+10;
QAM128_Counter = QAM128_Counter+10;
}
else if ((4<absCum83)&&(absCum83<6))
{
QPSK_Counter = QPSK_Counter+10;
QAM128_Counter = QAM128_Counter+10;
}
else if (30<absCum83)
BPSK_Counter = BPSK_Counter + 10;

if (absCum83<3)
    APSK16_Counter = APSK16_Counter+10;

if (absCum83<4)
  APSK32_Counter = APSK32_Counter+10;

if ((0.05<absCum83)&&(absCum83<5))
   APSK64_Counter = APSK64_Counter+10;


if ((1<absCum83)&&(absCum83<13))
   ASK4_Counter = ASK4_Counter+10;

if ((15<absCum83)&&(absCum83<45))
   ASK2_Counter = ASK2_Counter+10;

if ((4<absCum84)&&(absCum84<11))
{
PSK8_Counter = PSK8_Counter+10; 
QPSK_Counter = QPSK_Counter+10;
}
else if ((11<absCum84)&&(absCum84<12))
{
    QAM16_Counter = QAM16_Counter+10;
     QAM32_Counter = QAM32_Counter+10;
	PSK8_Counter = PSK8_Counter+10; 
	QPSK_Counter = QPSK_Counter+10;
}
else if ((12<absCum84)&&(absCum84<12.5))
{
    QAM16_Counter = QAM16_Counter+10; 
      QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
}
else if ((12.5<absCum84)&&(absCum84<13))
{
    QAM16_Counter = QAM16_Counter+10; 
      QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;
       QAM64_Counter = QAM64_Counter+10;
}
else if ((13<absCum84)&&(absCum84<13.5))
{
  QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;  
    QAM16_Counter = QAM16_Counter+10;
       QAM64_Counter = QAM64_Counter+10;
}
else if ((13.5<absCum84)&&(absCum84<14))
{
  QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;  
    QAM16_Counter = QAM16_Counter+10;
       QAM64_Counter = QAM64_Counter+10;
}
else if ((14<absCum84)&&(absCum84<15))
{
  QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;  
    QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
}
      
else if ((15<absCum84)&&(absCum84<16))

{
	QAM128_Counter = QAM128_Counter+10;
    QAM32_Counter = QAM32_Counter+10;  
     QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
}
else if ((16<absCum84)&&(absCum84<17))
{
         QAM32_Counter = QAM32_Counter+10;
  QAM128_Counter = QAM128_Counter+10; 
     QAM16_Counter = QAM16_Counter+10;
    QAM64_Counter = QAM64_Counter+10;
}
else if ((17<absCum84)&&(absCum84<18))
{
  QAM128_Counter = QAM128_Counter+10;
     QAM64_Counter = QAM64_Counter+10;
}
else if ((18<absCum84)&&(absCum84<19))
{
 QAM128_Counter = QAM128_Counter+10;
     QAM64_Counter = QAM64_Counter+10;  
}
else if  ((30<absCum84)&&(absCum84<33))
    APSK16_Counter = APSK16_Counter+10; 
else if (70<absCum84)
    BPSK_Counter = BPSK_Counter + 10;

if ((10<absCum84)&&(absCum84<17))
    APSK16_Counter = APSK16_Counter+10;

if ((14<absCum84)&&(absCum84<20))
  APSK32_Counter = APSK32_Counter+10;

if ((11<absCum84)&&(absCum84<17))
   APSK64_Counter = APSK64_Counter+10;


if ((50<absCum84)&&(absCum84<70))
   ASK4_Counter = ASK4_Counter+10;

if ((30<absCum84)&&(absCum84<45))
   ASK2_Counter = ASK2_Counter+10;


double Cums[14]={Cum20,Cum21,absCum40,absCum41,absCum42,absCum60,absCum61,absCum62,absCum63,absCum80,absCum81,absCum82,absCum83,absCum84};



Demod_counters[0]=BPSK_Counter;
Demod_counters[1]=QPSK_Counter;	
Demod_counters[2]=PSK8_Counter;
Demod_counters[3]=QAM16_Counter; 	
Demod_counters[4]=QAM32_Counter; 
Demod_counters[5]=QAM64_Counter; 	
Demod_counters[6]=QAM128_Counter;
Demod_counters[7]=APSK16_Counter;	
Demod_counters[8]=APSK32_Counter;
Demod_counters[9]=APSK64_Counter;	
Demod_counters[10]=ASK4_Counter;
Demod_counters[11]=ASK2_Counter;

if (std_Mag<0.2)
ippsMaxIndx_64f(Demod_counters,3, &Max_counter,&Max_counter_Indx);
else
ippsMaxIndx_64f(Demod_counters+3,9, &Max_counter,&Max_counter_Indx);

int dd=0;
if (std_Mag<0.2)
{
if (Max_counter_Indx==0)
{
	Amr_Out=0;
}else if(Max_counter_Indx==1)
{
	if (Cum40.re>0.005)
	{
		Amr_Out=1;
	}else
	{
		Amr_Out=15;
	}
}else if(Max_counter_Indx==2)
{
	Amr_Out=2;
}
else if (Cums[9]>100&&Cums[10]>50&&Cums[11]>100)
		{
			if(Cums[4]<0.25)
				Amr_Out=11;//4ask
			else
				Amr_Out=10;//2ask

		}

}
else
{
if(Max_counter_Indx==3-3)
{
	Amr_Out=3;
}else if(Max_counter_Indx==4-3)
{
	Amr_Out=4;
}else if(Max_counter_Indx==5-3)
{
	Amr_Out=5;
}else if(Max_counter_Indx==6-3)
{
	Amr_Out=6;
}else if(Max_counter_Indx==7-3)
{
	Amr_Out=7;
}else if(Max_counter_Indx==8-3)
{
	Amr_Out=8;
}else if(Max_counter_Indx==9-3)
{
	Amr_Out=9;
}else if(Max_counter_Indx==10-3)
{
	Amr_Out=11;
}else if(Max_counter_Indx==11-3)
{
	Amr_Out=10;
}
int Hist_max1,Hist_max,*Hist;Hist=new int[18]; memset(Hist,0,18*sizeof(int));
int *InexSort; InexSort= new int[18];memset(InexSort,0,18*sizeof(int));
int *diff_buff; diff_buff= new int[3];memset(diff_buff,0,3*sizeof(int));
int *diff_buff_sign; diff_buff_sign= new int[5];memset(diff_buff_sign,0,5*sizeof(int));
bool APSK64_Flag=0;

int *AbsHist; AbsHist=new int[18];
	   double *bins;bins=new double[18];
	   double *dis;dis=new double[18];
	   double pMin;
       int pIndx;
	   Ipp64f MagMax;
	   Ipp32s Maxx01;

	   ippsMax_64f(Mag_C,L,&MagMax);
	   MagMax=1.7/MagMax;
	   ippsMulC_64f_I(MagMax,Mag_C,L);


	   for (int k = 0; k < 18; k++)
	   {
		   bins[k]=k*0.1;
	   }
		for (int j = 0; j < L; j++)
		{
			for (int i = 0; i < 18; i++)
			{
			dis[i]=abs(Mag_C[j]-bins[i]);
			}
			

			ippsMinIndx_64f(dis, 18,&pMin,&pIndx);
			Hist[pIndx]=Hist[pIndx]+1;



		}
		
		int maxhist;
		ippsMax_32s(Hist,18,&maxhist);
		


	// -----For Distingush between ASK64 and QAM32 -------- //

	int cont = 0;
	int *buff_ind; buff_ind= new int[17];
	int *buff; buff= new int[17];
	int flag_True=0;

	for (int i = 1; i < 17; i++)
	{
		if ((Hist[i]>=Hist[i-1]) && (Hist[i]>=Hist[i+1]) && (Hist[i]>0))
		{
			buff_ind[cont] = i;
			buff[cont] = Hist[i];
			cont = cont+1;
		}
	}


	ippsSortIndexDescend_32s_I(buff,InexSort,18);


	if ((cont == 4) && (buff[0]>5*buff[3] ))
		APSK64_Flag=1;
	else
		APSK64_Flag=0;

	

	// ---------------------------------------------------//

		double A,B,C,D,E,F,G,H,I,J,K,M;
		A=-18.44*pow(SNR,-2.309)+0.4387;
		B=21.7*pow(SNR,-2.935)+0.06662;
		C=12.69*pow(SNR,-2.443)+0.8123;
		D=7.882*pow(SNR,-2.294)+0.8723;
		E=17.93*pow(SNR,-3.324)+2.676;
		F=13.44*pow(SNR,-2.455)+2.525;
		G=223.5*pow(SNR,-3.9)+0.8736;
		H=2.869*pow(SNR,-1.818)+0.9374;
		I=10.74*pow(SNR,-2.298)+0.743;
		J=9.382*pow(SNR,-2.31)+0.8114;
		K=7.192*pow(SNR,-2.225)+0.8342;
		M=10.54*pow(SNR,-2.346)+0.8;


		if (Cums[13]>24&&Cums[13]<38&&Cums[12]>15&&Cums[12]<31&&Cums[11]>10&&Cums[11]<80&&Cums[10]>0&&Cums[10]<18&&Cums[9]>0&&Cums[9]<60&&Cums[8]>0&&Cums[8]<2.5&&Cums[7]>0.8&&Cums[7]<2&&Cums[6]>0&&Cums[6]<4&&Cums[5]>0&&Cums[5]<4.5&&Cums[4]>0.7&&Cums[4]<0.95&&Cums[3]>0.5&&Cums[3]<1.6&&Cums[2]>0&&Cums[2]<0.16&&Cums[1]>0.48&&Cums[1]<0.55&&Cums[0]>0.2&&Cums[0]<0.55)
		{
			
			int cntr_9qpr=0,cntr_25qpr=0;  //QPR9 or 25QPR  (NEW)
			
			if (abs(Cums[12]-22.98)<abs(Cums[12]-27.04))
				cntr_9qpr=cntr_9qpr+2;
			else
				cntr_25qpr=cntr_25qpr+2;
			
			if (abs(Cums[11]-33.21)<abs(Cums[11]-47.8))
				cntr_9qpr=cntr_9qpr+1;
			else
				cntr_25qpr=cntr_25qpr+1;
			
			if (abs(Cums[8]-1.655)<abs(Cums[8]-1.136))
				cntr_9qpr=cntr_9qpr+1;
			else
				cntr_25qpr=cntr_25qpr+1;
			
			if (abs(Cums[0]-0.3326)<abs(Cums[0]-0.3976))
				cntr_9qpr=cntr_9qpr+1;
			else
				cntr_25qpr=cntr_25qpr+1;
			
			if (/*cntr_9qpr<cntr_25qpr ||*/ (cont > 3))  //(cntr_9qpr<cntr_25qpr && (cont > 3))
				Amr_Out=20; //25QPR
			else
				Amr_Out=19;//9QPR
		}
		//else if (Cums[13]>25&&Cums[13]<40&&Cums[12]>23&&Cums[12]<31&&Cums[11]>25&&Cums[11]<80&&Cums[10]>0&&Cums[10]<20&&Cums[9]>0&&Cums[9]<60&&Cums[8]>0.1&&Cums[8]<2&&Cums[7]>0.7&&Cums[7]<2&&Cums[6]>0&&Cums[6]<5&&Cums[5]>0&&Cums[5]<5&&Cums[4]>0.74&&Cums[4]<1&&Cums[3]>0.8&&Cums[3]<1.6&&Cums[2]>0.02&&Cums[2]<0.13&&Cums[1]>0.48&&Cums[1]<0.55&&Cums[0]>0.32&&Cums[0]<0.52)
		//{
		//	Amr_Out=20;//25QPR
		//}
		else if (Cums[13]>25&&Cums[13]<55&&Cums[12]>0&&Cums[12]<30&&Cums[11]>150&&Cums[11]<300&&Cums[10]>50&&Cums[10]<250&&Cums[9]>150&&Cums[9]<700&&Cums[8]>1.5&&Cums[8]<6&&Cums[7]>0&&Cums[7]<1.5&&Cums[6]>1&&Cums[6]<12&&Cums[5]>0&&Cums[5]<18&&Cums[4]>0.2&&Cums[4]<0.6&&Cums[3]>2&&Cums[3]<3&&Cums[2]>0&&Cums[2]<0.2&&Cums[1]>0.48&&Cums[1]<0.58&&Cums[0]>0.7&&Cums[0]<1)
		{
			Amr_Out=21;//49QPR (NEW)
		}
		else if(Cums[13]>8&&Cums[13]<22&&Cums[12]<8&&Cums[11]<7&&Cums[10]<9&&Cums[9]<16&&Cums[8]>2.4&&Cums[8]<2.8&&Cums[7]<0.8&&Cums[6]<3&&Cums[5]<2.5&&Cums[4]>0.7&&Cums[4]<1&&Cums[3]<0.35&&Cums[2]<0.9&&Cums[1]>0.49&&Cums[1]<0.59&&Cums[0]<0.1)
		{
		if (abs(Cums[2]-A)<abs(Cums[2]-B))
		{
			if (abs(Cums[4]-C)<abs(Cums[4]-D) && (cont < 5))
			Amr_Out=3;//QAM16 
			else
			Amr_Out=5;//QAM64
		
		}
		else if (abs(Cums[8]-E)<abs(Cums[8]-F)&&Cums[4]>=G&&Cums[4]<=H)
		{
			Amr_Out=8;//32apsk
		}
		else if (abs(Cums[4]-I)<abs(Cums[4]-J) && (cont == 2) )
		{
			Amr_Out=7;//16apsk
		}
		else if (abs(Cums[4]-K)<abs(Cums[4]-M) && (cont > 5))
		{
			Amr_Out=6;//128QAM
		}
		else
		{
			
			if  (APSK64_Flag) //(maxhist>350)//(APSK_Flag) //(Hist[3]>=55&&Hist[3]<=120&&Hist[4]>=65&&Hist[4]<=150&&Hist[7]>=60&&Hist[7]<=225&&Hist[8]>=140&&Hist[8]<=290&&Hist[12]>=120&&Hist[12]<=320)
			{
				Amr_Out=9;//64APSK
			}

			else
			{
				Amr_Out=4;//32QAM
			}
		}
		}
		else if (Cums[9]>100&&Cums[10]>50&&Cums[11]>100)
		{
			if(Cums[4]<0.25)
				Amr_Out=11;//4ask
			else
				Amr_Out=10;//2ask

		}
		
		else

				Amr_Out=-1;//Unknown

		/*if (Max_counter_Indx==8)
			Amr_Out=10;*/
}

  Amr_outt=&Amr_Out;

  if (Demod_counters[0]>=50)
	Amr_outt_FH = 0;
  else if (Demod_counters[1]>=50)
	Amr_outt_FH = 1;
  else
	Amr_outt_FH = -1;
	

}
}
