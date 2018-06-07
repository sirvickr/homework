#include "stdafx.h"

#include "Resource.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

// Common Controls (красивый вид элементов управления)
#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

// Автомат по производству кофе
CoffeeMashine* coffeeMashine = nullptr;

HWND hlstCoffee = nullptr;
HWND hlstOutput = nullptr;

// Главная оконная процедура (обработчик сообщений главного окна приложения)
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256];
	UINT ctrlID = 0;
	switch(uMsg) {
	// Сообщение от одного из элементов управления, размещённых на диалоговом окне
	case WM_COMMAND:
		// Идентификатор элемента управления, являющегося источниом сообщения
		ctrlID = LOWORD(wParam);
		switch (ctrlID) {
		case IDCANCEL: // Нажатие ESC
			SetDlgItemText(hDlg, txtInputSum, _T("0.0"));
			return TRUE;
		case btnClose: // Нажатие кнопки "Закрыть"
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case btnCook: // Нажатие кнопки "Приготовить"
			// очистить список результатов
			SendMessage(hlstOutput, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
			try {
				// прочитать денежную сумму
				GetDlgItemText(hDlg, txtInputSum, buffer, sizeof(buffer) / sizeof(buffer[0]) - 1);
				tistringstream is(buffer);
				double sum = 0;
				is >> sum;
				// прочитать выбранный вид кофе
				TCHAR text[256];
				int index = SendMessage(hlstCoffee, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				SendMessage(hlstCoffee, LB_GETTEXT, (WPARAM)index, (LPARAM)text);
				// приготовить кофе и получить сдачу
				std::pair<Coffee, CoffeeMashine::Cash> result = coffeeMashine->Cook(text, sum);
				// вывод результатов
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)text);
				if (result.second.empty()) {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Сдача не требуется"));
				} else {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Сдача:"));
					for (auto it = result.second.begin(); it != result.second.end(); it++) {
						tostringstream os;
						os << _T("Номинал ") << it->first << _T(" количество ") << it->second;
						SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)os.str().c_str());
					}
				}
			}
			catch (CoffeeMashine::NotAvail& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Нет такого кофе"));
			}
			catch (CoffeeMashine::NotEnoughMoney& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Недостаточно денег"));
			}
			catch (CoffeeMashine::NoChange& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Нет сдачи"));
			}
			catch (CoffeeMashine::Error& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Не удалось приготовить кофе"));
			}
			return TRUE;
		}
		break;

	case WM_INITDIALOG:
		// В обработчике этого сообщения удобно проводить начальную настройку приложения
		hlstCoffee = GetDlgItem(hDlg, lstCoffeeKind);
		hlstOutput = GetDlgItem(hDlg, lstOutput);
		// Выделяем память для объекта "Автомат по производству кофе"
		coffeeMashine = new CoffeeMashine("coffee.cfg", "cash.cfg");

		SetDlgItemText(hDlg, txtInputSum, _T("100.0"));
		{
			CoffeeMashine::CoffeeAvail coffeeAvail = coffeeMashine->getCoffeeAvail();
			// Заполняем список выбора напитков
			for (const auto coffee : coffeeAvail) {
				SendMessage(hlstCoffee, LB_ADDSTRING, (WPARAM)0, (LPARAM)coffee.first.c_str());
			}
		}
		SendMessage(hlstCoffee, LB_SETCURSEL, (WPARAM)0, 0);
		break;

	case WM_CLOSE:
		/* Это сообщение вызывается перед закрытием окна. Здесь можно спросить 
		пользователя, действительно ли он хочет завершить работу (особенно при 
		наличии несохранённых данных). В этом приложении мы просто отдаём команду 
		на закрытие главного окна. */
		DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY: 
		/* Мы должны обработать это сообщение, чтобы сказать системе, что 
		хотим завершить основной поток приложения. Для этого служит функция 
		PostQuitMessage(). Сейчас есть последняя возможность для освобождения 
		выделенных ресурсов. */
		delete coffeeMashine;
		PostQuitMessage(0);
		return TRUE;

	}
	return FALSE;
}

// Точка входа приложения
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL ret;
	InitCommonControls();
	// Создаём главное окно
	HWND hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(mainWnd), 0, DialogProc, 0);
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
