#pragma once
#include "MorseTree.h"

namespace NS_DSP
{

	class CMorseCode
	{
	public:
		
		//some status for decoding CW
		enum STATUSE
		{
			START,
			WORKING,
			FINISH,
			ERR
		};
		STATUSE m_statuse,wordStatuse;

		std::string m_strText;
		std::string m_strCharacter,m_strWord;
		
		std::string m_chBuffer;
		int    m_iBufferLength;
		char   m_chLastCharInPrvsBuff;
		int    m_iZeroCntInLastBuff;
		int    m_iOneCntInLastBuff;
		bool   m_bZeroInLastBuffer;
		bool   m_bOneInLastBuffer;

		CMorseTree<std::string>* m_cMorseTree;

		//Public Functions
		CMorseCode(void);
		~CMorseCode(void);
		CMorseTree<std::string>* getTree();
		void SetCMCData(unsigned char* Buff,int BuffLen);
		void setStatus(STATUSE  );
		std::string Morse2Tex();

		//Private Functions
	private:
		//Depth First Search Function 
		void DFS(CMorseNode<std::string>* Root,std::string chSsearchItem,CMorseNode<std::string>** Result);
		//building Tree
		void enterLettersInTree(CMorseNode<std::string>* node,std::string leftData,std::string RightData);
		void InitializeMorseTree();
		//Search Letterr In Tree
		std::string FindLetter(std::string strLetter);
	};

}//namespace