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
	void Init(int width,int height);
	void UpdateMap();
	void Release();
	list<pNode> GetPath(){return m_lPath;}

protected:
	Node** m_ppiMap;
	list<pNode> m_lPath;
	int** m_ppnHeightGraph;
	int m_nWidth;
	int m_nHeight;
	int** m_ppnGScore;
	int** m_ppnFScore;
	int** m_ppnHScore;
	bool** m_ppbIsInCloseList;

	list<pNode> GetNeighbor(pNode current);
	int Distance(pNode current,pNode neighbor);
	int GetHScore(pNode current,pNode end);
	bool FindNode(pNode node,list<pNode> nodeList);
	void SetPath(int startX,int startY,int endX,int endY);
};

#endif