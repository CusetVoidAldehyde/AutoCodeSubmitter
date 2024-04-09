#include "WordEnter.h"

void WordEnter::Create(HWND parentHWnd, int x, int y, int w, int h)
{
	hiex::SysEdit::PreStyle pre;
	pre.multiline = true;
	pre.vscroll = false;
	pre.hscroll = true;
	pre.auto_vscroll = false;
	pre.auto_hscroll = true;
	edit.PreSetStyle(pre);
	edit.Create(parentHWnd, x, y, w, h, L"");
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->parentHWnd = parentHWnd;
}

void WordEnter::CreateChecker(std::wstring ws)
{
	checker.Create(parentHWnd, x + w, y, buttonHeight, buttonHeight, ws);
}

void WordEnter::CreateChecker(std::wstring ws, int ButtonWidth, int ButtonHeight, int xx, int yy, bool bv)
{
	if (bv) checker.Create(parentHWnd, x + w, y, ButtonWidth, ButtonHeight, ws);
	else checker.Create(parentHWnd, xx, yy, ButtonWidth, ButtonHeight, ws);
}

bool WordEnter::Clicked()
{
	return checker.IsClicked();
}

std::wstring WordEnter::GetText()
{
	return edit.GetText();
}

void WordEnter::CreateNew()
{
	edit.SetText(L"");
}

void WordEnter::SetText(std::wstring ws)
{
	edit.SetText(ws);
}
