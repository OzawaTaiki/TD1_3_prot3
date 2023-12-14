#pragma once
#include "UI_defines.h"
#include "Scroll.h"

class Stage;

class UI_Manager
{
private:
	static	SpriteData scrollBox;
	static	Scroller* scroll;
	static	Stage* ptr_stage;

public:
	static	void	Init(Stage* ptr_stage);
	static	void	Update();
	static	void	Draw();

	static	void	IsAbleToStorage();

};
