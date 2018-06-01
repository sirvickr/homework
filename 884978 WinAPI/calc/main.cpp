#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDCANCEL: // declared in WinUser.h, which is included in Windows.h
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		}
		break;

	case WM_CLOSE: /* is called just prior to window closing. If you want to ask the user if 
		he really wants to close the program, here is the place to put this check. To close 
		the window, we call DestroyWindow – if we don’t call it, the window won’t be closed. */
		if(MessageBox(hDlg, TEXT("Close the window?"), TEXT("Close"), MB_ICONQUESTION | MB_YESNO) == IDYES)
			DestroyWindow(hDlg);
		return TRUE; /* just continue reading on... */

	case WM_DESTROY: /* We must handle this message, telling Windows we want to quit 
			the main program thread. We do this by calling the PostQuitMessage function.
			Here is the best place to free resources that were allocated by the program 
			and are still waiting to be deallocated – it’s final cleanup time. 
			But don’t forget to do the cleanup BEFORE calling PostQuitMessage.*/
		PostQuitMessage(0);
		return TRUE;

	}
	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hDlg;
	MSG msg;
	BOOL ret;

	InitCommonControls();
	hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
	ShowWindow(hDlg, nCmdShow);

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if(ret == -1)
			return -1;

		if(!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
