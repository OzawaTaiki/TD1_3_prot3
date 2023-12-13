#pragma once
#include <Vector2.h>

class Player
{
	int posX, posY;
	Vector2 size;
	Vector2 moveDir;
	unsigned int color;

	const Vector2 startPos[10] = {
		{1,10},
		{8,12},
		{12,3},
		{1,16},
		{1,16},
		{1,16},
		{1,16},
		{1,16},
		{1,16},
		{1,16}
	};

public:
	Vector2 GetMoveDir() { return moveDir; };

	int GetPosX() { return posX; };
	int GetPosY() { return posY; };

	Player();

	void SetPos(int x, int y) { posX = x; posY = y; };

	void Init(int stageNo);

	void Update(char* keys, char* preKeys);

	void Draw(int mapchipSize);

};