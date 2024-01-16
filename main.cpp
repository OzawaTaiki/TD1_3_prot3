#include <Novice.h>
#define _USE_MATH_DEFINES
#include <Vector2.h>
#include <math.h>
#include"stage.h"

const char kWindowTitle[] = "1304_ゲームタイトル";

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
	Vector2 keyBoxPos = { 1500,500 };	/// キー入力情報描画位置 仮実装

	//SceneChange* sceneChange_from_stage = nullptr;

	//int clearTexture = Novice::LoadTexture("");

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

		if (stage)
		{
			stage->Update(keys, preKeys);

			//if (stage->GetIsClear() && !sceneChange_from_stage)
			//{
			//	sceneChange_from_stage = stage->GetSceneChgPtr();
			//}

			//if (sceneChange_from_stage)
			//{
			//	if (sceneChange_from_stage->GetIsTileEnd())
			//	{
			//		delete stage;
			//		stage = nullptr;
			//	}
			//}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		if (stage) stage->Draw();

		Novice::DrawBox(int(keyBoxPos.x), int(keyBoxPos.y), 63, 63, 0, keys[DIK_W] ? 0xd00000ff : 0xd0d0d0ff, kFillModeSolid);
		Novice::DrawBox(int(keyBoxPos.x - 64), int(keyBoxPos.y + 64), 63, 63, 0, keys[DIK_A] ? 0xd00000ff : 0xd0d0d0ff, kFillModeSolid);
		Novice::DrawBox(int(keyBoxPos.x), int(keyBoxPos.y + 64), 63, 63, 0, keys[DIK_S] ? 0xd00000ff : 0xd0d0d0ff, kFillModeSolid);
		Novice::DrawBox(int(keyBoxPos.x + 64), int(keyBoxPos.y + 64), 63, 63, 0, keys[DIK_D] ? 0xd00000ff : 0xd0d0d0ff, kFillModeSolid);

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
