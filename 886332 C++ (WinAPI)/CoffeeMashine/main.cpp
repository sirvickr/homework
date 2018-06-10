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

// ������ ��������� �������� ���������
typedef std::pair<CashValue, tstring> CashChoice;
const CashChoice cashChoice[] = {
	{ note500, _T("500 ������") },
	{ note100, _T("100 ������") },
	{ note50, _T("50 ������") },
	{ coin10, _T("10 ������") },
	{ coin5, _T("5 ������") },
	{ coin2, _T("2 �����") },
	{ coin1, _T("1 �����") },
};

// ��������������� ��������� �����
TCHAR szText[256];
// ������� �� ������������ ����
CoffeeMashine* coffeeMashine = nullptr;
// ������� ����������� ��������� ��������� ���������� (������� ����������)
HWND hlstOutput = nullptr;
HWND hlvwCoffee = nullptr;
HWND hlstMoney = nullptr;
// ���������� ��� ������ � ListView
LVCOLUMN lvColumn;
LVITEM lvItem;
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
			//SetDlgItemText(hDlg, txtInputSum, _T("0.0"));
			return TRUE;
		case btnClose: // ������ "�������"
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case btnDeposit: // ������ "������ ������"
			// �������� ������ �����������
			SendMessage(hlstOutput, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
			try {
				// ��������� �������� �����
				int index = SendMessage(hlstMoney, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				double balance = coffeeMashine->depositMoney(cashChoice[index].first);
				// ���������� ������� ������
				tostringstream os;
				os << std::fixed << std::setprecision(2) << balance;
				SetDlgItemText(hDlg, txtBalance, os.str().c_str());
			}
			catch (CoffeeMashine::Error& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("�� ������� ������ ������"));
			}
			return TRUE;
		case btnCook: // ������ "�����������"
			try {
				// ��������� ��������� ��� ����
				int index = ListView_GetNextItem(hlvwCoffee, -1, LVNI_SELECTED);
				ListView_GetItemText(hlvwCoffee, index, 0, szText, sizeof(szText) / sizeof(szText[0]));

				// ����������� ���� � �������� �����
				Coffee coffee = coffeeMashine->Cook(szText);
				// ����� �����������
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)szText);
				// ���������� ������� ������
				tostringstream os;
				os << std::fixed << std::setprecision(2) << coffeeMashine->getBalance();
				SetDlgItemText(hDlg, txtBalance, os.str().c_str());
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
		case btnChange:
			try {
				CoffeeMashine::Cash change = coffeeMashine->getChange();
				if (change.empty()) {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("����� ���"));
				} else {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("�����:"));
					for (auto it = change.begin(); it != change.end(); it++) {
						tostringstream os;
						os << _T("������� ") << it->first << _T(" ���������� ") << it->second;
						SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)os.str().c_str());
					}
				}
				// ���������� ������� ������
				tostringstream os;
				os << std::fixed << std::setprecision(2) << coffeeMashine->getBalance();
				SetDlgItemText(hDlg, txtBalance, os.str().c_str());
			}
			catch (CoffeeMashine::NoChange& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("��� �����"));
			}
			return TRUE;
		}
		return TRUE;
	case WM_INITDIALOG:
		// � ����������� ����� ��������� ������ ��������� ��������� ��������� ����������
		// �������� ������� �����������, ������� ����������� � ������
		hlstOutput = GetDlgItem(hDlg, lstOutput);
		hlvwCoffee = GetDlgItem(hDlg, lvwCoffee);
		hlstMoney = GetDlgItem(hDlg, lstMoney);
		// �������� ������ ��� ������� "������� �� ������������ ����"
		coffeeMashine = new CoffeeMashine(_T("coffee.cfg"), _T("cash.cfg"));
		// ��������� ������ ��������� �������� ������
		for (const auto item : cashChoice)
			SendMessage(hlstMoney, CB_ADDSTRING, (WPARAM)0, (LPARAM)item.second.c_str());
		SendMessage(hlstMoney, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
		// ��������� ������� ������ � ��������� ����
		SetDlgItemText(hDlg, txtBalance, _T("0.00"));
		// ��������� ������ ��������
		SendMessage(hlvwCoffee, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		memset(&lvColumn, 0x00, sizeof(lvColumn));
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lvColumn.cx = 80; // ������ �������
		lvColumn.pszText = _T("����");
		SendMessage(hlvwCoffee, LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&lvColumn);
		
		lvColumn.cx = 40;
		lvColumn.pszText = _T("����");
		SendMessage(hlvwCoffee, LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&lvColumn);
		// ��������� ������ ������ ��������
		memset(&lvItem, 0x00, sizeof(lvItem));
		lvItem.mask = LVIF_TEXT; // ��������� �����
		lvItem.cchTextMax = 256; // ������������ ������ ������
		{
			CoffeeMashine::CoffeeAvail coffeeAvail = coffeeMashine->getCoffeeAvail();
			int i = 0;
			for (const auto coffee : coffeeAvail) {
				lvItem.iItem = 0;
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPTSTR)coffee.first.c_str();
				SendMessage(hlvwCoffee, LVM_INSERTITEM, (WPARAM)0, (LPARAM)&lvItem);
				// ����
				lvItem.iSubItem = 1;
				_stprintf(szText, _T("%.2f"), coffee.second.getPrice());
				lvItem.pszText = szText;
				SendMessage(hlvwCoffee, LVM_SETITEM, (WPARAM)0, (LPARAM)&lvItem);
				i++;
			}
			// �������� ������ �������
			ListView_SetItemState(hlvwCoffee, 0, LVFIS_FOCUSED | LVIS_SELECTED, 0x000F);
		}
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
