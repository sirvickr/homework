#include "stdafx.h"

#include "Resource.h"
#include "Display.h"
#include "Operation.h"

HINSTANCE g_hInst = nullptr;
// ������� ��������
Operation* op = nullptr;
// ������� ������������
Display* display = nullptr;
// ���� "������������ ����� ��������"
bool reset = false;
// ������� ������
double memory = 0.0;
// ����� ������������: 0 - �������, 1 - ����������
int mode = 0;
// ������� ���� ��� �������
const int modeWidth[] = { 215, 300 };
const int modeHeight[] = { 310, 310 };

// ��������� �������� ���� ��� ������� "�������" � "����������"
void SetMode(HWND hDlg, int mode)
{
	SetWindowPos(hDlg, NULL, 0, 0, modeWidth[mode], modeHeight[mode], SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hDlg, txtDisplay), hDlg, 0, 0, modeWidth[mode] - 40, 23, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hDlg, btnSwitchView), hDlg, 0, 0, modeWidth[mode] - 40, 25, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

// ������� ��������� ������� ������ ������
INT_PTR CALLBACK DlgProcMode(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT ctrlID = 0;
	switch(uMsg) {
	// ��������� �� ������ �� ��������� ����������, ����������� �� ���������� ����
	case WM_COMMAND:
		// ������������� �������� ����������, ����������� ��������� ���������
		ctrlID = LOWORD(wParam);
		switch(ctrlID) {
		case IDCANCEL: // ������� ESC
			EndDialog(hDlg, FALSE);
			//SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		case IDOK: // ������� OK
			if(IsDlgButtonChecked(hDlg, optSimple))
				mode = 0;
			else
				mode = 1;
			EndDialog(hDlg, TRUE);
			break;
		}
		return TRUE;
	case WM_INITDIALOG:
		switch(mode) {
		case 0:
			CheckDlgButton(hDlg, optSimple, BST_CHECKED);
			break;
		case 1:
			CheckDlgButton(hDlg, optAdvanced, BST_CHECKED);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

// ������� ������� ��������� (���������� ��������� �������� ���� ����������)
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT ctrlID = 0;
	switch(uMsg) {
	// ��������� �� ������ �� ��������� ����������, ����������� �� ���������� ����
	case WM_COMMAND:
		// ������������� �������� ����������, ����������� ��������� ���������
		ctrlID = LOWORD(wParam);
		// ������������ ������� �������� ������
		if(btn0 <= ctrlID && ctrlID <= btn9) {
			// ����� �������� �������� (���� �� ��� ������������)
			if(reset) {
				reset = false;
				display->Reset();
			}
			// ��������� ��������� ����� � �������� ��������
			display->AddDigit(ctrlID - btn0);
			return TRUE;
		}
		switch(ctrlID) {
		case IDCANCEL: // ������� ESC
			display->Reset();
			return TRUE;
		case IDM_MODE:
			{
				// ���������� ������ ������ ������
				if(DialogBox(g_hInst, MAKEINTRESOURCE(dlgMode), hDlg, DlgProcMode) == IDOK) {
					SetMode(hDlg, mode);
				}
			}
			return TRUE;
		case btnSwitchView: // ������� ������ "����������� �����"
			mode ^= 1;
			SetMode(hDlg, mode);
			display->Reset();
			return TRUE;
		case btnDot: // ������� ������ "."
			display->AddDecimalDot();
			return TRUE;
		case btnPlus: // ������� ������ "+"
			op->Code(opSum);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnMinus: // ������� ������ "-"
			op->Code(opSub);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnMul: // ������� ������ "*"
			op->Code(opMul);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnDiv: // ������� ������ "/"
			op->Code(opDiv);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnSign: // ������� ������ "+/-"
			display->InvertSign();
			reset = true;
			return TRUE;
		case btn1divX: // ������� ������ "1/x"
			display->Value(1.0 / display->Value());
			reset = true;
			return TRUE;
		case btnSqrt: // ������� ������ "sqrt"
			display->Value(sqrt(display->Value()));
			reset = true;
			return TRUE;
		case btnPercent: // ������� ������ "%"
			display->Value(display->Value() * 100.0);
			reset = true;
			return TRUE;
		case btnPI: // ������� ������ "PI"
			display->Value(M_PI);
			reset = true;
			return TRUE;
		case btnSin: // ������� ������ "sin"
			display->Value(sin(display->Value()));
			reset = true;
			return TRUE;
		case btnCos: // ������� ������ "cos"
			display->Value(cos(display->Value()));
			reset = true;
			return TRUE;
		case btnTan: // ������� ������ "tan"
			display->Value(tan(display->Value()));
			reset = true;
			return TRUE;
		case btnPowEX: // ������� ������ "e^x"
			display->Value(exp(display->Value()));
			reset = true;
			return TRUE;
		case btnPowX2: // ������� ������ "x^2"
			display->Value(display->Value() * display->Value());
			reset = true;
			return TRUE;
		case btnPowX3: // ������� ������ "x^3"
			display->Value(display->Value() * display->Value() * display->Value());
			reset = true;
			return TRUE;
		case btnPow10X: // ������� ������ "10^x"
			display->Value(pow(10.0, display->Value()));
			reset = true;
			return TRUE;
		case btnPowXY: // ������� ������ "x^y"
			op->Code(opPow);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnLog: // ������� ������ "log"
			display->Value(log10(display->Value()));
			reset = true;
			return TRUE;
		case btnMod: // ������� ������ "mod"
			op->Code(opMod);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnFact: // ������� ������ "n!"
			{
				UINT64 result = 1;
				UINT64 value = (UINT64)(display->Value() + 0.5);
				for(UINT64 i = 2; i <= value; ++i)
					result *= i;
				display->Value(result);
			}
			reset = true;
			return TRUE;
		case btnMC: // ������� ������ "MC" (����� �������� ������)
			memory = 0.0;
			return TRUE;
		case btnMS: // ������� ������ "MS" (���������� �������� ������)
			memory = display->Value();
			return TRUE;
		case btnMR: // ������� ������ "MR" (������ �������� ������)
			display->Value(memory);
			reset = true;
			return TRUE;
		case btnMPlus: // ������� ������ "M+" (����������� � �������� ������)
			memory += display->Value();
			return TRUE;
		case btnMMinus: // ������� ������ "M-" (��������� �� �������� ������)
			memory -= display->Value();
			return TRUE;
		case btnResult: // ������� ������ "="
			op->Right()->Value(display->Value());
			display->Value(op->Calculate());
			reset = true;
			return TRUE;
		case btnBS: // ������� ������ "<--" (Backspace)
			display->Backspace();
			return TRUE;
		case btnC: // ������� ������ "C"
			display->Reset();
			return TRUE;
		case btnCE: // ������� ������ "CE"
			display->Reset();
			return TRUE;
		}
		break;

	case WM_INITDIALOG:
		// � ����������� ����� ��������� ������ ��������� ��������� ��������� ����������
		SetMenu(hDlg, LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1)));
		// �������� ������ ��� ������������� �������
		op = new Operation(new Operation, new Operation);
		// �������� ������ ��� ������� �������
		display = new Display(hDlg, txtDisplay);
		// ����� ������� ���� ��� �������� ������
		SetMode(hDlg, mode);
		return TRUE;

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
		delete op;
		delete display;
		PostQuitMessage(0);
		return TRUE;

	}
	return FALSE;
}

// ����� ����� ����������
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL ret;
	// ���������� ���������� ����������
	g_hInst = hInst;
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
