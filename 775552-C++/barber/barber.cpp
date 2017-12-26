// barber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
// один поток парикмахера и второй - посетителей
static const int THREAD_COUNT = 2;
// количество мест в зале ожидания
static const int SLOT_COUNT = 3;
// количество итераций (т.е. обработанных клиентов за день)
static const int CLIENT_COUNT = 10;

DWORD WINAPI BarberThread(LPVOID);
DWORD WINAPI ClientThread(LPVOID);

struct WaitingSlot {
	bool busy = false;
	bool barber = false;
	int clientId = -1;
};

// Зал ожидания (очередь кресел)
WaitingSlot WaitingHall[SLOT_COUNT];
int qHead = 0;
int qSize = 0;
// Мутекс зала ожидания
HANDLE hMutex = NULL;
// Событие, на котором засыпает парикмахер
HANDLE hEvent = NULL;
// рабочий день еще не закончен
bool active = true;

void PrintHallQueue();

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE  hThreadArray[THREAD_COUNT];

	// сначала создаем поток парикмахера, который займет пустое место в зале
	hThreadArray[0] = CreateThread(NULL, 0, BarberThread, NULL, 0, NULL);
	// затем создаем поток клиентов
	hThreadArray[1] = CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);

	// Ожидаем завершения обоих потоков
	WaitForMultipleObjects(THREAD_COUNT, hThreadArray, TRUE, INFINITE);

	// Освобождаем выделенные ресурсы
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

		cout << "Парикмахер:";
		if (qSize > 0) { // кто-то сидит в заде ожидания

			// забираем его к себе для стрижки
			cout << " стригу клиента [" << qHead << "] №" << WaitingHall[qHead].clientId << "..." << endl;
			WaitingHall[qHead].busy = false;
			WaitingHall[qHead].barber = false;
			WaitingHall[qHead].clientId = -1;
			qHead = (qHead + 1) % SLOT_COUNT;
			qSize--;

		}
		else { // никого нет
			// занимает первое место в зале ожидания и спит
			qHead = 0; // обнулим  "голову" пустой очереди (просто для удобства отладки)
			int qTail = qHead;
			WaitingHall[qTail].busy = true;
			WaitingHall[qHead].barber = true; // обозначаем, что это парикмахер
			WaitingHall[qTail].clientId = -1;
			qSize++;
			cout << " засыпаю" << endl;
			SignalObjectAndWait(hMutex, hEvent, INFINITE, FALSE);
			// в момент пробуждения семафор зала снова заблокирован
			cout << " Парикмахер: проснулся, стригу клиента[" << qHead << "] №" << WaitingHall[qHead].clientId << "..." << endl;
			// забираем разбудившего клиента к себе для стрижки
			WaitingHall[qHead].busy = false;
			WaitingHall[qHead].barber = false;
			WaitingHall[qHead].clientId = -1;
			qHead = (qHead + 1) % SLOT_COUNT;
			qSize--;
		}

		// разблокируем зал для поступления новых клиентов и начинаем стрижку
		ReleaseMutex(hMutex);
		Sleep(rand() % 4000 + 1000); // процесс стрижки (1..5с)
		cout << "Парикмахер: закончил стрижку" << endl;

	} // while (active)
	cout << "Leave barber thread" << endl;

	return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	for (int i = 0; i < CLIENT_COUNT; i++) {
		Sleep(rand() % 1800 + 200); // 0.2-2 сек между клиентами
		WaitForSingleObject(hMutex, INFINITE);

		cout << "Клиент №" << i << ": ";
		PrintHallQueue();
		// клиент приходит, проверяет доступность парикмахера
		if (qSize < SLOT_COUNT) { 

			int qTail = 0;
			if (WaitingHall[qTail].barber) { // обнаружил спящего парикмахера
				// будит парикмахера, занимая при этом его место (единственное занятое в зале)
				WaitingHall[qTail].busy = true;
				WaitingHall[qHead].barber = false;
				WaitingHall[qTail].clientId = i;
				cout << " парикмахер спит (свободен)" << endl;
				ReleaseMutex(hMutex);
				SetEvent(hEvent); // будим парикмахера
				continue;
			}
			else {
				qTail = (qHead + qSize) % SLOT_COUNT;
				// занимает очередь в зале, т.к. парикмахер занят
				WaitingHall[qTail].busy = true;
				WaitingHall[qHead].barber = false;
				WaitingHall[qTail].clientId = i;
				cout << " занял место [" << qTail << "]" << endl;
				qSize++;
			}

		}
		else {
			cout << " всё занято, ухожу" << endl; // иначе (т.е. все кресла заняты) просто уходит
		}

		ReleaseMutex(hMutex);
	}
	// конец рабочего дня
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
