#pragma once
#include "UI_defines.h"
#include "Scroll.h"

class UI_Manager
{
private:
	static	SpriteData scrollBox;
	static	Scroller* scroll;

public:
	static	void	Init();
	static	void	Update();
	static	void	Draw();

};
