﻿#pragma once
#include <Vector2.h>
#include <vector>

class Player;
//class CSV_Loader;

const int kMaxFieldSize_ = 18;
const int kMaxPieceNum_ = 10;
const int kMaxTileNum_ = 3;

class Stage
{
	const int kMapchipSize_ = 40;

	std::vector<std::vector<int>>* field_;
	std::vector<std::vector<int>> collision_;
	std::vector<std::vector<std::vector<int>>>* piece_;
	std::vector<Vector2> piecePos_;
	std::vector<float> scal_;

	const float kKeyScal_[2] = {
		1.0f,0.75f
	};

	const unsigned int kTileColor_[kMaxTileNum_] = {
		0xffffffff,
		0x000000ff,
		0xff0000ff
	};

	//pieceの色(ChatGPT君セレクト)
	const unsigned int color_[kMaxPieceNum_] = {
		0xFF5733a0,
		0xA8B820a0,
		0x3D9140a0,
		0x61C4F2a0,
		0xE7F16Fa0,
		0xE52B50a0,
		0x6F2DA8a0,
		0xF28C61a0,
		0x91403Da0,
		0x2B50E7a0
	};

	Vector2 sub_ = { 0,0 };
	int isHave_ = -1;
	int mx_, my_;
	int selectStage_ = 0;
	bool isNext_;

	//ファイル名一括管理 すべてここに入力
	const char* stageFilePath_[64] = {
		"./data/mapchip.csv",
		"./data/mapchip1.csv",
	};

	Player* player_;

	void collisionArrReset();

	void PieceMove();

	void playerCollision();

public:
	Stage();

	void Init(int stageNo);

	void Update(char* keys, char* preKeys);

	void Draw(int windowWidth, int windowHeight);

};