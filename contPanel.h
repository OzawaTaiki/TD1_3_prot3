#pragma once
#include "intVec2.h"
#include<vector>

class ControlPanel
{
	std::vector<intVec2>* pos_;
	bool isInPiece_[4];

	int GH_[4];//画像 配列で

	unsigned int color_[4] = {//仮
		0xff8000ff,
		0xffff00ff,
		0xff00ffff,
		0xff0080ff,
	};

public:
	ControlPanel();
	void Init();
	void Update();
	void Draw(int x, int y, int num);
};