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
		//先放到堆末尾，然后依次和当前位置的1/2处父节点比较大小并交换
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
			if ((childIndex << 1) + 1 <= heap.size()-1)	//存在2个子节点
			{
				if (score[heap[childIndex]->postionX][heap[childIndex]->postionY] > score[heap[childIndex<<1]->postionX][heap[childIndex<<1]->postionY])
					fatherIndex = childIndex << 1;		//父节点比左子节点大
				if(score[heap[fatherIndex]->postionX][heap[fatherIndex]->postionY] > score[heap[(childIndex<<1)+1]->postionX][heap[(childIndex<<1)+1]->postionY])
					fatherIndex = (childIndex << 1) + 1;	//父节点比右子节点大 或者 左子节点 大于 右子节点
			}
			else if ((childIndex << 1) <= heap.size()-1)	//只存在左子节点
			{
				if(score[heap[childIndex]->postionX][heap[childIndex]->postionY] > score[heap[childIndex<<1]->postionX][heap[childIndex<<1]->postionY])
					fatherIndex = childIndex << 1;
			}
			if(fatherIndex != childIndex)	//父节点要大于其中一个或者所有子节点
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

	//此处得到的size可以直接使用以取得堆最后一个元素
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
		//某点的F值变小，所以先查找到该点在堆中位置，然后像push一样依次跟父节点比较
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

