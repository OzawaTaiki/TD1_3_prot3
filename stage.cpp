﻿#include "stage.h"
#include "PhilliaFunction/Phill.h"	
#include <Novice.h>
#include "SceneChange.h"
#include "player.h"
#include "contPanel.h"
#include "CSVLoader.h"
#include <assert.h>


void Stage::collisionArrReset()
{
	int i, j;
	fieldSize_ = { 0,0 };

	// collisionのリセット
	//fieldのサイズを保存(正方形
	collision_.resize((*field_).size());
	for (i = 0; i < (*field_).size(); i++)
	{
		collision_[i].resize((*field_)[i].size());
		for (j = 0; j < (*field_)[i].size(); j++)
		{
			collision_[i][j] = (*field_)[i][j];
		}
		if (j > fieldSize_.x)
			fieldSize_.x = (float)j;
	}
	if (i > fieldSize_.y)
		fieldSize_.y = (float)i;
}

void Stage::PieceMove()
{
	Novice::GetMousePosition(&mx_, &my_);

	//押したとき
	//所持フラグをたてる
	if (Novice::IsTriggerMouse(0))
	{
		for (int i = 0; i < (*piece_).size(); i++)
		{
			if (piecePos_[i].x < mx_ &&
				piecePos_[i].x + pieceSize_[i].x * kMapchipSize_ * scal_[i] > mx_ &&
				piecePos_[i].y < my_ &&
				piecePos_[i].y + pieceSize_[i].y * kMapchipSize_ * scal_[i] > my_ &&
				isHave_ == -1)
			{
				sub_.x = (piecePos_[i].x - mx_) / scal_[i];
				sub_.y = (piecePos_[i].y - my_) / scal_[i];
				if ((*piece_)[i][int(-sub_.y * scal_[i] / (kMapchipSize_ * scal_[i]))][int(-sub_.x * scal_[i] / (kMapchipSize_ * scal_[i]))] != 0)
				{
					piecePos_[i].x = sub_.x + mx_;
					piecePos_[i].y = sub_.y + my_;

					scal_[i] = kKeyScal_[0];
					isHave_ = i;
					piecePrePos_ = piecePos_[i];

					/// 影の表示位置バグ仮修正
					if (piecePos_[isHave_].x - fieldKeyPos_.x < 0)
						scanX_ = int((piecePos_[isHave_].x - fieldKeyPos_.x - 40) / kMapchipSize_ + (int(piecePos_[isHave_].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
					else
						scanX_ = int((piecePos_[isHave_].x - fieldKeyPos_.x) / kMapchipSize_ + (int(piecePos_[isHave_].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));

					if (piecePos_[isHave_].y - fieldKeyPos_.y < 0)
						scanY_ = int((piecePos_[isHave_].y - fieldKeyPos_.y - 40) / kMapchipSize_ + (int(piecePos_[isHave_].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
					else
						scanY_ = int((piecePos_[isHave_].y - fieldKeyPos_.y) / kMapchipSize_ + (int(piecePos_[isHave_].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
					break;
				}
			}
		}
	}

	//押してるとき
	//pieceをマウスと同じように移動
	else if (Novice::IsPressMouse(0) && !Novice::IsTriggerMouse(0) && isHave_ != -1)
	{
#ifdef _DEBUG
		if (scanX_ == 0 && scanY_ == 4)
		{
			int a = 0;
			a++;
		}
#endif // _DEBUG
		piecePos_[isHave_].x = mx_ + sub_.x;
		piecePos_[isHave_].y = my_ + sub_.y;

		if (piecePos_[isHave_].x - fieldKeyPos_.x < 0)
			scanX_ = int((piecePos_[isHave_].x - fieldKeyPos_.x - 40) / kMapchipSize_ + (int(piecePos_[isHave_].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
		else
			scanX_ = int((piecePos_[isHave_].x - fieldKeyPos_.x) / kMapchipSize_ + (int(piecePos_[isHave_].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));

		if (piecePos_[isHave_].y - fieldKeyPos_.y < 0)
			scanY_ = int((piecePos_[isHave_].y - fieldKeyPos_.y - 40) / kMapchipSize_ + (int(piecePos_[isHave_].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
		else
			scanY_ = int((piecePos_[isHave_].y - fieldKeyPos_.y) / kMapchipSize_ + (int(piecePos_[isHave_].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
	}

	//押してないとき，離したとき
	//マップチップにはめる
	//当たり判定更新
	else
	{
		collisionArrReset();
		for (int i = 0; i < 4; i++)
		{
			CP_->isInPiece_[i] = false;
		}

		for (int i = 0; i < (*piece_).size(); i++)
		{
			//bool isInFrame[4] = { false,0,0,0 };
			bool isInFrame = false;
			bool isUnstackInPiece = false;			//重ねられないブロックがピース内にあるかのフラグ

#ifdef _DEBUG
			if (isHave_ != -1)
			{
				int a = 0;
				a++;
			}
#endif // _DEBUG

			piecePosInMapchip_[i].x = int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_ + (int(piecePos_[i].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));
			piecePosInMapchip_[i].y = int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_ + (int(piecePos_[i].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1));

			piecePos_[i].x = float(int(piecePos_[i].x / kMapchipSize_ + (int(piecePos_[i].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);
			piecePos_[i].y = float(int(piecePos_[i].y / kMapchipSize_ + (int(piecePos_[i].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);


			scal_[i] = kKeyScal_[1];//0.75

			for (int y = 0; y < (*piece_)[i].size(); y++)
			{
				for (int x = 0; x < (*piece_)[i][y].size(); x++)
				{
					if ((*piece_)[i][y][x] == 2)
						(*piece_)[i][y][x] = 1;

					scanX_ = int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x;
					scanY_ = int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y;

					//画面内に走査中のピースのブロックがないとき
					if (scanX_ < 0 ||
						scanY_ < 0 ||
						scanX_ >= fieldSize_.x ||
						scanY_ >= fieldSize_.y)
						continue;


					//走査中の座標にプレイヤーがいるとき
					if (scanX_ == player_->GetPosX() &&
						scanY_ == player_->GetPosY())
					{
						//プレイヤーとピースの枠が重なってるとき

						if ((*piece_)[i][int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_))][int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_))] == 1)
						{
							//isInFrame = false;
							//break;
						}

						//isInFrame = isInPiece(player_->GetPosX(), player_->GetPosY(), x, y, i);
					}
					isInFrame = true;


					//枠内にあるかどうか
					if (int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x >= 0 &&
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x < (*field_)[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y].size() &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y >= 0 &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y < (*field_).size())
					{
						if (UnStackBlockCheck(int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x, int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y))
							isUnstackInPiece = true;

						//走査中のマスがCPのとき
						else if ((*field_)[scanY_][scanX_] > 3)
						{
							CP_->isInPiece_[(*field_)[scanY_][scanX_] - 3] = isInPiece(scanX_, scanY_, x, y, i);
						}

						if ((*piece_)[i][y][x] != 0)
						{
							collision_[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y][int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x] = 1;
							if ((*field_)[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y][int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x] != 9)
								scal_[i] = kKeyScal_[0];
						}
					}
				}
			}

			/*********************************/
			/*↓↓↓↓↓↓↓↓↓↓↓↓修正予定↓↓↓↓↓↓↓↓↓↓↓↓*/
			/*********************************/
			if (scal_[i] != kKeyScal_[0])
				piecePrePos_ = { 1000.0f,30.0f + i * 200.0f };

			if (!isInFrame || isUnstackInPiece)
			{
				piecePos_[i] = piecePrePos_;
			}
		}

		isHave_ = -1;

		for (int i = 0; i < (*piece_).size() - 1; i++)
			isAdjacent(i);
	}

	/*Novice::ScreenPrintf(1000, 900, "frame = %s", isInFrame ? "true" : "false");
	Novice::ScreenPrintf(1000, 920, "unstack = %s", isUnstackInPiece ? "true" : "false");
	Novice::ScreenPrintf(900, 1020, "%.1f,%.1f", piecePrePos_.x, piecePrePos_.y);*/
	Novice::ScreenPrintf(900, 1020, "%d", isHave_);
	Novice::ScreenPrintf(900, 1040, "%d,%d", scanX_, scanY_);

}


bool Stage::UnStackBlockCheck(int x, int y)
{
	if ((*field_)[y][x] == 3)	return true;
	return false;
}


bool Stage::isInPiece(int checkX, int checkY, int x, int y, int pieceNum)
{
	bool isInFrame[4] = { false,0,0,0 };

	for (int dir = 0; dir < 4; dir++)
	{
		int move = 0;			//判定移動分
		bool isExit = false;	//強制退場用フラグ
		while (!isInFrame[dir])
		{
			switch (dir)
			{
			case 0://上
				if (int(checkY - int((piecePos_[pieceNum].y - fieldKeyPos_.y) / kMapchipSize_) - move) < 0)
				{//pieceの端を超えた時
					isExit = true;
					break;
				}
				if ((*piece_)[pieceNum][int(checkY - int((piecePos_[pieceNum].y - fieldKeyPos_.y) / kMapchipSize_) - move)][x] != 0)
				{//走査中にpieceのブロックにあたったとき
					isInFrame[0] = true;
				}

				break;
			case 1://左
				if (int(checkX - int((piecePos_[pieceNum].x - fieldKeyPos_.x) / kMapchipSize_) - move) < 0)
				{
					isExit = true;
					break;
				}
				if ((*piece_)[pieceNum][y][int(checkX - int((piecePos_[pieceNum].x - fieldKeyPos_.x) / kMapchipSize_) - move)] != 0)
				{
					isInFrame[1] = true;
				}
				break;
			case 2://下
				if (int(checkY - int((piecePos_[pieceNum].y - fieldKeyPos_.y) / kMapchipSize_) + move) > (*piece_)[pieceNum].size())
				{
					isExit = true;
					break;
				}
				if ((*piece_)[pieceNum][int(checkY - int((piecePos_[pieceNum].y - fieldKeyPos_.y) / kMapchipSize_) + move)][x] != 0)
				{
					isInFrame[2] = true;
				}
				break;
			case 3://右
				if (int(checkX - int((piecePos_[pieceNum].x - fieldKeyPos_.x) / kMapchipSize_) + move) > (*piece_)[pieceNum][y].size())
				{
					isExit = true;
					break;
				}
				if ((*piece_)[pieceNum][y][int(checkX - int((piecePos_[pieceNum].x - fieldKeyPos_.x) / kMapchipSize_) + move)] != 0)
				{
					isInFrame[3] = true;
				}
				break;
			default:
				break;
			}
			move++;
			if (isExit)
				break;
		}
		if (!isInFrame[dir])//中になかったら即抜け
			return false;
	}

	return true;
}

void Stage::DrawPieceShadow()
{
	if (isHave_ != -1)
	{
		for (int y = 0; y < (*piece_)[isHave_].size(); y++)
		{
			for (int x = 0; x < (*piece_)[isHave_][y].size(); x++)
			{
				if (int((scanX_ + x) * kMapchipSize_ * scal_[isHave_]) / kMapchipSize_ >= 0 &&
					int((scanX_ + x) * kMapchipSize_ * scal_[isHave_]) / kMapchipSize_ < fieldSize_.x &&
					int((scanY_ + y) * kMapchipSize_ * scal_[isHave_]) / kMapchipSize_ >= 0 &&
					int((scanY_ + y) * kMapchipSize_ * scal_[isHave_]) / kMapchipSize_ < (*field_).size())

					Novice::DrawBox(int(fieldKeyPos_.x + (scanX_ + x) * kMapchipSize_ * scal_[isHave_]), int(fieldKeyPos_.y + (scanY_ + y) * kMapchipSize_ * scal_[isHave_]), int(kMapchipSize_ * scal_[isHave_]) - 1, int(kMapchipSize_ * scal_[isHave_]) - 1, 0, (*piece_)[isHave_][y][x] == 0 ? 0 : 0xff, kFillModeSolid);
			}
		}
	}
}

bool Stage::isAdjacent(int _pieceNum)
{
	bool isAdjacent = false;
	for (int i = _pieceNum + 1; i < piece_->size(); i++)
	{
		if (i == _pieceNum)
			continue;

		if (piecePosInMapchip_[i].x <= piecePosInMapchip_[_pieceNum].x + pieceSize_[_pieceNum].x - 1 &&
			piecePosInMapchip_[i].x >= piecePosInMapchip_[_pieceNum].x ||
			piecePosInMapchip_[i].x + pieceSize_[i].x - 1 <= piecePosInMapchip_[_pieceNum].x + pieceSize_[_pieceNum].x - 1 &&
			piecePosInMapchip_[i].x + pieceSize_[i].x - 1 >= piecePosInMapchip_[_pieceNum].x)
		{
			if (piecePosInMapchip_[i].y - 1 == piecePosInMapchip_[_pieceNum].y + pieceSize_[_pieceNum].y - 1 ||
				piecePosInMapchip_[i].y + pieceSize_[i].y - 1 == piecePosInMapchip_[_pieceNum].y - 1)
			{
				Novice::ScreenPrintf(1500, 800, "x");
				AdjacentPos(_pieceNum, i, 'x');
				isAdjacent = true;
			}
		}
		else if (piecePosInMapchip_[i].y <= piecePosInMapchip_[_pieceNum].y + pieceSize_[_pieceNum].y - 1 &&
			piecePosInMapchip_[i].y >= piecePosInMapchip_[_pieceNum].y ||
			piecePosInMapchip_[i].y + pieceSize_[i].y - 1 <= piecePosInMapchip_[_pieceNum].y + pieceSize_[_pieceNum].y - 1 &&
			piecePosInMapchip_[i].y + pieceSize_[i].y - 1 >= piecePosInMapchip_[_pieceNum].y)
		{
			if (piecePosInMapchip_[i].x - 1 == piecePosInMapchip_[_pieceNum].x + pieceSize_[_pieceNum].x - 1 ||
				piecePosInMapchip_[i].x + pieceSize_[i].x - 1 == piecePosInMapchip_[_pieceNum].x - 1)
			{
				Novice::ScreenPrintf(1500, 800, "y");
				AdjacentPos(_pieceNum, i, 'y');
				isAdjacent = true;
			}
		}
		if (isAdjacent)
		{
			AdjacentPieceDelete(_pieceNum, i);
			break;
		}
	}
	return false;
}

void Stage::AdjacentPos(int _pieceNum1, int _pieceNum2, char _dir)
{
	adjacentDir_.push_back(_dir);
	switch (_dir)
	{
	case 'x':
		if (piecePosInMapchip_[_pieceNum1].x > piecePosInMapchip_[_pieceNum2].x)
		{
			if (piecePosInMapchip_[_pieceNum1].y < piecePosInMapchip_[_pieceNum2].y)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum1].x,piecePosInMapchip_[_pieceNum2].y - 1 });
			else if (piecePosInMapchip_[_pieceNum1].y > piecePosInMapchip_[_pieceNum2].y)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum1].x,piecePosInMapchip_[_pieceNum1].y - 1 });
		}
		else if (piecePosInMapchip_[_pieceNum1].x < piecePosInMapchip_[_pieceNum2].x)
		{
			if (piecePosInMapchip_[_pieceNum1].y < piecePosInMapchip_[_pieceNum2].y)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum2].x,piecePosInMapchip_[_pieceNum2].y - 1 });
			else if (piecePosInMapchip_[_pieceNum1].y > piecePosInMapchip_[_pieceNum2].y)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum2].x,piecePosInMapchip_[_pieceNum1].y - 1 });
		}
		else if (piecePosInMapchip_[_pieceNum1].x == piecePosInMapchip_[_pieceNum2].x)
		{
			if (piecePosInMapchip_[_pieceNum1].y < piecePosInMapchip_[_pieceNum2].y)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum2].x,piecePosInMapchip_[_pieceNum2].y - 1 });
			else if (piecePosInMapchip_[_pieceNum1].y > piecePosInMapchip_[_pieceNum2].y)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum1].x,piecePosInMapchip_[_pieceNum1].y - 1 });
		}
		break;
	case 'y':
		if (piecePosInMapchip_[_pieceNum1].y > piecePosInMapchip_[_pieceNum2].y)
		{
			if (piecePosInMapchip_[_pieceNum1].x > piecePosInMapchip_[_pieceNum2].x)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum1].x - 1,piecePosInMapchip_[_pieceNum1].y });
			else if (piecePosInMapchip_[_pieceNum1].x < piecePosInMapchip_[_pieceNum2].x)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum2].x - 1,piecePosInMapchip_[_pieceNum1].y });
		}
		else if (piecePosInMapchip_[_pieceNum1].y < piecePosInMapchip_[_pieceNum2].y)
		{
			if (piecePosInMapchip_[_pieceNum1].x > piecePosInMapchip_[_pieceNum2].x)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum1].x - 1,piecePosInMapchip_[_pieceNum2].y });
			else if (piecePosInMapchip_[_pieceNum1].x < piecePosInMapchip_[_pieceNum2].x)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum2].x - 1,piecePosInMapchip_[_pieceNum2].y });
		}
		else if (piecePosInMapchip_[_pieceNum1].y == piecePosInMapchip_[_pieceNum2].y)
		{
			if (piecePosInMapchip_[_pieceNum1].x < piecePosInMapchip_[_pieceNum2].x)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum2].x - 1,piecePosInMapchip_[_pieceNum2].y });
			else if (piecePosInMapchip_[_pieceNum1].x > piecePosInMapchip_[_pieceNum2].x)
				adjacentPos_.push_back({ piecePosInMapchip_[_pieceNum1].x - 1,piecePosInMapchip_[_pieceNum1].y });
		}
		break;
	default:
		assert(_dir == 'x' || _dir == 'y');
		break;
	}
	for (int i = 0; i < adjacentPos_.size(); i++)
	{
		if (adjacentPos_.back().x == adjacentPos_[i].x &&
			adjacentPos_.back().y == adjacentPos_[i].y &&
			adjacentPos_.size() - 1 != i)
		{
			adjacentPos_.pop_back();
			break;
		}
	}
}

void Stage::AdjacentPieceDelete(int _pieceNum1, int _pieceNum2)
{
	for (int i = 0; i < adjacentDir_.size(); i++)
	{
		int count1 = 1;
		int count2 = 1;
		int temp1;
		int temp2;

		switch (adjacentDir_[i])
		{
		case 'x':

			temp1 = adjacentPos_[i].x - piecePosInMapchip_[_pieceNum1].x;
			temp2 = adjacentPos_[i].x - piecePosInMapchip_[_pieceNum2].x;

			if (piecePosInMapchip_[_pieceNum1].y < piecePosInMapchip_[_pieceNum2].y)
			{
				while (temp1 + count1 < (*piece_)[_pieceNum1][(*piece_)[_pieceNum1].size() - 1].size() - 1 &&
					temp2 + count2 < (*piece_)[_pieceNum2][0].size() - 1)
				{
					(*piece_)[_pieceNum1][(*piece_)[_pieceNum1].size() - 1][temp1 + count1++] = 2;
					(*piece_)[_pieceNum2][0][temp2 + count2++] = 2;
				}
			}
			else if (piecePosInMapchip_[_pieceNum1].y > piecePosInMapchip_[_pieceNum2].y)
			{
				while (temp1 + count1 < (*piece_)[_pieceNum1][0].size() - 1 &&
					temp2 + count2 < (*piece_)[_pieceNum2][(*piece_)[_pieceNum2].size() - 1].size() - 1)
				{
					(*piece_)[_pieceNum1][0][temp1 + count1++] = 2;
					(*piece_)[_pieceNum2][(*piece_)[_pieceNum2].size() - 1][temp2 + count2++] = 2;
				}
			}
			break;
		case 'y':
			temp1 = adjacentPos_[i].y - piecePosInMapchip_[_pieceNum1].y;
			temp2 = adjacentPos_[i].y - piecePosInMapchip_[_pieceNum2].y;

			if (piecePosInMapchip_[_pieceNum1].x > piecePosInMapchip_[_pieceNum2].x)
			{
				while (temp1 + count1 < (*piece_)[_pieceNum1].size() - 1 &&
					temp2 + count2 < (*piece_)[_pieceNum2].size() - 1)
				{
					(*piece_)[_pieceNum1][temp1 + count1++][0] = 2;
					(*piece_)[_pieceNum2][temp2 + count2++][(*piece_)[_pieceNum2][temp2 + count2].size() - 1] = 2;
				}
			}
			else if (piecePosInMapchip_[_pieceNum1].x < piecePosInMapchip_[_pieceNum2].x)
			{
				while (temp1 + count1 < (*piece_)[_pieceNum1].size() - 1 &&
					temp2 + count2 < (*piece_)[_pieceNum2].size() - 1)
				{
					(*piece_)[_pieceNum1][temp1 + count1++][(*piece_)[_pieceNum1][temp1 + count1].size() - 1] = 2;
					(*piece_)[_pieceNum2][temp2 + count2++][0] = 2;
				}
			}
			break;
		default:
			break;
		}
	}
}

int Stage::playerCollision()
{
	if ((*field_)[player_->GetPosY()][player_->GetPosX()] == 2)
		isNext_ = true;
	if (player_->GetMoveDir().x != 0 || player_->GetMoveDir().y != 0)
	{
		int k = 1;
		if (collision_[int(player_->GetPosY() + player_->GetMoveDir().y * k)][int(player_->GetPosX() + player_->GetMoveDir().x * k)] == 1)
		{
			k = 0;
		}

		if (player_->GetMoveDir().x != 0)
			return int(player_->GetPosX() + player_->GetMoveDir().x * k);
		if (player_->GetMoveDir().y != 0)
			return int(player_->GetPosY() + player_->GetMoveDir().y * k);
	}

	return -1;
}

Stage::Stage()
{
	sceneChange_ = nullptr;
	backGroundTexture = Novice::LoadTexture("./img/background.png");
	blockTexture = Novice::LoadTexture("./img/block.png");
	pieceTexture = Novice::LoadTexture("./img/pieceBlock.png");
	goalTexture = Novice::LoadTexture("./img/goal.png");
	obstacleTexture = Novice::LoadTexture("./img/obstacleBlock.png");
	stageCntTexture[0] = Novice::LoadTexture("./img/stage1.png");
	stageCntTexture[1] = Novice::LoadTexture("./img/stage2.png");
	stageCntTexture[2] = Novice::LoadTexture("./img/stage3.png");
	slashTexture = Novice::LoadTexture("./img/sl.png");
	descriptionTexture = Novice::LoadTexture("./img/desc.png");
	controllTexture = Novice::LoadTexture("./img/ctrl.png");

	player_ = new Player(kMapchipSize_);
	CP_ = new ControlPanel;
}

void Stage::Init(int _stageNo)
{
	fieldKeyPos_ = { kWindowWidth * 1 / 3,kWindowHeight / 2 };

	piecePos_.clear();
	scal_.clear();
	if (field_ != nullptr)		field_->clear();
	if (piece_ != nullptr)		piece_->clear();
	adjacentPos_.clear();
	adjacentDir_.clear();

	if (_stageNo < maxStages)
	{
		CSV_Loader::LoadFromCSV_s(stageFilePath_[_stageNo], '\n');

		field_ = CSV_Loader::GetPointerMapchip();
		piece_ = CSV_Loader::GetPointerPiece();

		piecePos_.resize(piece_->size());
		piecePosInMapchip_.resize(piece_->size());
		pieceSize_.resize(piece_->size());
		scal_.resize(piece_->size());

		isViewDescription = 0;

		for (int i = 0; i < piecePos_.size(); i++)
		{
			piecePos_[i].x = 1000.0f;
			piecePos_[i].y = 30.0f + i * 200.0f;

			piecePrePos_ = piecePos_[i];
		}
		collisionArrReset();

		fieldKeyPos_.x -= fieldSize_.x / 2 * kMapchipSize_;
		fieldKeyPos_.x = float((int)fieldKeyPos_.x / kMapchipSize_ * kMapchipSize_);
		fieldKeyPos_.y -= fieldSize_.y / 2 * kMapchipSize_;
		fieldKeyPos_.y = float((int)fieldKeyPos_.y / kMapchipSize_ * kMapchipSize_);



		for (int i = 0; i < (*piece_).size(); i++)
		{
			pieceSize_[i] = { 0,0 };

			for (int j = 0; j < (*piece_)[i].size(); j++)
			{
				if (pieceSize_[i].x < (*piece_)[i][j].size())
					pieceSize_[i].x = (float)(*piece_)[i][j].size();
			}
			if (pieceSize_[i].y < (*piece_)[i].size())
				pieceSize_[i].y = (float)(*piece_)[i].size();
		}

		isNext_ = false;

		player_->Init(_stageNo);

	}
	else
	{
		// クリア判定
		isClear = true;
	}

}

void Stage::Update(char* keys, char* preKeys)
{
	if (keys[DIK_RETURN] && !preKeys[DIK_RETURN] && !sceneChange_)
		isNext_ = true;

	/// リセット，仮実装
	if (keys[DIK_R] && !preKeys[DIK_R] && !sceneChange_)
	{
		isNext_ = true;
		selectStage_--;
	}

	if (keys[DIK_TAB] && !preKeys[DIK_TAB])
	{
		isViewDescription++;
		isViewDescription %= 2;
	}

	if (isNext_ || sceneChange_)
	{
		if (!sceneChange_)
		{
			sceneChange_ = new SceneChange;
		}

		if (sceneChange_)
		{
			sceneChange_->Update();
			if (sceneChange_->GetIsTileEnd() && isNext_)
			{
				selectStage_++;
				if (selectStage_ > maxStages)
				{
					selectStage_ = 0;
				}
				Init(selectStage_);
			}
		}

		return;
	}

	adjacentPos_.clear();
	adjacentDir_.clear();
	PieceMove();



	player_->Input(keys, preKeys, CP_->isInPiece_);

	player_->Move(playerCollision());

}

void Stage::Draw()
{


	Novice::DrawSprite(0, 0, backGroundTexture, 1.0f, 1.0f, 0.0f, 0xffffffff);
	Novice::DrawBox(0, 0, 1920, 1080, 0, 0x80, kFillModeSolid);

	// Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0, 0x00000080, kFillModeSolid);

	//Novice::ScreenPrintf(0, 1000, "%.1f,%.1f", fieldSize_.x, fieldSize_.y);

	//Novice::ScreenPrintf(mx_, my_, "( %4d , %4d )", mx_, my_);

	Phill::DrawQuadPlus(240, 180, 32, 52, 1.0f, 1.0f, 0.0f, 0, 0, 32, 52, stageCntTexture[selectStage_], 0xffffffff, DrawMode_LeftTop);
	Phill::DrawQuadPlus(240 + 70, 172, 32, 64, 1.0f, 1.0f, 0.0f, 0, 0, 32, 64, slashTexture, 0xffffffff, DrawMode_LeftTop);
	Phill::DrawQuadPlus(240 + 48 * 3, 180, 32, 52, 1.0f, 1.0f, 0.0f, 0, 0, 32, 52, stageCntTexture[2], 0x000000ff, DrawMode_LeftTop);

	for (int y = 0; y < (*field_).size(); y++)
	{
		for (int x = 0; x < (*field_)[y].size(); x++)
		{
			if ((*field_)[y][x] != 9)
			{
				if ((*field_)[y][x] > 3)//CP描画
				{
					CP_->Draw(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), (*field_)[y][x] - 3);
				}
				if ((*field_)[y][x] != 9 && (*field_)[y][x] != 2)
					Novice::DrawBox(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), kMapchipSize_ - 1, kMapchipSize_ - 1, 0, kTileColor_[(*field_)[y][x]], kFillModeSolid);
				if ((*field_)[y][x] == 1)
					Phill::DrawQuadPlus(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), kMapchipSize_ - 1, kMapchipSize_ - 1, 1.0f, 1.0f, 0.0f, 7 * 64, 0, 64, 64, blockTexture, 0xffffffff, PhillDrawMode::DrawMode_LeftTop);

				if ((*field_)[y][x] == 2)
					Phill::DrawQuadPlus(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), kMapchipSize_ - 1, kMapchipSize_ - 1, 1.0f, 1.0f, 0.0f, 0, 0, 64, 64, goalTexture, 0xffffffff, PhillDrawMode::DrawMode_LeftTop);

				if ((*field_)[y][x] == 3)
					Phill::DrawQuadPlus(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), kMapchipSize_ - 1, kMapchipSize_ - 1, 1.0f, 1.0f, 0.0f, 0, 0, 64, 64, obstacleTexture, 0xffffffff, PhillDrawMode::DrawMode_LeftTop);
			}

			//Novice::ScreenPrintf(1000 + x * 20, y * 20, "%d", collision_[y][x]);

		}
	}

	DrawPieceShadow();

	for (int i = 0; i < (*piece_).size(); i++)
	{
		//Novice::ScreenPrintf(0, 1020 + i * 20, "%.1f,%.1f", pieceSize_[i].x, pieceSize_[i].y);
		Novice::ScreenPrintf(900, 900 + i * 20, "%d,%d", piecePosInMapchip_[i].x, piecePosInMapchip_[i].y);

		for (int y = 0; y < (*piece_)[i].size(); y++)
		{
			for (int x = 0; x < (*piece_)[i][y].size(); x++)
			{
				if ((*piece_)[i][y][x] == 1)
					Phill::DrawQuadPlus(int(piecePos_[i].x + x * kMapchipSize_ * scal_[i]), int(piecePos_[i].y + y * kMapchipSize_ * scal_[i]), int(kMapchipSize_ * scal_[i]) - 1, int(kMapchipSize_ * scal_[i]) - 1, 1.0f, 1.0f, 0.0f, (i % 7) * 120, 0, 120, 120, pieceTexture, 0xffffffda, PhillDrawMode::DrawMode_LeftTop);
			}
		}
	}

	for (int i = 0; i < adjacentPos_.size(); i++)
	{
		Novice::ScreenPrintf(1400, 720 + i * 20, "%d,%d", adjacentPos_[i].x, adjacentPos_[i].y);
		Novice::DrawBox(int(fieldKeyPos_.x + adjacentPos_[i].x * kMapchipSize_), int(fieldKeyPos_.y + adjacentPos_[i].y * kMapchipSize_), kMapchipSize_, kMapchipSize_, 0, RED, kFillModeWireFrame);
	}

	player_->Draw(fieldKeyPos_);

	Phill::DrawQuadPlus(1920 / 2, 1080 / 2, 640, 360, 1.0f, 1.0f, 0.0f, 0, 0, 640, 360, descriptionTexture, 0xffffffff * isViewDescription, DrawMode_Center);
	Phill::DrawQuadPlus(50, 800, 550, 220, 1.0f, 1.0f, 0.0f, 0, 0, 550, 220, controllTexture, 0xffffffff, DrawMode_LeftTop);

	if (sceneChange_)
	{
		sceneChange_->Draw();
		if (sceneChange_->GetIsEnd())
		{
			delete sceneChange_;
			sceneChange_ = nullptr;
		}
	}

}

int Stage::GetIsClear() const
{
	return isClear;
}

SceneChange* Stage::GetSceneChgPtr()
{
	return sceneChange_;
}
