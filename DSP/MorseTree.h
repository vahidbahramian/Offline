#pragma once
#include<iostream>
#include<string.h>

namespace NS_DSP
{

	template<class T>
	class CMorseNode{
	public:
		CMorseNode<T>*    m_pLeftChild;
		CMorseNode<T>*    m_pRightChild;
		T			 Data;

		CMorseNode()
		{
			m_pLeftChild=NULL;
			m_pRightChild=NULL;
		}

		virtual ~CMorseNode()
		{
			SAFERELEASE(m_pRightChild);
			SAFERELEASE(m_pLeftChild);
		}
	};

	template<class T>
	class CMorseTree
	{

	private:
		CMorseNode<T>*      m_pRoot;
		int x;
	public:
		/**	constructor	**/
		CMorseTree(T Data)
		{
			m_pRoot=new CMorseNode<T>;
			m_pRoot->Data=Data;	
		}
		
		virtual ~CMorseTree(void)
		{
			SAFERELEASE(m_pRoot);
		}

		CMorseNode<T>*  get_Root()
		{
			return m_pRoot;
		}

		void set_Root(CMorseNode<T>* newRoot)
		{
			m_pRoot=newRoot;
		}

		CMorseNode<T>*  get_LftRgthchild(CMorseNode<T>& node,CMorseNode<T>& newlftNode,CMorseNode<T>& newRghtNode)
		{
			return m_pRoot;
		}

		void set_LftRgthchild(CMorseNode<T>* node,CMorseNode<T>* newlftNode,CMorseNode<T>* newRghtNode)
		{
			node->m_pLeftChild=newlftNode;
			node->m_pRightChild=newRghtNode;
		}
	};
}//namespace