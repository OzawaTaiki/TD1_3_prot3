#include "player.h"
#include "PhilliaFunction/Phill.h"	// イージングをするために必要
#include <Novice.h>

void Player::Input(char* keys, char* preKeys, bool* panel)
{
	moveDir = { 0,0 };

	if (keys[DIK_W] && !preKeys[DIK_W] && panel[0])
	{
		moveDir.y = -1;
	}
	if (keys[DIK_A] && !preKeys[DIK_A] && panel[1])
	{
		moveDir.x = -1;
	}
	if (keys[DIK_S] && !preKeys[DIK_S] && panel[2])
	{
		moveDir.y = 1;
	}
	if (keys[DIK_D] && !preKeys[DIK_D] && panel[3])
	{
		moveDir.x = 1;
	}
}

void Player::Move(int _endPos)
{
	if (t >= 1)
	{
		t = 0;
		easedT = 0;
		keepDir = { 0,0 };
		posX = (int)DrawPos.x / 40;
		posY = (int)DrawPos.y / 40;
		isMove = false;
	}
	else if (moveDir.x != 0 || moveDir.y != 0 || isMove)
	{
		if (!isMove)
		{
			keepDir = moveDir;
			endPos = _endPos;
			isMove = true;
		}
		t += addT;
		if (t >= 1)
			t = 1;
	}

	easedT = Phill::EaseInOutQuart(t);

	if (keepDir.x != 0)
		DrawPos.x = ((1.0f - easedT) * posX + easedT * endPos) * 40;
	if (keepDir.y != 0)
		DrawPos.y = ((1.0f - easedT) * posY + easedT * endPos) * 40;

	/*Novice::ScreenPrintf(0, 0, "t : %f", t);
	Novice::ScreenPrintf(0, 20, "dir : %.1f,%.1f", keepDir.x, keepDir.y);
	Novice::ScreenPrintf(0, 40, "isMove : %d", isMove);
	Novice::ScreenPrintf(0, 60, "pos : %d,%d", posX, posY);*/
}

Player::Player(int mapchipSize)
{
	posX = (int)startPos[0].x;
	posY = (int)startPos[0].y;
	DrawPos = { (float)posX * mapchipSize,(float)posY * mapchipSize };
	t = 0;
	easedT = 0;
	size = { 32,32 };
	moveDir = { 0,0 };
	keepDir = { 0,0 };
	color = 0x0000ffff;
	isMove = false;
	playerTexture = Novice::LoadTexture("./img/player.png");
}

void Player::Init(int stageNo)
{

	posX = (int)startPos[stageNo].x;
	posY = (int)startPos[stageNo].y;
	DrawPos = { (float)posX * 40,(float)posY * 40 };
	moveDir = { 0,0 };
}


void Player::Draw(Vector2 keyPos)
{
	Phill::DrawQuadPlus(int(keyPos.x + DrawPos.x) + 20, int(keyPos.y + DrawPos.y) + 20, int(size.x), int(size.y), 1.0f, 1.0f, 0.0f, 0, 0, 64, 64, playerTexture, 0xffffffff, PhillDrawMode::DrawMode_Center);
	//Novice::DrawBox(int(keyPos.x + DrawPos.x), int(keyPos.y + DrawPos.y), int(size.x), int(size.y), 0, color, kFillModeSolid);
}
