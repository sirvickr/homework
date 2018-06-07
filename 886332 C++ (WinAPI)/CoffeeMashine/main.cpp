#include "stdafx.h"

#include "Resource.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

// Common Controls (�������� ��� ��������� ����������)
#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

// ������� �� ������������ ����
CoffeeMashine* coffeeMashine = nullptr;

HWND hlstCoffee = nullptr;
HWND hlstOutput = nullptr;

// ������� ������� ��������� (���������� ��������� �������� ���� ����������)
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256];
	UINT ctrlID = 0;
	switch(uMsg) {
	// ��������� �� ������ �� ��������� ����������, ����������� �� ���������� ����
	case WM_COMMAND:
		// ������������� �������� ����������, ����������� ��������� ���������
		ctrlID = LOWORD(wParam);
		switch (ctrlID) {
		case IDCANCEL: // ������� ESC
			SetDlgItemText(hDlg, txtInputSum, _T("0.0"));
			return TRUE;
		case btnClose: // ������� ������ "�������"
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case btnCook: // ������� ������ "�����������"
			// �������� ������ �����������
			SendMessage(hlstOutput, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
			try {
				// ��������� �������� �����
				GetDlgItemText(hDlg, txtInputSum, buffer, sizeof(buffer) / sizeof(buffer[0]) - 1);
				tistringstream is(buffer);
				double sum = 0;
				is >> sum;
				// ��������� ��������� ��� ����
				TCHAR text[256];
				int index = SendMessage(hlstCoffee, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				SendMessage(hlstCoffee, LB_GETTEXT, (WPARAM)index, (LPARAM)text);
				// ����������� ���� � �������� �����
				std::pair<Coffee, CoffeeMashine::Cash> result = coffeeMashine->Cook(text, sum);
				// ����� �����������
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)text);
				if (result.second.empty()) {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("����� �� ���������"));
				} else {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("�����:"));
					for (auto it = result.second.begin(); it != result.second.end(); it++) {
						tostringstream os;
						os << _T("������� ") << it->first << _T(" ���������� ") << it->second;
						SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)os.str().c_str());
					}
				}
			}
			catch (CoffeeMashine::NotAvail& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("��� ������ ����"));
			}
			catch (CoffeeMashine::NotEnoughMoney& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("������������ �����"));
			}
			catch (CoffeeMashine::NoChange& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("��� �����"));
			}
			catch (CoffeeMashine::Error& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("�� ������� ����������� ����"));
			}
			return TRUE;
		}
		break;

	case WM_INITDIALOG:
		// � ����������� ����� ��������� ������ ��������� ��������� ��������� ����������
		hlstCoffee = GetDlgItem(hDlg, lstCoffeeKind);
		hlstOutput = GetDlgItem(hDlg, lstOutput);
		// �������� ������ ��� ������� "������� �� ������������ ����"
		coffeeMashine = new CoffeeMashine("coffee.cfg", "cash.cfg");

		SetDlgItemText(hDlg, txtInputSum, _T("100.0"));
		{
			CoffeeMashine::CoffeeAvail coffeeAvail = coffeeMashine->getCoffeeAvail();
			// ��������� ������ ������ ��������
			for (const auto coffee : coffeeAvail) {
				SendMessage(hlstCoffee, LB_ADDSTRING, (WPARAM)0, (LPARAM)coffee.first.c_str());
			}
		}
		SendMessage(hlstCoffee, LB_SETCURSEL, (WPARAM)0, 0);
		break;

	case WM_CLOSE:
		/* ��� ��������� ���������� ����� ��������� ����. ����� ����� �������� 
		������������, ������������� �� �� ����� ��������� ������ (�������� ��� 
		������� ������������ ������). � ���� ���������� �� ������ ����� ������� 
		�� �������� �������� ����. */
		DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY: 
		/* �� ������ ���������� ��� ���������, ����� ������� �������, ��� 
		����� ��������� �������� ����� ����������. ��� ����� ������ ������� 
		PostQuitMessage(). ������ ���� ��������� ����������� ��� ������������ 
		���������� ��������. */
		delete coffeeMashine;
		PostQuitMessage(0);
		return TRUE;

	}
	return FALSE;
}

// ����� ����� ����������
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL ret;
	InitCommonControls();
	// ������ ������� ����
	HWND hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(mainWnd), 0, DialogProc, 0);
	// ���������� ��� �� ������
	ShowWindow(hDlg, nCmdShow);
	// ����������� ������� ������������ ����������� ��������� � �����
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
