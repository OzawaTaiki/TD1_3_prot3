#include "stage.h"
#include <Novice.h>
#include "player.h"

void Stage::collisionArrReset()
{
	// collisionのリセット
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			collision[i][j] = field[i][j];
		}
	}
}

void Stage::PieceMove()
{
	Novice::GetMousePosition(&mx, &my);

	//押したとき
		//所持フラグをたてる
	if (Novice::IsTriggerMouse(0))
	{
		for (int i = 0; i < 10; i++)
		{
			if (piecePos[i].x < mx &&
				piecePos[i].x + 10 * 40 > mx &&
				piecePos[i].y < my &&
				piecePos[i].y + 7 * 40 > my &&
				!isHave[i])
			{
				isHave[i] = true;
				sub.x = piecePos[i].x - mx;
				sub.y = piecePos[i].y - my;
				break;
			}
		}
	}
	//押してるとき
	//pieceをマウスと同じように移動
	else if (Novice::IsPressMouse(0) && !Novice::IsTriggerMouse(0))
	{
		for (int i = 0; i < 10; i++)
		{
			if (isHave[i])
			{
				piecePos[i].x = mx + sub.x;
				piecePos[i].y = my + sub.y;
				break;
			}
		}
	}

	//押してないとき，離したとき
	//マップチップにはめる
	//当たり判定更新
	else
	{
		for (int i = 0; i < 10; i++)
		{
			isHave[i] = false;
			piecePos[i].x = int(piecePos[i].x / 40 + (int(piecePos[i].x) % kMapchipSize < kMapchipSize / 2 ? 0 : 1)) * 40.0f;
			piecePos[i].y = int(piecePos[i].y / 40 + (int(piecePos[i].y) % kMapchipSize < kMapchipSize / 2 ? 0 : 1)) * 40.0f;

			if (int(piecePos[i].x / 40) < 18 && int(piecePos[i].y / 40) < 18)
			{
				switch (i)
				{
				case 0:
					for (int y = 0; y < 7; y++)
					{
						for (int x = 0; x < 10; x++)
						{
							if (piece1[y][x] != 0 &&
								int(piecePos[i].x / 40) + x >= 0 && int(piecePos[i].x / 40) + x < 18 &&
								int(piecePos[i].y / 40) + y >= 0 && int(piecePos[i].y / 40) + y < 18)
							{
								collision[int(piecePos[i].y / 40) + y][int(piecePos[i].x / 40) + x] = 1;
							}
						}
					}
					break;
				case 1:
					for (int y = 0; y < 5; y++)
					{
						for (int x = 0; x < 5; x++)
						{
							if (piece2[y][x] != 0 &&
								int(piecePos[i].x / 40) + x >= 0 && int(piecePos[i].x / 40) + x < 18 &&
								int(piecePos[i].y / 40) + y >= 0 && int(piecePos[i].y / 40) + y < 18)
							{
								collision[int(piecePos[i].y / 40) + y][int(piecePos[i].x / 40) + x] = 1;
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void Stage::playerCollision()
{
	if (player->GetMoveDir().x != 0 || player->GetMoveDir().y != 0)
	{
		int k = 0;
		while (collision[int(player->GetPosY() + player->GetMoveDir().y * k)][int(player->GetPosX() + player->GetMoveDir().x * k)] == 0)
		{
			k++;
		}
		k--;
		player->SetPos(int(player->GetPosX() + player->GetMoveDir().x * k), int(player->GetPosY() + player->GetMoveDir().y * k));
	}
}

Stage::Stage()
{
	player = new Player;
}

void Stage::Update(char* keys, char* preKeys)
{
	collisionArrReset();
	PieceMove();

	player->Update(keys, preKeys);
	playerCollision();
}

void Stage::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0, 0x00000080, kFillModeSolid);

	for (int y = 0; y < 18; y++)
	{
		for (int x = 0; x < 18; x++)
		{
			Novice::DrawBox(x * 40, y * 40, 39, 39, 0, field[y][x] == 0 ? 0xffffffa0 : BLACK, kFillModeSolid);
			Novice::ScreenPrintf(900 + x * 20, y * 20, "%d", collision[y][x]);
		}
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Novice::DrawBox(int(piecePos[0].x) + j * 40, int(piecePos[0].y) + i * 40, 39, 39, 0, piece1[i][j] == 0 ? 0 : 0xaa0000a0, kFillModeSolid);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Novice::DrawBox(int(piecePos[1].x) + j * 40, int(piecePos[1].y) + i * 40, 39, 39, 0, piece2[i][j] == 0 ? 0 : 0x00aa00a0, kFillModeSolid);
		}
	}

	player->Draw(kMapchipSize);
}
