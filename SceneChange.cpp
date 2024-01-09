#include "SceneChange.h"
#include "PhilliaFunction/Phill.h"
#include <math.h>
#include <stdlib.h>
#include <Novice.h>

/*

16 x 9 no 120 x 120; 

*/

void SceneChange::shuffle(int* arr, int len)
{
    for (int i = len - 1; i > 0; i--)
    {
        int j = rand() % i;
        int temp = arr[j];
        arr[j] = arr[i];
        arr[i] = temp;
    }
}

SceneChange::SceneChange()
{
    // しょきか
    initialized                 = false;
    frameCount_current          = 0;
    frameCount_target           = 60;
    frameCount_interval         = 1;        // タイルを開く間隔(1F~)
    tileSize                    = 120;
    seqOrderIndex               = 0;        
    openPerOnce                 = 2;        // 一度に開くタイル数を指定できる
    kWhiteTexture = Novice::LoadTexture("white1x1.png");
    
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 16; j++)
        {
            tileFlapped[i][j] = 0;
            frameCount_buffer[i][j] = 0;
            constantT[i][j] = 0.0f;
            easedT[i][j] = 1.0f;
            color[i][j] = 0;
            sequentialOrder[16 * i + j] = 16 * i + j;
        }
    shuffle(sequentialOrder, 144);

    /// ランダムが不具合だって？シードを渡したかい？
}

void SceneChange::Update()
{
    /// 初期化時に...
    if (!initialized)
    {
        // 色をランダムに決める
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 16; j++)
            {
                int R = rand() % 0xff;
                int G = rand() % 0xff;
                int B = rand() % 0xff;
                color[i][j] = R;
                color[i][j] = (color[i][j] << 8) + G;
                color[i][j] = (color[i][j] << 8) + B;
                color[i][j] = (color[i][j] << 8) + 0xff;
            }
        initialized = true;
    }

    /// インターバル毎にtileをtrueに変えていく...
    if (frameCount_current % frameCount_interval == 0 && seqOrderFinished != 1)
    {
        // openPerOnceの回数分ループ
        for (int i = 0; i < openPerOnce && seqOrderIndex < 144; i++)
        {
            int rows = sequentialOrder[seqOrderIndex] / 16;
            int cols = sequentialOrder[seqOrderIndex] % 16;
            tileFlapped[rows][cols] = 1;

            seqOrderIndex++;
        }
    }
    /// すべてのタイルが終了したかどうか...
    if (seqOrderIndex >= 144) seqOrderFinished = 1;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 16; j++)
        {
            /// tileがtrueになったら...
            if (tileFlapped[i][j] == 1)
            {
                if (frameCount_buffer[i][j] == 0)
                {
                    // 最初に、タイル毎のbeginFrameをカキコ
                    frameCount_buffer[i][j] = frameCount_current;
                }

                constantT[i][j] = float(frameCount_current - frameCount_buffer[i][j]) / float(frameCount_target);
                /// タイルの演出が終わったら...
                if (constantT[i][j] == 1.0f) tileFlapped[i][j] = 0;
            }

            // イージングを算出し代入する
            // TODO: イージングの種類を変更できる
            easedT[i][j] = Phill::EaseOutBounce(constantT[i][j]);
        }
    

    // 現在フレームを更新(一番最後に処理)
    frameCount_current++;
}

void SceneChange::Draw()
{
    /// 描画のみ
    for (int rows = 0; rows < 9; rows++)
    {
        for (int cols = 0; cols < 16; cols++)
        {
            Phill::DrawQuadPlus(
                cols * tileSize + tileSize / 2, rows * tileSize + tileSize / 2,
                tileSize, tileSize,
                easedT[rows][cols], easedT[rows][cols],
                0.0f,
                0, 0,
                1, 1,
                kWhiteTexture,
                color[rows][cols],
                PhillDrawMode::DrawMode_Center
            );
        }
    }

}
