#include "stdafx.h"

#include "Resource.h"
#include "Display.h"
#include "Operation.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

Operation* op = nullptr;
Display* display = nullptr;
bool reset = false;
double memory = 0.0;

int mode = 0; // режим: 0 - обычный, 1 - инжереный
const int modeWidth[] = { 215, 300 };
const int modeHeight[] = { 290, 290 };

// Установка размеров окна для режимов "Простой" и "Инженерный"
void SetMode(HWND hDlg, int mode)
{
	SetWindowPos(hDlg, NULL, 0, 0, modeWidth[mode], modeHeight[mode], SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hDlg, txtDisplay), hDlg, 0, 0, modeWidth[mode] - 40, 23, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hDlg, btnSwitchView), hDlg, 0, 0, modeWidth[mode] - 40, 25, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

// Главная оконная процедура (обработчик сообщений главного окна приложения)
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT ctrlID = 0;
	switch(uMsg) {
	// Сообщение от одного из элементов управления, размещённых на диалоговом окне
	case WM_COMMAND:
		// Идентификатор элемента управления, являющегося источниом сообщения
		ctrlID = LOWORD(wParam);
		// Обрабатываем нажатие цифровой кнопки
		if(btn0 <= ctrlID && ctrlID <= btn9) {
			// Сброс текущего значения (если он был запланирован)
			if(reset) {
				reset = false;
				display->Reset();
			}
			// Добавляем очередную цифру к текущему значению
			display->AddDigit(ctrlID - btn0);
			return TRUE;
		}
		switch(ctrlID) {
		case IDCANCEL: // Нажатие ESC
			display->Reset();
			return TRUE;
		case btnSwitchView: // Нажатие кнопки "Переключить режим"
			mode ^= 1;
			SetMode(hDlg, mode);
			display->Reset();
			return TRUE;
		case btnDot: // Нажатие кнопки "."
			display->AddDecimalDot();
			return TRUE;
		case btnPlus: // Нажатие кнопки "+"
			op->Code(opSum);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnMinus: // Нажатие кнопки "-"
			op->Code(opSub);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnMul: // Нажатие кнопки "*"
			op->Code(opMul);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnDiv: // Нажатие кнопки "/"
			op->Code(opDiv);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnSign: // Нажатие кнопки "+/-"
			display->InvertSign();
			reset = true;
			return TRUE;
		case btn1divX: // Нажатие кнопки "1/x"
			display->Value(1.0 / display->Value());
			reset = true;
			return TRUE;
		case btnSqrt: // Нажатие кнопки "sqrt"
			display->Value(sqrt(display->Value()));
			reset = true;
			return TRUE;
		case btnPercent: // Нажатие кнопки "%"
			display->Value(display->Value() * 100.0);
			reset = true;
			return TRUE;
		case btnPI: // Нажатие кнопки "PI"
			display->Value(M_PI);
			reset = true;
			return TRUE;
		case btnSin: // Нажатие кнопки "sin"
			display->Value(sin(display->Value()));
			reset = true;
			return TRUE;
		case btnCos: // Нажатие кнопки "cos"
			display->Value(cos(display->Value()));
			reset = true;
			return TRUE;
		case btnTan: // Нажатие кнопки "tan"
			display->Value(tan(display->Value()));
			reset = true;
			return TRUE;
		case btnPowEX: // Нажатие кнопки "e^x"
			display->Value(exp(display->Value()));
			reset = true;
			return TRUE;
		case btnPowX2: // Нажатие кнопки "x^2"
			display->Value(display->Value() * display->Value());
			reset = true;
			return TRUE;
		case btnPowX3: // Нажатие кнопки "x^3"
			display->Value(display->Value() * display->Value() * display->Value());
			reset = true;
			return TRUE;
		case btnPow10X: // Нажатие кнопки "10^x"
			display->Value(pow(10.0, display->Value()));
			reset = true;
			return TRUE;
		case btnPowXY: // Нажатие кнопки "x^y"
			op->Code(opPow);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnLog: // Нажатие кнопки "log"
			display->Value(log10(display->Value()));
			reset = true;
			return TRUE;
		case btnMod: // Нажатие кнопки "mod"
			op->Code(opMod);
			op->Left()->Value(display->Value());
			reset = true;
			return TRUE;
		case btnFact: // Нажатие кнопки "n!"
			{
				UINT64 result = 1;
				UINT64 value = (UINT64)(display->Value() + 0.5);
				for(UINT64 i = 2; i <= value; ++i)
					result *= i;
				display->Value(result);
			}
			reset = true;
			return TRUE;
		case btnMC: // Нажатие кнопки "MC" (сброс регистра памяти)
			memory = 0.0;
			return TRUE;
		case btnMS: // Нажатие кнопки "MS" (присвоение регистру памяти)
			memory = display->Value();
			return TRUE;
		case btnMR: // Нажатие кнопки "MR" (чтение регистра памяти)
			display->Value(memory);
			reset = true;
			return TRUE;
		case btnMPlus: // Нажатие кнопки "M+" (прибавление к регистру памяти)
			memory += display->Value();
			return TRUE;
		case btnMMinus: // Нажатие кнопки "M-" (вычитание из регистра памяти)
			memory -= display->Value();
			return TRUE;
		case btnResult: // Нажатие кнопки "="
			op->Right()->Value(display->Value());
			display->Value(op->Calculate());
			reset = true;
			return TRUE;
		case btnBS: // Нажатие кнопки "<--" (Backspace)
			display->Backspace();
			return TRUE;
		case btnC: // Нажатие кнопки "C"
			display->Reset();
			return TRUE;
		case btnCE: // Нажатие кнопки "CE"
			display->Reset();
			return TRUE;
		}
		break;

	case WM_INITDIALOG:
		// В обработчике этого сообщения удобно проводить начальную настройку приложения
		// Выделяем память для вычислитеного объекта
		op = new Operation(new Operation, new Operation);
		// Выделяем память для объекта дисплея
		display = new Display(hDlg, txtDisplay);
		// Задаём размеры окна для текущего режима
		SetMode(hDlg, mode);
		break;

	case WM_CLOSE:
		/* Это сообщение вызывается перед закрытием окна. Здесь можно спросить пользователя, 
		действительно ли он хочет завершить работу (особенно при наличии несохранённых данных).
		В приложении калькулятора мы просто отдаём команду на закрытие окна приложения. */
		DestroyWindow(hDlg);
		return TRUE; /* just continue reading on... */

	case WM_DESTROY: 
		/* Мы должны обработать это сообщение, чтобы сказать системе, что хотим 
		завершить основной поток приложения. Для этого служит функция PostQuitMessage(). 
		Сейчас есть последняя воможность для освобождения выделенных ресурсов. */
		delete op;
		delete display;
		PostQuitMessage(0);
		return TRUE;

	}
	return FALSE;
}

// Точка входа приложения
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hDlg;
	MSG msg;
	BOOL ret;

	InitCommonControls();
	// Создаём главное окно
	hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(mainWnd), 0, DialogProc, 0);
	// Отображаем его на экране
	ShowWindow(hDlg, nCmdShow);
	// Стандартным образом обрабатываем поступающие сообщения в цикле
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
