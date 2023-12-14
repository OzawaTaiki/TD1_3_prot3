#include "UI_Manager.h"
#include <Novice.h>
#include "CursorManager/CursorManager.h"

SpriteData UI_Manager::scrollBox	= SpriteData();
Scroller* UI_Manager::scroll		= new Scroller(&scrollBox);
float t;
Transform curpos;
Transform pos;
const Transform kWindowPos	= { 1000, 15 };
const Size		kWindowSize = { 800,1030 };

void UI_Manager::Init()
{
	// 画像データ
	scrollBox.srcPos = Transform(0, 0);
	scrollBox.srcSize = Size(1, 1);
	scrollBox.textureHandle = Novice::LoadTexture("white1x1.png");

	// オブジェクトの設定
	scroll->SetPosition(Transform(1770, 100));
	scroll->SetBarSize(Size(40, 520));
	scroll->SetBoxSize(Size(10, 70));
	pos.x = 1000; 
	pos.y = 200;
}

void UI_Manager::Update()
{
	CursorManager::GetCursorPos(&curpos);
	scroll->UpdateStatus();
	t = scroll->GetValue();
	pos.y = int(t * -1280.0f + (1.0f - t) * 200.0f);
}

void UI_Manager::Draw()
{
	// ウィンドウ背景
	Novice::DrawBox(kWindowPos.x, kWindowPos.y, kWindowSize.width, kWindowSize.height, 0.0f, 0x121212ff, kFillModeSolid);
	// テストオブジェクト
	Novice::DrawBox(pos.x, pos.y, 120, 120, 0.0f, 0xdfdfdfff, kFillModeSolid);

	scroll->Draw();
	Novice::ScreenPrintf(15, 15, "%.4f", t);
	Novice::ScreenPrintf(15, 45, "%d, %d", curpos.x, curpos.y);
}
