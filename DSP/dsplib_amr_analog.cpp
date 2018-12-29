#include "stdafx.h"
#include "dsplib_amr_analog.h"
#include "dsplib_delay.h"

namespace NS_DSP
{
//Double Precision
//----------------------------//
CHFAnalogAMRvmld::CHFAnalogAMRvmld(void)
{
	m_nFftSize = 0;
	m_pFFTDesc = NULL;
	m_fSamplesFFT = NULL;
	m_fFFTAbs =NULL;
	m_fFFTAbsSum = NULL;
	m_fBM=m_fEnvelope=m_fInstFreq=m_fInstPhase=NULL;
	m_fNL_phase=m_fEnvPow2=NULL;
	m_fEnvelope2 = NULL;
	m_cHilbertOut=NULL;
	m_pFFTDesc = NULL;
	m_pFFTDesc2 = NULL;
	m_pHilbert	=	NULL;
	m_pFirBPF	=	NULL;

}

CHFAnalogAMRvmld::~CHFAnalogAMRvmld(void)
{
	Release();
}

BOOL CHFAnalogAMRvmld::Initialize(int nFftSize)
{
	int ret;
	m_nFftSize = nFftSize;
	//Init FFT------------------------------------
	//-------init forward dft
	long Status = DftiCreateDescriptor( &m_pFFTDesc, DFTI_DOUBLE,DFTI_REAL, 1, nFftSize );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	//	/   Set placement of result DFTI_NOT_INPLACE
	Status = DftiSetValue( m_pFFTDesc, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiSetValue TEST FAIL\n");  
		return DSP_RET_FFT_INIT_ERROR;
	}

	//	/   Commit Dfti descriptor
	Status = DftiCommitDescriptor( m_pFFTDesc );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}

	//-------------------Init FFT 2N points---------------------------
	//-------init forward dft
	Status = DftiCreateDescriptor( &m_pFFTDesc2, DFTI_DOUBLE,DFTI_REAL, 1, nFftSize*2 );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCreateDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}
	//	/   Set placement of result DFTI_NOT_INPLACE
	Status = DftiSetValue( m_pFFTDesc2, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiSetValue TEST FAIL\n");  
		return DSP_RET_FFT_INIT_ERROR;
	}

	///   Commit Dfti descriptor
	Status = DftiCommitDescriptor( m_pFFTDesc2 );
	if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
		//printf(" DftiCommitDescriptor TEST FAIL\n"); 
		return DSP_RET_FFT_INIT_ERROR;
	}

	//-------------Allocate Memory--------------------------------
	m_fSamplesFFT = new MKL_Complex16[m_nFftSize*2];
	if(!m_fSamplesFFT) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fFFTAbs = new double[m_nFftSize];
	if(!m_fFFTAbs) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fFFTAbsSum = new double[m_nFftSize];
	if(!m_fFFTAbsSum) {Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}

	m_pHilbert = new CHilbertTransformd;
	if (!m_pHilbert){Release();return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	if((ret = m_pHilbert->Create(8000))!= DSP_RET_SUCCESS)
		return ret;
	return DSP_RET_SUCCESS;
}

void CHFAnalogAMRvmld::Release()
{
	if (m_fSamplesFFT )  delete[] m_fSamplesFFT;
	if (m_fFFTAbs )  delete[] m_fFFTAbs;
	if (m_fFFTAbsSum )  delete[] m_fFFTAbsSum;
	m_fSamplesFFT = NULL;
	m_fFFTAbs =NULL;
	m_fFFTAbsSum = NULL;

	if (m_fBM )  delete[] m_fBM;
	if (m_fEnvelope )  delete[] m_fEnvelope;
	if (m_fInstFreq )  delete[] m_fInstFreq;
	if (m_fInstPhase )  delete[] m_fInstPhase;
	if(m_fNL_phase)	delete[] m_fNL_phase;
	if(m_fEnvPow2) delete[] m_fEnvPow2;
	if(m_cHilbertOut) delete[] m_cHilbertOut;
	if(m_fEnvelope2) delete[] m_fEnvelope2;
	m_fBM=m_fEnvelope=m_fInstFreq=m_fInstPhase=m_fNL_phase=m_fEnvPow2 =NULL;
	m_cHilbertOut=NULL;m_fEnvelope2=NULL;


	if(m_pFFTDesc) DftiFreeDescriptor(&m_pFFTDesc);
	if(m_pFFTDesc2) DftiFreeDescriptor(&m_pFFTDesc2);
	m_pFFTDesc = NULL;
	m_pFFTDesc2 = NULL;
	if (m_pHilbert)
	{
		m_pHilbert->Release();
		m_pHilbert=NULL;
	}
}

size_t CHFAnalogAMRvmld::CarrierFreqIndex(double *m_fSamples,int m_nNumberSamples)
{
	int i;
	MKL_LONG Status;
	int m_nNumberBlocks = m_nNumberSamples / m_nFftSize;
	//%%%%%%%%%%% first frequecy calculation %%%%%%%%%%%%%%
	memset(m_fFFTAbsSum,0,m_nFftSize/2 * sizeof(double));
	for (i=0;i<m_nNumberBlocks;i++)
	{
		Status = DftiComputeForward( m_pFFTDesc, m_fSamples + (i*m_nFftSize),
			m_fSamplesFFT);
		vzAbs(m_nFftSize/2,m_fSamplesFFT,m_fFFTAbs);
		vdAdd(m_nFftSize/2,m_fFFTAbs,m_fFFTAbsSum,m_fFFTAbsSum);
	}
	size_t m_iMaxIndex = cblas_idamax(m_nFftSize/2,m_fFFTAbsSum,1);
	return m_iMaxIndex;
}

void CHFAnalogAMRvmld::ComputeP(double frequency,double fs,double *m_fSamples,int m_nNumberSamples,double &P,double &PL,double &PU)
{
	int i;
	MKL_LONG Status;
	int m_nNumberBlocks = m_nNumberSamples / (2*m_nFftSize);
	//%%%%%%%%%%% P %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	memset(m_fFFTAbsSum,0,m_nFftSize * sizeof(double));
	for (i=0;i<m_nNumberBlocks;i++)
	{
		Status = DftiComputeForward( m_pFFTDesc2, m_fSamples + (i*2*m_nFftSize),
			m_fSamplesFFT);
		vzAbs(m_nFftSize,m_fSamplesFFT,m_fFFTAbs);
		vdAdd(m_nFftSize,m_fFFTAbs,m_fFFTAbsSum,m_fFFTAbsSum);
	}
	int m_iMaxIndex = cblas_idamax(m_nFftSize,m_fFFTAbsSum,1);

	int fcn=int(frequency*2*m_nFftSize/fs);
	int leng=min(fcn,m_nFftSize-fcn);
	vdPowx(m_nFftSize,m_fFFTAbsSum,2,m_fFFTAbsSum);
	PL = cblas_dasum(leng-8,m_fFFTAbsSum+fcn-leng,1);
	//	for u=fcn-leng+1:fcn-8  % -1 is due to inexact fcn
	//			PL=PL+(abs(spectrum_av1(u))).^2;
	//	end
	PU = cblas_dasum(leng-8,m_fFFTAbsSum+fcn+7,1);
	//	for u=fcn+8:fcn+leng-1
	//		PU=PU+(abs(spectrum_av1(u))).^2; % +1 is due to inexact fcn
	//	end
	P=(PL-PU)/(PL+PU);
}

BOOL CHFAnalogAMRvmld::ComputeGamma(double *m_fSamples,int m_nNumberSamples,double &m_fMeanGamma)
{
	int i,j,m_iMaxIndex;
	MKL_LONG Status;
	int m_nNumberBlocks = (m_nNumberSamples-1024) / m_nFftSize;
	double *m_envelope = new double[m_nFftSize];
	if(!m_envelope) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	double *m_gamma = new double[m_nNumberBlocks];
	if(!m_gamma){delete[] m_envelope; return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	double m_fMeanEnvelope;
	//%%%%%%%%%%% first frequecy calculation %%%%%%%%%%%%%%
	for (i=0;i<m_nNumberBlocks;i++)
	{
		cblas_dcopy(m_nFftSize,m_fSamples +1024+ (i*m_nFftSize),1,m_envelope,1 );
		m_fMeanEnvelope = cblas_dasum(m_nFftSize,m_envelope,1) / m_nFftSize;
		cblas_dscal(m_nFftSize,1/m_fMeanEnvelope,m_envelope,1);
		for(j=0;j<m_nFftSize;j++) m_envelope[j]--;
		Status = DftiComputeForward( m_pFFTDesc, m_envelope,m_fSamplesFFT);
		vzAbs(m_nFftSize/2,m_fSamplesFFT,m_fFFTAbs);
		vdPowx(m_nFftSize/2,m_fFFTAbs,2,m_fFFTAbsSum);
		m_iMaxIndex = cblas_idamax(m_nFftSize/2,m_fFFTAbsSum,1);
		m_gamma[i] = m_fFFTAbsSum[m_iMaxIndex] / m_nFftSize;
	}
	m_fMeanGamma = cblas_dasum(m_nNumberBlocks,m_gamma,1) / m_nNumberBlocks;
	delete[] m_envelope;
	delete[] m_gamma;
	return DSP_RET_SUCCESS;
}

BOOL CHFAnalogAMRvmld::DoAMR(double *m_fSamples,double fs,double fStart,double fStop,int m_nNumberSamples,int &modType)
{//with BPF
	int ret;
	int m_nNumberSamplesFIR=0;
	double *m_fFirOut = new double [2*m_nNumberSamples];
	if(!m_fFirOut) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	m_pFirBPF = new CFIROLAvmld;
	if (!m_pFirBPF) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	if((ret = m_pFirBPF->CreateBPF(8192,1024,fStart,fStop,fs))!=DSP_RET_SUCCESS)
	{
		delete m_pFirBPF;
		m_pFirBPF=NULL;
		return ret;
	}
	ret = m_pFirBPF->Filter(m_fSamples,m_nNumberSamples,m_fFirOut,m_nNumberSamplesFIR);
	if(ret != DSP_RET_SUCCESS) {
		delete m_pFirBPF;
		m_pFirBPF=NULL;
		return ret;
	}
	int res = DoAMR(m_fFirOut,fs,m_nNumberSamplesFIR,modType);
	delete[] m_fFirOut;
	m_pFirBPF->Release();
	delete m_pFirBPF;
	m_pFirBPF=NULL;
	return res;
}


BOOL CHFAnalogAMRvmld::DoAMR(double *m_fSamples,double fs,int m_nNumberSamples,int &modType)
{
	int i;
	double line;
	m_fBM = new double[m_nNumberSamples];
	if(!m_fBM) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	m_fEnvelope = new double[m_nNumberSamples];
	if(!m_fEnvelope) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fInstFreq = new double[m_nNumberSamples];
	if(!m_fInstFreq) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fInstPhase = new double[m_nNumberSamples];
	if(!m_fInstPhase) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fNL_phase = new double[m_nNumberSamples];
	if(!m_fNL_phase) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fEnvPow2 = new double[m_nNumberSamples];
	if(!m_fEnvPow2) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_fEnvelope2 = new double[m_nNumberSamples+10000];
	if(!m_fEnvelope2) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	m_cHilbertOut = new MKL_Complex16[m_nNumberSamples+10000];
	if(!m_cHilbertOut) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	//set MKL mode
	UINT vmlMode = VML_DOUBLE_CONSISTENT;
	//vmlsetmode(&vmlMode);
#ifdef _DSPLIB_SAVE_AMR_INPUT
	FILE *fp1 = fopen(_DSPLIB_SAVE_AMR_INPUT_FILE,"wt");
	for(i=0;i<m_nNumberSamples;i++) fprintf(fp1,"%f\n",m_fSamples[i]);
	fclose(fp1);
#endif

	//%%%%%%%%%%% first frequecy calculation %%%%%%%%%%%%%%
	int fc = CarrierFreqIndex(m_fSamples, m_nNumberSamples);
	double frequency = (fc * fs)/m_nFftSize;
	//%%%%%%%%%%%%%%%%%instantaneous parameter calculation%%%%%%%%%%%%%%%
	//	[bm, env, instp, instf] = gammatone_c(indata,fs,frequency,0);
	BOOL res = GammatoneFiter(m_fSamples,fs,m_nNumberSamples,frequency,0,
		m_fBM,m_fEnvelope,m_fInstFreq,m_fInstPhase);

	if(!res) return DSP_RET_UNKNOWN_ERROR;

	//x=[0:N-1];...
	//so mean(x) = (N-1)/2	&	var(x)=((N-1)*(2*N-1))/6 - (((N-1)^2)/4)
	//sum((x-mx).^2) = (N*(N-1)*(2*N-1))/6 - ((((N-1)^2)/4)*(N))
	double mx = ((m_nNumberSamples-1)/2);//mean(x)
	double fDenom = (double(m_nNumberSamples)*double(m_nNumberSamples-1)*double(2*m_nNumberSamples-1))/6 -
		(double(m_nNumberSamples)*double(mx)*double(mx));
	double m_fMeanInstPhase; 
	if((res = vmldMean(m_fInstPhase,m_nNumberSamples,m_fMeanInstPhase))!=DSP_RET_SUCCESS)
		return res;

	double fNum=0;
	//sum((x-mx).*(y-my))
	for(i=0;i<m_nNumberSamples;i++)
	{
		fNum += ((m_fInstPhase[i]-m_fMeanInstPhase)*(i- mx));
	}
	double b1 = fNum/fDenom;
	double b0= m_fMeanInstPhase-b1*mx;//b0=my(1)-b1*mx(1);...
	//line=b1*[0:length(mx)-1]'+b0*ones(length(mx),1); % regression line
	//	NL_phase = (y-line)';
	for(i=0;i<m_nNumberSamples;i++)
	{
		line = b1 * i + b0;
		m_fNL_phase[i] =  m_fInstPhase[i] - line;
	}
	double m_fMeanNL_phase;
	if((res = vmldMean(m_fNL_phase,m_nNumberSamples,m_fMeanNL_phase))!=DSP_RET_SUCCESS)
		return res;
	//sigma_dp = var(NL_phase);
	m_fSigmaDP=0;
	if((res=vmldVar(m_fNL_phase,m_nNumberSamples,m_fSigmaDP))!=DSP_RET_SUCCESS)
		return res;

	int len=(m_nNumberSamples-1024/m_nFftSize);
	m_fMeanGamma=0;
	//compute evn = abs(hilbert(input))
	int m_iNumHilbertSamples=m_nNumberSamples;
	res = m_pHilbert->Filter(m_fSamples,m_nNumberSamples,m_cHilbertOut,m_iNumHilbertSamples);
	if(res != DSP_RET_SUCCESS) return res;
	vzAbs(m_iNumHilbertSamples,m_cHilbertOut,m_fEnvelope2);
	if((res =ComputeGamma(m_fEnvelope2,m_iNumHilbertSamples,m_fMeanGamma))!=DSP_RET_SUCCESS) 
	{
		return res;
	}

	//	%%%%%%%%%%Ratio of different envelope statistic%%%%%%%%%%%%%%%%%%%%%
	double m_fRNum,m_fRDen;
	double *m_fRArray = new double[m_nNumberSamples/m_nFftSize];
	if(!m_fRArray) return FALSE;
	for (i=0;i<m_nNumberSamples/m_nFftSize;i++)
	{
		vdPowx(m_nFftSize,m_fEnvelope+i*m_nFftSize,2,m_fEnvPow2+i*m_nFftSize);
		if((res= vmldVar(m_fEnvPow2+i*m_nFftSize,m_nFftSize,m_fRNum))!=DSP_RET_SUCCESS)
			return res;
		if((res =vmldMean(m_fEnvPow2+i*m_nFftSize,m_nFftSize,m_fRDen))!=DSP_RET_SUCCESS)
			return res;
		m_fRArray[i] = m_fRNum / (m_fRDen*m_fRDen);//	double m_fR = var (env.^2) / ((mean(env.^2))^2);
	}
	if((res = vmldMean(m_fRArray,m_nNumberSamples/m_nFftSize,m_fR))!=DSP_RET_SUCCESS)
		return res;


	//%%%%%%%%%%%%%%%P%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	ComputeP(frequency,fs,m_fBM+1024,m_nNumberSamples-1024,P,PL,PU);	

	//-------------Decision Switches-------------------------------------
	modType=AMR_ANALOG_UNKNOWN;
	if (m_fSigmaDP < .2){
		modType = AMR_ANALOG_AM;
	}else{//m_fSigmaDP>.2
		if (m_fMeanGamma < 6){
			modType = AMR_ANALOG_FM;
		}else{//m_fMeanGamma >20
			if (m_fR <  2.1 && m_fR>1.105 && abs(P)<.5)
				modType = AMR_ANALOG_DSB;
			else if( m_fR<=1.105){
				if (P < -.01)
					modType = AMR_ANALOG_USB;
				else if( P>=.01)
					modType = AMR_ANALOG_LSB;
				else
					modType = AMR_ANALOG_UNKNOWN;
			}else{
				modType = AMR_ANALOG_UNKNOWN;
			}//m_fR
		}//m_fMeanGamma
	}//m_fSigmaDP
	vmlMode = VML_RESTORE;
	//vmlsetmode(&vmlMode);
	delete[] m_fRArray;
	return DSP_RET_SUCCESS;
}


/*=======================
* Main Function
*=======================
*/
BOOL CHFAnalogAMRvmld::GammatoneFiter(double *m_fSamples,double fs,int m_nNumberSamples,double fc
									,int m_iHRect, double *m_fBM, double *m_fEnvelope,
									double *m_fInstFreq, double *m_fInstPhase)
{
	double *x, *bm, *env, *instp, *instf;
	int t,  nsamples, hrect;
	double cf, a, tpt, tptbw, gain;
	double p0r, p1r, p2r, p3r, p4r, p0i, p1i, p2i, p3i, p4i;
	double a1, a2, a3, a4, a5, u0r, u0i; /*, u1r, u1i;*/
	double qcos, qsin, oldcs, coscf, sincf, oldphase, dp, dps;


	x = m_fSamples;   /* waveform */
	nsamples = m_nNumberSamples;
	cf = fc;   /* centre frequency */

	/*=========================================
	* output arguments
	*=========================================
	*/
	bm = m_fBM;
	env = m_fEnvelope;
	instf = m_fInstFreq;
	instp = m_fInstPhase;
	hrect = m_iHRect;

	/*=========================================
	* Initialising variables
	*=========================================
	*/
	oldphase = 0.0;
	tpt = ( M_PI + M_PI ) / fs;
	tptbw = tpt * erb ( cf ) * BW_CORRECTION;
	a = exp ( -tptbw );

	/* based on integral of impulse response */
	gain = ( tptbw*tptbw*tptbw*tptbw ) / 3;

	/* Update filter coefficiants */
	a1 = 4.0*a; a2 = -6.0*a*a; a3 = 4.0*a*a*a; a4 = -a*a*a*a; a5 = 4.0*a*a;
	p0r = 0.0; p1r = 0.0; p2r = 0.0; p3r = 0.0; p4r = 0.0;
	p0i = 0.0; p1i = 0.0; p2i = 0.0; p3i = 0.0; p4i = 0.0;

	/*===========================================================
	* exp(a+i*b) = exp(a)*(cos(b)+i*sin(b))
	* q = exp(-i*tpt*cf*t) = cos(tpt*cf*t) + i*(-sin(tpt*cf*t))
	* qcos = cos(tpt*cf*t)
	* qsin = -sin(tpt*cf*t)
	*===========================================================
	*/
	coscf = cos ( tpt * cf );
	sincf = sin ( tpt * cf );
	qcos = 1; qsin = 0;   /* t=0 & q = exp(-i*tpt*t*cf)*/
	for ( t=0; t<nsamples; t++ )
	{
		/* Filter part 1 & shift down to d.c. */
		p0r = qcos*x[t] + a1*p1r + a2*p2r + a3*p3r + a4*p4r;
		p0i = qsin*x[t] + a1*p1i + a2*p2i + a3*p3i + a4*p4i;

		/* Filter part 2 */
		u0r = p0r + a1*p1r + a5*p2r;
		u0i = p0i + a1*p1i + a5*p2i;

		/* Update filter results */
		p4r = p3r; p3r = p2r; p2r = p1r; p1r = p0r;
		p4i = p3i; p3i = p2i; p2i = p1i; p1i = p0i;

		/*==========================================
		* Basilar membrane response
		* 1/ shift up in frequency first: (u0r+i*u0i) * exp(i*tpt*cf*t) = (u0r+i*u0i) * (qcos + i*(-qsin))
		* 2/ take the real part only: bm = real(exp(j*wcf*kT).*u) * gain;
		*==========================================
		*/
		bm[t] = ( u0r * qcos + u0i * qsin ) * gain;
		if ( 1 == hrect && bm[t] < 0 ) {
			bm[t] = 0;  /* half-wave rectifying */
		}
		/*==========================================
		* Instantaneous Hilbert envelope
		* env = abs(u) * gain;
		*==========================================
		*/
		//	if ( nlhs > 1 ) {
		env[t] = sqrt ( u0r * u0r + u0i * u0i ) * gain;
		//	}
		/*==========================================
		* Instantaneous phase
		* instp = unwrap(angle(u));
		*==========================================
		*/
		//	if (nlhs > 2) {
		instp[t] = atan2 ( u0i, u0r );
		/* unwrap it */
		dp = instp[t] - oldphase;
		if ( abs ( dp ) > M_PI ) {
			dps = myMod ( dp + M_PI, 2 * M_PI) - M_PI;
			if ( dps == -M_PI && dp > 0 ) {
				dps = M_PI;
			}
			instp[t] = instp[t] + dps - dp;
		}
		oldphase = instp[t];
		//}
		/*==========================================
		* Instantaneous frequency
		* instf = cf + [diff(instp) 0]./tpt;
		*==========================================
		*/
		//if ( nlhs > 3 && t > 0 ) {
		if(t>0){
			instf[t-1] = cf + ( instp[t] - instp[t-1] ) / tpt;
		}

		/*====================================================
		* The basic idea of saving computational load:
		* cos(a+b) = cos(a)*cos(b) - sin(a)*sin(b)
		* sin(a+b) = sin(a)*cos(b) + cos(a)*sin(b)
		* qcos = cos(tpt*cf*t) = cos(tpt*cf + tpt*cf*(t-1))
		* qsin = -sin(tpt*cf*t) = -sin(tpt*cf + tpt*cf*(t-1))
		*====================================================
		*/
		qcos = coscf * ( oldcs = qcos) + sincf * qsin;
		qsin = coscf * qsin - sincf * oldcs;
	}
	//	if ( nlhs > 3 ) {
	instf[nsamples-1] = cf;
	//	}
	return TRUE;
}


}//namespace

///////////////////////////////////////////////////////////////////////


//power estimation for validity of input signal
BOOL IsValidAMRSignal(float *m_fSamples,int m_nSize,float m_fThreshold,int &result)
{
	int ret;
	result = FALSE;
	//AR filter for smoothing :	y(n) = (1-b)*y(n-1) + b*x(n)^2
	float ynD1=0,yn=0;//y(n-1),y(n)
	int i;
	float b = 1.0f/1024.0f;
	float a = 1-b;
	for (i=0;i<m_nSize;i++)
	{
		yn = a* ynD1 + b * (m_fSamples[i] * m_fSamples[i]);
		m_fSamples[i] = yn;//use the same buffer for y(n)
		ynD1 = yn; 
	}
	//MA filter for smoothing : y(n) = sum(x(n-1023:n))/1024
	float delay=0;
	NS_DSP::CDelayLines *del1024 = new NS_DSP::CDelayLines;
	if(!del1024) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	if((ret = del1024->Create(2048))!=DSP_RET_SUCCESS) 
		return ret;
	for(i=0;i<m_nSize;i++)
	{
		yn = delay / 8192.0f;
		delay = m_fSamples[i] - del1024->Delay(m_fSamples[i]) + delay;
		m_fSamples[i] = yn;//use the same buffer
	}

	//test
	//	FILE *fp = fopen("C:\\samples\\MA.txt","wt");
	//	for(i=0;i<m_nSize;i++) fprintf(fp,"%f\n",m_fSamples[i]);
	//	fclose(fp);
	//test

	//compute std variation for each block 8192
	int j,blk = m_nSize / 8192;
	float mean,ms;
	for (i=0;i<blk;i++)
	{
		if(i+8192<m_nSize){
			if((ret=vmlsMean(m_fSamples + i*8192,8192,mean))!=DSP_RET_SUCCESS)
				return ret;
			ms=0;
			for(j=0;j<8192;j++)	{
				//m_fSamples[i*8192+j] = abs(m_fSamples[i*8192+j] - mean);//s
				//find max sample
				if(ms<m_fSamples[i*8192+j]) ms = m_fSamples[i*8192+j];
			}
			m_fSamples[i]=ms;//store maximums
		}else{
			if((ret=vmlsMean(m_fSamples + i*8192,m_nSize-i,mean))!=DSP_RET_SUCCESS)
				return ret;
			ms=0;
			for(j=0;j<m_nSize-i;j++){
				//m_fSamples[i*8192+j] = abs(m_fSamples[i*8192+j] - mean);//s
				//find max sample
				if(ms<m_fSamples[i*8192+j]) ms = m_fSamples[i*8192+j];
			}
			m_fSamples[i]=ms;//store maximums
		}
	}
#ifdef _DSPLIB_POWER_EST_SAVE_S
	FILE *fp = fopen(_DSPLIB_POWER_EST_SAVE_FILE,"wt");
	for(i=0;i<m_nSize;i++) fprintf(fp,"%.10f\n",m_fSamples[i]);
	fclose(fp);
#endif
	//check for validity
	result = TRUE;
	for(i=0;i<blk;i++)
		if(m_fSamples[i] < m_fThreshold) result = FALSE;//1e-5
	delete del1024;
	return DSP_RET_SUCCESS;
}

//power estimation for validity of input signal
BOOL IsValidAMRSignal(double *m_fSamples,int m_nSize ,double m_fThreshold,int &result)
{
	//AR filter for smoothing :	y(n) = (1-b)*y(n-1) + b*x(n)^2
	double ynD1=0,yn=0;//y(n-1),y(n)
	int i;
	result = FALSE;
	int ret;
	double b = 1.0f/1024.0f;
	double a = 1-b;
	for (i=0;i<m_nSize;i++)
	{
		yn = a* ynD1 + b * (m_fSamples[i] * m_fSamples[i]);
		m_fSamples[i] = yn;//use the same buffer for y(n)
		ynD1 = yn; 
	}
	//MA filter for smoothing : y(n) = sum(x(n-1023:n))/1024
	double delay=0;
	NS_DSP::CDelayLined *del1024 = new NS_DSP::CDelayLined;
	if(!del1024) return DSP_RET_MEMORY_ALLOCATE_ERROR;
	if((ret = del1024->Create(2048))!= DSP_RET_SUCCESS) 
		return ret;
	for(i=0;i<m_nSize;i++)
	{
		yn = delay / 8192.0f;
		delay = m_fSamples[i] - del1024->Delay(m_fSamples[i]) + delay;
		m_fSamples[i] = yn;//use the same buffer
	}

	//test
	//	FILE *fp = fopen("C:\\samples\\MA.txt","wt");
	//	for(i=0;i<m_nSize;i++) fprintf(fp,"%f\n",m_fSamples[i]);
	//	fclose(fp);
	//test

	//compute std variation for each block 8192
	int j,blk = m_nSize / 8192;
	double mean,ms;
	for (i=0;i<blk;i++)
	{
		if(i+8192<m_nSize){
			if((ret=vmldMean(m_fSamples + i*8192,8192,mean))!=DSP_RET_SUCCESS)
				return ret;
			ms=0;
			for(j=0;j<8192;j++)	{
				//m_fSamples[i*8192+j] = abs(m_fSamples[i*8192+j] - mean);//s
				//find max sample
				if(ms<m_fSamples[i*8192+j]) ms = m_fSamples[i*8192+j];
			}
			m_fSamples[i]=ms;//store maximums
		}else{
			if((ret=vmldMean(m_fSamples + i*8192,m_nSize-i,mean))!=DSP_RET_SUCCESS)
				return ret;
			ms=0;
			for(j=0;j<m_nSize-i;j++){
				//m_fSamples[i*8192+j] = abs(m_fSamples[i*8192+j] - mean);//s
				//find max sample
				if(ms<m_fSamples[i*8192+j]) ms = m_fSamples[i*8192+j];
			}
			m_fSamples[i]=ms;//store maximums
		}
	}
#ifdef _DSPLIB_POWER_EST_SAVE_S
	FILE *fp = fopen(_DSPLIB_POWER_EST_SAVE_FILE,"wt");
	for(i=0;i<m_nSize;i++) fprintf(fp,"%.10f\n",m_fSamples[i]);
	fclose(fp);
#endif
	//check for validity
	result = TRUE;
	for(i=0;i<blk;i++)
		if(m_fSamples[i] < m_fThreshold) result = FALSE;//1e-5
	delete del1024;
	return DSP_RET_SUCCESS;
}
