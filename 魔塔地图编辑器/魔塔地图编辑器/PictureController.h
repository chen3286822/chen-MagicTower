#ifndef PICTURE_CONTROLLER_H
#define PICTURE_CONTROLLER_H

#include <map>
#include "hgegui.h"

struct RectLines
{
	float topX,topY;
	float leftX,leftY;
	float rightX,rightY;
	float bottomX,bottomY;
};

class PictureController :
	public hgeGUIObject
{
public:
	PictureController(void);
	PictureController(HGE* _hge,float _left,float _top,float _right,float _bottom,int _picNum);
	~PictureController(void);

	void init();

 	virtual void  Render();
 	virtual void  Update(float dt);
 
 	virtual void  Enter();
 	virtual void  Leave();
// 	virtual bool  IsDone();
// 	virtual void  Focus(bool bFocused);
// 	virtual void  MouseOver(bool bOver);
// 
 	virtual bool  MouseLButton(bool bDown);
// 	virtual bool  KeyClick(int key, int chr);


	std::map<HTEXTURE,int>& getTexMap() {return m_mTexNum;}
private:
	void loadTexList();

	HGE* m_hge;									//hge interface
	HTEXTURE* m_TexList;				//the picture list handles
	int m_PicNum;								//the picture number
	int m_EachLineNum;					//the pictures' number of every line contained in the controller 
	int m_LineNum;							//the number of the line in the controller
	hgeSprite** m_TexSprList;			//the sprite list to show all the pictures in the controller 
	int m_WhichTexture;					//record the number of the first texture showing now in the controller 

	RectLines m_Lines;						//the coordinates of the red bounding box
	float m_width;								//width of the controller
	float m_height;								//height of the controller
	float m_SmallSquareWidth;		//width of one square in a picture
	float m_SmallSquareHeight;		//height of one square in a picture
	hgeSprite* m_FullShowTex;		//the full look of the texture chosen
	float m_FullWidth;						//full width of a picture
	float m_FullHeight;						//full height of a picture
	float m_FullTexStartY;					//full texture's y start position

	std::map<HTEXTURE,int> m_mTexNum;		//给每种被载入的图片一个唯一的int ID
};


#endif