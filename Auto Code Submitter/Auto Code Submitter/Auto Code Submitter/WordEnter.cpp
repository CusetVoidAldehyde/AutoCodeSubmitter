#include "WordEnter.h"

void WordEnter::Create(HWND parentHWnd, int x, int y, int w, int h)
{
	hiex::SysEdit edit;
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

void WordEnter::CreateChecker()
{
	checker.Create(parentHWnd, x + w, y, buttonHeight, buttonHeight);
}
