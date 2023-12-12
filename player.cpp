#include "player.h"
#include <Novice.h>

Player::Player()
{
	posX = 1;
	posY = 16;
	size = { 32,32 };
	moveDir = { 0,0 };
	color = 0xff0000ff;
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
}

void Player::Draw(int mapchipSize)
{
	Novice::DrawBox(posX * mapchipSize, posY * mapchipSize, int(size.x), int(size.y), 0, color, kFillModeSolid);
}
