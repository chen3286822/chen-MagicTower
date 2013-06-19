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
	void run(int startX,int startY,int endX,int endY);
	void init();
	void updateMap(int startX,int startY,int endX,int endY);
	void release();

	void setPath(int startX,int startY,int endX,int endY);
	list<pNode> getPath(){return path;}

protected:
	Node map[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	list<pNode> path;
	int heightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM];

	list<pNode> getNeighbor(pNode current);
	int distance(pNode current,pNode neighbor);
	int getHScore(pNode current,pNode end);
	bool findNode(pNode node,list<pNode> nodeList);
};

#endif