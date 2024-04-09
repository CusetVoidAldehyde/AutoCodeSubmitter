#include "HiEasyX.h"
#include "FileSearcher.h"
#include "FileWatcher.h"
#include "WordEnter.h"
#include "SystemOptGetter.h"
#include "resource2.h"
#include <easyx.h>
#include <graphics.h>
#include <comutil.h>  
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include <cstdio>
#include <chrono>
#define RENEW(wnd) if (wnd.BeginTask())\
		{\
			wnd.EndTask();\
			wnd.Redraw();\
		}
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "winmm.lib")
#pragma warning(disable: 4996)

const std::string SaveName = "save.cu";

std::string CommitFilePath;

std::vector<std::string> suffixes;

WordEnter filePath, gitPath, fileSuffix, SetTime, UserName, UserEmail;
SystemOptGetter SOG;
hiex::SysButton ClearBtn, Start, End;
hiex::Window wnd;
hiex::Canvas canvas, title;
IMAGE titleImage;

int DeltaT;

bool dirExists(const std::string& dir)
{
	DWORD attribs = ::GetFileAttributesA(dir.c_str());
	if (attribs == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

std::string wstring2string(const std::wstring& ws)
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	std::string result = pchar;
	return result;
}

std::wstring string2wstring(const std::string& s)
{
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	std::wstring result = pwchar;
	return result;
}

bool fileExists(const std::string& file) {
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}

std::string getTime() {
	tm time;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> tpMicro;
	time_t timestamp_s = std::chrono::system_clock::to_time_t(tpMicro);
	gmtime_s(&time, &timestamp_s);
	char szTime[128];
	sprintf_s(szTime, "%04d%02d%02d%02d%02d",
		time.tm_year + 1954, time.tm_mon + 1, time.tm_mday, (time.tm_hour + 8) % 24, time.tm_min);
	return std::string(szTime);
}

inline void init() {
	hiex::SetCustomIcon(MAKEINTRESOURCE(IDI_ICON1), MAKEINTRESOURCE(IDI_ICON1));
	wnd.Create(550, 650);
	wnd.BindCanvas(&canvas);
	wnd.SetTitle(L"Auto Code Submitter 1.2.7 by Cuset");
	::SetWindowLongPtr(wnd.GetHandle(), GWL_STYLE, GetWindowLong(wnd.GetHandle(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME);
	canvas.Clear(true, BLACK);
	loadimage(&titleImage, _T("JPG"), MAKEINTRESOURCE(IDB_BITMAP1));
	title.BindToImage(&titleImage);
	putimage(-10, -10, title.GetImagePointer());
	wnd.Redraw();
	settextcolor(WHITE);
	outtextxy(50, 100, L"键入所要提交的文件的文件夹路径（末尾应带反斜杠“\\”）：");
	filePath.Create(wnd.GetHandle(), 50, 125, 425, 42);
	filePath.CreateChecker(L"");
	outtextxy(50, 175, L"键入远程仓库地址（https://github.com/xxx/xxx.git）：");
	gitPath.Create(wnd.GetHandle(), 50, 200, 450, 42);
	outtextxy(50, 250, L"键入所要提交文件的后缀名（示例：.cpp）");
	outtextxy(50, 275, L"点“确定”添加，点“清除”清空列表，请依次添加");
	fileSuffix.Create(wnd.GetHandle(), 50, 300, 375, 60);
	fileSuffix.CreateChecker(L"确认", 80, 30, 80, 30);
	ClearBtn.Create(wnd.GetHandle(), 425, 330, 80, 30, L"清除");
	outtextxy(50, 365, L"键入每次提交的间隔时间（单位：秒）：");
	SetTime.Create(wnd.GetHandle(), 50, 390, 455, 42);
	End.Enable(false);
	outtextxy(50, 437, L"键入Github用户名：");
	outtextxy(280, 437, L"键入Github绑定邮箱：");
	UserName.Create(wnd.GetHandle(), 50, 460, 220, 42);
	UserEmail.Create(wnd.GetHandle(), 280, 460, 220, 42);
	if (fileExists("save.cu")) {
		freopen("save.cu", "r", stdin);
		std::string un, ue;
		std::cin >> un >> ue;
		UserName.SetText(string2wstring(un));
		UserEmail.SetText(string2wstring(ue));
		fclose(stdin);
	}
	Start.Create(wnd.GetHandle(), 150, 525, 120, 50, L"开始提交");
	End.Create(wnd.GetHandle(), 280, 525, 120, 50, L"停止提交");
}

int main() {
	init();
	while (wnd.IsAlive()) {
		
		RENEW(wnd);

		if (filePath.Clicked()) {
			std::wstring filepath = filePath.GetText();
			CommitFilePath = wstring2string(filepath);
			if (!dirExists(CommitFilePath)) {
				MessageBox(wnd.GetHandle(), L"文件夹不存在！", L"Warning", MB_OK);
			}
			else {
				MessageBox(wnd.GetHandle(), L"文件夹合法。", L"Check", MB_OK);
			}
		}
		if (fileSuffix.Clicked()) {
			std::string temp;
			temp = wstring2string(fileSuffix.GetText());
			if (temp != "") {
				if (temp[0] == '.') {
					suffixes.push_back(temp);
					fileSuffix.CreateNew();
				}
				else {
					fileSuffix.CreateNew();
					MessageBox(wnd.GetHandle(), L"后缀不合法！", L"Warning", MB_OK);
				}
			}
			else {
				MessageBox(wnd.GetHandle(), L"输入为空！", L"Warning", MB_OK);
			}
		}
		if (ClearBtn.IsClicked()) {
			suffixes.clear();
			fileSuffix.CreateNew();
		}
		if (Start.IsClicked()) {
			DeltaT = atoi(wstring2string(SetTime.GetText()).c_str());
			Start.Enable(false);
			End.Enable(true);
			std::string cmds;
			std::string un = wstring2string(UserName.GetText());
			std::string ue = wstring2string(UserEmail.GetText());
			std::string gitp = wstring2string(gitPath.GetText());
			std::string result;
			std::string datfile = wstring2string(filePath.GetText()) + "submit.bat";
			if (un == "" || ue == "") {
				MessageBox(wnd.GetHandle(), L"用户名或邮箱为空！", L"Warning", MB_OK);
				goto label;
			}
			if (!fileExists("save.cu")) {
				freopen("save.cu", "w", stdout);
				std::cout << wstring2string(UserName.GetText()) << "\n";
				std::cout << wstring2string(UserEmail.GetText()) << "\n";
				fclose(stdout);
			}
			remove(SaveName.c_str());
			freopen("save.cu", "w", stdout);
			std::cout << wstring2string(UserName.GetText()) << "\n";
			std::cout << wstring2string(UserEmail.GetText()) << "\n";
			fclose(stdout);
			system("git init");// 初始化本地 git 仓库
			cmds = "git config user.name \"" + un + "\"";
			system(cmds.c_str());// 配置用户名
			cmds = "git config user.email \"" + ue + "\"";
			system(cmds.c_str());// 配置用户邮箱
			cmds = "git pull " + gitp;
			result = SOG.cmd(cmds);
			if (result == "fatal: not a git repository (or any of the parent directories): .git") {
				MessageBox(wnd.GetHandle(), L"不存在该.git仓库！", L"Warning", MB_OK);
				goto label;
			}
			remove(datfile.c_str());
			if (!fileExists(datfile.c_str())) {
				freopen(datfile.c_str(), "w", stdout);
				std::cout << "git init\n";
				cmds = "git config user.name \"" + un + "\"";
				std::cout << cmds << "\n";
				cmds = "git config user.email \"" + ue + "\"";
				std::cout << cmds << "\n";
				std::cout << "git branch -M main\n";
				cmds = "git pull " + gitp + "\n";
				std::cout << cmds;
				for (auto i : suffixes) {
					cmds = "git add *" + i + "\n";
					std::cout << cmds;
				}
				cmds = "git commit -m \"" + getTime() + "\"\n";
				std::cout << cmds;
				cmds = "git push --set-upstream " + gitp + " main\n";
				std::cout << cmds;
				fclose(stdout);
			}
			while (!End.IsClicked() || !End.IsEnable()) {
				cmds = wstring2string(filePath.GetText()) + "submit.bat";
				for (int i = 1; i <= DeltaT * 32ll; i++) {
					RENEW(wnd);
					if (End.IsClicked()) {
						goto label;
					}
					Sleep(31);
				}
				system(cmds.c_str());
			}
		label:
			Start.Enable(true);
			End.Enable(false);
		}
		// 平衡帧率
		static hiex::tDelayFPS recond;
		hiex::DelayFPS(recond, 75);
	}
	hiex::AutoExit();
	return 0;
}
/*
	20240405 upd: 未完成：上传代码。
				  已完成：开始/获取命令行输出/更新用户文件。
	by Cuset 喵喵~
*/