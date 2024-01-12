#pragma once
#include "UI/UI_ToolKit_Defines.h"



class PauseScreen
{
private:

    enum KeyFrames : int
    {
        KeyF_TurnDark,                                  // 暗転
        KeyF_Pop,                                       // ポップ
        KeyF_TurnLight,                                 // 明転

        KeyF_COUNT,                                     // 要素数
    };

    // スプライトデータ
    SpriteData buttonSpr[3];

    /// テクスチャハンドル
    int kWhiteTexture;
    
    /// ターゲットフレーム / フレームカウント / バッファ
    int frameCount_current;                             // 現在フレームカウント
    int frameCount[KeyF_COUNT];                         // フレームカウント                 
    int targetFrame[KeyF_COUNT];                        // ターゲットカウント
    int frameBuffer[KeyF_COUNT];                        // 現在フレームをバッファしておく

    /// ターゲット値 / 代入先変数
    int bgAlphaValue;                                   // アルファ値(加算用)
    int bgAlphaValue_target;                            // 背景暗転時アルファ値目標

    /// 媒介変数 (イージング)
    float constantT[KeyF_COUNT];                        // 線形
    float easedT[KeyF_COUNT];                           // イージング適用後

    /// ボタン関連のフラグ
    int isResame;
    int isHow2;
    int isBack;

    /// その他
    int buttonMargin;
    int isDeletable;
    int isDrawMain;
    int isCloseReq;

public:
    PauseScreen();

    void	Update();
    void	Draw();
    int     Deletable() const;
    void    ClosePop();
};