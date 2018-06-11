#ifndef menuH
#define menuH
//---------------------------------------------------------------------------
extern "C" {
#include "list.h"
}
//---------------------------------------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

// ����� ��������� ��������� ����
#define MENU_DRAW_WND 0 // ������ ��������� ��������� ����
#define MENU_DRAW_SEL 1 // ��������� ��������� ���� � ��������� ��������
#define MENU_MSG_LOOP 2 // ����������� ������ ���� ���� � ������ ���������
// ������������ ����� ����� � ����� ������ ���� (������� ����)
#define MAX_CELLS 50

// ��������� �� ������� void f(void) - ��� ����� ��������� ������ ����
typedef int(*FUN) (MENU*);

// ��������� ����������� �������� ����
typedef struct {
	char* str[MAX_CELLS]; // ������������ ������ ���� (��������� �����)
	FUN cb; // �������, ����������� � ������ ����
} ITEM_DEF;

// ��������� ��� �������� �������� ����
typedef struct {
	int x, y; // ������� � ������ �������
	char* str; // ������������ ������ ���� (��������� �����)
	FUN cb; // �������, ����������� � ������ ����
} ITEM;

// ������� ���� ����
typedef struct {
	SMALL_RECT rect; // ���������� ����� ����������� ����
	int M, N; // ������ ������� ��������
	char** m; // ������� ��������
} MENU_WND;

// ������� �������
typedef struct {
	int code;
	FUN cb; // ����������
} HOT_KEY;

// ��������� ��� ����� ����
typedef struct MENU {
	// ��������� �� ������������ ���� (�� ����������, � ��, ������� �����������)
	struct MENU* parent;
	// ���������� ����������, ������������ � �������� ����
	HANDLE hStdOut;// = INVALID_HANDLE_VALUE; // ���������� ����������� ����
	///SMALL_RECT consolRect; // ���������� ����� ����������� ����
	COORD curspos;//={0,1}; // ���������� ���������� �������
	WORD workWindowAttributes;// = 158;// �������� ������� �������
	WORD inactiveItemAttributes;// = 31; // �������� ����� ����������� ������ ����
	WORD activeItemAttributes;// = 160; // �������� ����� ��������� ������ ����

	MENU_WND wnd;

	int last_key;
	LIST1 hk_list;

	char* hdr;
	ITEM* items;
	int item_count;
	int cell_count;
	int item_width;
	int orient;
	int border; // 0 - ��� �����, 1 - ���� �����
	int left_pad;
	int top_pad;
	int current; // ������� ����� ����
} MENU;

// ������������� ����� ���������� ����
int menu_init(MENU* menu, MENU* parent, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
		int orient, const SMALL_RECT* prect, int border, char* headers[]);
// �������� ���������� ������� �������
int menu_add_hotkey(MENU* menu, int code, FUN cb);

// ������� ����� ���������� ����
void menu_clear(MENU* menu);
// ���������� ����
void menu_draw(MENU* menu, int loop);
// ��������� �������� ��������� ��������� ������
void menu_active_color(MENU* menu, WORD attr);
// ��������� �������� ��������� ����������� ������
void menu_inactive_color(MENU* menu, WORD attr);
// �������� ���������� ����� ����
void menu_prev(MENU* menu);
// �������� ��������� ����� ����
void menu_next(MENU* menu);
// ������� ������� � ����� x, y
void gotoxy(MENU* menu, int x, int y);
// �������� ����� ����
void itemMenu(MENU* menu, bool activate);
 // ��������� ��������� �������
void saveCursorPosition(MENU* menu);
// ������� ����
void menu_cls(MENU* menu);
// ��������� ���������� ������� � ���������� curspos
void showCursor(MENU* menu, bool visible); // ������/�������� ������

//---------------------------------------------------------------------------
#endif
