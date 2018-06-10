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

// Список доступных денежных номиналов
typedef std::pair<CashValue, tstring> CashChoice;
const CashChoice cashChoice[] = {
	{ note500, _T("500 рублей") },
	{ note100, _T("100 рублей") },
	{ note50, _T("50 рублей") },
	{ coin10, _T("10 рублей") },
	{ coin5, _T("5 рублей") },
	{ coin2, _T("2 рубля") },
	{ coin1, _T("1 рубль") },
};

// Вспомогательный текстовый буфер
TCHAR szText[256];
// Автомат по производству кофе
CoffeeMashine* coffeeMashine = nullptr;
// Оконные дескрипторы некоторых элементов управления (которые используем)
HWND hlstOutput = nullptr;
HWND hlvwCoffee = nullptr;
HWND hlstMoney = nullptr;
// Переменные для работы с ListView
LVCOLUMN lvColumn;
LVITEM lvItem;
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
		case IDCANCEL: // Клавиша ESC
			//SetDlgItemText(hDlg, txtInputSum, _T("0.0"));
			return TRUE;
		case btnClose: // Кнопка "Закрыть"
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case btnDeposit: // Кнопка "Внести деньги"
			// очистить список результатов
			SendMessage(hlstOutput, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
			try {
				// прочитать денежную сумму
				int index = SendMessage(hlstMoney, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				double balance = coffeeMashine->depositMoney(cashChoice[index].first);
				// отобразить текущий баланс
				tostringstream os;
				os << std::fixed << std::setprecision(2) << balance;
				SetDlgItemText(hDlg, txtBalance, os.str().c_str());
			}
			catch (CoffeeMashine::Error& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Не удалось внести деньги"));
			}
			return TRUE;
		case btnCook: // Кнопка "Приготовить"
			try {
				// прочитать выбранный вид кофе
				int index = ListView_GetNextItem(hlvwCoffee, -1, LVNI_SELECTED);
				ListView_GetItemText(hlvwCoffee, index, 0, szText, sizeof(szText) / sizeof(szText[0]));

				// приготовить кофе и получить сдачу
				Coffee coffee = coffeeMashine->Cook(szText);
				// вывод результатов
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)szText);
				// отобразить текущий баланс
				tostringstream os;
				os << std::fixed << std::setprecision(2) << coffeeMashine->getBalance();
				SetDlgItemText(hDlg, txtBalance, os.str().c_str());
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
		case btnChange:
			try {
				CoffeeMashine::Cash change = coffeeMashine->getChange();
				if (change.empty()) {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Сдачи нет"));
				} else {
					SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Сдача:"));
					for (auto it = change.begin(); it != change.end(); it++) {
						tostringstream os;
						os << _T("Номинал ") << it->first << _T(" количество ") << it->second;
						SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)os.str().c_str());
					}
				}
				// отобразить текущий баланс
				tostringstream os;
				os << std::fixed << std::setprecision(2) << coffeeMashine->getBalance();
				SetDlgItemText(hDlg, txtBalance, os.str().c_str());
			}
			catch (CoffeeMashine::NoChange& e) {
				SendMessage(hlstOutput, LB_ADDSTRING, (WPARAM)0, (LPARAM)_T("Нет сдачи"));
			}
			return TRUE;
		}
		return TRUE;
	case WM_INITDIALOG:
		// В обработчике этого сообщения удобно проводить начальную настройку приложения
		// Получаем оконные дескрипторы, которые понадобятся в работе
		hlstOutput = GetDlgItem(hDlg, lstOutput);
		hlvwCoffee = GetDlgItem(hDlg, lvwCoffee);
		hlstMoney = GetDlgItem(hDlg, lstMoney);
		// Выделяем память для объекта "Автомат по производству кофе"
		coffeeMashine = new CoffeeMashine(_T("coffee.cfg"), _T("cash.cfg"));
		// Заполняем список доступных денежных знаков
		for (const auto item : cashChoice)
			SendMessage(hlstMoney, CB_ADDSTRING, (WPARAM)0, (LPARAM)item.second.c_str());
		SendMessage(hlstMoney, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
		// Указываем нудевой баланс в текстовом поле
		SetDlgItemText(hDlg, txtBalance, _T("0.00"));
		// Заголовки списка напитков
		SendMessage(hlvwCoffee, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		memset(&lvColumn, 0x00, sizeof(lvColumn));
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lvColumn.cx = 80; // Ширина столбца
		lvColumn.pszText = _T("Кофе");
		SendMessage(hlvwCoffee, LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&lvColumn);
		
		lvColumn.cx = 40;
		lvColumn.pszText = _T("Цена");
		SendMessage(hlvwCoffee, LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&lvColumn);
		// Заполняем список выбора напитков
		memset(&lvItem, 0x00, sizeof(lvItem));
		lvItem.mask = LVIF_TEXT; // текстовый стиль
		lvItem.cchTextMax = 256; // максимальный размер строки
		{
			CoffeeMashine::CoffeeAvail coffeeAvail = coffeeMashine->getCoffeeAvail();
			int i = 0;
			for (const auto coffee : coffeeAvail) {
				lvItem.iItem = 0;
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPTSTR)coffee.first.c_str();
				SendMessage(hlvwCoffee, LVM_INSERTITEM, (WPARAM)0, (LPARAM)&lvItem);
				// цена
				lvItem.iSubItem = 1;
				_stprintf(szText, _T("%.2f"), coffee.second.getPrice());
				lvItem.pszText = szText;
				SendMessage(hlvwCoffee, LVM_SETITEM, (WPARAM)0, (LPARAM)&lvItem);
				i++;
			}
			// выделяем первый элемент
			ListView_SetItemState(hlvwCoffee, 0, LVFIS_FOCUSED | LVIS_SELECTED, 0x000F);
		}
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
