﻿#include "stage.h"
#include <Novice.h>

#include "player.h"
#include "CSVLoader.h"
#include "UI_Manager.h"

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
				piecePos_[i].x + (*piece_)[i][0].size() * kMapchipSize_ * scal_[i] > mx_ &&
				piecePos_[i].y < my_ &&
				piecePos_[i].y + (*piece_)[i].size() * kMapchipSize_ * scal_[i]> my_ &&
				isHave_ == -1)
			{
				sub_.x = piecePos_[i].x - mx_;
				sub_.y = piecePos_[i].y - my_;
				if ((*piece_)[i][int(-sub_.y / (kMapchipSize_ * scal_[i]))][int(-sub_.x / (kMapchipSize_ * scal_[i]))] != 0)
				{
					scal_[i] = kKeyScal_[0];
					isHave_ = i;
					break;
				}
			}
		}
	}
	//押してるとき
	//pieceをマウスと同じように移動
	else if (Novice::IsPressMouse(0) && !Novice::IsTriggerMouse(0) && isHave_ != -1)
	{
		piecePos_[isHave_].x = mx_ + sub_.x;
		piecePos_[isHave_].y = my_ + sub_.y;
	}

	//押してないとき，離したとき
	//マップチップにはめる
	//当たり判定更新
	else
	{
		for (int i = 0; i < (*piece_).size(); i++)
		{
			if (isHave_ != -1)
			{
				int a = 0;
				a++;
			}

			isHave_ = -1;
			piecePos_[i].y = float(int(piecePos_[i].y / kMapchipSize_ + (int(piecePos_[i].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);
			piecePos_[i].x = float(int(piecePos_[i].x / kMapchipSize_ + (int(piecePos_[i].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);

			scal_[i] = kKeyScal_[1];

			for (int y = 0; y < (*piece_)[i].size(); y++)
			{
				for (int x = 0; x < (*piece_)[i][y].size(); x++)
				{
					if (int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x < 0 ||
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y < 0 ||
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x >= fieldSize_.x ||
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y >= fieldSize_.y)
						continue;

					if ((*piece_)[i][y][x] != 0 &&
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x >= 0 &&
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x < (*field_)[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y].size() &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y >= 0 &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y < (*field_).size())
					{
						collision_[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y][int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x] = 1;
						scal_[i] = kKeyScal_[0];
					}
				}
			}
		}
	}
}

void Stage::playerCollision()
{
	if (player_->GetMoveDir().x != 0 || player_->GetMoveDir().y != 0)
	{
		int k = 0;
		while (collision_[int(player_->GetPosY() + player_->GetMoveDir().y * k)][int(player_->GetPosX() + player_->GetMoveDir().x * k)] != 1)
		{
			k++;
		}
		k--;
		player_->SetPos(int(player_->GetPosX() + player_->GetMoveDir().x * k), int(player_->GetPosY() + player_->GetMoveDir().y * k));
	}

	if ((*field_)[player_->GetPosY()][player_->GetPosX()] == 2)
		isNext_ = true;
}

Stage::Stage()
{
	player_ = new Player;
}

void Stage::MoveScrollWindow(int _posX, int _posY, int _num)
{
	piecePos_[_num].x = float(_posX);
	piecePos_[_num].y = float(_posY);
}

void Stage::Init(int _stageNo)
{
	fieldKeyPos_ = { kWindowWidth * 1 / 3,kWindowHeight / 2 };

	piecePos_.clear();
	scal_.clear();
	if (field_ != nullptr)		field_->clear();
	if (piece_ != nullptr)		piece_->clear();

	CSV_Loader::LoadFromCSV_s(stageFilePath_[_stageNo], '\n');

	field_ = CSV_Loader::GetPointerMapchip();
	piece_ = CSV_Loader::GetPointerPiece();

	piecePos_.resize(piece_->size());
	scal_.resize(piece_->size());

	for (int i = 0; i < piecePos_.size(); i++)
	{
		piecePos_[i].x = 1000.0f;
		piecePos_[i].y = 30.0f + i * 200.0f;
	}
	collisionArrReset();

	fieldKeyPos_.x -= fieldSize_.x / 2 * kMapchipSize_;
	fieldKeyPos_.x = float((int)fieldKeyPos_.x / kMapchipSize_ * kMapchipSize_);
	fieldKeyPos_.y -= fieldSize_.y / 2 * kMapchipSize_;
	fieldKeyPos_.y = float((int)fieldKeyPos_.y/ kMapchipSize_ * kMapchipSize_);

	isNext_ = false;

	player_->Init(_stageNo);
}

void Stage::Update(char* keys, char* preKeys)
{
	if (keys[DIK_RETURN] && !preKeys[DIK_RETURN])
		isNext_ = true;

	if (isNext_)
	{
		selectStage_++;
		Init(selectStage_);
		return;
	}

	collisionArrReset();
	PieceMove();

	player_->Update(keys, preKeys);
	playerCollision();
}

void Stage::Draw()
{
	Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0, 0x00000080, kFillModeSolid);

	Novice::ScreenPrintf(0, 1000, "%.1f,%.1f", fieldSize_.x, fieldSize_.y);

	for (int y = 0; y < (*field_).size(); y++)
	{
		for (int x = 0; x < (*field_)[y].size(); x++)
		{
			Novice::DrawBox(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), kMapchipSize_ - 1, kMapchipSize_ - 1, 0, kTileColor_[(*field_)[y][x]], kFillModeSolid);
			//Novice::ScreenPrintf(1000 + x * 20, y * 20, "%d", collision_[y][x]);
		}
	}

	for (int i = 0; i < (*piece_).size(); i++)
	{
		for (int y = 0; y < (*piece_)[i].size(); y++)
		{
			for (int x = 0; x < (*piece_)[i][y].size(); x++)
			{
				Novice::DrawBox(int(piecePos_[i].x + x * kMapchipSize_ * scal_[i]), int(piecePos_[i].y + y * kMapchipSize_ * scal_[i]), int(kMapchipSize_ * scal_[i]) - 1, int(kMapchipSize_ * scal_[i]) - 1, 0, (*piece_)[i][y][x] == 0 ? 0 : color_[i], kFillModeSolid);
			}
		}
	}

	player_->Draw(kMapchipSize_, fieldKeyPos_);
}

bool Stage::IsPieceOnRect(Vector2 _pos, float _width, float _height, int* _isHave)
{
	if (isHave_ != -1)
	{
		*_isHave = isHave_;
		if (_pos.x				< float(mx_) &&
			_pos.x + _width		> float(mx_) &&
			_pos.y				< float(my_) &&
			_pos.y + _height	> float(my_))
		{
			return true;
		}
	}
	return false;
}
