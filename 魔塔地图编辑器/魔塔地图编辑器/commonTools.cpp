#include "commonTools.h"

LBUTTON_STATE getLButtonState(HGE* hge)
{
	static bool lastPressed = false;
	static bool currentPressed = false;
	LBUTTON_STATE state = LBUTTON_NULL;
	currentPressed = hge->Input_GetKeyState(HGEK_LBUTTON);

	if (currentPressed==false && lastPressed==false)
		state =  LBUTTON_NULL;
	else if(currentPressed==true && lastPressed==false)
		state =  LBUTTON_DOWN;
	else if(currentPressed==true && lastPressed==true)
		state =  LBUTTON_HOLD;
	else if(currentPressed==false && lastPressed==true)
		state =  LBUTTON_UP;

	lastPressed = currentPressed;
	return state;
}