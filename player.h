#pragma once
#include <Vector2.h>

class Player
{
	int posX, posY;
	Vector2 size;
	Vector2 moveDir;
	unsigned int color;

public:
	Vector2 GetMoveDir() { return moveDir; };

	int GetPosX() { return posX; };
	int GetPosY() { return posY; };

	Player();

	void SetPos(int x, int y) { posX = x; posY = y; };

	void Init();

	void Update(char* keys, char* preKeys);

	void Draw(int mapchipSize);

};