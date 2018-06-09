#ifndef menuH
#define menuH
//---------------------------------------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

// ��������� �� ������� void f(void) - ��� ����� ��������� ������ ����
typedef int(*FUN) (MENU*);
// ��������� ��� �������� ����
typedef struct {
	int x, y; // ������� � ������ �������
	char *str; // ������������ ������ ����
	FUN cb; // �������, ����������� � ������ ����
} ITEM;

// ������� ���� ����
typedef struct {
	SMALL_RECT rect; // ���������� ����� ����������� ����
	int M, N; // ������ ������� ��������
	char** m; // ������� ��������
} MENU_WND;

// ��������� ��� ����� ����
typedef struct MENU {
	// ���������� ����������, ������������ � �������� ����
	HANDLE hStdOut;// = INVALID_HANDLE_VALUE; // ���������� ����������� ����
	///SMALL_RECT consolRect; // ���������� ����� ����������� ����
	COORD curspos;//={0,1}; // ���������� ���������� �������
	WORD workWindowAttributes;// = 158;// �������� ������� �������
	WORD inactiveItemAttributes;// = 31; // �������� ����� ����������� ������ ����
	WORD activeItemAttributes;// = 160; // �������� ����� ��������� ������ ����

	MENU_WND wnd;

	ITEM* items;
	int item_count;
	int item_width;
	int orient;
	int current;
} MENU;

// ������������� ��������� ���� (0 - �����)
int InitMenu(MENU* menu, ITEM* items, int item_count, int orient, const SMALL_RECT*);
// ������������� ������ ����
void ClearMenu(MENU* menu);

// �������� ����
void DrawMenu(MENU* menu, int loop);
void gotoxy(MENU* menu, int x, int y); // ������� ������� � ����� x, y
void itemMenu(MENU* menu, bool activate); // �������� ����� ����
void menu_cls(MENU* menu); // ������� ����
void saveCursorPosition(MENU* menu); // ��������� ��������� �������
// � ���������� ���������� curspos
void showCursor(MENU* menu, bool visible); // ������/�������� ������

//---------------------------------------------------------------------------
#endif
