#pragma once
#include<vector>
#include<Vector2.h>
#include<intVec2.h>

class DigPiece
{
	std::vector<std::vector<int>> digAreaArr_;
	std::vector<intVec2>* digArea_ = new std::vector<intVec2>;							//掘り出す場所
	std::vector<std::vector < std::vector<int>>>* dPiece_;	//掘り出したピース格納
	std::vector<int>* dPieceSize_;							//ピースサイズ
	std::vector<Vector2>* dPiecePos_;						//ピース座標

	int mx_, my_;					//マウス座標
	int ishave_ = -1;				//ピース所持フラグ -1:もってない 0~:番号のピース所持

	bool isDiging_ = false;			//選択中
	bool isDigOut_ = false;			//ピース掘り出しフラグ

	void FieldDig(const Vector2& _fieldKeyPos, const Vector2& _fieldSize, int _mapchipSize);
	bool isMouseInField(const Vector2& _fieldSize, intVec2 _mP);
	bool IsDigOutFin();

public:
	DigPiece();
	void Init(const Vector2& _fieldSize);
	void Update(const Vector2& _fieldKeyPos, const Vector2& _fieldSize, int _mapchipSize);
	void Draw(const Vector2& _fieldKeyPos, int _mapchipSize);
};