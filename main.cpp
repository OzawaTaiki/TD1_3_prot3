#include <Novice.h>
#define _USE_MATH_DEFINES
#include <Vector2.h>
#include <math.h>
#include"stage.h"

const char kWindowTitle[] = "LC1A_07_オザワ_タイキ_タイトル";

static const int kWindowWidth = 1920;
static const int kWindowHeight = 1080;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Stage* stage = new Stage;
	stage->Init(0);

	bool isFullSize = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		if (keys[DIK_F11] && !preKeys[DIK_F11])
		{
			isFullSize = isFullSize ? false : true;
			Novice::SetWindowMode(isFullSize ? kFullscreen : kWindowed);
		}


		///
		/// ↓更新処理ここから
		///

		stage->Update(keys, preKeys);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		stage->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
