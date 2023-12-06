#include <windows.h>
#include <processthreadsapi.h>
#include <securitybaseapi.h>
#include <WinBase.h>
#include <userenv.h>
#include <string.h>
#include <string>

#include "ProcMgr.h"

#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")

bool createProcessWithAdmin(const std::wstring& process_name, LPPROCESS_INFORMATION process)
{
	HANDLE hToken = NULL;
	HANDLE hTokenDup = NULL;

	if (process_name.empty()) {
		return false;
	}

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		return false;
	}


	if (!DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL, SecurityAnonymous, TokenPrimary, &hTokenDup))
	{
		CloseHandle(hToken);
		return false;
	}

	STARTUPINFO si;
	LPVOID pEnv = NULL;
	DWORD dwSessionId = WTSGetActiveConsoleSessionId();

	ZeroMemory(&si, sizeof(STARTUPINFO));

	if (!SetTokenInformation(hTokenDup, TokenSessionId, &dwSessionId, sizeof(DWORD)))
	{
		CloseHandle(hToken);
		CloseHandle(hTokenDup);
		return false;
	}

	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = (LPWSTR)"WinSta0\\Default";
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;

	if (!CreateEnvironmentBlock(&pEnv, hTokenDup, FALSE))
	{
		CloseHandle(hToken);
		CloseHandle(hTokenDup);
		return false;
	}

	if (!CreateProcessAsUser(hTokenDup, process_name.c_str(), NULL, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
		pEnv, NULL, &si, process))
	{
		CloseHandle(hToken);
		CloseHandle(hTokenDup);
		return false;
	}

	if (pEnv)
	{
		DestroyEnvironmentBlock(pEnv);
	}

	CloseHandle(hToken);
	CloseHandle(hTokenDup);
	return true;
}

bool createProc()
{
	char cWindowsDirectory[MAX_PATH];
	
//LPTSTR 与 wchar_t* 等价(Unicode环境下)  
	LPTSTR cWinDir = new TCHAR[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cWinDir);

	//LPTSTR sConLin = wcscat(cWinDir, L"E:\\Meta5Tester\\zchat.exe");
	TCHAR szCommandLine[] = TEXT("E:\\Meta5Tester\\MetaTester64.exe");
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	using namespace std;
	if (CreateProcess(
			NULL,   //  指向一个NULL结尾的、用来指定可执行模块的宽字节字符串  
		szCommandLine, // 命令行字符串  
			NULL, //    指向一个SECURITY_ATTRIBUTES结构体，这个结构体决定是否返回的句柄可以被子进程继承。  
			NULL, //    如果lpProcessAttributes参数为空（NULL），那么句柄不能被继承。<同上>  
			true,//    指示新进程是否从调用进程处继承了句柄。   
			0,  //  指定附加的、用来控制优先类和进程的创建的标  
			//  CREATE_NEW_CONSOLE  新控制台打开子进程  
			//  CREATE_SUSPENDED    子进程创建后挂起，直到调用ResumeThread函数  
			NULL, //    指向一个新进程的环境块。如果此参数为空，新进程使用调用进程的环境  
			NULL, //    指定子进程的工作路径  
			& si, // 决定新进程的主窗体如何显示的STARTUPINFO结构体  
			& pi  // 接收新进程的识别信息的PROCESS_INFORMATION结构体  
			))
	{
		
		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);
	}
	

	return true;
}