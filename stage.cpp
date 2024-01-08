#include "stage.h"
#include <Novice.h>

#include "player.h"
#include "CSVLoader.h"

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
			bool isInFrame[4] = { false,0,0,0 };
			bool isUnstackInPiece = false;			//重ねられないブロックがピース内にあるかのフラグ

			//デバッグ用
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
					//画面内に走査中のピースのブロックがないとき
					if (int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x < 0 ||
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y < 0 ||
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x >= fieldSize_.x ||
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y >= fieldSize_.y)
						continue;

					//走査中の座標にプレイヤーがいるとき
					if (int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x == player_->GetPosX() &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y == player_->GetPosY())
					{
						//プレイヤーとピースの枠が重なってるとき
						if ((*piece_)[i][int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_))][int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_))] == 1)
						{
							isInFrame[0] = true;
							break;
						}

						for (int dir = 0; dir < 4; dir++)
						{
							int move = 0;
							bool isExit = false;
							while (!isInFrame[dir])
							{
								switch (dir)
								{
								case 0://上
									if (int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) - move) < 0)
									{
										isExit = true;
										break;
									}
									if ((*piece_)[i][int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) - move)][x] == 1)
									{
										isInFrame[0] = true;
									}

									break;
								case 1://左
									if (int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) - move) < 0)
									{
										isExit = true;
										break;
									}
									if ((*piece_)[i][y][int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) - move)] == 1)
									{
										isInFrame[1] = true;
									}
									break;
								case 2://下
									if (int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + move) > (*piece_)[i].size())
									{
										isExit = true;
										break;
									}
									if ((*piece_)[i][int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + move)][x] == 1)
									{
										isInFrame[2] = true;
									}
									break;
								case 3://右
									if (int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + move) > (*piece_)[i][y].size())
									{
										isExit = true;
										break;
									}
									if ((*piece_)[i][y][int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + move)] == 1)
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
						}
					}


					//枠内にあるかどうか
					if (int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x >= 0 &&
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x < (*field_)[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y].size() &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y >= 0 &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y < (*field_).size())
					{
						if (UnStackBlockCheck(int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x, int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y))
							isUnstackInPiece = true;

						if ((*piece_)[i][y][x] != 0)
						{
							collision_[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y][int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x] = 1;
							if ((*field_)[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y][int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x] != 9)
								scal_[i] = kKeyScal_[0];
						}
					}
				}
			}
			if (scal_[i] != kKeyScal_[0])
				piecePrePos_ = { 1000.0f,30.0f + i * 200.0f };

			if (!isInFrame[0] || !isInFrame[1] || !isInFrame[2] || !isInFrame[3] || isUnstackInPiece)
			{
				piecePos_[i] = piecePrePos_;
			}

		}
	}
	Novice::ScreenPrintf(900, 1020, "%.1f,%.1f", piecePrePos_.x, piecePrePos_.y);

}


bool Stage::UnStackBlockCheck(int x, int y)
{
	if ((*field_)[y][x] == 3)	return true;
	return false;
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
	pieceSize_.resize(piece_->size());
	scal_.resize(piece_->size());

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
			if ((*field_)[y][x] != 9)
				Novice::DrawBox(int(fieldKeyPos_.x + x * kMapchipSize_), int(fieldKeyPos_.y + y * kMapchipSize_), kMapchipSize_ - 1, kMapchipSize_ - 1, 0, kTileColor_[(*field_)[y][x]], kFillModeSolid);
			Novice::ScreenPrintf(1000 + x * 20, y * 20, "%d", collision_[y][x]);
		}
	}


	for (int i = 0; i < (*piece_).size(); i++)
	{
		Novice::ScreenPrintf(0, 1020 + i * 20, "%.1f,%.1f", pieceSize_[i].x, pieceSize_[i].y);

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
