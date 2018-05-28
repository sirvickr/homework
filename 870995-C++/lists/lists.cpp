// 5. В составе программы описать функцию, которая находит наибольший элемент списка и его порядковый номер

#include <iostream>
#include <time.h>

using namespace std;

// элемент односвязного списка с одним информационным полем
template<class Data>
struct Node {
	Data data;
	Node* next;
};

// тип информационных данных - int
typedef int Data;
// псевдонимы для элемента списка и указателя на него
typedef Node<Data> IntNode, *IntNodePtr;

// создание односвязного списка (добавляем элементы в начало, как в стек),
// заполненного случайными числами от -9 до 9
IntNodePtr createList(size_t n) {
	// указатель на начало списка
	IntNodePtr head = nullptr;
	// указатель текущий элемент
	IntNodePtr curr = nullptr;
	// создаём n элементов в списке
	for (size_t i = 0; i < n; i++) {
		curr = new IntNode;
		curr->data = (rand() % 19) - 9;
		curr->next = head;
		head = curr;
	}
	return curr;
}

// вывод односвязного списка на экран
void printList(IntNodePtr head) {
	for (IntNodePtr curr = head; curr; curr = curr->next)
		cout << " " << curr->data;
	cout << endl;
}

// поиск максимального элемента и его индекса
IntNodePtr maxListItem(IntNodePtr list, size_t* pMaxIndex = nullptr) {
	if (!list) // с нулевым указателем делать нечего, выходим
		return nullptr;
	// текущий элемент
	IntNodePtr curr = list; // инициализируется первым элементом списка
	// текущий индекс
	size_t index = 1; // инициализируем 1, т.к. цикл начнётся со второго элемента
	// максимальный элемент
	IntNodePtr maxItem = curr;
	// максимальный индекс
	size_t maxIndex = 0;
	// начинаем цикл со сравнения второго элемента с первым
	for (curr = curr->next; curr; curr = curr->next) {
		// если очередной элемент больше макисмального, сохранить как максимальный
		if (curr->data > maxItem->data) {
			maxItem = curr;
			maxIndex = index;
		}
		index++;
	}
	// возврат результатов
	if (pMaxIndex)
		*pMaxIndex = maxIndex;
	return maxItem;
}

int main()
{
	// инициализируем генератор случайных чисел текущим временем
	srand(time(NULL));
	// создаём целочисленный список, заполненный случайными значениями
	IntNodePtr list = createList(10);
	// в случае неудачи выходим из программы
	if (!list) {
		cerr << "no list available" << endl;
		return 0;
	}
	// распечатываем созданный список на экране
	printList(list);
	// ищем максимальный элемент и его индекс
	size_t maxIndex = 0;
	IntNodePtr maxItem = maxListItem(list, &maxIndex);
	// выводим результат
	if(maxItem) 
		cout << "max: index " << maxIndex << " data " << maxItem->data << endl;

	return 0;
}
