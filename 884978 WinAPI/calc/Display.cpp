#include "stdafx.h"

#include "Display.h"

Display::Display(HWND hDlg, int nCtlID) 
	: value(0), count(1), hDlg(hDlg), nCtlID(nCtlID)
{
	buffer[0] = _T('0');
	buffer[1] = _T('\0');
}

void Display::AddDigit(int digit) {
	_itot_s(digit, str, arrayLength(str), 10);
	if(1 == count && _T('0') == buffer[0]) {
		buffer[0] = str[0];
	} else {
		buffer[count++] = str[0];
	}
	buffer[count] = 0;
	Show();
}

void Display::AddDecimalDot() {
	buffer[count++] = _T('.');
	buffer[count] =_T('\0');
	Show();
}

void Display::Backspace() {
	if(count > 1) {
		count--;
	} else {
		buffer[0] = _T('0');
	}
	buffer[count] = _T('\0');
	Show();
}

void Display::Value(double val) {
	value = val;
	_stprintf_s(buffer, arrayLength(buffer), szRealFmt, val);
	Show(true);
}

void Display::Show(bool cut) {
	if(cut) {
		for(int i = _tcslen(buffer) - 1; i > 0; --i) {
			if(_T('.') == buffer[i]) {
				// дошли то точки, не встретив значащих цифр
				buffer[i] = _T('\0');
				break;
			} else if(_T('0') != buffer[i]) {
				buffer[i + 1] = _T('\0');
				// дошли то крайней справа значащей цифры
				break;
			}
		}
	}
	value = _tcstod(buffer, NULL);
	SetDlgItemText(hDlg, nCtlID, buffer);
}

void Display::InvertSign() {
	if(Round(value) != 0) {
		value = -value;
		_stprintf_s(buffer, arrayLength(buffer), szRealFmt, value);
		Show(true);
	}
}

void Display::Reset()
{
	value = 0.0;
	count = 1;
	buffer[0] = _T('0');
	buffer[1] = _T('\0');
	Show();
}
