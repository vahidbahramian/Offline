#ifndef _DSPLIB_MOVING_AVERAGE
#define _DSPLIB_MOVING_AVERAGE

#pragma once
#include "dsplib_general_func.h"

namespace NS_DSP
{
	//this class computes s(k) = sum(k-N+1:k)/N
	//s(k+1) = s(k) + (x(k)-x(K-N))/N
	template <class T>
	class CMovingAverage
	{
	public:
		CMovingAverage(void)
		{
			m_nWindowSize	=	0; //N
			m_pDelayBuffer	=	0;
			m_nMaxInputSize	=	0;
			m_pOutBuffer	=	0;
			m_tLastOutValue	=	0;
		}

		void Release()
		{
			if (m_pDelayBuffer) delete[] m_pDelayBuffer;
			if (m_pOutBuffer) delete[] m_pOutBuffer;
			m_pDelayBuffer=0;
			m_pOutBuffer=0;
		}

		~CMovingAverage(void)
		{
			Release();
		}
		
		void Reset()
		{
			memset(m_pDelayBuffer,0,(m_nWindowSize+1)*sizeof(T));
			memset(m_pOutBuffer,0,(m_nMaxInputSize+1)*sizeof(T));
			m_tLastOutValue=0;
		}

		BOOL Create(int nMaxInputBufferSize, int nWindowSize)
		{
			ASSERT (nMaxInputBufferSize>0);
			ASSERT (nWindowSize>0);

			m_nMaxInputSize = nMaxInputBufferSize;
			m_nWindowSize   = nWindowSize;
			m_pDelayBuffer	= new T[nWindowSize+1];
			m_pOutBuffer	= new T[nMaxInputBufferSize+1];
			if (!m_pOutBuffer || !m_pDelayBuffer)
			{
				Release(); return DSP_RET_MEMORY_ALLOCATE_ERROR;
			}
			Reset();
			return DSP_RET_SUCCESS;
		}

		BOOL AddBuffer(T *pBuffer,int nInSize,T **pOutBuffer,int &nOutSize)
		{
			*pOutBuffer = m_pOutBuffer+1;
			nOutSize	= nInSize;
			ASSERT (nInSize <= m_nMaxInputSize);
			int i,j;
			m_pOutBuffer[0] = m_tLastOutValue;
			for (i=1,j=0;i<=nInSize;i++,j++)
			{
				m_pDelayBuffer[m_nWindowSize] = pBuffer[j];
				m_pOutBuffer[i] = m_pOutBuffer[j] 
					+ ((m_pDelayBuffer[m_nWindowSize]-m_pDelayBuffer[0])/m_nWindowSize);
				memmove(m_pDelayBuffer,m_pDelayBuffer+1,m_nWindowSize*sizeof(T));
			}
			m_tLastOutValue = m_pOutBuffer[nInSize];
			return TRUE;
		}

		static void Test()
		{
			int i,len;
			int val;
			int buf[20000],oBuf[20000];
			FILE *fp = fopen("C:\\2FSK_TIMING.TXT","rt");
			FILE *ofp = fopen("C:\\moving_avg_OUT.TXT","wt");
			CMovingAverage<int> obj;
			if(!fp){
				printf("cannot open input file\n");
				return;
			}
			if(!ofp){
				printf("cannot open output file\n");
				return;
			}

			BOOL bRet = obj.Create(16384,10);
			DWORD end,st = GetTickCount();
			int *pOutBuffer,nOutSize=0;
			int size=0;
			srand((unsigned int) time(NULL));
			while(!feof(fp))
			{
				int dd=rand()%10000;
				for(i=0;i<dd;i++)
				{
					if(feof(fp)) break;
					fscanf(fp,"%d",&val);
					buf[i]=val;
				}
				size +=i;
				//len = TimingRecovery3(buf,i);
				bRet = obj.AddBuffer(buf,i,&pOutBuffer,nOutSize);
				for (i=0;i<nOutSize;i++){
					//if(pOutBuffer[i]>0) fprintf(ofp,"1\n");
					//else fprintf(ofp,"0\n");
					fprintf(ofp,"%d\n",pOutBuffer[i]);
				}
			}
			end = GetTickCount();
			printf("rate = %f",(double)size * 1000.0f /(double) (end - st));
			fclose(fp);
			fclose(ofp);
		}


	public:
		int		m_nWindowSize; //N
		T		*m_pDelayBuffer;
		int		m_nMaxInputSize;
		T		*m_pOutBuffer;
		T		m_tLastOutValue;
	};


}//namespace

#endif //_DSPLIB_MOVING_AVERAGE
