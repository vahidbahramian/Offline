#pragma once
#include "stdio.h"
#include "math.h"
#include "conio.h"
#include "stdlib.h"
#include <vector>

namespace NS_DSP
{

	class CMC_gen
	{
	public:
		CMC_gen();
		CMC_gen(int);
		~CMC_gen(void);
		void Init(int n);



	//char String_Holder[LENGTH];
	int    GroupLength;
	int    GroupNumber;
	int    bl         ;
	int    m          ;
	int    Duration   ;
	int    MinDuration;
	int    OutLen     ;
	double AverageError;
	int    Min[5];
	int    MinimumError ;
	//std::vector<float> LengthVector;
	//std::vector<int> SampleVector;
	//std::vector<int> OutputVector;
	std::vector<int>& MC_Generator(double* xInputData,int InputLength, int firstTime);

	public:
		std::vector<double> BufferVector;
		std::vector<int>	OutputVector;
	};

}//namespace