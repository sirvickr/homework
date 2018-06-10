#ifndef menuH
#define menuH
//---------------------------------------------------------------------------
struct MENU;

#define MENU_ORIENT_HORZ 0
#define MENU_ORIENT_VERT 1

// Этапы частичной отрисовки меню
#define MENU_DRAW_WND 0 // Только отрисовка элементов меню
#define MENU_DRAW_SEL 1 // Отрисовка элементов меню и выделение текцщего
#define MENU_MSG_LOOP 2 // Полноценный запуск окна меню с циклом сообщений
// Максимальное число ячеек в одном пункте меню (минимум одна)
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
	// Указатель на родительское меню (не вызывающее, а то, которое перекрываем)
	struct MENU* parent;
	// Глобальные переменные, используемые в функциях меню
	HANDLE hStdOut;// = INVALID_HANDLE_VALUE; // дескриптор консольного окна
	///SMALL_RECT consolRect; // координаты углов консольного окна
	COORD curspos;//={0,1}; // координаты текстового курсора
	WORD workWindowAttributes;// = 158;// атрибуты рабочей области
	WORD inactiveItemAttributes;// = 31; // атрибуты цвета неактивного пункта меню
	WORD activeItemAttributes;// = 160; // атрибуты цвета активного пункта меню

	MENU_WND wnd;

	int hk_list[20];
	FUN hk_cb[20];
	int hk_count;

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

// Инициализация экземляра меню (0 - успех)
int menu_init(MENU* menu, MENU* parent, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
		int orient, const SMALL_RECT* prect, int border, char* headers[]);
// Добавить обработчик горячей клавиши
int menu_add_hotkey(MENU* menu, int code, FUN cb) {
	if(20 == menu->hk_count)
		return -1;
	menu->hk_list[menu->hk_count] = code;
	menu->hk_cb[menu->hk_count] = cb;
	return ++menu->hk_count;
}
// Очистка меню
void menu_clear(MENU* menu);
// Прорисовка меню
void menu_draw(MENU* menu, int loop);
//
void menu_active_color(MENU* menu, WORD attr);
void menu_inactive_color(MENU* menu, WORD attr);
// Перевод курсора в точку x, y
void gotoxy(MENU* menu, int x, int y);
// Выделить пункт меню
void itemMenu(MENU* menu, bool activate);
 // Запомнить положение курсора
void saveCursorPosition(MENU* menu);
// Очистка окна
void menu_cls(MENU* menu);
// Сохранить координаты курсора в переменную curspos
void showCursor(MENU* menu, bool visible); // скрыть/показать курсор

//---------------------------------------------------------------------------
#endif
