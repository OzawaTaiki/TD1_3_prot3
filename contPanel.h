#pragma once
#include "intVec2.h"
#include<vector>

class ControlPanel
{
	std::vector<intVec2>* pos_;

	int GH_;//画像 配列で
	intVec2 GHSize_ = { 160,40 };
	intVec2 GHDrawSize_ = { 40,40 };

	unsigned int color_[4] = {//仮
		0xff8000ff,
		0xffff00ff,
		0xff00ffff,
		0xff0080ff,
	};

public:

	bool isInPiece_[4];
	
	ControlPanel();
	void Init();
	void Update();
	void Draw(int x, int y, int num);
};