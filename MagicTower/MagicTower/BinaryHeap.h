#ifndef BINARY_HEAP
#define BINARY_HEAP
#include "commonTools.h"
using namespace std;


struct Node
{
	int postionX,postionY;
	bool achiveble;
	Node* previous;
};
typedef Node *pNode;


class Binary_heap
{
public:
	Binary_heap()
	{
		m_vHeap.clear();
		m_vHeap.push_back(NULL);
		m_nWidth = g_nMapWidthNum;
		m_nHeight = g_nMapHeightNum;
		m_ppbIsInHeap = new bool*[m_nWidth];
		for (int i=0;i<m_nWidth;i++)
		{
			m_ppbIsInHeap[i] = new bool[m_nHeight];
			for(int j=0;j<m_nHeight;j++)
			{
				m_ppbIsInHeap[i][j] = false;
			}
		}
	}
	Binary_heap(int width,int height):m_nWidth(width),m_nHeight(height)
	{
		m_vHeap.clear();
		m_vHeap.push_back(NULL);
		m_ppbIsInHeap = new bool*[m_nWidth];
		for (int i=0;i<m_nWidth;i++)
		{
			m_ppbIsInHeap[i] = new bool[m_nHeight];
			for(int j=0;j<m_nHeight;j++)
			{
				m_ppbIsInHeap[i][j] = false;
			}
		}
	}
	Binary_heap::~Binary_heap()
	{
		for (int i=0;i<m_nWidth;i++)
		{
			if(m_ppbIsInHeap)
				gSafeDeleteArray(m_ppbIsInHeap[i]);
		}
		gSafeDeleteArray(m_ppbIsInHeap);
	}

	void push(const pNode& node,int** score,int m,int n)
	{
		//先放到堆末尾，然后依次和当前位置的1/2处父节点比较大小并交换
		m_vHeap.push_back(node);
		m_ppbIsInHeap[node->postionX][node->postionY] = true;
		int num = m_vHeap.size()-1;
		while(num != 1)
		{
			int halfPosition = num >> 1;
			if(score[m_vHeap[num]->postionX][m_vHeap[num]->postionY] < score[m_vHeap[halfPosition]->postionX][m_vHeap[halfPosition]->postionY])
			{
				pNode temp = m_vHeap[halfPosition];
				m_vHeap[halfPosition] = m_vHeap[num];
				m_vHeap[num] = temp;
				num = halfPosition;
			}
			else
				break;
		}
	}

	void pop(int** score,int m,int n)
	{
		if(m_vHeap.size()-1 <= 0)
			return;
		m_ppbIsInHeap[m_vHeap[1]->postionX][m_vHeap[1]->postionY] = false;
		m_vHeap[1] = m_vHeap.back();
		m_vHeap.pop_back();
		int fatherIndex = 1;
		while (1)
		{
			int childIndex = fatherIndex;
			if ((childIndex << 1) + 1 <= m_vHeap.size()-1)	//存在2个子节点
			{
				if (score[m_vHeap[childIndex]->postionX][m_vHeap[childIndex]->postionY] > score[m_vHeap[childIndex<<1]->postionX][m_vHeap[childIndex<<1]->postionY])
					fatherIndex = childIndex << 1;		//父节点比左子节点大
				if(score[m_vHeap[fatherIndex]->postionX][m_vHeap[fatherIndex]->postionY] > score[m_vHeap[(childIndex << 1) + 1]->postionX][m_vHeap[(childIndex << 1) + 1]->postionY])
					fatherIndex = (childIndex << 1) + 1;	//父节点比右子节点大 或者 左子节点 大于 右子节点
			}
			else if ((childIndex << 1) <= m_vHeap.size()-1)	//只存在左子节点
			{
				if(score[m_vHeap[childIndex]->postionX][m_vHeap[childIndex]->postionY] > score[m_vHeap[childIndex<<1]->postionX][m_vHeap[childIndex<<1]->postionY])
					fatherIndex = childIndex << 1;
			}
			if(fatherIndex != childIndex)	//父节点要大于其中一个或者所有子节点
			{
				pNode temp = m_vHeap[fatherIndex];
				m_vHeap[fatherIndex] = m_vHeap[childIndex];
				m_vHeap[childIndex] = temp;
			}
			else
				break;
		}
	}

	pNode& front()
	{
		return m_vHeap[1];
	}

	//此处得到的size可以直接使用以取得堆最后一个元素
	int size()
	{
		return m_vHeap.size() - 1;
	}

	void clear()
	{
		m_vHeap.clear();
		m_vHeap.push_back(NULL);
		for (int i=0;i<m_nWidth;i++)
			for(int j=0;j<m_nHeight;j++)
			{
				m_ppbIsInHeap[i][j] = false;
			}
	}

	bool empty()
	{
		if(m_vHeap.size()-1 > 0)
			return false;
		return true;
	}

	bool find(const pNode& node)
	{
		return m_ppbIsInHeap[node->postionX][node->postionY];
	}

	void update(const pNode& node,int** score,int m,int n)
	{
		//某点的F值变小，所以先查找到该点在堆中位置，然后像push一样依次跟父节点比较
		int index = 1;
		for (index=1;index < m_vHeap.size();index++)
		{
			if(m_vHeap[index]->postionX==node->postionX && m_vHeap[index]->postionY==node->postionY)
				break;
		}
		while(index != 1)
		{
			int halfPosition = index >> 1;
			if(score[m_vHeap[index]->postionX][m_vHeap[index]->postionY] < score[m_vHeap[halfPosition]->postionX][m_vHeap[halfPosition]->postionY])
			{
				pNode temp = m_vHeap[halfPosition];
				m_vHeap[halfPosition] = m_vHeap[index];
				m_vHeap[index] = temp;
				index = halfPosition;
			}
			else
				break;
		}
	}
private:
	vector<pNode> m_vHeap;
	bool** m_ppbIsInHeap;
	int m_nWidth;
	int m_nHeight;
};

#endif

