#include "AStar.h"
#include "MapManager.h"

void AStar::Init(int width,int height)
{
	Map* currentMap = MapManager::sInstance().GetCurrentMap();
	Block* block = NULL;
	m_nWidth = width;
	m_nHeight = height;
	m_ppiMap = new Node*[m_nWidth];
	m_ppnHeightGraph = new int*[m_nWidth];
	m_ppnGScore = new int*[m_nWidth];
	m_ppnFScore = new int*[m_nWidth];
	m_ppnHScore = new int*[m_nWidth];
	m_ppbIsInCloseList = new bool*[m_nWidth];
	for (int i=0;i<m_nWidth;i++)
	{
		m_ppiMap[i] = new Node[m_nHeight];
		m_ppnHeightGraph[i] = new int[m_nHeight];
		m_ppnGScore[i] = new int[m_nHeight];
		m_ppnFScore[i] = new int[m_nHeight];
		m_ppnHScore[i] = new int[m_nHeight];
		m_ppbIsInCloseList[i] = new bool[m_nHeight];
		for (int j=0;j<m_nHeight;j++)
		{
			block = currentMap->GetBlock(i,j);
			m_ppnHeightGraph[i][j] = g_getTerrainCost(getTerrain(block->attri));
		}
	}
}

//四方向
list<pNode> AStar::GetNeighbor(pNode current)
{
	list<pNode> neighbor;
	neighbor.clear();
	if(current->postionX>=1 && m_ppiMap[current->postionX-1][current->postionY].achiveble)
	{
		neighbor.push_back(&m_ppiMap[current->postionX-1][current->postionY]);	//top
// 		if(current->postionY>=1)
// 			neighbor.push_back(&map[current->postionX-1][current->postionY-1]); //left top
// 		if(current->postionY+1<=MAP_LENGTH_NUM-1)
// 			neighbor.push_back(&map[current->postionX-1][current->postionY+1]);	 //right top		
	}
	if(current->postionX+1<=m_nWidth-1 && m_ppiMap[current->postionX+1][current->postionY].achiveble)
	{
		neighbor.push_back(&m_ppiMap[current->postionX+1][current->postionY]);	//bottom
// 		if(current->postionY>=1)
// 			neighbor.push_back(&map[current->postionX+1][current->postionY-1]);	//left bottom
// 		if(current->postionY+1<=MAP_LENGTH_NUM-1)
// 			neighbor.push_back(&map[current->postionX+1][current->postionY+1]);	//right bottom
	}
	if(current->postionY>=1 && m_ppiMap[current->postionX][current->postionY-1].achiveble)
		neighbor.push_back(&m_ppiMap[current->postionX][current->postionY-1]);	//left
	if(current->postionY+1<=m_nHeight-1 && m_ppiMap[current->postionX][current->postionY+1].achiveble)
		neighbor.push_back(&m_ppiMap[current->postionX][current->postionY+1]);	//right
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
	int result = m_ppnHeightGraph[neighbor->postionX][neighbor->postionY];

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
	if(!(currentMap->GetSpecificRange().empty()))
	{		
		for (int i=0;i<m_nWidth;i++)
		{
			for (int j=0;j<m_nHeight;j++)
			{
				m_ppiMap[i][j].postionX = i;
				m_ppiMap[i][j].postionY = j;
				m_ppiMap[i][j].previous = NULL;
				m_ppiMap[i][j].achiveble = false;
			}
		}
		for (std::vector<Block*>::iterator it=currentMap->GetSpecificRange().begin();it!=currentMap->GetSpecificRange().end();it++)
		{
			m_ppiMap[(*it)->xpos][(*it)->ypos].achiveble = true;
			if(!IsCanCross(currentMap->GetBlock((*it)->xpos,(*it)->ypos)->attri))
				m_ppiMap[(*it)->xpos][(*it)->ypos].achiveble = false;
		}
	}
	else
	{
		for (int i=0;i<m_nWidth;i++)
		{
			for (int j=0;j<m_nHeight;j++)
			{
				m_ppiMap[i][j].postionX = i;
				m_ppiMap[i][j].postionY = j;
				m_ppiMap[i][j].previous = NULL;
				m_ppiMap[i][j].achiveble = true;

				//地形因素不可达
				//单位在上面不可达
				if(!IsCanCross(currentMap->GetBlock(i,j)->attri))
					m_ppiMap[i][j].achiveble = false;
			}
		}
	}

	m_lPath.clear();
}

void AStar::Release()
{
	for (int i=0;i<m_nWidth;i++)
	{
		if(m_ppiMap)
			gSafeDeleteArray(m_ppiMap[i]);
		if(m_ppnHeightGraph)
			gSafeDeleteArray(m_ppnHeightGraph[i]);
		if(m_ppnGScore)
			gSafeDeleteArray(m_ppnGScore[i]);
		if(m_ppnFScore)
			gSafeDeleteArray(m_ppnFScore[i]);
		if(m_ppnHScore)
			gSafeDeleteArray(m_ppnHScore[i]);
		if(m_ppbIsInCloseList)
			gSafeDeleteArray(m_ppbIsInCloseList[i]);
	}
	gSafeDeleteArray(m_ppiMap);
	gSafeDeleteArray(m_ppnHeightGraph);
	gSafeDeleteArray(m_ppnGScore);
	gSafeDeleteArray(m_ppnFScore);
	gSafeDeleteArray(m_ppnHScore);
	gSafeDeleteArray(m_ppbIsInCloseList);
}


void AStar::SetPath(int startX,int startY,int endX,int endY)
{
	pNode temp = &m_ppiMap[endX][endY];
	while (temp->previous!=NULL)
	{
		m_lPath.push_front(temp);
		temp = temp->previous;
	}
	m_lPath.push_front(&m_ppiMap[startX][startY]);
}

void AStar::Run(int startX,int startY,int endX,int endY)
{
	Binary_heap openHeap(m_nWidth,m_nHeight);
	list<pNode> closeList;
	for (int i=0;i<m_nWidth;i++)
	{
		for (int j=0;j<m_nHeight;j++)
		{
			m_ppnGScore[i][j] = 0;
			m_ppnFScore[i][j] = 0;
			m_ppnHScore[i][j] = 0;
			m_ppbIsInCloseList[i][j] = false;
		}
	}


	closeList.clear();
	m_ppnGScore[startX][startY] = 0;
	m_ppnHScore[startX][startY] = GetHScore(&m_ppiMap[startX][startY],&m_ppiMap[endX][endY]);
	m_ppnFScore[startX][startY] = m_ppnGScore[startX][startY] + m_ppnHScore[startX][startY];
	openHeap.push(&m_ppiMap[startX][startY],m_ppnFScore,m_nWidth,m_nHeight);

	while(!openHeap.empty())
	{
		pNode current = openHeap.front();
		openHeap.pop(m_ppnFScore,m_nWidth,m_nHeight);

		if(current->postionX == endX && current->postionY == endY)
		{
			SetPath(startX,startY,endX,endY);
			return; //找到路径
		}

		closeList.push_back(current);
		m_ppbIsInCloseList[current->postionX][current->postionY] = true;

		list<pNode> neighbor = GetNeighbor(current);
		for (list<pNode>::iterator itn=neighbor.begin();itn!=neighbor.end();itn++)
		{
			if(m_ppbIsInCloseList[(*itn)->postionX][(*itn)->postionY])
				continue;
			int tempGScore = m_ppnGScore[current->postionX][current->postionY] + Distance(current,*itn);
			if(tempGScore >= MAX_DISTANCE)
				continue;

			if(!openHeap.find((*itn)))	//不在openHeap中
			{
				(*itn)->previous = current;
				m_ppnHScore[(*itn)->postionX][(*itn)->postionY] = GetHScore(*itn,&m_ppiMap[endX][endY]);
				m_ppnGScore[(*itn)->postionX][(*itn)->postionY] = tempGScore;
				m_ppnFScore[(*itn)->postionX][(*itn)->postionY] = tempGScore + m_ppnHScore[(*itn)->postionX][(*itn)->postionY];
				openHeap.push((*itn),m_ppnFScore,m_nWidth,m_nHeight);

				// 				if((*itn)->postionX == endX && (*itn)->postionY == endY)
				// 					return; 
			}
			else if(tempGScore < m_ppnGScore[(*itn)->postionX][(*itn)->postionY])
			{
				(*itn)->previous = current;
				m_ppnGScore[(*itn)->postionX][(*itn)->postionY] = tempGScore;
				m_ppnFScore[(*itn)->postionX][(*itn)->postionY] = tempGScore + m_ppnHScore[(*itn)->postionX][(*itn)->postionY];
				openHeap.update((*itn),m_ppnFScore,m_nWidth,m_nHeight);
			}
		}
	}
	//没有找到，清空路线
	m_lPath.clear();
}