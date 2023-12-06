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
	
//LPTSTR �� wchar_t* �ȼ�(Unicode������)  
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
			NULL,   //  ָ��һ��NULL��β�ġ�����ָ����ִ��ģ��Ŀ��ֽ��ַ���  
		szCommandLine, // �������ַ���  
			NULL, //    ָ��һ��SECURITY_ATTRIBUTES�ṹ�壬����ṹ������Ƿ񷵻صľ�����Ա��ӽ��̼̳С�  
			NULL, //    ���lpProcessAttributes����Ϊ�գ�NULL������ô������ܱ��̳С�<ͬ��>  
			true,//    ָʾ�½����Ƿ�ӵ��ý��̴��̳��˾����   
			0,  //  ָ�����ӵġ���������������ͽ��̵Ĵ����ı�  
			//  CREATE_NEW_CONSOLE  �¿���̨���ӽ���  
			//  CREATE_SUSPENDED    �ӽ��̴��������ֱ������ResumeThread����  
			NULL, //    ָ��һ���½��̵Ļ����顣����˲���Ϊ�գ��½���ʹ�õ��ý��̵Ļ���  
			NULL, //    ָ���ӽ��̵Ĺ���·��  
			& si, // �����½��̵������������ʾ��STARTUPINFO�ṹ��  
			& pi  // �����½��̵�ʶ����Ϣ��PROCESS_INFORMATION�ṹ��  
			))
	{
		
		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);
	}
	

	return true;
}