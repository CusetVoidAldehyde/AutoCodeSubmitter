#include "HiEasyX.h"
#include "FileSearcher.h"
#include "FileWatcher.h"
#include "WordEnter.h"
#include "resource.h"
#include <easyx.h>
#include <graphics.h>
#define RENEW(wnd) if (wnd.BeginTask())\
		{\
			wnd.EndTask();\
			wnd.Redraw();\
		}
#pragma lib(winnm.lib)

WordEnter filePath, gitPath;
hiex::SysStatic FILEPATH, GITPATH;
hiex::Window wnd;
hiex::Canvas canvas, title;
IMAGE titleImage;


inline void init() {
	wnd.Create(550, 300);
	wnd.BindCanvas(&canvas);
	wnd.SetTitle(L"Auto Code Submitter 1.0.0 by Cuset");
	::SetWindowLongPtr(wnd.GetHandle(), GWL_STYLE, GetWindowLong(wnd.GetHandle(), GWL_STYLE) & ~WS_MAXIMIZEBOX);
	canvas.Clear(true, BLACK);
	loadimage(&titleImage, _T("JPG"), MAKEINTRESOURCE(101));
	title.BindToImage(&titleImage);
	putimage(-10, -10, title.GetImagePointer());
	wnd.Redraw();
	settextcolor(WHITE);
	outtextxy(50, 95, L"键入所要提交的文件的文件夹路径：");
	filePath.Create(wnd.GetHandle(), 50, 120, 425, 40);
	filePath.CreateChecker();
	gitPath.Create(wnd.GetHandle(), 50, 195, 425, 40);
	gitPath.CreateChecker();
}

int main() {
	init();
	while (wnd.IsAlive()) {
		
		RENEW(wnd);
		// 平衡帧率
		static hiex::tDelayFPS recond;
		hiex::DelayFPS(recond, 75);
	}
	hiex::AutoExit();
	return 0;
}
