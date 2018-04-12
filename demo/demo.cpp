/*
	wow64fusion demo

	Copyright (c) 2010-2018 <http://ez8.co> <orca.zhang@yahoo.com>
	This library is released under the MIT License.

	Please see LICENSE file or visit https://github.com/ez8-co/wow64fusion for details.
*/
#include "stdafx.h"
#include "../wow64fusion.hpp"

#include <TlHelp32.h>

#if 0
typedef DWORD64(WINAPI* FUNC)(DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64);
struct Param1 {
	DWORD64 ret;
	FUNC func;
	DWORD64 arg1;
	DWORD64 arg2;
	DWORD64 arg3;
	DWORD64 arg4;
	DWORD64 arg5;
	DWORD64 arg6;
	DWORD64 arg7;
	DWORD64 arg8;
	DWORD64 arg9;
	DWORD64 arg10;
};

__declspec(noinline) DWORD WINAPI Delegator(Param1* param)
{
	return param ? (param->ret ? (param->ret = param->func(param->arg1, param->arg2, param->arg3, param->arg4, param->arg5, param->arg6, param->arg7, param->arg8, param->arg9, param->arg10))
		: (param->func(param->arg1, param->arg2, param->arg3, param->arg4, param->arg5, param->arg6, param->arg7, param->arg8, param->arg9, param->arg10))) : 0;
}
#endif

int main()
{
#if 0
	Param1 param1;
	Delegator(&param1);
#endif

#if 0
	// bellow shows how to use like windows API
	X64Caller RtlCreateUserThread("RtlCreateUserThread");
	if (!RtlCreateUserThread) return 0;
	X64Caller LdrUnloadDll("LdrUnloadDll");
	if (!LdrUnloadDll) return 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	if (Process32First(hSnapshot, &pe32)) {
		do {
			// _tprintf_s(_T("%s [%u]\n"), pe32.szExeFile, pe32.th32ProcessID);
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			BOOL isRemoteWow64 = FALSE;
			IsWow64Process(hProcess, &isRemoteWow64);
			if (!isRemoteWow64) {
				DWORD64 wow64DllBaseAddr = GetModuleHandle64(hProcess, _T("x64.dll"));
				if (wow64DllBaseAddr) {
					DWORD64 ret = RtlCreateUserThread(hProcess, NULL, FALSE, 0, 0, NULL, LdrUnloadDll, wow64DllBaseAddr, NULL, NULL);
				}
			}
		} while (Process32Next(hSnapshot, &pe32));
	}
#endif

#if 0
	typedef int (NTAPI *RTL_ADJUST_PRIVILEGE)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
	RTL_ADJUST_PRIVILEGE RtlAdjustPrivilege = (RTL_ADJUST_PRIVILEGE)GetProcAddress(detail::hNtDll, "RtlAdjustPrivilege");
	RtlAdjustPrivilege(20, 1, 0, NULL);
#endif

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32 = { sizeof(pe32) };
	if (Process32First(hSnapshot, &pe32)) {
		do {
			if (_tcsicmp(pe32.szExeFile, _T("explorer.exe")))
				continue;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

			DWORD64 user32Dll = GetModuleHandle64(hProcess, _T("user32.dll"));
			if(!user32Dll) continue;

#if 0
			DWORD64 kernel32Dll = GetModuleHandle64(hProcess, _T("kernel32.dll"));
			RemoteProcessCaller GetModuleHandle(hProcess, kernel32Dll, sizeof(TCHAR) == sizeof(char) ? "GetModuleHandleA" : "GetModuleHandleW");
			// sample show you how to solve CreateRemoteThread + GetExitCodeThread return partial (4/8 bytes) result problem under x64
			GetModuleHandle.EnableDWORD64Return();
			DWORD64 user32Dll1 = GetModuleHandle(_T("user32.dll"));
#endif

#if 0
			/*
				DWORD WINAPI MessageBoxDelegator(MessageBoxParam* param)
			01042340 85 C9                test        ecx,ecx  
			01042342 74 11                je          MessageBoxDelegator+15h (01042355h)  
			01042344 FF 71 20             push        dword ptr [ecx+20h]  
			01042347 8B 01                mov         eax,dword ptr [ecx]  
			01042349 FF 71 10             push        dword ptr [ecx+10h]  
			0104234c FF 71 18             push        dword ptr [ecx+18h]  
			0104234F FF 71 08             push        dword ptr [ecx+8]  
			01042352 FF D0                call        eax  
			   120: }
			01042354 C3                   ret  
			   119: 	return param ? (param->MessageBoxA((HWND)param->hWnd, (LPCSTR)param->lpText, (LPCSTR)param->lpCaption, param->uType)) : 0;
			01042355 33 C0                xor         eax,eax  
			   120: }
			01042357 C3                   ret
			*/
			/*
				DWORD WINAPI MessageBoxDelegator(MessageBoxParam* param)
				{
			00007FF69EAA1080 48 8B C1             mov         rax,rcx  
					return param ? (param->MessageBoxA(param->hWnd, param->lpCaption, param->lpText, param->uType)) : 0;
			00007FF69EAA1083 48 85 C9             test        rcx,rcx  
			00007FF69EAA1086 74 13                je          MessageBoxDelegator+1Bh (07FF69EAA109Bh)  
			00007FF69EAA1088 44 8B 49 20          mov         r9d,dword ptr [rcx+20h]  
			00007FF69EAA108C 4C 8B 41 10          mov         r8,qword ptr [rcx+10h]  
			00007FF69EAA1090 48 8B 51 18          mov         rdx,qword ptr [rcx+18h]  
			00007FF69EAA1094 48 8B 49 08          mov         rcx,qword ptr [rcx+8]  
			00007FF69EAA1098 48 FF 20             jmp         qword ptr [rax]  
			00007FF69EAA109B 33 C0                xor         eax,eax  
				}
			00007FF69EAA109D C3                   ret 
			*/
			const unsigned char shellcode_x86[] = { 0x85, 0xc9, 0x74, 0x11, 0xff, 0x71, 0x20, 0x8b, 0x01, 0xff, 0x71, 0x10, 0xff, 0x71, 0x18, 0xff, 
													0x71, 0x08, 0xff, 0xd0, 0xc3, 0x33, 0xc0, 0xc3 };
			const unsigned char shellcode[] = { 0x48, 0x8b, 0xc1, 0x48, 0x85, 0xc9, 0x74, 0x13, 0x44, 0x8b, 0x49, 0x20, 0x4c, 0x8b, 0x41, 0x10, 
												0x48, 0x8b, 0x51, 0x18, 0x48, 0x8b, 0x49, 0x08, 0x48, 0xff, 0x20, 0x33, 0xc0, 0xc3 };

			struct MessageBoxParam {
				DWORD64 MessageBoxA;
				DWORD64 hWnd;
				DWORD64 lpCaption;
				DWORD64 lpText;
				DWORD64 uType;
			} param = { NULL, NULL, NULL, NULL, MB_OK };

			const TCHAR* szCaption = _T("From ez8.co");
			const TCHAR* szText = _T("Hello World!");

#ifdef UNICODE
			param.MessageBoxA = GetProcAddress64(hProcess, user32Dll, "MessageBoxW");
#else
			param.MessageBoxA = GetProcAddress64(hProcess, user32Dll, "MessageBoxA");
#endif

			if(!param.MessageBoxA) continue;

			RemoteWriter caption(hProcess, szCaption, (lstrlen(szCaption) + 1) * sizeof(TCHAR));
			if (!(param.lpCaption = caption)) continue;

			RemoteWriter text(hProcess, szText, (lstrlen(szText) + 1) * sizeof(TCHAR));
			if (!(param.lpText = text)) continue;

			RemoteWriter p(hProcess, &param, sizeof(param));
			if (!p) continue;

			RemoteWriter sc(hProcess, detail::is64BitOS ? shellcode : shellcode_x86, (detail::is64BitOS ? sizeof(shellcode) : sizeof(shellcode_x86)) + 1, PAGE_EXECUTE_READWRITE);
			if (!sc) continue;
			sc.SetDontRelese();

			HANDLE handle = CreateRemoteThread64(hProcess, NULL, 0, sc, p, 0, NULL);
			WaitForSingleObject(handle, 1000);
#else
			RemoteProcessCaller MessageBox(hProcess, user32Dll, sizeof(TCHAR) == sizeof(char) ? "MessageBoxA" : "MessageBoxW");
			int ret = MessageBox(NULL, _T("Hello World!"), _T("From ez8.co"), MB_OK);
#endif
		} while (Process32Next(hSnapshot, &pe32));
	}

    return 0;
}
