// 5. � ������� ��������� ������� �������, ������� ������� ���������� ������� ������ � ��� ���������� �����

#include <iostream>
#include <time.h>

using namespace std;

// ������� ������������ ������ � ����� �������������� �����
template<class Data>
struct Node {
	Data data;
	Node* next;
};

// ��� �������������� ������ - int
typedef int Data;
// ���������� ��� �������� ������ � ��������� �� ����
typedef Node<Data> IntNode, *IntNodePtr;

// �������� ������������ ������ (��������� �������� � ������, ��� � ����),
// ������������ ���������� ������� �� -9 �� 9
IntNodePtr createList(size_t n) {
	// ��������� �� ������ ������
	IntNodePtr head = nullptr;
	// ��������� ������� �������
	IntNodePtr curr = nullptr;
	// ������ n ��������� � ������
	for (size_t i = 0; i < n; i++) {
		curr = new IntNode;
		curr->data = (rand() % 19) - 9;
		curr->next = head;
		head = curr;
	}
	return curr;
}

// ����� ������������ ������ �� �����
void printList(IntNodePtr head) {
	for (IntNodePtr curr = head; curr; curr = curr->next)
		cout << " " << curr->data;
	cout << endl;
}

// ����� ������������� �������� � ��� �������
IntNodePtr maxListItem(IntNodePtr list, size_t* pMaxIndex = nullptr) {
	if (!list) // � ������� ���������� ������ ������, �������
		return nullptr;
	// ������� �������
	IntNodePtr curr = list; // ���������������� ������ ��������� ������
	// ������� ������
	size_t index = 1; // �������������� 1, �.�. ���� ������� �� ������� ��������
	// ������������ �������
	IntNodePtr maxItem = curr;
	// ������������ ������
	size_t maxIndex = 0;
	// �������� ���� �� ��������� ������� �������� � ������
	for (curr = curr->next; curr; curr = curr->next) {
		// ���� ��������� ������� ������ �������������, ��������� ��� ������������
		if (curr->data > maxItem->data) {
			maxItem = curr;
			maxIndex = index;
		}
		index++;
	}
	// ������� �����������
	if (pMaxIndex)
		*pMaxIndex = maxIndex;
	return maxItem;
}

int main()
{
	// �������������� ��������� ��������� ����� ������� ��������
	srand(time(NULL));
	// ������ ������������� ������, ����������� ���������� ����������
	IntNodePtr list = createList(10);
	// � ������ ������� ������� �� ���������
	if (!list) {
		cerr << "no list available" << endl;
		return 0;
	}
	// ������������� ��������� ������ �� ������
	printList(list);
	// ���� ������������ ������� � ��� ������
	size_t maxIndex = 0;
	IntNodePtr maxItem = maxListItem(list, &maxIndex);
	// ������� ���������
	if(maxItem) 
		cout << "max: index " << maxIndex << " data " << maxItem->data << endl;

	return 0;
}
