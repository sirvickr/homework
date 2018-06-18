#ifndef menuH
#define menuH
//--------------------------------------------
#include "list.h"
//--------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

#define MAX_MENU_HDR 256 // максимальная длина заголовка

// Этапы частичной отрисовки меню
#define MENU_FLAG_WND 0x1   // отрисовка фонового окна меню
#define MENU_FLAG_ITEMS 0x2 // отрисовка элементов меню
#define MENU_DRAW_SEL 0x4   // выделение текущего элемента
#define MENU_NAVIGATOR 0x8  // навигация по умолчанию (стрелки и Enter)
#define MENU_HOTKEYS 0x10   // обработка горячих клавиш
// полноценный запуск окна меню с циклом сообщений
#define MENU_FULL_FLAGS 0x1F
// Максимальное число ячеек в одном пункте меню (минимум одна)
#define MAX_CELLS 50
// Направление перемещение курсора
#define MENU_CURR_FWD 0 // вперёд
#define MENU_CURR_REV 1 // назад

struct ITEM;

// Указатели на функции void f(void) - они будут выполнять пункты меню
typedef int(*ExecuteHotketCB)(struct MENU*);
typedef int(*ExecuteCurrentCB)(struct MENU*, struct ITEM*);
typedef void(*CurrentChangedCB)(struct MENU*, int direction, int wrap);

// Структура определения элемента меню
typedef struct {
	char* str[MAX_CELLS]; // Наименование пункта меню (несколько ячеек)
	ExecuteCurrentCB cb; // Функция, привязанная к пункту меню
} ITEM_DEF;

// Структура для хранения элемента меню
typedef struct ITEM {
	int x, y; // Столбец и строка консоли
	int index;
	char* str; // Наименование пункта меню (несколько ячеек)
	ExecuteCurrentCB cb; // Функция, привязанная к пункту меню
} ITEM;

// Область окна меню
typedef struct {
	SMALL_RECT rect; // координаты углов консольного окна
	int M, N; // размер матрицы символов
	char** m; // матрица символов
} MENU_WND;

// Горячая клавиша
typedef struct {
	int code;
	ExecuteHotketCB cb; // обработчик
} HOT_KEY;

// Структура для всего меню
typedef struct MENU {
	// Указатель на родительское меню (не вызывающее, а то, которое перекрываем)
	struct MENU* parent;
	// Глобальные переменные, используемые в функциях меню
	HANDLE hStdOut;// = INVALID_HANDLE_VALUE; // дескриптор консольного окна
	///SMALL_RECT consolRect; // координаты углов консольного окна
//	COORD curspos;//={0,1}; // координаты текстового курсора
	WORD workWindowAttributes;// = 158;// атрибуты рабочей области
	WORD inactiveItemAttributes;// = 31; // атрибуты цвета неактивного пункта меню
	WORD activeItemAttributes;// = 160; // атрибуты цвета активного пункта меню

	MENU_WND wnd;

	int cb_retcode;
	int last_key;
	LIST1 hk_list;

	CurrentChangedCB changed_cb;

	char* hdr;
	LIST1 items;

	int cell_count; // количество ячеек
	int cell_width; // ширина ячейки
	int cell_start[MAX_CELLS]; // индексы начала ячеек внутри строки каждого пункта
	int item_width; // количество символов в строке каждого пункта
	int orient; // ориентация (MENU_ORIENT_HORZ/MENU_ORIENT_VERT)
	int has_headers; // 0 - нет заголовка, 1 - есть заголовок
	int has_border; // 0 - нет рамки, 1 - есть рамка
	// индекс строки с нижней границей (для правильной отрисовки)
	int border_bottom_index;
	// отступы слева и сверху, с учётом наличия рамки и заголовка
	int left_pad;
	int top_pad;
	// пользовательская метка
	int user_tag;
} MENU;

// Инициализация полей экземпляра меню
int menu_init(MENU* menu, MENU* parent, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
		int orient, const SMALL_RECT* prect, int border, char* headers[]);
// Добавить обработчик горячей клавиши
int menu_add_hotkey(MENU* menu, int code, ExecuteHotketCB cb);

// Очистка полей экземпляра меню
void menu_clear(MENU* menu);
// Прорисовка фона и рамки
void menu_fill_wnd(MENU* menu, int items_count);
// Прорисовка меню
int menu_draw(MENU* menu, int flags);
// Установка цветовых атрибутов активного пункта
void menu_active_color(MENU* menu, WORD attr);
// Установка цветовых атрибутов неактивного пункта
void menu_inactive_color(MENU* menu, WORD attr);
// Выделить предыдущий пункт меню
void menu_prev(MENU* menu);
// Выделить следующий пункт меню
void menu_next(MENU* menu);
// Перевод курсора в точку x, y
void menu_gotoxy(MENU* menu, int x, int y);
// Выделить пункт меню
void itemMenu(MENU* menu, int activate);
 // Запомнить положение курсора
void saveCursorPosition(MENU* menu);
// Очистка окна
void menu_cls(MENU* menu);
// Сохранить координаты курсора в переменную curspos
void showCursor(MENU* menu, int visible); // скрыть/показать курсор
// Удалить текущий элемент
void menu_del_curr(MENU* menu);

//--------------------------------------------
#endif
