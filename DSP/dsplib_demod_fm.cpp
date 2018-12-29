#include "stdafx.h"
#include "dsplib_demod_fm.h"

namespace NS_DSP
{
//----------single precision-------------------------
//---------------------------------------------------
CFMvmls::CFMvmls(void)
{
	float e[]= {0.0006380719101204,-0.002205097589591,  -0.0105605382716, -0.01828662259717,
		-0.01363336341862, 0.003368557402947,  0.01374178374312, 0.002384125659935,
		-0.01553735583486, -0.01048665015542,  0.01506356363451,  0.02130864309771,
		-0.0095859684916, -0.03380798713414,-0.003734769205436,  0.04634106552116,
		0.02964673829405, -0.05706550128433, -0.08383220657546,  0.06429366198318,
		0.3105567503963,   0.4331560637015,   0.3105567503963,  0.06429366198318,
		-0.08383220657546, -0.05706550128433,  0.02964673829405,  0.04634106552116,
		-0.003734769205436, -0.03380798713414,  -0.0095859684916,  0.02130864309771,
		0.01506356363451, -0.01048665015542, -0.01553735583486, 0.002384125659935,
		0.01374178374312, 0.003368557402947, -0.01363336341862, -0.01828662259717,
		-0.0105605382716,-0.002205097589591,0.0006380719101204};

	memcpy(m_fFir43Coefs , e, 43*sizeof(float));
	firX = NULL;
	firY = NULL;
	atanOut = NULL;
	atanX1 = atanY1 = NULL;
	atanX2 = atanY2 = NULL;
}

CFMvmls::~CFMvmls(void)
{
	Release();
}

void CFMvmls::Release()
{
	if(firX) delete[] firX;
	if(firY) delete[] firY;

	if(atanOut) delete[] atanOut;
	if(atanX1) delete[] atanX1;
	if(atanY1) delete[] atanY1;
	if(atanX2) delete[] atanX2;
	if(atanY2) delete[] atanY2;
	
	firX = NULL;
	firY = NULL;
	atanOut = NULL;
	atanX1 = atanY1 = NULL;
	atanX2 = atanY2 = NULL;
}

BOOL CFMvmls::Create(int nSize)
{
	int ret;
	if((ret = fir43[0].Create(4096,43,m_fFir43Coefs))!=DSP_RET_SUCCESS)
		return ret;
	if((ret = fir43[1].Create(4096,43,m_fFir43Coefs))!=DSP_RET_SUCCESS)
		return ret;

	firX = new float[nSize]; 
	if(!firX ) {Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	firY = new float[nSize]; 
	if(!firY ){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}

	atanOut = new float[nSize]; 
	if(!atanOut ){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	atanX1 = new float[nSize]; 
	if(!atanX1 ){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	atanY1 = new float[nSize]; 
	if(!atanY1 ){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	atanX2 = new float[nSize]; 
	if(!atanX2 ){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}
	atanY2 = new float[nSize]; 
	if(!atanY2 ){Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;}

	nBulkSize = nSize;
	Reset();
	if((ret = cic.Create(4,4,nSize)) != DSP_RET_SUCCESS) return ret;
	return DSP_RET_SUCCESS;
}

void CFMvmls::Reset()
{
	fir43[0].Reset();fir43[1].Reset();
	memset(firX , 0, nBulkSize * sizeof(float));
	memset(firY , 0, nBulkSize * sizeof(float));
	memset(atanOut , 0, nBulkSize * sizeof(float));
	memset(atanX1 , 0, nBulkSize * sizeof(float));
	memset(atanY1 , 0, nBulkSize * sizeof(float));
	memset(atanX2 , 0, nBulkSize * sizeof(float));
	memset(atanY2 , 0, nBulkSize * sizeof(float));
	firX_old = 0;
	firY_old = 0;
	cic.Reset();
}

int CFMvmls::Demodulate(float *m_inputX,float *m_inputY, float *output, int nSize)
{
	int nOutSizeX,nOutSizeY;
	fir43[0].Filter(m_inputX,nSize,firX,nOutSizeX);
	fir43[1].Filter(m_inputY,nSize,firY,nOutSizeY);
	//memcpy(firX,m_inputX,nSize*sizeof(float));
	//memcpy(firY,m_inputY,nSize*sizeof(float));
	nSize = min(nOutSizeX,nOutSizeY);
	vsMul(nSize-1,firX,firY+1,atanX1+1); //Iold * Q
	atanX1[0] = firX_old * firY[0];
	vsMul(nSize-1,firX+1,firY,atanX2+1); //I * Qold
	atanX2[0] = firY_old * firX[0];

	vsMul(nSize-1,firX,firX+1,atanY1+1); //Iold * I
	atanY1[0] = firX_old * firX[0];
	vsMul(nSize-1,firY+1,firY,atanY2+1); //Q * Qold
	atanY2[0] = firY_old * firY[0];

	vsSub(nSize,atanX1,atanX2,atanX1); // Iold*Q - Qold*I
	vsAdd(nSize,atanY1,atanY2,atanY1); // Iold*I + Qold*Q

	vsAtan2(nSize,atanX1,atanY1,atanOut);
	//	for(int i=0;i<nSize;i++) atanOut[i] = atan2(atanY1[i],atanX1[i]);

	firY_old = firY[nSize-1];
	firX_old = firX[nSize-1];
	return cic.Filter(atanOut,output,nSize);
}

}//namespace