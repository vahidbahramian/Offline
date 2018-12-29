#include "StdAfx.h"
#include "MorseCode_gen.h"

namespace NS_DSP
{

	CMC_gen::CMC_gen()
	{
		CMC_gen(0);
	}

	CMC_gen::CMC_gen(int InputLength)
	{
		Init( InputLength);
	}

	void CMC_gen::Init(int InputLength)
	{

		GroupLength   = 0;
		GroupNumber   = 0;
		bl            = 1;
		m             = 0;
		Duration      = 0;
		AverageError  = 0;
		Min[0]        = InputLength;
		Min[1]        = InputLength;
		Min[2]        = InputLength;
		Min[3]        = InputLength;
		Min[4]        = InputLength;
		MinimumError  = InputLength;
		OutLen        = 0;
		MinDuration   = InputLength;
	}

	CMC_gen::~CMC_gen(void)
	{
	}
	//float min_FS(std::vector<float> v,int nn,int mm)
	//{
	//	float    mindata;
	//	long    i;
	//
	//	mindata=v[nn-1];
	//	for (i=nn-1;i<mm;i++) {
	//		if (v[i]<mindata) {
	//			mindata=v[i];
	//		}
	//	}
	//	return mindata;
	//}

	std::vector<int>& CMC_gen::MC_Generator(double* InputData,int InputLength, int firstTime)
	{
		OutputVector.clear();
		double * pData = new double[InputLength + BufferVector.size()];
		double * pTmpData =pData;
	//	if(BufferVector.size()>0)
	//	 memcpy((void*)pData,(void*)&BufferVector[0],sizeof(double)*BufferVector.size());
	///
	///	
	///	memcpy((void*)(pData+ BufferVector.size()),(void*)InputData ,sizeof(double)*InputLength);
	//

		for(size_t i=0;i<BufferVector.size();i++)
			pData[i]=BufferVector[i];

		for (size_t i=0;i<InputLength;i++)
		{
			pData[BufferVector.size()+i]= InputData[i];
		}
		InputLength += BufferVector.size();

		BufferVector.clear();

		std::vector<float> LengthVector;
		std::vector<int> SampleVector;

		GroupLength  = 0;
		GroupNumber  = 0;
		bl           = 1;
		m            = 0;
		Duration     = 0;
		AverageError = 0;
		MinimumError = InputLength;
		OutLen       = 0;
		MinDuration  = InputLength;
		Min[0]       = InputLength;
		Min[1]       = InputLength;
		Min[2]       = InputLength;
		Min[3]       = InputLength;
		Min[4]       = InputLength;


	for (int ii=0; ii<InputLength ; ii++ )
	{    
		//CMC_gen.m
		if (ii >= m)
		{
			m=ii;
			while(bl==1)
			{
				if(m<InputLength)
				{
					if (pData[m+1]!= pData[m])
					{
						bl=0;
						GroupNumber = GroupNumber+1;		
					}
				}					
				else
				{
					bl =0;
					GroupNumber = GroupNumber+1;	
				}
				m++;
				GroupLength++;
			}

			LengthVector.push_back(GroupLength);
			SampleVector.push_back(pData[m-1]);
			GroupLength=1;
			bl=1;				
				}
	}
	int Size = LengthVector.size(); 
	/////////////////////////////////////////////////////////////////////////////////////////
	for(int i=0;i<5;i++)
	{
		for (int j=i*(Size/5); j<(i+1)*(Size/5); j++ )
		{
			if ((LengthVector[j]<Min[i])&&(LengthVector[j]>30)&&(j>0)&&(j<Size-1))
				Min[i] = LengthVector[j];	
		}
		//float Min[i]=min_FS(LengthVector,i*(Size/5)+2,(i+1)*(Size/5)-1);

	}


	///////////////////////////////////////////////////////////////////////////
	static int mMin;
	if( firstTime == 1)
	{
		mMin=Min[3];
		float ave=(Min[0]+Min[1]+Min[2]+Min[3]+Min[4])/5.0;
		for (int j=0; j<5; j++ )
		{
			if ((fabs((float)(ave-Min[j]))<fabs((float)(ave-mMin)))&&(Min[j]>ave))
			{
				mMin=Min[j];
			}
		}
		MinDuration=mMin;
	}
	MinDuration = mMin;
	/////////////////////////////////////////////////////////////////////////////
	for (int k=-20; k<100 ; k++)
	{
		Duration     = mMin+k;
		AverageError =0;
		for (int l=1; l<Size-1; l++) 
			AverageError+= fabs(((floor(((float)LengthVector[l]/Duration)+0.5))*Duration)-LengthVector[l]);
		
		if (AverageError<MinimumError)
		{
			MinimumError= AverageError;
			MinDuration = Duration;
		}

	}

	int Len=0;

	for (int n=0; n<Size-1; n++)
	{
		Len=floor((LengthVector[n]/MinDuration)+0.5);

		while(Len>0)
		{
			OutputVector.push_back(SampleVector[n]);
			Len--;
		}
	}

	OutLen = OutputVector.size();


	for (int q=1; q<LengthVector[Size-1]; q++)
		BufferVector.push_back(SampleVector[Size-1]);

	if(pData)
	   delete[] pData;
	pData =0;

	return OutputVector;
	}//func

}//namespace