#ifndef ASTAR_H
#define ASTAR_H
#include "BinaryHeap.h"
#include <list>

class AStar
{
public:
	AStar()
	{
	}
	void Run(int startX,int startY,int endX,int endY);
	void Init();
	void UpdateMap();
	void Release();
	list<pNode> GetPath(){return m_lPath;}

protected:
	Node m_iMap[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	list<pNode> m_lPath;
	int m_nHeightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM];

	list<pNode> GetNeighbor(pNode current);
	int Distance(pNode current,pNode neighbor);
	int GetHScore(pNode current,pNode end);
	bool FindNode(pNode node,list<pNode> nodeList);
	void SetPath(int startX,int startY,int endX,int endY);
};

#endif