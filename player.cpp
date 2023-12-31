#include "player.h"
#include <Novice.h>

int kMoveS;

Player::Player()
{
	posX = (int)startPos[0].x;
	posY = (int)startPos[0].y;
	size = { 32,32 };
	moveDir = { 0,0 };
	color = 0x0000ffff;
}

void Player::Init(int stageNo)
{
	posX = (int)startPos[stageNo].x;
	posY = (int)startPos[stageNo].y;
	moveDir = { 0,0 };
	kMoveS = Novice::LoadAudio("./sounds/move_kouho1.mp3");
}

void Player::Update(char* keys, char* preKeys)
{
	moveDir = { 0,0 };

	if (keys[DIK_W] && !preKeys[DIK_W])
		moveDir.y = -1;
	if (keys[DIK_A] && !preKeys[DIK_A])
		moveDir.x = -1;
	if (keys[DIK_S] && !preKeys[DIK_S])
		moveDir.y = 1;
	if (keys[DIK_D] && !preKeys[DIK_D])
		moveDir.x = 1;

	if (moveDir.x != 0 || moveDir.y != 0)
	{
		if (Novice::IsPlayingAudio(kMoveS) == 1)
		{
			Novice::StopAudio(kMoveS);
		}
		Novice::PlayAudio(kMoveS, 0, 0.5f);
	}
}

void Player::Draw(int mapchipSize, Vector2 keyPos)
{
	Novice::DrawBox(int(keyPos.x + posX * mapchipSize), int(keyPos.y + posY * mapchipSize), int(size.x), int(size.y), 0, color, kFillModeSolid);
}
