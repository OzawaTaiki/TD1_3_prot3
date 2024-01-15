#include "contPanel.h"

#include<Novice.h>

ControlPanel::ControlPanel()
{
	GH_[0] = Novice::LoadTexture("white1x1.png");
	GH_[1] = Novice::LoadTexture("white1x1.png");
	GH_[2] = Novice::LoadTexture("white1x1.png");
	GH_[3] = Novice::LoadTexture("white1x1.png");

	pos_ = new std::vector<intVec2>;
}

void ControlPanel::Init()
{
	for (int i = 0; i < 4; i++)
	{
		isInPiece_[i] = false;
	}
}

void ControlPanel::Update()
{

}

void ControlPanel::Draw(int x, int y, int num)
{
	Novice::DrawSprite(x, y, GH_[num], 40, 40, 0, color_[num]);
}
