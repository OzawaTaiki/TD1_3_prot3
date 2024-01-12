#include "UI/UI_ToolKit.h"
#include "PauseScreen.h"
#include <Novice.h>

PauseScreen::PauseScreen()
{
    /// いにしゃらいず
    kWhiteTexture = Novice::LoadTexture("white1x1.png");
    buttonSpr[0].srcPos = Transform(0, 0);
    buttonSpr[0].srcSize = Size(1, 1);
    buttonSpr[0].trgSize = Size(300, 80);
    buttonSpr[0].textureHandle = kWhiteTexture;
    buttonSpr[0].drawMode = DrawMode_Center;

    buttonSpr[1].srcPos = Transform(0, 0);
    buttonSpr[1].srcSize = Size(1, 1);
    buttonSpr[1].trgSize = Size(300, 80);
    buttonSpr[1].textureHandle = kWhiteTexture;
    buttonSpr[1].drawMode = DrawMode_Center;

    buttonSpr[2].srcPos = Transform(0, 0);
    buttonSpr[2].srcSize = Size(1, 1);
    buttonSpr[2].trgSize = Size(300, 80);
    buttonSpr[2].textureHandle = kWhiteTexture;
    buttonSpr[2].drawMode = DrawMode_Center;


    frameCount_current = 0;

    targetFrame[KeyF_TurnDark] = 30;
    targetFrame[KeyF_Pop] = 0;
    targetFrame[KeyF_TurnLight] = 30;

    for (int i = 0; i < KeyF_COUNT; i++)
    {
        frameCount[i] = 0;
        frameBuffer[i] = 0;
        constantT[i] = 0;
        easedT[i] = 0;
    }

    /// ターゲット値 / 代入先変数
    bgAlphaValue = 0;
    bgAlphaValue_target = 0xab;

    /// ボタン関連のフラグ
    isResame = 0;
    isHow2 = 0;
    isBack = 0;

    /// その他
    buttonMargin = 20;
    isDeletable = 0;
    isDrawMain = 1;
    isCloseReq = 0;
}

void PauseScreen::Update()
{
    isResame += GUI_Toolkit::Button("##Resame", 1920 / 2, 1080 / 2 - buttonSpr[0].trgSize.height - buttonMargin, &buttonSpr[0], false);
    isHow2 += GUI_Toolkit::Button("##How2Ctrl", 1920 / 2, 1080 / 2, &buttonSpr[1], false);
    isBack += GUI_Toolkit::Button("##Back", 1920 / 2, 1080 / 2 + buttonSpr[2].trgSize.height + buttonMargin, &buttonSpr[2], false);

    isResame = isResame > 0 ? 1 : 0;
    isHow2 %= 2;
    isBack %= 2;

    /// Resameボタンが押されたら...
    if (isResame || isCloseReq)
    {
        if (frameBuffer[KeyF_TurnLight] == 0)
            frameBuffer[KeyF_TurnLight] = frameCount_current;
        frameCount[KeyF_TurnLight] = frameCount_current - frameBuffer[KeyF_TurnLight];
    }

    if (frameCount[KeyF_TurnDark] < targetFrame[KeyF_TurnDark])
    {
        frameCount[KeyF_TurnDark] = frameCount_current - 0;
    }

    /// 媒介変数の更新
    constantT[KeyF_TurnDark] = float(frameCount[KeyF_TurnDark]) / float(targetFrame[KeyF_TurnDark]);
    constantT[KeyF_TurnLight] = float(frameCount[KeyF_TurnLight]) / float(targetFrame[KeyF_TurnLight]);
    easedT[KeyF_TurnDark] = Phill::EaseOutQuart(constantT[KeyF_TurnDark]);
    easedT[KeyF_TurnLight] = Phill::EaseOutQuart(constantT[KeyF_TurnLight]);

    /// 媒介変数を用いて値を更新
    if (isResame || isCloseReq)
    {
        isDrawMain = 0;
        bgAlphaValue = int(easedT[KeyF_TurnLight] * 0 + (1.0f - easedT[KeyF_TurnLight]) * bgAlphaValue_target);
    }
    else
    {
        bgAlphaValue = int(easedT[KeyF_TurnDark] * bgAlphaValue_target);
    }

    /// ポップアップ削除
    if ((isResame || isCloseReq) && constantT[KeyF_TurnLight] == 1.0f)
    {
        GUI_Toolkit::DeleteComponent(Comp_Button, 3, "##Resame", "##How2Ctrl", "##Back");
        isDeletable = 1;
    }
    frameCount_current++;
}

void PauseScreen::Draw()
{
    Novice::DrawBox(0, 0, 1920, 1080, 0.0f, 0x00000000 + bgAlphaValue, kFillModeSolid);

    if (isDrawMain){
        Phill::DrawQuadPlus(1920 / 2, 1080 / 2, 400, 500, 1.0f, 1.0f, 0.0f, 0, 0, 1, 1, kWhiteTexture, 0x48c3aeff, DrawMode_Center);
        GUI_Toolkit::Draw(Comp_Button, "##Resame");
        GUI_Toolkit::Draw(Comp_Button, "##How2Ctrl");
        GUI_Toolkit::Draw(Comp_Button, "##Back");
    }
}

int PauseScreen::Deletable() const
{
    return isDeletable;
}

void PauseScreen::ClosePop()
{
    isCloseReq = 1;
}