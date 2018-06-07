#include "stdafx.h"

#include "Resource.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

// ������� �� ������������ ����
CoffeeMashine* coffeeMashine = nullptr;

HWND hlstCoffee = nullptr;

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
			try {
				// ��������� �������� �����
				GetDlgItemText(hDlg, txtInputSum, buffer, sizeof(buffer) / sizeof(buffer[0]) - 1);
				std::istringstream is(buffer);
				double sum = 0;
				is >> sum;
				// ��������� ��������� ��� ����
				TCHAR text[256];
				int index = SendMessage(hlstCoffee, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				SendMessage(hlstCoffee, LB_GETTEXT, (WPARAM)index, (LPARAM)text);
				// ����������� ���� � �������� �����
				std::pair<Coffee, CoffeeMashine::Cash> result = coffeeMashine->Cook(text, sum);
				
				std::string message, title;
				{
					std::ostringstream os;
					os << _T("����� (�����: ") << std::fixed << std::setprecision(2) << sum << _T(" ���.)");
					title = os.str();
				}
				if (result.second.empty()) {
					message = _T("����� �� ���������");
				} else {
					std::ostringstream os;
					for (auto it = result.second.begin(); it != result.second.end(); it++) {
						os << _T("������� ") << it->first << _T(" ���������� ") << it->second << _T('\n');
					}
					message = os.str();
				}
				MessageBox(hDlg, message.c_str(), title.c_str(), MB_ICONINFORMATION);
			}
			catch (CoffeeMashine::NotAvail& e) {
				MessageBox(hDlg, _T("��� ������ ����"), _T("������ ��������� ��������"), MB_ICONERROR);
			}
			catch (CoffeeMashine::NotEnoughMoney& e) {
				MessageBox(hDlg, _T("������������ �����"), _T("������ ��������� ��������"), MB_ICONERROR);
			}
			catch (CoffeeMashine::NoChange& e) {
				MessageBox(hDlg, _T("��� �����"), _T("������ ��������� ��������"), MB_ICONERROR);
			}
			catch (CoffeeMashine::Error& e) {
				MessageBox(hDlg, _T("�� ������� ����������� ����"), _T("������ ��������� ��������"), MB_ICONERROR);
			}
			return TRUE;
		}
		break;

	case WM_INITDIALOG:
		// � ����������� ����� ��������� ������ ��������� ��������� ��������� ����������
		hlstCoffee = GetDlgItem(hDlg, lstCoffeeKind);
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
		/* ��� ��������� ���������� ����� ��������� ����. ����� ����� �������� ������������, 
		������������� �� �� ����� ��������� ������ (�������� ��� ������� ������������ ������).
		� ���������� ������������ �� ������ ����� ������� �� �������� ���� ����������. */
		DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY: 
		/* �� ������ ���������� ��� ���������, ����� ������� �������, ��� ����� 
		��������� �������� ����� ����������. ��� ����� ������ ������� PostQuitMessage(). 
		������ ���� ��������� ���������� ��� ������������ ���������� ��������. */
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
