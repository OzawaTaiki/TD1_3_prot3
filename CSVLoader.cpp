#include "CSVLoader.h"

std::vector<std::vector<int>> CSV_Loader::mapchip;
std::vector<std::vector<std::vector<int>>> CSV_Loader::piece;
char CSV_Loader::defaultPath[64];

CSV_Loader::CSV_Loader(const char* _path)
{
	// デフォルトとして保存
	strcpy_s(defaultPath, 64, _path);
	// 読み込み
	LoadFromCSV(defaultPath);
}

int CSV_Loader::LoadFromCSV(const char* _path)
{
	FILE* fp = nullptr;

	// ふぁいるおぺん
	fopen_s(&fp, _path, "rt");

	// とくん
	char* token = nullptr;
	// さゔぇ
	char* saveptr = nullptr;
	// ばっふぁーらいん
	char bufferLine[1024]{};

	if (fp == nullptr)
	{
		// 読込できなかったときの処理
		return 1;
	}
	else
	{
		mapchip.clear();

		int rowsCnt = 0;   int colsCnt = 0;

		while (fgets(bufferLine, sizeof(bufferLine), fp))
		{
			// とくんにだいにゅ
			token = strtok_s(bufferLine, ",", &saveptr);

			// 行の追加
			mapchip.push_back(std::vector<int>(0));
			// 列の追加
			mapchip[rowsCnt].push_back(0);
			// 代入
			mapchip[rowsCnt][colsCnt] = atoi(token);
			// 列をインクリメント
			colsCnt++;

			while (true)
			{
				token = strtok_s(nullptr, ",", &saveptr);

				if (token == nullptr || *token == '\0' || *token == '\n')   break;

				mapchip[rowsCnt].push_back(0);
				mapchip[rowsCnt][colsCnt] = atoi(token);

				colsCnt++;
			}
			rowsCnt++;
			colsCnt = 0;

		}

		fclose(fp);

	}

	

	return 0;
}

int CSV_Loader::LoadFromCSV_s(const char* _path, char _separator)
{
	/*******************************/
	/************ 特注品 ************/
	/*******************************/

	FILE* fp = nullptr;
	fopen_s(&fp, _path, "rt");

	char* token = nullptr;
	char* saveptr = nullptr;
	char bufferLine[1024]{};

	if (fp == nullptr)
	{
		// 読込できなかったときの処理
		return 1;
	}
	else
	{
		mapchip.clear();

		int rowsCnt = 0;   int colsCnt = 0;

		while (fgets(bufferLine, sizeof(bufferLine), fp))
		{
			// とくんにだいにゅ
			token = strtok_s(bufferLine, ",", &saveptr);

			// 列の初っ端改行されたら終了
			if (*token == _separator) break;

			// 行の追加
			mapchip.push_back(std::vector<int>(0));
			// 列の追加
			mapchip[rowsCnt].push_back(0);
			// 代入
			mapchip[rowsCnt][colsCnt] = atoi(token);
			// 列をインクリメント
			colsCnt++;

			while (true)
			{
				token = strtok_s(nullptr, ",", &saveptr);

				if (token == nullptr || *token == '\0' || *token == '\n')   break;

				mapchip[rowsCnt].push_back(0);
				mapchip[rowsCnt][colsCnt] = atoi(token);

				colsCnt++;
			}
			rowsCnt++;
			colsCnt = 0;

		}

		/// pieceの読み込み
		int elmNum = 0;		rowsCnt = 0;	colsCnt = 0;
		bool exit = true;
		while (fgets(bufferLine, sizeof(bufferLine), fp))
		{
			// piece[elmNum][][]として新しく作成
			piece.push_back(std::vector<std::vector<int>>(0));
			exit = true;
			while (exit)
			{
				token = strtok_s(bufferLine, ",", &saveptr);
				if (*token == _separator && rowsCnt != 0)	break;
				
				else if (*token == '\n' && rowsCnt == 0)
				{
					fgets(bufferLine, sizeof(bufferLine), fp);
					continue;	// フォーマットによる不具合をなるべく解消
				}
				// 行の追加
				piece[elmNum].push_back(std::vector<int>(0));
				// 列の追加
				piece[elmNum][rowsCnt].push_back(0);
				// 代入
				piece[elmNum][rowsCnt][colsCnt] = atoi(token);
				// 列をインクリメント
				colsCnt++;

				while (true)
				{
					token = strtok_s(nullptr, ",", &saveptr);
					// 行末でbreak
					if (token == nullptr || *token == '\0' || *token == '\n')
					{
						exit = fgets(bufferLine, sizeof(bufferLine), fp);
						break;
					}
					piece[elmNum][rowsCnt].push_back(0);
					piece[elmNum][rowsCnt][colsCnt] = atoi(token);
					colsCnt++;
				}
				rowsCnt++;
				colsCnt = 0;
			}
			elmNum++;
			rowsCnt = 0;
		}

		fclose(fp);
	}

	return 0;
}

std::vector<std::vector<int>>* CSV_Loader::GetPointerMapchip()
{
	return &mapchip;
}

std::vector<std::vector<std::vector<int>>>* CSV_Loader::GetPointerPiece()
{
	return &piece;
}
