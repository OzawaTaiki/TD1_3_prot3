#pragma once

class SceneChange
{
private:
    bool    initialized;                // 初期化されているか

    /// 目標フレーム・フレームカウント
    int     frameCount_current;         // 現在フレーム数
    int     frameCount_finished;        // 全タイルオープン時フレーム

    int     frameBuffer[9][16];         // 過去フレーム数
    int     frameBuffer_beginExit;      // 閉め始め時のフレーム

    int     frameTarget_interval;       // インターバル
    int     frameTarget_open;           // 個タイルオープン目標フレーム数
    int     frameTarget_between;        // タイル登場終了(finished)と退場との間隔 目標フレーム
    int     frameTarget_exit;           // 退場にかかるフレーム

    /// 各種フラグ
    int     beginExit;
    int     seqOrderFinished;           // 全タイルがオープンしたかどうか 
    int     isEnd;                      // 完全に終了したかどうか
    
    /// パス
    const char*   imgPath;              // 画像パス

    /// テクスチャハンドル
    int     kWhiteTexture;              // 白地デフォルト
    int     blockTexture;               // ブロック画像

    /// その他
    int     color[9][16];               // どの色をタイルに適応するか - 配列
    int     openPerOnce;                // 一回毎にいくつ開くか
    int     tileSize;                   // タイルサイズ
    int     sequentialOrder[144];       // 順番[0~143]
    int     seqOrderIndex;              // インデックス
    int     tileFlapped[9][16];         // タイルの裏返り配列
    int     posX_interpo;               // ｘ座標 線形補間

    int     rows_buffer;
    int     cols_buffer;

    float   constantT_open[9][16];      // タイルオープン こんすたんと
    float   constantT_exit;             // 退場 コンスタント
    float   easedT_open[9][16];         // タイルオープン イージング後
    float   easedT_exit;                // 退場 イージング後

    /// 関数
    void    shuffle(int* arr, int len); // ふぃっしゃーいえーつ

public:

    SceneChange();

    void    Update();
    void    Draw();

    int     GetIsTileEnd() const;
    int     GetIsEnd() const;
};