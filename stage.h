#pragma once
#include <Vector2.h>
#include <vector>
#include "SceneChange.h"

class Player;
class ControlPanel;

static const int kWindowWidth = 1920;
static const int kWindowHeight = 1080;

const int kMaxFieldSize_ = 18;
const int kMaxPieceNum_ = 10;
const int kMaxTileNum_ = 4;//NONE,WALL,GOAL,unStackBlock

class Stage
{
	const int kMapchipSize_ = 40;
	
	int blockTexture;
	int pieceTexture;
	int goalTexture;
	int backGroundTexture;
	int obstacleTexture;
	int descriptionTexture;
	int controllTexture;

	int stageCntTexture[3];
	int slashTexture;

	int maxStages = 4;

	int isViewDescription;

	int isClear;

	std::vector<std::vector<int>>* field_;
	std::vector<std::vector<int>> collision_;
	std::vector<std::vector<std::vector<int>>>* piece_;
	std::vector<Vector2> piecePos_;
	std::vector<Vector2> pieceSize_;
	std::vector<float> scal_;

	const float kKeyScal_[2] = {
		1.0f,0.75f
	};

	const unsigned int kTileColor_[kMaxTileNum_] = {
		0xc5edbeff,
		0x000000ff,
		0xff0000ff,
		0x00ff00ff
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

	Vector2 fieldSize_ = { 0,0 };										// fieldの最大のサイズ
	Vector2 fieldKeyPos_ = { kWindowWidth * 1 / 3,kWindowHeight / 2 };	// fieldの基準座標
	Vector2 sub_ = { 0,0 };												// マウスとpieceの差
	Vector2 piecePrePos_;												// pieceの前の座標
	int isHave_ = -1;													// piece所持フラグ -1:もってない 0~:番号のピース所持
	int mx_, my_;														// マウスカーソルの座標
	int selectStage_ = 0;												// ステージナンバー
	bool isNext_;														// クリアフラグ

	int scanX_;
	int scanY_;
	int haveBuf;

	//ファイル名一括管理 すべてここに入力
	const char* stageFilePath_[64] = {
		"./data/-_testStage2.csv",
		"./data/-_testStage3.csv",
		"./data/-_test.csv",
		"./data/-_testStage4.csv"
	};

	SceneChange* sceneChange_;

	Player* player_;
	ControlPanel *CP_;

	void collisionArrReset();

	void PieceMove();

	int playerCollision();

	bool UnStackBlockCheck(int x, int y);

	bool isInPiece(int checkX, int checkY,int x, int y, int pieceNum);

	void DrawPieceShadow();

public:

	Stage();

	void Init(int stageNo);

	void Update(char* keys, char* preKeys);

	void Draw();

	int GetIsClear() const;

	SceneChange* GetSceneChgPtr();
};