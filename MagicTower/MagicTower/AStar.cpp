#include "AStar.h"


list<pNode> getNeighbor(Node map[MAP_WIDTH_NUM][MAP_LENGTH_NUM],pNode current)
{
	list<pNode> neighbor;
	neighbor.clear();
	if(current->postionX>=1)
	{
		neighbor.push_back(&map[current->postionX-1][current->postionY]);	//top
		if(current->postionY>=1)
			neighbor.push_back(&map[current->postionX-1][current->postionY-1]); //left top
		if(current->postionY+1<=MAP_LENGTH_NUM-1)
			neighbor.push_back(&map[current->postionX-1][current->postionY+1]);	 //right top		
	}
	if(current->postionX+1<=MAP_WIDTH_NUM-1)
	{
		neighbor.push_back(&map[current->postionX+1][current->postionY]);	//bottom
		if(current->postionY>=1)
			neighbor.push_back(&map[current->postionX+1][current->postionY-1]);	//left bottom
		if(current->postionY+1<=MAP_LENGTH_NUM-1)
			neighbor.push_back(&map[current->postionX+1][current->postionY+1]);	//right bottom
	}
	if(current->postionY>=1)
		neighbor.push_back(&map[current->postionX][current->postionY-1]);	//left
	if(current->postionY+1<=MAP_LENGTH_NUM-1)
		neighbor.push_back(&map[current->postionX][current->postionY+1]);	//right
	return neighbor;
}

int distance(pNode current,pNode neighbor,int _heightGraph[MAP_WIDTH_NUM][MAP_LENGTH_NUM])
{
	if(neighbor->achiveble==false)
		return MAX_DISTANCE;
	// 	if(neighbor->postionX!=current->postionX && neighbor->postionY!=current->postionY)
	// 		return weight*14/10;
	// 	return weight;
	int result = abs(_heightGraph[neighbor->postionX][neighbor->postionY]-_heightGraph[current->postionX][current->postionY])+weight;
	if(neighbor->postionX!=current->postionX && neighbor->postionY!=current->postionY)
		return result*14/10;
	return result;
}

int getHScore(pNode current,pNode end)
{
	int x = 0,y = 0;
	x = abs(end->postionX-current->postionX);
	y = abs(end->postionY-current->postionY);
	int smaller = x<y? x : y;
	return (smaller*weight*14/10 + abs(x-y)*weight);
	//	return abs(x-y)*weight;
}

bool findNode(pNode node,list<pNode> nodeList)
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

void Algorithm::init()
{
	for (int i=0;i<width;i++)
	{
		for (int j=0;j<length;j++)
		{
			block[i][j] = new hgeSprite(0,0,0,block_size-1,block_size-1);
		}
	}
}

void Algorithm::initMap(int heightGraph[width][length],int startX,int startY,int endX,int endY)
{
	for (int i=0;i<width;i++)
	{
		for (int j=0;j<length;j++)
		{
			map[i][j].postionX = i;
			map[i][j].postionY = j;
			map[i][j].previous = NULL;
			map[i][j].achiveble = true;
			if(heightGraph[i][j] >= 200 && !((i==startX && j==startY) ||(i==endX && j==endY)))
				map[i][j].achiveble = false;

			DWORD colorR = heightGraph[i][j];
			DWORD color = ARGB(255,255-colorR,255-colorR,255-colorR);
			block[i][j]->SetColor(color);

			if(map[i][j].achiveble==false)
				block[i][j]->SetColor(0xFF191919);
		}
	}
	path.clear();
	history.clear();
}

void Algorithm::release()
{
	for (int i=0;i<width;i++)
	{
		for (int j=0;j<length;j++)
		{
			gSafeDelete(block[i][j]);
		}
	}
}

void Algorithm::render(int _renderX,int _renderY)
{
	for (int i=0;i<width;i++)
	{
		for (int j=0;j<length;j++)
		{
			block[i][j]->Render(_renderX+j*block_size,_renderY+i*block_size);
		}
	}
}

void Algorithm::setPath(int startX,int startY,int endX,int endY,int heightGraph[width][length])
{
	cost = 0;
	pNode temp = &map[endX][endY];
	while (temp->previous!=NULL)
	{
		path.push_front(temp);
		cost += abs(distance(temp,temp->previous,heightGraph));
		temp = temp->previous;
	}
	path.push_front(&map[startX][startY]);
}

void Algorithm::renderHisAndPath()
{
	if(!history.empty())
	{
		list<int>::iterator it = history.begin();
		block[(*it)/length][(*it)%length]->SetColor(0xFF00FF00);
		history.pop_front();
	}
	if(history.empty() && !path.empty())
	{
		pNode tempNode = path.front();
		block[tempNode->postionX][tempNode->postionY]->SetColor(0xFFFF0000);
		path.pop_front();
	}
}