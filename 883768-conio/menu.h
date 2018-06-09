#ifndef menuH
#define menuH
//---------------------------------------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

// Указатели на функции void f(void) - они будут выполнять пункты меню
typedef int(*FUN) (MENU*);
// Структура для элемента меню
typedef struct {
	int x, y; // Столбец и строка консоли
	char *str; // Наименование пункта меню
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

	ITEM* items;
	int item_count;
	int item_width;
	int orient;
	int current;
} MENU;

// инициализация экземляра меню (0 - успех)
int InitMenu(MENU* menu, ITEM* items, int item_count, int orient, const SMALL_RECT*);
// инициализация очитка меню
void ClearMenu(MENU* menu);

// создание меню
void DrawMenu(MENU* menu, int loop);
void gotoxy(MENU* menu, int x, int y); // перевод курсока в точку x, y
void itemMenu(MENU* menu, bool activate); // выделить пункт меню
void menu_cls(MENU* menu); // очистка окна
void saveCursorPosition(MENU* menu); // запомнить полодение курсора
// в глобальную переменную curspos
void showCursor(MENU* menu, bool visible); // скрыть/показать курсор

//---------------------------------------------------------------------------
#endif
