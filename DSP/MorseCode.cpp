#include "StdAfx.h"
#include "MorseCode.h"
#include <string>

namespace NS_DSP
{


	CMorseCode::CMorseCode(void)
	{
		m_iBufferLength=0;
		m_iZeroCntInLastBuff=0;
		m_iOneCntInLastBuff=0;
		m_bZeroInLastBuffer=0;
		m_bOneInLastBuffer=0;

		m_strCharacter="";
		m_strWord="";

		wordStatuse=START;
		m_statuse=START;
		InitializeMorseTree();
	}

	CMorseCode::~CMorseCode(void)
	{
		SAFERELEASE(m_cMorseTree);
	}
	//////////////////////////////////////////////////////////

	//This Function Create a Standard Morse Tree 
	//Refrence For This Tree:Wikipedia
	void CMorseCode::InitializeMorseTree()
	{
		CMorseNode<std::string> * pRootNode=0;
		m_cMorseTree=new CMorseTree<std::string>("");
		CMorseNode<std::string> * pResultNode=0;
		pRootNode= m_cMorseTree->get_Root();

		enterLettersInTree(pRootNode,"E","T");

		DFS(pRootNode,"E",&pResultNode);
		enterLettersInTree(pResultNode,"I","A");


		pResultNode=NULL;
		DFS(pRootNode,"T",&pResultNode);
		enterLettersInTree(pResultNode,"N","M");

		pResultNode=NULL;
		DFS(pRootNode,"I",&pResultNode);
		enterLettersInTree(pResultNode ,"S","U");

		pResultNode=NULL;
		DFS(pRootNode,"A",&pResultNode);
		enterLettersInTree(pResultNode,"R","W");

		pResultNode=NULL;
		DFS(pRootNode,"N",&pResultNode);
		enterLettersInTree(pResultNode,"D","K");

		pResultNode=NULL;
		DFS(pRootNode,"M",&pResultNode);
		enterLettersInTree(pResultNode,"G","O");

		pResultNode=NULL;
		DFS(pRootNode,"S",&pResultNode);
		enterLettersInTree(pResultNode,"H","V");

		pResultNode=NULL;
		DFS(pRootNode,"U",&pResultNode);
		enterLettersInTree(pResultNode,"F","^");

		pResultNode=NULL;
		DFS(pRootNode,"R",&pResultNode);
		enterLettersInTree(pResultNode,"L","^");

		pResultNode=NULL;
		DFS(pRootNode,"W",&pResultNode);
		enterLettersInTree(pResultNode,"P","J");

		pResultNode=NULL;
		DFS(pRootNode,"D",&pResultNode);
		enterLettersInTree(pResultNode ,"B","X");

		pResultNode=NULL;
		DFS(pRootNode,"K",&pResultNode);
		enterLettersInTree(pResultNode ,"C","Y");

		pResultNode=NULL;
		DFS(pRootNode,"G",&pResultNode);
		enterLettersInTree(pResultNode ,"Z","Q");

		pResultNode=NULL;
		DFS(pRootNode,"O",&pResultNode);
		enterLettersInTree(pResultNode ,"^","^");

		pResultNode=NULL;
		DFS(pRootNode,"H",&pResultNode);
		enterLettersInTree(pResultNode ,"5","4");

		pResultNode=NULL;
		DFS(pRootNode,"V",&pResultNode);
		enterLettersInTree(pResultNode ,"^","3");

		pResultNode=NULL;
		DFS(pRootNode,"F",&pResultNode);
		enterLettersInTree(pResultNode ,"^","^");

		pResultNode=NULL;
		DFS(pRootNode,"U",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"^","2");

		pResultNode=NULL;
		DFS(pRootNode,"L",&pResultNode);
		enterLettersInTree(pResultNode ,"&","^");

		pResultNode=NULL;
		DFS(pRootNode,"R",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"+","^");

		pResultNode=NULL;
		DFS(pRootNode,"P",&pResultNode);
		enterLettersInTree(pResultNode ,"^","^");

		pResultNode=NULL;
		DFS(pRootNode,"J",&pResultNode);
		enterLettersInTree(pResultNode ,"^","1");

		pResultNode=NULL;
		DFS(pRootNode,"B",&pResultNode);
		enterLettersInTree(pResultNode ,"6","=");

		pResultNode=NULL;
		DFS(pRootNode,"X",&pResultNode);
		enterLettersInTree(pResultNode ,"/","^");

		pResultNode=NULL;
		DFS(pRootNode,"C",&pResultNode);
		enterLettersInTree(pResultNode ,"^","START SIGNAL");

		pResultNode=NULL;
		DFS(pRootNode,"Y",&pResultNode);
		enterLettersInTree(pResultNode ,"(","^");

		pResultNode=NULL;
		DFS(pRootNode,"Z",&pResultNode);
		enterLettersInTree(pResultNode ,"7","^");

		pResultNode=NULL;
		DFS(pRootNode,"Q",&pResultNode);
		enterLettersInTree(pResultNode ,"^","^");

		pResultNode=NULL;
		DFS(pRootNode,"O",&pResultNode);
		enterLettersInTree(pResultNode->m_pLeftChild,"8","^");

		pResultNode=NULL;
		DFS(pRootNode,"O",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"9","0");

		pResultNode=NULL;
		DFS(pRootNode,"V",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"^","^");

		pResultNode=NULL;
		DFS(pRootNode,"U",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild->m_pLeftChild,"?","_");

		pResultNode=NULL;
		DFS(pRootNode,"L",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"\"","^");

		pResultNode=NULL;
		DFS(pRootNode,"+",&pResultNode);
		enterLettersInTree(pResultNode ,"^",".");

		pResultNode=NULL;
		DFS(pRootNode,"P",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"@","^");

		pResultNode=NULL;
		DFS(pRootNode,"1",&pResultNode);
		enterLettersInTree(pResultNode ,",","^");

		pResultNode=NULL;
		DFS(pRootNode,"6",&pResultNode);
		enterLettersInTree(pResultNode ,"^","-");

		pResultNode=NULL;
		DFS(pRootNode,"C",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,";","!");

		pResultNode=NULL;
		DFS(pRootNode,"(",&pResultNode);
		enterLettersInTree(pResultNode ,"^",")");

		pResultNode=NULL;
		DFS(pRootNode,"Z",&pResultNode);
		enterLettersInTree(pResultNode->m_pRightChild,"^","\'");

		pResultNode=NULL;
		DFS(pRootNode,"8",&pResultNode);
		enterLettersInTree(pResultNode ,":","^");

		pResultNode=NULL;
		DFS(pRootNode,"V",&pResultNode);
		enterLettersInTree(pResultNode->m_pLeftChild,"^","^");

		pResultNode=NULL;
		DFS(pRootNode,"V",&pResultNode);
		enterLettersInTree(pResultNode->m_pLeftChild->m_pLeftChild,"^","$");
		return;
	}

	//This Function Write Data In Left & Right Children
	void CMorseCode::enterLettersInTree(CMorseNode<std::string> * node,const std::string leftData,const std::string RightData)
	{
		CMorseNode<std::string>* pLChild;
		pLChild=new CMorseNode<std::string>;
		pLChild->Data=leftData;

		CMorseNode<std::string> * pRChild;
		pRChild=new CMorseNode<std::string>;
		pRChild->Data=RightData;

		node->m_pLeftChild=pLChild;
		node->m_pRightChild=pRChild;
		return;
	}


	//This Function Search Tree With A Recursion Mechanism.
	//It gives Root Of The Tree & Searching Item & Return Its Location IN Its Result Parameter.
	void CMorseCode::DFS(CMorseNode<std::string>* Root,std::string chSsearchItem,CMorseNode<std::string>** Result)
	{
		if(Root->Data.find(chSsearchItem)!=-1)
		{
			*Result=Root;
			return ;
		}
		if(Root->m_pLeftChild && *Result==NULL )
			DFS(Root->m_pLeftChild,chSsearchItem,Result);
		if(Root->m_pRightChild && *Result==NULL )
			DFS(Root->m_pRightChild,chSsearchItem,Result);
		return ;
	}
	///////////////////////////////////////////////////////////
	//This Function Return A Tree Class In OutPut
	CMorseTree<std::string>*  CMorseCode::getTree()
	{
		return m_cMorseTree;
	}
	//////////////////////////////////////////////////////////
	//This Function Get Buffer In Morse Code & Save It In A 
	//Buffer & Its Length In A Variable In Class
	void CMorseCode::SetCMCData(unsigned char* Buff,int BuffLen)
	{
		m_chBuffer.resize(BuffLen);
		memcpy(&m_chBuffer[0],Buff,BuffLen);
		m_iBufferLength=BuffLen;
		return;
	}
	//////////////////////////////////////////////////////////
	//Heat Of The Program.
	//This Section Decode Morse Code & Save It In A std::string
	std::string CMorseCode::Morse2Tex()
	{
		m_strText="";
		int numZero=0;
		int cntOne=0;
		std::string strSentence="";
		int i=0;
		/******************    Complete Last Sentence In Last Buffer     **************************/
		if(m_statuse==START)
		{
			m_statuse=WORKING;
			for(i=0;m_chBuffer[i]=='0';i++);
			m_iZeroCntInLastBuff=0;
			m_iOneCntInLastBuff=0;
		}
		else if(m_statuse==WORKING || m_statuse==FINISH)
		{
			m_bZeroInLastBuffer=m_bOneInLastBuffer=TRUE;

			if(m_chBuffer[0]=='0' && m_chLastCharInPrvsBuff=='0')
			{
				for(i=0;m_chBuffer[i]=='0';i++);
				m_iZeroCntInLastBuff+=i;

				cntOne=m_iOneCntInLastBuff;
				numZero=m_iZeroCntInLastBuff;
			}
			else if(m_chBuffer[0]=='0' && m_chLastCharInPrvsBuff=='1')
			{
				int lasti=i;
				for(;m_chBuffer[i]=='0';i++);

				numZero=i-lasti;
				cntOne=m_iOneCntInLastBuff;
			}
			else if(m_chBuffer[0]=='1' && m_chLastCharInPrvsBuff=='1')
			{
				for(i=0;m_chBuffer[i]=='1';i++);
				m_iOneCntInLastBuff+=i;

				cntOne=m_iOneCntInLastBuff;
				m_bZeroInLastBuffer=FALSE;

			}
			else if(m_chBuffer[0]=='1' && m_chLastCharInPrvsBuff=='0')
			{
				cntOne=m_iOneCntInLastBuff;
				numZero=m_iZeroCntInLastBuff;
			}
		}

		/******************End OF: Complete Last Sentence In Last Buffer **************************/
		while(i< m_iBufferLength)
		{
			//counting one
			if(m_bOneInLastBuffer==FALSE)
			{
				std::string::size_type pos=m_chBuffer.find('0',i);
				if(pos!=-1)
					cntOne=pos-i;
				else 
					m_iOneCntInLastBuff=cntOne=m_iBufferLength-i;
				i+=cntOne;
			}

			//counting zero
			if(m_bZeroInLastBuffer==FALSE && i<m_iBufferLength)
			{
				int lasti=i;
				for(;m_chBuffer[i]=='0';i++);
				numZero=i-lasti;
			}
			if(m_bOneInLastBuffer==TRUE || m_bZeroInLastBuffer==TRUE)
			{
				m_bZeroInLastBuffer=m_bOneInLastBuffer=FALSE;
			}
	/************************************************************************/
	/*							Finding Related std::string                      */
	/************************************************************************/
			if(i<m_iBufferLength)
			{
				//if Number Of Zeros Is 1 Then We Have A Dot Or Dash That Create A Character
				//A Dot Is One 1 & A Dash Is Three 1
				if(numZero==1)
				{
					if(cntOne==1)
						m_strCharacter+='.';//Dot
					else if(cntOne==3)
						m_strCharacter+='-';//Dash
					//Oops.Error!
					//If We Have Any Character Later Add It To The std::string
					else if(m_strCharacter!="")
					{
						//Finding Letter Related To Dots & Dashes That We Have Until Now
						m_strCharacter=FindLetter(m_strCharacter);
						if(m_strCharacter.compare("")!=0 && m_strCharacter.compare("^")!=0)
							m_strWord+=m_strCharacter;
						m_strCharacter="";
						strSentence+=m_strWord;

						if(cntOne!=1 && cntOne!=3)
						{
							if(wordStatuse==START)
								wordStatuse=WORKING;
							strSentence+=" ";
						}

						m_strWord="";
						wordStatuse=WORKING;
					}
				}
				//If Number Of Zero Is Three,We Find A Letter
				else if (numZero==3)
				{
					if(cntOne==1)
						m_strCharacter+='.';//Dot
					else if(cntOne==3)
						m_strCharacter+='-';//Dash
					if(m_strCharacter!="")
					{
						m_strCharacter=FindLetter(m_strCharacter);
						if(m_strCharacter.compare("")!=0 && m_strCharacter.compare("^")!=0)
						m_strWord+=m_strCharacter;
						m_strCharacter="";
						
						strSentence+=m_strWord;
						if(cntOne!=1 && cntOne!=3)
						{
							if(wordStatuse==START)
								wordStatuse=WORKING;
							strSentence+=" ";
						}


						m_strWord="";
						wordStatuse=WORKING;
					//	continue;
					}
				}
				//If Number Of Zero Is Three,We Find A Word
				else if (numZero==5)
				{
					if(cntOne==1)
						m_strCharacter+='.';//Dot
					else if(cntOne==3)
						m_strCharacter+='-';//Dash

					if(m_strCharacter!="")
					{
						m_strCharacter=FindLetter(m_strCharacter);
						if(m_strCharacter.compare("")!=0 && m_strCharacter.compare("^")!=0)
						m_strWord+=m_strCharacter;
						m_strCharacter="";
						strSentence+=m_strWord;
						strSentence+=" ";		
					}
					m_strWord="";
					wordStatuse=WORKING;
				}
				//If Number Of Zero Is Seven Or More Than It Not Equal To Eight,We Find A Sentence
				else if(numZero!=8 && numZero>6)
				{
					if(cntOne==1)
						m_strCharacter+='.';//Dot
					else if(cntOne==3)
						m_strCharacter+='-';//Dash
					if(m_strCharacter!="")
					{

						m_strCharacter=FindLetter(m_strCharacter);
						if(m_strCharacter.compare("")!=0 && m_strCharacter.compare("^")!=0)
						m_strWord+=m_strCharacter;
						m_strCharacter="";
						
						strSentence+=m_strWord;
						if(cntOne!=1 && cntOne!=3)
						{
							if(wordStatuse==START)
								wordStatuse=WORKING;
							strSentence+=" ";
						}

						if(strSentence.find(" ")==0)
							strSentence.erase(0,1);
						m_strText+=strSentence;
						m_strText+=".";
					}
					m_strWord="";
					strSentence="";
				}
				//If Number Of Zero Is Eight,We Have An Error In Data.
				else if(numZero==8)
					m_statuse=ERR;

				//Any Other Issue. Only For Relaibility.
				else
				{
					if(cntOne==1)
						m_strCharacter+='.';//Dot
					else if(cntOne==3)
						m_strCharacter+='-';//Dash

					if(m_strCharacter!="")
					{
						m_strCharacter=FindLetter(m_strCharacter);
						if(m_strCharacter.compare("")!=0 && m_strCharacter.compare("^")!=0)
						m_strWord+=m_strCharacter;
						m_strCharacter="";

						strSentence+=m_strWord;			
						if(cntOne!=1 && cntOne!=3)
						{
							if(wordStatuse==START)
								wordStatuse=WORKING;
							strSentence+=" ";
						}

						m_strWord="";
						wordStatuse=WORKING;
					}
				//	i++;
				}
				
			}
			//end Of Buffer.
			//save Number Of Last Zeros & Number Of Last Ones In Last Buffer.
			else
			{
				m_iZeroCntInLastBuff=numZero;
				m_iOneCntInLastBuff=cntOne;
			}
		}
		//If Operation Is Finished And We Have Not Any Other Buffer
		//Save All Data In Letters Words & Sentences.
		if(m_statuse==FINISH)
		{
			if(m_strCharacter!="")
			{
				if(m_iOneCntInLastBuff==1)
					m_strCharacter+='.';
				else if(m_iOneCntInLastBuff==3)
					m_strCharacter+='-';
				m_strCharacter=FindLetter(m_strCharacter);
				if(m_strCharacter.compare("")!=0 && m_strCharacter.compare("^")!=0)
				m_strWord+=m_strCharacter;
			}
			if(m_strWord!="")
			{
				strSentence+=m_strWord;
				strSentence+=" ";
			}
			if(strSentence!="")
			{
				m_strText+=strSentence;
				m_strText+=".";
			}
			m_iZeroCntInLastBuff=m_iOneCntInLastBuff=0;
		}
		//If Operation Is Continuing Add Last Sentence To The Text If There Is Any,
		//And Keep Letters And Words For Completing With Data In Next Buffer.  
		else if(strSentence!="" )
		{
			m_strText+=strSentence;
		}
		/****************************************/
		//Save Last Data InLast Buffer It Is 1 Or 0.
		if(m_chBuffer[m_iBufferLength-1]=='0')
			m_chLastCharInPrvsBuff='0';
		else
			m_chLastCharInPrvsBuff='1';

		return m_strText ;
	}
	//This Function Gte Some Dots & Dashes & Return Its Related Letter
	std::string CMorseCode::FindLetter(std::string strLetter)
	{
		CMorseNode<std::string> * node;
		node=m_cMorseTree->get_Root();

		int len=strLetter.size();
		for(int i=0;i<len;i++)
			if(strLetter[i]=='.')
			{
				if(node->m_pLeftChild)
					node=node->m_pLeftChild;
				else
					return "";
			}

			else if(strLetter[i]=='-')
			{
				if(node->m_pRightChild)
				node=node->m_pRightChild;
				else
					return"";
			}
		return node->Data;
	}
	//////////////////////////////////////////////////////////
	//This Function Set Statuse To Begin In First Or End In Finishing Input Data
	void CMorseCode::setStatus(STATUSE statuse)
	{
		m_statuse=statuse;
		return;
	}
	//////////////////////////////////////////////////////////
}//namespace