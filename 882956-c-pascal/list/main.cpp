#include <iostream>
#include "list.h"

using namespace std;

void Task_4_1()
{
	int size = 10;
	List *head = Creat_random_list(size);
	List *p = head, *next;
	int first, last;
	if (!p) {
		cout << " List empty!" << endl;
		return;
	}
	Print_list(head);
	first = p->value;
	while (true)
	{
		next = p->next;
		if(!next) {
			last = p->value;
			break;
		}
		p = next;
	}
	p = head;
	int count = 0;
	while (p)
	{
		if (p->value == first || p->value == last) {
			count++;
		}
		p = p->next;
	}
	cout << "count = " << count << endl;
}

void Task_4_2()
{
	List *L1 = Creat_random_list(5);
	List *L2 = Creat_random_list(3);
	Print_list(L1);
	Print_list(L2);
	List *L3 = nullptr;
	int length = Concat_lists(L1, L2, &L3);
	cout << "length = " << length << endl;
	Print_list(L3);
}

int main()
{
	int i, k, n, num;
	int choice = 0;
//	char *ss[] = { "\n 0-Print list"," 1-Insert SORT"," 2-Insert first"," 3-Add last"," 4-Insert Number"," 5-Delete first"," 6-Delete last"," 7-Delete value"," 8-Delete number"," 9-Task 4.1 (Count)"," 10-Task 4.2 (Concat)"," 11-EXIT" };
	char *ss[] = { "\n 0 - EXIT"," 1 - Task 4.1 (Count)"," 2 - Task 4.2 (Concat)" };
	List *head = nullptr;
	k = sizeof(ss) / sizeof(ss[0]);
	for (;;)
	{
		for (i = 0; i < k; i++) puts(ss[i]);
		cin >> choice;
#if 1
		switch (choice) {
		case 0:
			return 0;
		case 1:
			Task_4_1();
			break;
		case 2:
			Task_4_2();
			break;
		}
#else
		switch (choice) {
		case 0: 
			Print_list(head); 
			break;
		case 1: 
			printf("n = "); scanf("%d", &num);
			head = Ins_Sort(num, head); break;
		case 2: printf("n = ");
			scanf("%d", &num);
			head = Ins_first(num, head); break;
		case 3: printf("n = "); scanf("%d", &num);
			head = Add_last(num, head); break;
		case 4: printf("Number= "); scanf("%d", &num);
			printf("n= "); scanf("%d", &n);
			head = Ins_Number(num, n, head); break;
		case 5: head = Del_first(num, head); break;
		case 6: head = Del_last(num, head); break;
		case 7: printf("value = "); scanf("%d", &num);
			head = Del_value(num, head); break;
		case 8: printf("Number = "); scanf("%d", &num);
			head = Del_Number(num, head); break;
		case 9: 
			Task_4_1();
			break;
		case 10:
			Task_4_2();
			break;
		case 11:
			return 0;
		}
#endif
	}

	return 0;
}
