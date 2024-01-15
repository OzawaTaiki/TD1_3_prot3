#pragma once
#include <Vector2.h>

class Player
{
	int posX, posY;
	Vector2 DrawPos;
	Vector2 size;
	Vector2 moveDir;
	Vector2 keepDir;
	unsigned int color;
	bool isMove;
	int endPos;

	float t = 0;
	float easedT = 0;
	float addT = 0.05f;

	const Vector2 startPos[10] = {
		{1,10},
		{8,12},
		{1,11},
		{1,6},
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
	bool GetIsMove() { return isMove; };


	Player(int mapchipSize);

	void SetPos(int x, int y) { posX = x; posY = y; };

	void Init(int stageNo);

	//void Update(char* keys, char* preKeys);

	void Input(char* keys, char* preKeys);

	void Move(int _endPos);

	void Draw(Vector2 keyPos);

};