#include "UI_Manager.h"
#include <Novice.h>
SpriteData UI_Manager::scrollBox = SpriteData();
Scroller* UI_Manager::scroll = new Scroller(&scrollBox);
float t;

void UI_Manager::Init()
{
	scrollBox.srcPos = Transform(0, 0);
	scrollBox.srcSize = Size(1, 1);
	scrollBox.textureHandle = Novice::LoadTexture("white1x1.png");
	scroll->SetPosition(Transform(720, 100));
	scroll->SetBarSize(Size(40, 300));
	scroll->SetBoxSize(Size(10, 30));
}

void UI_Manager::Update()
{
	scroll->UpdateStatus();
	t = scroll->GetValue();
}

void UI_Manager::Draw()
{
	scroll->Draw();
	Novice::ScreenPrintf(15, 15, "%.4f", t);
}
