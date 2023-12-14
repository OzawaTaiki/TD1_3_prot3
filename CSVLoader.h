#pragma once
#include <vector>

class CSV_Loader
{

public:

	CSV_Loader(const char* _path);

	static int LoadFromCSV(const char* _path = defaultPath);
	static int LoadFromCSV_s(const char* _path, char _separate);

	static std::vector<std::vector<int>>* GetPointerMapchip();
	static std::vector<std::vector<std::vector<int>>>* GetPointerPiece();

private:

	static std::vector<std::vector<int>> mapchip;
	static std::vector<std::vector<std::vector<int>>> piece;

	static char defaultPath[64];

};