#include "digPiece.h"
#include <Novice.h>

void DigPiece::FieldDig(const Vector2& _fieldKeyPos, const Vector2& _fieldSize, int _mapchipSize)
{
	Novice::GetMousePosition(&mx_, &my_);

	intVec2 mPos = {//filedPos基準のマウスのマップチップ座標
		int(mx_ - _fieldKeyPos.x) / _mapchipSize,
		int(my_ - _fieldKeyPos.y) / _mapchipSize,
	};

	/**************************
	関数にまとめたい
	selectDigArea()
	****************************:*/
	if (Novice::IsTriggerMouse(0) && isMouseInField(_fieldSize, mPos))
	{
		isDiging_ = true;
	}

	else if (Novice::IsPressMouse(0) && isDiging_)
	{
		//マウスがフィールド枠内にあるか否か
		if (isMouseInField(_fieldSize, mPos))
		{
			//エラー回避
			if (digArea_->empty())		(*digArea_).push_back({ mPos });

			//選択されていないとき
			if (digAreaArr_[mPos.y][mPos.x] != 1)
			{
				(*digArea_).push_back({ mPos });		//選択した座標を格納
				digAreaArr_[mPos.y][mPos.x] = 1;		//座標を選択済みに
			}
			else if (digAreaArr_[mPos.y][mPos.x] == 1 &&
				(*digArea_)[0].x == mPos.x &&
				(*digArea_)[0].y == mPos.y)
			{
				isDigOut_ = true;
			}
		}
	}
	else if (!isDigOut_)
	{
		isDiging_ = false;
		for (int i = 0; i < digAreaArr_.size(); i++)
			std::fill(digAreaArr_[i].begin(), digAreaArr_[i].end(), 0);
		digArea_->clear();
	}
	/*****************
	ここまで
	selectDigArea()
	***********************/

	if (Novice::IsTriggerMouse(0) && isDigOut_)
	{
		if (digAreaArr_[mPos.y][mPos.x] == 1)
		{

		}
	}


	if (digArea_ != nullptr)
		for (int i = 0; i < (*digArea_).size(); i++)
			Novice::ScreenPrintf(1500, 20 + i * 20, "%d,%d", (*digArea_)[i].x, (*digArea_)[i].y);
}

bool DigPiece::isMouseInField(const Vector2& _fieldSize, intVec2 _mP)
{
	if (_mP.x < 0 || _mP.y < 0 || _mP.x >= _fieldSize.x || _mP.y >= _fieldSize.y)
		return false;
	return true;
}

bool DigPiece::IsDigOutFin()
{

	return false;
}

DigPiece::DigPiece()
{

}

void DigPiece::Init(const Vector2& _fieldSize)
{
	if (digArea_ != nullptr)		digArea_->clear();
	if (dPiece_ != nullptr)			dPiece_->clear();
	if (dPieceSize_ != nullptr)		dPieceSize_->clear();
	if (dPiecePos_ != nullptr)		dPiecePos_->clear();

	ishave_ = -1;
	isDiging_ = false;
	isDigOut_ = false;
	digAreaArr_.resize(int(_fieldSize.y), std::vector<int>(int(_fieldSize.x)));
}

void DigPiece::Update(const Vector2& _fieldKeyPos, const Vector2& _fieldSize, int _mapchipSize)
{
	FieldDig(_fieldKeyPos, _fieldSize, _mapchipSize);
}

void DigPiece::Draw(const Vector2& _fieldKeyPos, int _mapchipSize)
{
	if (digArea_ != nullptr)
		for (auto& i : *digArea_)
			Novice::DrawBox(int(_fieldKeyPos.x + i.x * _mapchipSize), int(_fieldKeyPos.y + i.y * _mapchipSize), _mapchipSize - 1, _mapchipSize - 1, 0, 0xffff00ff, kFillModeSolid);

}
