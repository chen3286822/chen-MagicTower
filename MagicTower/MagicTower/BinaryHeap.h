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
		heap.clear();
		heap.push_back(NULL);
		for (int i=0;i<MAP_WIDTH_NUM;i++)
			for(int j=0;j<MAP_LENGTH_NUM;j++)
			{
				isInHeap[i][j] = false;
			}
	}
	void push(const pNode& node,int score[MAP_WIDTH_NUM][MAP_LENGTH_NUM])
	{
		//�ȷŵ���ĩβ��Ȼ�����κ͵�ǰλ�õ�1/2�����ڵ�Ƚϴ�С������
		heap.push_back(node);
		isInHeap[node->postionX][node->postionY] = true;
		int num = heap.size()-1;

		while(num != 1)
		{
			int harfPosition = num >> 1;
			if(score[heap[num]->postionX][heap[num]->postionY] < score[heap[harfPosition]->postionX][heap[harfPosition]->postionY])
			{
				pNode temp = heap[harfPosition];
				heap[harfPosition] = heap[num];
				heap[num] = temp;
				num = harfPosition;
			}
			else
				break;
		}
	}

	void pop(int score[MAP_WIDTH_NUM][MAP_LENGTH_NUM])
	{
		if(heap.size()-1 <= 0)
			return;
		isInHeap[heap[1]->postionX][heap[1]->postionY] = false;
		heap[1] = heap.back();
		heap.pop_back();
		int fatherIndex = 1;
		while (1)
		{
			int childIndex = fatherIndex;
			if ((childIndex << 1) + 1 <= heap.size()-1)	//����2���ӽڵ�
			{
				if (score[heap[childIndex]->postionX][heap[childIndex]->postionY] > score[heap[childIndex<<1]->postionX][heap[childIndex<<1]->postionY])
					fatherIndex = childIndex << 1;		//���ڵ�����ӽڵ��
				if(score[heap[fatherIndex]->postionX][heap[fatherIndex]->postionY] > score[heap[(childIndex<<1)+1]->postionX][heap[(childIndex<<1)+1]->postionY])
					fatherIndex = (childIndex << 1) + 1;	//���ڵ�����ӽڵ�� ���� ���ӽڵ� ���� ���ӽڵ�
			}
			else if ((childIndex << 1) <= heap.size()-1)	//ֻ�������ӽڵ�
			{
				if(score[heap[childIndex]->postionX][heap[childIndex]->postionY] > score[heap[childIndex<<1]->postionX][heap[childIndex<<1]->postionY])
					fatherIndex = childIndex << 1;
			}
			if(fatherIndex != childIndex)	//���ڵ�Ҫ��������һ�����������ӽڵ�
			{
				pNode temp = heap[fatherIndex];
				heap[fatherIndex] = heap[childIndex];
				heap[childIndex] = temp;
			}
			else
				break;
		}
	}

	pNode& front()
	{
		return heap[1];
	}

	//�˴��õ���size����ֱ��ʹ����ȡ�ö����һ��Ԫ��
	int size()
	{
		return heap.size() - 1;
	}

	void clear()
	{
		heap.clear();
		heap.push_back(NULL);
		for (int i=0;i<MAP_WIDTH_NUM;i++)
			for(int j=0;j<MAP_LENGTH_NUM;j++)
			{
				isInHeap[i][j] = false;
			}
	}

	bool empty()
	{
		if(heap.size()-1 > 0)
			return false;
		return true;
	}

	bool find(const pNode& node)
	{
		return isInHeap[node->postionX][node->postionY];
	}

	void update(const pNode& node,int score[MAP_WIDTH_NUM][MAP_LENGTH_NUM])
	{
		//ĳ���Fֵ��С�������Ȳ��ҵ��õ��ڶ���λ�ã�Ȼ����pushһ�����θ����ڵ�Ƚ�
		int index = 1;
		for (index=1;index < heap.size();index++)
		{
			if(heap[index]->postionX==node->postionX && heap[index]->postionY==node->postionY)
				break;
		}
		while(index != 1)
		{
			int harfPosition = index >> 1;
			if(score[heap[index]->postionX][heap[index]->postionY] < score[heap[harfPosition]->postionX][heap[harfPosition]->postionY])
			{
				pNode temp = heap[harfPosition];
				heap[harfPosition] = heap[index];
				heap[index] = temp;
				index = harfPosition;
			}
			else
				break;
		}
	}
private:
	vector<pNode> heap;
	bool isInHeap[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
};

#endif

