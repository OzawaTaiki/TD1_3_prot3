#include "stage.h"
#include <Novice.h>

#include "player.h"
#include "CSVLoader.h"

void Stage::collisionArrReset()
{
	// collisionのリセット
	collision_.resize((*field_).size());
	for (int i = 0; i < (*field_).size(); i++)
	{
		collision_[i].resize((*field_)[i].size());
		for (int j = 0; j < (*field_)[i].size(); j++)
		{
			collision_[i][j] = (*field_)[i][j];
		}
	}
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
				piecePos_[i].x + (*piece_)[i][0].size() * kMapchipSize_ > mx_ &&
				piecePos_[i].y < my_ &&
				piecePos_[i].y + (*piece_)[i].size() * kMapchipSize_ > my_ &&
				isHave_ == -1)
			{
				isHave_ = i;
				sub_.x = piecePos_[i].x - mx_;
				sub_.y = piecePos_[i].y - my_;
				break;
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
			isHave_ = -1;
			piecePos_[i].y = float(int(piecePos_[i].y / kMapchipSize_ + (int(piecePos_[i].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);
			piecePos_[i].x = float(int(piecePos_[i].x / kMapchipSize_ + (int(piecePos_[i].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);

			if (int(piecePos_[i].x / kMapchipSize_) < (*field_)[int(piecePos_[i].y / kMapchipSize_)].size() && int(piecePos_[i].y / kMapchipSize_) < (*field_).size())
			{
				for (int y = 0; y < (*piece_)[i].size(); y++)
				{
					for (int x = 0; x < (*piece_)[i][y].size(); x++)
					{
						if ((*piece_)[i][y][x] != 0 &&
							int(piecePos_[i].x / kMapchipSize_) + x >= 0 && int(piecePos_[i].x / kMapchipSize_) + x < (*field_)[int(piecePos_[i].y / kMapchipSize_)].size() &&
							int(piecePos_[i].y / kMapchipSize_) + y >= 0 && int(piecePos_[i].y / kMapchipSize_) + y < (*field_).size())
						{
							collision_[int(piecePos_[i].y / kMapchipSize_) + y][int(piecePos_[i].x / kMapchipSize_) + x] = 1;
						}
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
		while (collision_[int(player_->GetPosY() + player_->GetMoveDir().y * k)][int(player_->GetPosX() + player_->GetMoveDir().x * k)] == 0)
		{
			k++;
		}
		k--;
		player_->SetPos(int(player_->GetPosX() + player_->GetMoveDir().x * k), int(player_->GetPosY() + player_->GetMoveDir().y * k));
	}
}

Stage::Stage()
{
	player_ = new Player;
}

void Stage::Init(int _stageNo)
{
	CSV_Loader::LoadFromCSV_s(stageFilePath_[_stageNo], '\n');

	field_ = CSV_Loader::GetPointerMapchip();
	piece_ = CSV_Loader::GetPointerPiece();

	piecePos_.resize(piece_->size());
}

void Stage::Update(char* keys, char* preKeys)
{
	collisionArrReset();
	PieceMove();

	player_->Update(keys, preKeys);
	playerCollision();
}

void Stage::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0, 0x00000080, kFillModeSolid);

	for (int y = 0; y < (*field_).size(); y++)
	{
		for (int x = 0; x < (*field_)[y].size(); x++)
		{
			Novice::DrawBox(x * kMapchipSize_, y * kMapchipSize_, kMapchipSize_ - 1, kMapchipSize_ - 1, 0, (*field_)[y][x] == 0 ? 0xffffffa0 : BLACK, kFillModeSolid);
		}
	}

	for (int i = 0; i < (*piece_).size(); i++)
	{
		for (int y = 0; y < (*piece_)[i].size(); y++)
		{
			for (int x = 0; x < (*piece_)[i][y].size(); x++)
			{
				Novice::DrawBox(int(piecePos_[i].x) + x * kMapchipSize_, int(piecePos_[i].y) + y * kMapchipSize_, kMapchipSize_ - 1, kMapchipSize_ - 1, 0, (*piece_)[i][y][x] == 0 ? 0 : color_[i], kFillModeSolid);
			}
		}
	}

	player_->Draw(kMapchipSize_);
}
