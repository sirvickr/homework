// barber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
// ���� ����� ����������� � ������ - �����������
static const int THREAD_COUNT = 2;
// ���������� ���� � ���� ��������
static const int SLOT_COUNT = 3;
// ���������� �������� (�.�. ������������ �������� �� ����)
static const int CLIENT_COUNT = 10;

DWORD WINAPI BarberThread(LPVOID);
DWORD WINAPI ClientThread(LPVOID);

struct WaitingSlot {
	bool busy = false;
	bool barber = false;
	int clientId = -1;
};

// ��� �������� (������� ������)
WaitingSlot WaitingHall[SLOT_COUNT];
int qHead = 0;
int qSize = 0;
// ������ ���� ��������
HANDLE hMutex = NULL;
// �������, �� ������� �������� ����������
HANDLE hEvent = NULL;
// ������� ���� ��� �� ��������
bool active = true;

void PrintHallQueue();

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE  hThreadArray[THREAD_COUNT];

	// ������� ������� ����� �����������, ������� ������ ������ ����� � ����
	hThreadArray[0] = CreateThread(NULL, 0, BarberThread, NULL, 0, NULL);
	// ����� ������� ����� ��������
	hThreadArray[1] = CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);

	// ������� ���������� ����� �������
	WaitForMultipleObjects(THREAD_COUNT, hThreadArray, TRUE, INFINITE);

	// ����������� ���������� �������
	for (int i = 0; i < THREAD_COUNT; i++) {
		CloseHandle(hThreadArray[i]);
	}
	CloseHandle(hMutex);

    return 0;
}

DWORD WINAPI BarberThread(LPVOID lpParam)
{
	while (active) {

		WaitForSingleObject(hMutex, INFINITE);

		cout << "����������:";
		if (qSize > 0) { // ���-�� ����� � ���� ��������

			// �������� ��� � ���� ��� �������
			cout << " ������ ������� [" << qHead << "] �" << WaitingHall[qHead].clientId << "..." << endl;
			WaitingHall[qHead].busy = false;
			WaitingHall[qHead].barber = false;
			WaitingHall[qHead].clientId = -1;
			qHead = (qHead + 1) % SLOT_COUNT;
			qSize--;

		}
		else { // ������ ���
			// �������� ������ ����� � ���� �������� � ����
			qHead = 0; // �������  "������" ������ ������� (������ ��� �������� �������)
			int qTail = qHead;
			WaitingHall[qTail].busy = true;
			WaitingHall[qHead].barber = true; // ����������, ��� ��� ����������
			WaitingHall[qTail].clientId = -1;
			qSize++;
			cout << " �������" << endl;
			SignalObjectAndWait(hMutex, hEvent, INFINITE, FALSE);
			// � ������ ����������� ������� ���� ����� ������������
			cout << " ����������: ���������, ������ �������[" << qHead << "] �" << WaitingHall[qHead].clientId << "..." << endl;
			// �������� ������������ ������� � ���� ��� �������
			WaitingHall[qHead].busy = false;
			WaitingHall[qHead].barber = false;
			WaitingHall[qHead].clientId = -1;
			qHead = (qHead + 1) % SLOT_COUNT;
			qSize--;
		}

		// ������������ ��� ��� ����������� ����� �������� � �������� �������
		ReleaseMutex(hMutex);
		Sleep(rand() % 4000 + 1000); // ������� ������� (1..5�)
		cout << "����������: �������� �������" << endl;

	} // while (active)
	cout << "Leave barber thread" << endl;

	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	for (int i = 0; i < CLIENT_COUNT; i++) {
		Sleep(rand() % 1800 + 200); // 0.2-2 ��� ����� ���������
		WaitForSingleObject(hMutex, INFINITE);

		cout << "������ �" << i << ": ";
		PrintHallQueue();
		// ������ ��������, ��������� ����������� �����������
		if (qSize < SLOT_COUNT) { 

			int qTail = 0;
			if (WaitingHall[qTail].barber) { // ��������� ������� �����������
				// ����� �����������, ������� ��� ���� ��� ����� (������������ ������� � ����)
				WaitingHall[qTail].busy = true;
				WaitingHall[qHead].barber = false;
				WaitingHall[qTail].clientId = i;
				cout << " ���������� ���� (��������)" << endl;
				ReleaseMutex(hMutex);
				SetEvent(hEvent); // ����� �����������
				continue;
			}
			else {
				qTail = (qHead + qSize) % SLOT_COUNT;
				// �������� ������� � ����, �.�. ���������� �����
				WaitingHall[qTail].busy = true;
				WaitingHall[qHead].barber = false;
				WaitingHall[qTail].clientId = i;
				cout << " ����� ����� [" << qTail << "]" << endl;
				qSize++;
			}

		}
		else {
			cout << " �� ������, �����" << endl; // ����� (�.�. ��� ������ ������) ������ ������
		}

		ReleaseMutex(hMutex);
	}
	// ����� �������� ���
	active = false; 
	SetEvent(hEvent);
	cout << "Leave client thread" << endl;
	return 0;
}

void PrintHallQueue() {
	int qTail = (qHead + qSize) % SLOT_COUNT;
	int index = qHead;
	cout << "hall (head = " << qHead << " tail " << qTail << " size " << qSize << "): {";
	for (int i = 0; i < SLOT_COUNT; i++)
		cout << " " << WaitingHall[i].clientId;
	cout << " }";
}
