#pragma once

struct Transform { int x; int y; };
struct Size { int width; int height; };
struct SpriteData
{
	Transform srcPos;
	Size srcSize;
	Size trgSize;
	int textureHandle;
};
struct ScrollBox
{
	Transform	position;
	Size		size;
	int			y_pre;
	int			y_bottom;
	int			y_top;
	bool		isHover;
	bool		isPressed;
	bool		isPressing;
};
struct ScrollBar
{
	Transform	position;
	Size		size;
};