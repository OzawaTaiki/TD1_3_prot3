#include "stage.h"
#include "PhilliaFunction/Phill.h"	
#include <Novice.h>
#include "SceneChange.h"
#include "player.h"
#include "contPanel.h"
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
	//bool isInFrame[4] = { false,0,0,0 };
	bool isInFrame = false;
	bool isUnstackInPiece = false;			//重ねられないブロックがピース内にあるかのフラグ

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
		collisionArrReset();
		for (int i = 0; i < 4; i++)
		{
			CP_->isInPiece_[i] = false;
		}

		for (int i = 0; i < (*piece_).size(); i++)
		{
			//デバッグ用
			if (isHave_ != -1)
			{
				int a = 0;
				a++;
			}

			isHave_ = -1;
			piecePos_[i].y = float(int(piecePos_[i].y / kMapchipSize_ + (int(piecePos_[i].y) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);
			piecePos_[i].x = float(int(piecePos_[i].x / kMapchipSize_ + (int(piecePos_[i].x) % kMapchipSize_ < kMapchipSize_ / 2 ? 0 : 1)) * kMapchipSize_);

			scal_[i] = kKeyScal_[1];//0.75

			for (int y = 0; y < (*piece_)[i].size(); y++)
			{
				for (int x = 0; x < (*piece_)[i][y].size(); x++)
				{
					int scanX = int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x;
					int scanY = int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y;

					//画面内に走査中のピースのブロックがないとき
					if (scanX < 0 ||
						scanY < 0 ||
						scanX >= fieldSize_.x ||
						scanY >= fieldSize_.y)
						continue;


					//走査中の座標にプレイヤーがいるとき
					if (scanX == player_->GetPosX() &&
						scanY == player_->GetPosY())
					{
						//プレイヤーとピースの枠が重なってるとき

						if ((*piece_)[i][int(player_->GetPosY() - int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_))][int(player_->GetPosX() - int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_))] == 1)
						{
							isInFrame = false;
							break;
						}

						isInFrame = isInPiece(player_->GetPosX(), player_->GetPosY(), x, y, i);
					}


					//枠内にあるかどうか
					if (int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x >= 0 &&
						int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x < (*field_)[int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y].size() &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y >= 0 &&
						int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y < (*field_).size())
					{
						if (UnStackBlockCheck(int((piecePos_[i].x - fieldKeyPos_.x) / kMapchipSize_) + x, int((piecePos_[i].y - fieldKeyPos_.y) / kMapchipSize_) + y))
							isUnstackInPiece = true;

						//走査中のマスがCPのとき
						else if ((*field_)[scanY][scanX] > 3)
						{
							CP_->isInPiece_[(*field_)[scanY][scanX] - 3] = isInPiece(scanX, scanY, x, y, i);
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
	}

	Novice::ScreenPrintf(1000, 900, "frame = %s", isInFrame ? "true" : "false");
	Novice::ScreenPrintf(1000, 920, "unstack = %s", isUnstackInPiece ? "true" : "false");
	Novice::ScreenPrintf(900, 1020, "%.1f,%.1f", piecePrePos_.x, piecePrePos_.y);

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
				if ((*piece_)[pieceNum][int(checkY - int((piecePos_[pieceNum].y - fieldKeyPos_.y) / kMapchipSize_) - move)][x] == 1)
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
				if ((*piece_)[pieceNum][y][int(checkX - int((piecePos_[pieceNum].x - fieldKeyPos_.x) / kMapchipSize_) - move)] == 1)
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
				if ((*piece_)[pieceNum][int(checkY - int((piecePos_[pieceNum].y - fieldKeyPos_.y) / kMapchipSize_) + move)][x] == 1)
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
				if ((*piece_)[pieceNum][y][int(checkX - int((piecePos_[pieceNum].x - fieldKeyPos_.x) / kMapchipSize_) + move)] == 1)
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

	if (_stageNo < maxStages)
	{
		CSV_Loader::LoadFromCSV_s(stageFilePath_[_stageNo], '\n');

		field_ = CSV_Loader::GetPointerMapchip();
		piece_ = CSV_Loader::GetPointerPiece();

		piecePos_.resize(piece_->size());
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

	PieceMove();


	player_->Input(keys, preKeys, CP_->isInPiece_);

	player_->Move(playerCollision());

}

void Stage::Draw()
{
	Novice::DrawSprite(0, 0, backGroundTexture, 1.0f, 1.0f, 0.0f, 0xffffffff);

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



	for (int i = 0; i < (*piece_).size(); i++)
	{
		//Novice::ScreenPrintf(0, 1020 + i * 20, "%.1f,%.1f", pieceSize_[i].x, pieceSize_[i].y);

		for (int y = 0; y < (*piece_)[i].size(); y++)
		{
			for (int x = 0; x < (*piece_)[i][y].size(); x++)
			{
				Phill::DrawQuadPlus(int(piecePos_[i].x + x * kMapchipSize_ * scal_[i]), int(piecePos_[i].y + y * kMapchipSize_ * scal_[i]), int(kMapchipSize_ * scal_[i]) - 1, int(kMapchipSize_ * scal_[i]) - 1, 1.0f, 1.0f, 0.0f, (i % 7) * 120, 0, 120, 120, pieceTexture, (*piece_)[i][y][x] == 0 ? 0 : 0xffffffda, PhillDrawMode::DrawMode_LeftTop);
				//Novice::DrawBox(int(piecePos_[i].x + x * kMapchipSize_ * scal_[i]), int(piecePos_[i].y + y * kMapchipSize_ * scal_[i]), int(kMapchipSize_ * scal_[i]) - 1, int(kMapchipSize_ * scal_[i]) - 1, 0, (*piece_)[i][y][x] == 0 ? 0 : color_[i], kFillModeSolid);
			}
		}
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
