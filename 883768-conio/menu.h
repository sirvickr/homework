#ifndef menuH
#define menuH
//---------------------------------------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

#define MAX_CELLS 50

// Указатели на функции void f(void) - они будут выполнять пункты меню
typedef int(*FUN) (MENU*);

// Структура для элемента меню
typedef struct {
	char* str[MAX_CELLS]; // Наименование пункта меню (несколько ячеек)
	FUN cb; // Функция, привязанная к пункту меню
} ITEM_DEF;

// Структура для элемента меню
typedef struct {
	int x, y; // Столбец и строка консоли
	char* str; // Наименование пункта меню (несколько ячеек)
	FUN cb; // Функция, привязанная к пункту меню
} ITEM;

// Область окна меню
typedef struct {
	SMALL_RECT rect; // координаты углов консольного окна
	int M, N; // размер матрицы символов
	char** m; // матрица символов
} MENU_WND;

// Структура для всего меню
typedef struct MENU {
	// Глобальные переменные, используемые в функциях меню
	HANDLE hStdOut;// = INVALID_HANDLE_VALUE; // дескриптор консольного окна
	///SMALL_RECT consolRect; // координаты углов консольного окна
	COORD curspos;//={0,1}; // координаты текстового курсора
	WORD workWindowAttributes;// = 158;// атрибуты рабочей области
	WORD inactiveItemAttributes;// = 31; // атрибуты цвета неактивного пункта меню
	WORD activeItemAttributes;// = 160; // атрибуты цвета активного пункта меню

	MENU_WND wnd;

	char* hdr;
	ITEM* items;
	int item_count;
	int cell_count;
	int item_width;
	int orient;
	int border; // 0 - нет рамки, 1 - есть рамка
	int left_pad;
	int top_pad;
	int current; // текущий пункт меню
} MENU;

// инициализация экземляра меню (0 - успех)
int menu_init(MENU* menu, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
		int orient, const SMALL_RECT* prect, int border, const char* headers[]);
// инициализация очитка меню
void menu_clear(MENU* menu);
//
void menu_draw(MENU* menu, int loop);
//
void menu_active_color(MENU* menu, WORD attr);
void menu_inactive_color(MENU* menu, WORD attr);
// перевод курсока в точку x, y
void gotoxy(MENU* menu, int x, int y);
// выделить пункт меню
void itemMenu(MENU* menu, bool activate);
 // запомнить положение курсора
void saveCursorPosition(MENU* menu);
// очистка окна
void menu_cls(MENU* menu);
// в глобальную переменную curspos
void showCursor(MENU* menu, bool visible); // скрыть/показать курсор

//---------------------------------------------------------------------------
#endif
