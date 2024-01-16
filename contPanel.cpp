#include "contPanel.h"

#include<Novice.h>

ControlPanel::ControlPanel()
{
	GH_ = Novice::LoadTexture("./img/WASD.png");

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
	Novice::DrawSpriteRect(x, y, GHDrawSize_.x * num, 0, GHDrawSize_.x, GHDrawSize_.y, GH_, (float)GHDrawSize_.x / (float)GHSize_.x, (float)GHDrawSize_.y / (float)GHSize_.y, 0, /*isInPiece_[num] ? RED :*/ WHITE);

	/*for (int i = 0; i < 4; i++)
		Novice::ScreenPrintf(200, 20 + i * 20, "%s", isInPiece_[i] ? "true" : "false");*/

}
