#pragma once
#include "HiEasyX.h"
#include <easyx.h>
#include <graphics.h>
#include <Windows.h>
#include <string>

class WordEnter
{
private:
	hiex::SysButton checker;
	hiex::SysEdit edit;

	int x;
	int y;
	int w;
	int h;
	const int buttonHeight = 22;

	HWND parentHWnd;
public:
	void Create(HWND parentHWnd, int x, int y, int w, int h);
	void CreateChecker(std::wstring ws);
	void CreateChecker(std::wstring ws, int ButtonWidth, int ButtonHeight, int x, int y, bool bv = true);
	bool Clicked();
	std::wstring GetText();
	void CreateNew();
	void SetText(std::wstring ws);
};

