#pragma once
#include "PhilliaFunction/Phill.h"

struct Transform { int x; int y; };
struct Size { int width; int height; };
struct SpriteData
{
	Transform srcPos;
	Size srcSize;
	Size trgSize;
	int textureHandle;
	PhillDrawMode drawMode;		// ※一部のコンポーネントで無効
};
struct ButtonData
{
	Transform	position;		// ボタンの座標
	Size		size;			// サイズ
	bool		isPressed;		// ボタンが押された
	bool		isHover;		// ボタンにホバーされた
	bool		isPressing;		// ボタンが押されている
};
struct CheckboxData
{
	Transform	position;		// チェックボックスの座標
	Size		size;			// チェックボックスの座標
	bool		isPressed;		// チェックボックスが押された
	bool		isHover;		// チェックボックスにホバーされた
	bool		isPressing;		// チェックボックスが押されている
};
struct SliderData
{
	Transform	position;
	Size		size;
	Transform	cursor;
	// W.I.P.


};

enum Component
{
	Comp_Button,
};