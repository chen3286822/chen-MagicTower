#ifndef ASTAR_H
#define ASTAR_H
#include "BinaryHeap.h"
#include <list>
using namespace std;

list<pNode> getNeighbor(Node map[MAP_WIDTH_NUM][MAP_LENGTH_NUM],pNode current);
int distance(pNode current,pNode neighbor,int _heightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM]);
int getHScore(pNode current,pNode end);

class AStar
{
public:
	AStar()
	{
	}
	void run(HGE*,int _renderX,int _renderY,int startX,int startY,int endX,int endY,int heightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM]);
	void init();
	void initMap(int heightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM],int startX,int startY,int endX,int endY);
	void release();
	void render(int _renderX,int _renderY);

	void setPath(int startX,int startY,int endX,int endY,int heightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM]);
	void renderHisAndPath();

protected:
	Node map[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	hgeSprite* block[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	list<pNode> path;
	list<int> history;
};

#endif