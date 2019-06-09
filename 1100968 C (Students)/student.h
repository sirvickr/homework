#ifndef __STUDENT_H__
#define __STUDENT_H__

#define MAX_NAME_LENGTH 100

// ���������� ������ ��� �������� ���������� � ���������

// ������� ����������� ������
typedef struct Student {
	struct Student* next;
	struct Student* prev;
	char name[ MAX_NAME_LENGTH ];
	unsigned char age;
	float score;
} Student;

// ��������� ����������� ������
typedef struct Students
{
	// ��������� �� ������ � ��������� ������� ������
    struct Student *head;
    struct Student *tail;
} Students;

// ���������� � ������ ������ ��������
void Students_Add_Item( Students* list, Student* item );
// ���������� � ������ �� ���������� ����������
void Students_Add_Data( Students* list, const char* name, unsigned char age, float score );
// �������������� �������� ������ �� ���������
void Students_Edit( Student* item, const char* name, unsigned char age, float score );
// �������������� �������� ������ �� �������
void Students_Edit_Index( Students* list, int index, const char* name, unsigned char age, float score );
// �������� �� ������ �� ��������
void Students_Del( Students* list, Student* item );
// �������� �� ������ �� �������
void Students_Del_Index( Students* list, int index );
// ������ �������� ������
void Students_Print( Students* list );
// �������� ����� ������
void Students_Destroy( Students* list );

#endif // __STUDENT_H__

