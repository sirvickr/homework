//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "dict.h"
#include "menu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ===================== ������� ���� =======================================
// ������� ���� <����>. ����������� ����� ������������
int File() {
	printf("�� ������� ���� '����'\n");
	return 0;
}
// ������� ���� <��������>. ����������� ����� ������������
int Do() {
	long val = 0;
	printf("Action\n");
	return 0;
}
// ������� ���� <�����>. ����������� ����� ������������
int Exit() {
	int resp;
	//cout << "�� �������, ��� ������ ����� � ��������� (y/n)?";
	printf("Quit? (y/n)?");
	resp = getchar();
	if (resp == 'y' || resp == 'Y') {
		return 1;
	}
	return 0;
}
// ������� ���� <��������>. ����������� ����� ������������
int Clear(void)
{
	cls();
	return 0;                        
}

