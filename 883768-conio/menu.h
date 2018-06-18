#ifndef menuH
#define menuH
//--------------------------------------------
#include "list.h"
//--------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

#define MAX_MENU_HDR 256 // ������������ ����� ���������

// ����� ��������� ��������� ����
#define MENU_FLAG_WND 0x1   // ��������� �������� ���� ����
#define MENU_FLAG_ITEMS 0x2 // ��������� ��������� ����
#define MENU_DRAW_SEL 0x4   // ��������� �������� ��������
#define MENU_NAVIGATOR 0x8  // ��������� �� ��������� (������� � Enter)
#define MENU_HOTKEYS 0x10   // ��������� ������� ������
// ����������� ������ ���� ���� � ������ ���������
#define MENU_FULL_FLAGS 0x1F
// ������������ ����� ����� � ����� ������ ���� (������� ����)
#define MAX_CELLS 50
// ����������� ����������� �������
#define MENU_CURR_FWD 0 // �����
#define MENU_CURR_REV 1 // �����

struct ITEM;

// ��������� �� ������� void f(void) - ��� ����� ��������� ������ ����
typedef int(*ExecuteHotketCB)(struct MENU*);
typedef int(*ExecuteCurrentCB)(struct MENU*, struct ITEM*);
typedef void(*CurrentChangedCB)(struct MENU*, int direction, int wrap);

// ��������� ����������� �������� ����
typedef struct {
	char* str[MAX_CELLS]; // ������������ ������ ���� (��������� �����)
	ExecuteCurrentCB cb; // �������, ����������� � ������ ����
} ITEM_DEF;

// ��������� ��� �������� �������� ����
typedef struct ITEM {
	int x, y; // ������� � ������ �������
	int index;
	char* str; // ������������ ������ ���� (��������� �����)
	ExecuteCurrentCB cb; // �������, ����������� � ������ ����
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
	ExecuteHotketCB cb; // ����������
} HOT_KEY;

// ��������� ��� ����� ����
typedef struct MENU {
	// ��������� �� ������������ ���� (�� ����������, � ��, ������� �����������)
	struct MENU* parent;
	// ���������� ����������, ������������ � �������� ����
	HANDLE hStdOut;// = INVALID_HANDLE_VALUE; // ���������� ����������� ����
	///SMALL_RECT consolRect; // ���������� ����� ����������� ����
//	COORD curspos;//={0,1}; // ���������� ���������� �������
	WORD workWindowAttributes;// = 158;// �������� ������� �������
	WORD inactiveItemAttributes;// = 31; // �������� ����� ����������� ������ ����
	WORD activeItemAttributes;// = 160; // �������� ����� ��������� ������ ����

	MENU_WND wnd;

	int cb_retcode;
	int last_key;
	LIST1 hk_list;

	CurrentChangedCB changed_cb;

	char* hdr;
	LIST1 items;

	int cell_count; // ���������� �����
	int cell_width; // ������ ������
	int cell_start[MAX_CELLS]; // ������� ������ ����� ������ ������ ������� ������
	int item_width; // ���������� �������� � ������ ������� ������
	int orient; // ���������� (MENU_ORIENT_HORZ/MENU_ORIENT_VERT)
	int has_headers; // 0 - ��� ���������, 1 - ���� ���������
	int has_border; // 0 - ��� �����, 1 - ���� �����
	// ������ ������ � ������ �������� (��� ���������� ���������)
	int border_bottom_index;
	// ������� ����� � ������, � ������ ������� ����� � ���������
	int left_pad;
	int top_pad;
	// ���������������� �����
	int user_tag;
} MENU;

// ������������� ����� ���������� ����
int menu_init(MENU* menu, MENU* parent, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
		int orient, const SMALL_RECT* prect, int border, char* headers[]);
// �������� ���������� ������� �������
int menu_add_hotkey(MENU* menu, int code, ExecuteHotketCB cb);

// ������� ����� ���������� ����
void menu_clear(MENU* menu);
// ���������� ���� � �����
void menu_fill_wnd(MENU* menu, int items_count);
// ���������� ����
int menu_draw(MENU* menu, int flags);
// ��������� �������� ��������� ��������� ������
void menu_active_color(MENU* menu, WORD attr);
// ��������� �������� ��������� ����������� ������
void menu_inactive_color(MENU* menu, WORD attr);
// �������� ���������� ����� ����
void menu_prev(MENU* menu);
// �������� ��������� ����� ����
void menu_next(MENU* menu);
// ������� ������� � ����� x, y
void menu_gotoxy(MENU* menu, int x, int y);
// �������� ����� ����
void itemMenu(MENU* menu, int activate);
 // ��������� ��������� �������
void saveCursorPosition(MENU* menu);
// ������� ����
void menu_cls(MENU* menu);
// ��������� ���������� ������� � ���������� curspos
void showCursor(MENU* menu, int visible); // ������/�������� ������
// ������� ������� �������
void menu_del_curr(MENU* menu);

//--------------------------------------------
#endif
