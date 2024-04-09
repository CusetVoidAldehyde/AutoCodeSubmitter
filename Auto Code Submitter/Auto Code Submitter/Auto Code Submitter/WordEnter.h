#pragma once
#include "HiEasyX.h"
#include <easyx.h>
#include <graphics.h>
#include <Windows.h>

class WordEnter
{
private:
	hiex::SysButton checker;

	int x;
	int y;
	int w;
	int h;
	const int buttonHeight = 22;

	HWND parentHWnd;
public:
	void Create(HWND parentHWnd, int x, int y, int w, int h);
	void CreateChecker();
};

