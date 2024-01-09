#pragma once

class SceneChange
{
private:
    bool    initialized;                // 初期化されているか
    int     frameCount_current;         // 現在フレーム数
    int     frameCount_buffer[9][16];   // 過去フレーム数
    int     frameCount_target;          // 目標フレーム数
    int     frameCount_interval;        // インターバル
    int     tileSize;                   // タイルサイズ
    int     kWhiteTexture;              // テクスチャ
    unsigned int color[9][16];          // 色ランダム
    int     openPerOnce;                // 一回毎にいくつ開くか

    int     sequentialOrder[144];       // 順番[0~143]
    int     seqOrderIndex;              // インデックス
    int     seqOrderFinished;           // すべて終了したかどうか 
    int     tileFlapped[9][16];         // タイルの裏返り配列

    float   constantT[9][16];           // こんすたんと
    float   easedT[9][16];              // イージング後

    void    shuffle(int* arr, int len);          // ふぃっしゃーいえーつ

public:

    SceneChange();

    void    Update();
    void    Draw();
};