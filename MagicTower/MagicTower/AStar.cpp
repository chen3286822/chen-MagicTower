#include "AStar.h"
#include "MapManager.h"

void AStar::Init()
{
	Map* currentMap = MapManager::sInstance().GetCurrentMap();
	Block* block = NULL;
	for (int i=0;i<MAP_WIDTH_NUM;i++)
	{
		for (int j=0;j<MAP_LENGTH_NUM;j++)
		{
			block = currentMap->GetBlock(i,j);
			m_nHeightGraph[i][j] = g_getTerrainCost(getTerrain(block->attri));
		}
	}
}

//四方向
list<pNode> AStar::GetNeighbor(pNode current)
{
	list<pNode> neighbor;
	neighbor.clear();
	if(current->postionX>=1)
	{
		neighbor.push_back(&m_iMap[current->postionX-1][current->postionY]);	//top
// 		if(current->postionY>=1)
// 			neighbor.push_back(&map[current->postionX-1][current->postionY-1]); //left top
// 		if(current->postionY+1<=MAP_LENGTH_NUM-1)
// 			neighbor.push_back(&map[current->postionX-1][current->postionY+1]);	 //right top		
	}
	if(current->postionX+1<=MAP_WIDTH_NUM-1)
	{
		neighbor.push_back(&m_iMap[current->postionX+1][current->postionY]);	//bottom
// 		if(current->postionY>=1)
// 			neighbor.push_back(&map[current->postionX+1][current->postionY-1]);	//left bottom
// 		if(current->postionY+1<=MAP_LENGTH_NUM-1)
// 			neighbor.push_back(&map[current->postionX+1][current->postionY+1]);	//right bottom
	}
	if(current->postionY>=1)
		neighbor.push_back(&m_iMap[current->postionX][current->postionY-1]);	//left
	if(current->postionY+1<=MAP_LENGTH_NUM-1)
		neighbor.push_back(&m_iMap[current->postionX][current->postionY+1]);	//right
	return neighbor;
}

int AStar::Distance(pNode current,pNode neighbor)
{
	if(neighbor->achiveble==false)
		return MAX_DISTANCE;
	// 	if(neighbor->postionX!=current->postionX && neighbor->postionY!=current->postionY)
	// 		return weight*14/10;
	// 	return weight;
// 	int result = abs(_heightGraph[neighbor->postionX][neighbor->postionY]-_heightGraph[current->postionX][current->postionY]);
// 	if(neighbor->postionX!=current->postionX && neighbor->postionY!=current->postionY)
// 		return result*14/10;
	int result = m_nHeightGraph[neighbor->postionX][neighbor->postionY];

	return result;
}

int AStar::GetHScore(pNode current,pNode end)
{
	int x = 0,y = 0;
	x = abs(end->postionX-current->postionX);
	y = abs(end->postionY-current->postionY);
	return (x+y);
	//int smaller = x<y? x : y;
	//return (smaller*weight*14/10 + abs(x-y)*weight);
	//	return abs(x-y)*weight;
}

bool AStar::FindNode(pNode node,list<pNode> nodeList)
{
	if(nodeList.empty())
		return false;
	for (list<pNode>::iterator it=nodeList.begin();it!=nodeList.end();it++)
	{
		if(node->postionX==(*it)->postionX && node->postionY==(*it)->postionY)
			return true;
	}
	return false;
}

void AStar::UpdateMap()
{
	Map* currentMap = MapManager::sInstance().GetCurrentMap();
	for (int i=0;i<MAP_WIDTH_NUM;i++)
	{
		for (int j=0;j<MAP_LENGTH_NUM;j++)
		{
			m_iMap[i][j].postionX = i;
			m_iMap[i][j].postionY = j;
			m_iMap[i][j].previous = NULL;
			m_iMap[i][j].achiveble = true;
// 			if(heightGraph[i][j] >= 200 && !((i==startX && j==startY) ||(i==endX && j==endY)))
// 				map[i][j].achiveble = false;

			//地形因素不可达
			//单位在上面不可达
			if(!IsCanCross(currentMap->GetBlock(i,j)->attri))
				m_iMap[i][j].achiveble = false;
		}
	}
	m_lPath.clear();
}

void AStar::Release()
{
}


void AStar::SetPath(int startX,int startY,int endX,int endY)
{
	pNode temp = &m_iMap[endX][endY];
	while (temp->previous!=NULL)
	{
		m_lPath.push_front(temp);
		temp = temp->previous;
	}
	m_lPath.push_front(&m_iMap[startX][startY]);
}

void AStar::Run(int startX,int startY,int endX,int endY)
{
	int gScore[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	int fScore[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	int hScore[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	Binary_heap openHeap;
	list<pNode> closeList;
	bool isInCloseList[MAP_WIDTH_NUM][MAP_LENGTH_NUM];
	for (int i=0;i<MAP_WIDTH_NUM;i++)
		for (int j=0;j<MAP_LENGTH_NUM;j++)
			isInCloseList[i][j] = false;


	closeList.clear();
	gScore[startX][startY] = 0;
	hScore[startX][startY] = GetHScore(&m_iMap[startX][startY],&m_iMap[endX][endY]);
	fScore[startX][startY] = gScore[startX][startY] + hScore[startX][startY];
	openHeap.push(&m_iMap[startX][startY],fScore);

	while(!openHeap.empty())
	{
		pNode current = openHeap.front();
		openHeap.pop(fScore);

		if(current->postionX == endX && current->postionY == endY)
		{
			SetPath(startX,startY,endX,endY);
			return; //找到路径
		}

		closeList.push_back(current);
		isInCloseList[current->postionX][current->postionY] = true;

		list<pNode> neighbor = GetNeighbor(current);
		for (list<pNode>::iterator itn=neighbor.begin();itn!=neighbor.end();itn++)
		{
			if(isInCloseList[(*itn)->postionX][(*itn)->postionY])
				continue;
			int tempGScore = gScore[current->postionX][current->postionY] + Distance(current,*itn);
			if(tempGScore >= MAX_DISTANCE)
				continue;

			if(!openHeap.find((*itn)))	//不在openHeap中
			{
				(*itn)->previous = current;
				hScore[(*itn)->postionX][(*itn)->postionY] = GetHScore(*itn,&m_iMap[endX][endY]);
				gScore[(*itn)->postionX][(*itn)->postionY] = tempGScore;
				fScore[(*itn)->postionX][(*itn)->postionY] = tempGScore + hScore[(*itn)->postionX][(*itn)->postionY];
				openHeap.push((*itn),fScore);

				// 				if((*itn)->postionX == endX && (*itn)->postionY == endY)
				// 					return; 
			}
			else if(tempGScore < gScore[(*itn)->postionX][(*itn)->postionY])
			{
				(*itn)->previous = current;
				gScore[(*itn)->postionX][(*itn)->postionY] = tempGScore;
				fScore[(*itn)->postionX][(*itn)->postionY] = tempGScore + hScore[(*itn)->postionX][(*itn)->postionY];
				openHeap.update((*itn),fScore);
			}
		}
	}
	//没有找到，清空路线
	m_lPath.clear();
}