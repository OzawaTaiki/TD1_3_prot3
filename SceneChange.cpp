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
    frameTarget_interval        = 1;                            // タイルを開く間隔(1F~)
    frameCount_finished         = 0;
    tileSize                    = 120;
    seqOrderIndex               = 0;        
    openPerOnce                 = 2;                            // 一度に開くタイル数を指定する
    imgPath                     = "./img/pieceBlock.png";       // ブロックの画像データのパスを指定する
    posX_interpo                = 0;
    frameTarget_open            = 60;
    beginExit                   = 0;
    frameBuffer_beginExit       = 0;
    constantT_exit              = 0;
    easedT_exit                 = 0;
    frameTarget_between         = 120;
    frameTarget_exit            = 60;

    isEnd = 0;

    kWhiteTexture               = Novice::LoadTexture("white1x1.png");
    blockTexture                = Novice::LoadTexture(imgPath);
    
    
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 16; j++)
        {
            tileFlapped[i][j] = 0;
            frameBuffer[i][j] = 0;
            constantT_open[i][j] = 0.0f;
            easedT_open[i][j] = 1.0f;
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
        for(int i = 0; i < 9; i++)
            for (int j = 0; j < 16; j++)
            {
                color[i][j] = rand() % 7;
            }

        initialized = true;
    }

    /// インターバル毎にtileをtrueに変えていく...
    if (frameCount_current % frameTarget_interval == 0 && seqOrderFinished != 1)
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
    if (seqOrderIndex >= 144 && !seqOrderFinished)
    {
        seqOrderFinished = 1;
        frameCount_finished = frameCount_current;
    }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 16; j++)
        {
            /// tileがtrueになったら...
            if (tileFlapped[i][j] == 1)
            {
                if (frameBuffer[i][j] == 0)
                {
                    // 最初に、タイル毎のbeginFrameをカキコ
                    frameBuffer[i][j] = frameCount_current;
                }

                constantT_open[i][j] = float(frameCount_current - frameBuffer[i][j]) / float(frameTarget_open);
                /// タイルの演出が終わったら...
                if (constantT_open[i][j] == 1.0f) tileFlapped[i][j] = 0;
            }

            // イージングを算出し代入する
            // TODO: イージングの種類を変更できる
            easedT_open[i][j] = Phill::EaseOutBounce(constantT_open[i][j]);
        }

    if (seqOrderFinished)
    {
        if (frameCount_current - frameCount_finished > frameTarget_between && !beginExit)
        {
            beginExit = 1;
            frameBuffer_beginExit = frameCount_current;
        }

        if (beginExit)
        {
            constantT_exit = float(frameCount_current - frameBuffer_beginExit) / float(frameTarget_exit);

            // TODO: イージングの種類を変更できる
            easedT_exit = Phill::EaseInOutQuart(constantT_exit);

            posX_interpo = int(1920 * easedT_exit);
        }

        if (constantT_exit == 1.0f)
        {
            beginExit = 0;
        }

    }

    if (seqOrderFinished && beginExit == 0 && constantT_exit == 1.0f) isEnd = 1;

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
                cols * tileSize + tileSize / 2 + posX_interpo, rows * tileSize + tileSize / 2,
                tileSize, tileSize,
                easedT_open[rows][cols], easedT_open[rows][cols],
                0.0f,
                tileSize * color[rows][cols], 0,
                tileSize, tileSize,
                blockTexture,
                0xffffffff,
                PhillDrawMode::DrawMode_Center
            );
        }
    }

}

int SceneChange::GetIsEnd() const
{
    return isEnd;
}
