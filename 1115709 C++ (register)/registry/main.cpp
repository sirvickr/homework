// registry.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "resource.h"
#include "Registry.h"

// Common Controls (красивый вид элементов управления)
#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

using namespace std;

// Вспомогательный текстовый буфер
static const size_t textBufSize = 256;
TCHAR szText[textBufSize];
// Класс, реализующий работу регистратуры
Registry registry;

// Текущие параметры для обмена с диалоговым окном
Doctor* currDoctor = nullptr;
int currDay = 0;
tstring newTime;
tstring newSpan;
tstring newPatient;

// Глобальный дескриптор приложения
HINSTANCE hInst = nullptr;

// Оконные дескрипторы некоторых элементов управления (которые используем)
HWND hlstSpec = nullptr;
HWND hlstWeek = nullptr;
HWND hlvwDoctors = nullptr;
HWND hlvwSchedule = nullptr;

// Переменные для работы с ListView
LVCOLUMN lvColumn;
LVITEM lvItem;

Doctor* CurrentDoctor(int index = -1)
{
	LVITEM lvItem;
	memset(&lvItem, 0x00, sizeof(lvItem));
	if (-1 == index)
		index = ListView_GetNextItem(hlvwDoctors, -1, LVNI_SELECTED);
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = index;
	lvItem.iSubItem = 0;
	ListView_GetItem(hlvwDoctors, &lvItem);
	return (Doctor*)lvItem.lParam;
}

void UpdateSchedule(Doctor* doc) {
	ListView_DeleteAllItems(hlvwSchedule);
	memset(&lvItem, 0x00, sizeof(lvItem));
	lvItem.cchTextMax = textBufSize; // максимальный размер строки
	{
		int i = 0;
		for (auto item : registry.Schedule()) {
			TimeSchedule* schedule = dynamic_cast<TimeSchedule*>(item);
			int day = SendMessage(hlstWeek, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			if (schedule && schedule->DoctorID() == doc->Id() && schedule->Day() == day) {
				lvItem.mask = LVIF_TEXT | LVIF_PARAM; // текстовый стиль
				lvItem.iItem = i;
				lvItem.lParam = (LPARAM)schedule;
				// Начало
				lvItem.iSubItem = 0;
				lvItem.pszText = (LPTSTR)schedule->Time().c_str();
				SendMessage(hlvwSchedule, LVM_INSERTITEM, (WPARAM)0, (LPARAM)&lvItem);

				lvItem.mask = LVIF_TEXT; // текстовый стиль
				// Длительность
				lvItem.iSubItem = 1;
				lvItem.pszText = (LPTSTR)schedule->Span().c_str();
				SendMessage(hlvwSchedule, LVM_SETITEM, (WPARAM)0, (LPARAM)&lvItem);
				// Пациент
				lvItem.iSubItem = 2;
				_stprintf(szText, _T("%s"), schedule->Patient().c_str());
				lvItem.pszText = szText;
				SendMessage(hlvwSchedule, LVM_SETITEM, (WPARAM)0, (LPARAM)&lvItem);
				i++;
			}
		}
		// выделяем первый элемент
		ListView_SetItemState(hlvwSchedule, 0, LVFIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED);
	}
}


// Оконная процедура диалога добавления визита к врачу
INT_PTR CALLBACK AddVisitProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT ctrlID = 0;
	switch (uMsg) {
	// Сообщение от одного из элементов управления, размещённых на диалоговом окне
	case WM_COMMAND:
		// Идентификатор элемента управления, являющегося источниом сообщения
		ctrlID = LOWORD(wParam);
		switch (ctrlID) {
		case IDCANCEL: // Нажатие ESC
			EndDialog(hDlg, FALSE);
			//SendMessage(hDlg, WM_CLOSE, 0, 0);
			break;
		case IDOK: // Нажатие OK
			GetDlgItemText(hDlg, txtPatient, szText, textBufSize);
			newPatient = szText;
			GetDlgItemText(hDlg, txtTime, szText, textBufSize);
			newTime = szText;
			GetDlgItemText(hDlg, txtSpan, szText, textBufSize);
			newSpan = szText;
			EndDialog(hDlg, TRUE);
			break;
		}
		return TRUE;
	case WM_INITDIALOG:
		if (currDoctor) {
			SetDlgItemText(hDlg, txtDoctor, currDoctor->Name().c_str());
			SetDlgItemText(hDlg, txtDay, TimeSchedule::Week[currDay]);
		}
		return TRUE;
	}
	return FALSE;
}

// Главная оконная процедура (обработчик сообщений главного окна приложения)
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT ctrlID = 0;
	NMLISTVIEW* nm = nullptr;
	switch (uMsg) {
	case WM_NOTIFY:
		nm = (NMLISTVIEW*)lParam;
		if (nm->hdr.code == LVN_ITEMCHANGED && nm->hdr.idFrom == lvwDoctors && (nm->uNewState & LVIS_SELECTED)) {
			Doctor* doc = CurrentDoctor(nm->iItem);
			if (doc) {
				SetDlgItemText(hDlg, txtName, doc->Name().c_str());
				_stprintf(szText, _T("%d"), doc->CabinetNumber());
				SetDlgItemText(hDlg, txtCabinetNumber, szText);
				SetDlgItemText(hDlg, txtBirthDate, doc->BirthDate().c_str());
				//SendMessage(hlstSpec, LB_SETCURSEL, (WPARAM)doc->Specialization(), (LPARAM)0);
				SendMessage(hlstSpec, LB_FINDSTRING, (WPARAM)-1, (LPARAM)doc->Specialization().c_str());
				UpdateSchedule(doc);
			}

		}
		break;
		// Сообщение от одного из элементов управления, размещённых на диалоговом окне
	case WM_COMMAND:
		// Идентификатор элемента управления, являющегося источниом сообщения
		ctrlID = LOWORD(wParam);
		switch (ctrlID) {
		case IDCANCEL: // Клавиша ESC
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case lstWeek:
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				Doctor* doc = CurrentDoctor();
				if (doc) {
					UpdateSchedule(doc);
				}
			}
			return TRUE;
		case cmdAdd:
			currDoctor = CurrentDoctor();
			currDay = SendMessage(hlstWeek, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
#if 1
			if (DialogBox(hInst, MAKEINTRESOURCE(dlgAddVisit), hDlg, AddVisitProc) == IDOK) {
				registry.AddVisitToSchedule(currDoctor->Id(), currDay, newPatient, newTime, newSpan);
				UpdateSchedule(currDoctor);
			}
#else
			if (!IsWindow(hdlgAddVisit)) {
				hdlgAddVisit = CreateDialog(hInst, MAKEINTRESOURCE(dlgAddVisit), hDlg, (DLGPROC)AddVisitProc);
				ShowWindow(hdlgAddVisit, SW_SHOW);
			}
#endif
			currDoctor = nullptr;
			currDay = 0;
			break;
		}
		return TRUE;

	case WM_INITDIALOG:
		// В обработчике этого сообщения удобно проводить начальную настройку приложения
		// Получаем оконные дескрипторы, которые понадобятся в работе
		hlstSpec = GetDlgItem(hDlg, lstSpecialization);
		hlstWeek = GetDlgItem(hDlg, lstWeek);
		hlvwDoctors = GetDlgItem(hDlg, lvwDoctors);
		hlvwSchedule = GetDlgItem(hDlg, lvwSchedule);

		// Заполняем список доступных специализаций
		
		for (const auto& item : Doctor::specializations) {
			SendMessage(hlstSpec, LB_ADDSTRING, (WPARAM)0, (LPARAM)item.c_str());
		}

		// Заполняем список дней недели

		for (const auto& item : TimeSchedule::Week) {
			SendMessage(hlstWeek, LB_ADDSTRING, (WPARAM)0, (LPARAM)item);
		}
		SendMessage(hlstWeek, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		// Заполняем список докторов

		SendMessage(hlvwDoctors, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		memset(&lvColumn, 0x00, sizeof(lvColumn));
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		// Заголовки списка врачей
		lvColumn.cx = 50; // Ширина столбца
		lvColumn.pszText = (LPTSTR)_T("№");
		SendMessage(hlvwDoctors, LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&lvColumn);

		lvColumn.cx = 150; // Ширина столбца
		lvColumn.pszText = (LPTSTR)_T("ФИО");
		SendMessage(hlvwDoctors, LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&lvColumn);

		lvColumn.cx = 160;
		lvColumn.pszText = (LPTSTR)_T("Специализация");
		SendMessage(hlvwDoctors, LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&lvColumn);

		memset(&lvItem, 0x00, sizeof(lvItem));
		lvItem.cchTextMax = textBufSize; // максимальный размер строки
		{
			int i = 0;
			for (auto item : registry.Doctors()) {
				Doctor* doc = dynamic_cast<Doctor*>(item);
				if (doc) {
					lvItem.mask = LVIF_TEXT | LVIF_PARAM; // текстовый стиль
					lvItem.iItem = i;
					lvItem.lParam = (LPARAM)doc;
					// №
					lvItem.iSubItem = 0;
					_stprintf(szText, _T("%d"), doc->Id());
					lvItem.pszText = szText;// to_tstring(doc->Id()).c_str();
					SendMessage(hlvwDoctors, LVM_INSERTITEM, (WPARAM)0, (LPARAM)&lvItem);
					
					lvItem.mask = LVIF_TEXT; // текстовый стиль
					// ФИО
					lvItem.iSubItem = 1;
					lvItem.pszText = (LPTSTR)doc->Name().c_str();
					SendMessage(hlvwDoctors, LVM_SETITEM, (WPARAM)0, (LPARAM)&lvItem);
					// Специализация
					lvItem.iSubItem = 2;
					_stprintf(szText, _T("%s"), doc->Specialization().c_str());
					lvItem.pszText = szText;
					SendMessage(hlvwDoctors, LVM_SETITEM, (WPARAM)0, (LPARAM)&lvItem);
					i++;
				}
			}
			// выделяем первый элемент
			ListView_SetItemState(hlvwDoctors, 0, LVFIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED);
		}

		// Заполняем расписание

		SendMessage(hlvwSchedule, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
		memset(&lvColumn, 0x00, sizeof(lvColumn));
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		// Заголовки
		lvColumn.cx = 90;
		lvColumn.pszText = (LPTSTR)_T("Начало");
		SendMessage(hlvwSchedule, LVM_INSERTCOLUMN, (WPARAM)0, (LPARAM)&lvColumn);

		lvColumn.cx = 90;
		lvColumn.pszText = (LPTSTR)_T("Длительность");
		SendMessage(hlvwSchedule, LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&lvColumn);

		lvColumn.cx = 100;
		lvColumn.pszText = (LPTSTR)_T("Пациент");
		SendMessage(hlvwSchedule, LVM_INSERTCOLUMN, (WPARAM)2, (LPARAM)&lvColumn);
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
		//delete coffeeMashine;
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
	// Инициализируем компоненты
	InitCommonControls();
	// Запоминаем дескриптор приложения
	hInst = hInst;
	// Создаём главное окно
	HWND hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(mainWnd), 0, DialogProc, 0);
	// Отображаем его на экране
	ShowWindow(hDlg, nCmdShow);
	// Стандартным образом обрабатываем поступающие сообщения в цикле
	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if (ret == -1)
			return -1;

		if (!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
