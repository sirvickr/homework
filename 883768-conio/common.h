#ifndef __MODULE_HEADER__
#define __MODULE_HEADER__

#ifdef __DICTIONARY_DB_CODES_H__
#define KEY_ARROW_UP 72
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_DOWN 80

#define KEY_TAB 9
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8
#define KEY_SPACE 32
#define KEY_DEL 83
#define KEY_HOME 71
#define KEY_END 79

#define KEY_F1  59
#define KEY_F2  60
#define KEY_F3  61
#define KEY_F4  62
#define KEY_F5  63
#define KEY_F6  64
#define KEY_F7  65
#define KEY_F8  66
#define KEY_F9  67
#define KEY_F10 68
#define KEY_F11 133
#define KEY_F12 134

#define CHAR_BORDER_HORZ 205
#define CHAR_BORDER_VERT 186
#define CHAR_BORDER_LEFT_TOP 201
#define CHAR_BORDER_LEFT_BOTTOM 200
#define CHAR_BORDER_RIGHT_TOP 187
#define CHAR_BORDER_RIGHT_BOTTOM 188
#define CHAR_BORDER_LEFT_JOINT 204
#define CHAR_BORDER_RIGHT_JOINT 185
#define CHAR_BORDER_TOP_JOINT 203
#define CHAR_BORDER_BOTTOM_JOINT 202
#define CHAR_BORDER_CROSS_JOINT 206
#endif /*__DICTIONARY_DB_CODES_H__*/

#ifdef __LIST1_H__

struct LIST1_ITEM;

// структура элемента односвязного списка
typedef struct LIST1_ITEM {
	struct LIST1_ITEM* next;
	void* data;
} LIST1_ITEM;

// односвязный список

typedef void (*LIST_ITEM_FREE)(void* item);
typedef void (*LIST_ITEM_UPD)(void* item);
// если вернёт 0, обработка прекратится на этой итерации
typedef int (*LIST_ITEM_PROC)(void* item, int index, void* param);
typedef int (*LIST_ITEM_FIND)(void* item, void* param);
typedef int (*LIST_ITEM_COMP)(void* item1, void* item2, void* param);
// структура односвязного списка
typedef struct LIST1 {
	struct LIST1_ITEM *head, *tail, *curr;
	LIST_ITEM_FREE item_free;
	int count;
} LIST1;

//
LIST1* list1_new();
//
int list1_delete(LIST1* list);
//
int list1_init(LIST1* list);
//
int list1_clear(LIST1* list);
//
int list1_push_front(LIST1* list, void* data);
//
int list1_push_back(LIST1* list, void* item);
//
int list1_for_each(LIST1* list, LIST_ITEM_PROC find, void* param);
// Прокрутить текущий указатель вперёд
void* list1_curr_fwd(LIST1* list, int wrap);
// Прокрутить текущий указатель назад
void* list1_curr_rev(LIST1* list, int wrap);
//
void* list1_front(LIST1* list);
//
void* list1_back(LIST1* list);
//
void* list1_curr(LIST1* list);
// Удалить элемент списка
// возвращает текущий элемент (возможно, новый)
void* list1_erase(LIST1* list, LIST_ITEM_FIND cb, void* param);
// Удалить текущий элемент списка
// возвращает новый текущий элемент
void* list1_erase_current(LIST1* list);
//
int list1_get_current_index(LIST1* list);
//
int list1_set_current_index(LIST1* list, int index);
// поиск элемента (установит текущий и вернёт его порядковый номер)
int list1_search(LIST1* list, LIST_ITEM_FIND check, void* param);
// сортировка списка
void list1_sort(LIST1* list, LIST_ITEM_COMP compare, void* param);

#endif /*__LIST1_H__*/

#ifdef __DICT_H__
#define MAX_DICT_STR_LEN 32
#define DICT_FLD_CNT 3
// структура записи словаря
typedef struct DICT_ENTRY {
	char* field[DICT_FLD_CNT];
} DICT_ENTRY;
//--------------------------------------------
struct MENU;
//
DICT_ENTRY* dict_entry_new(const char* word_eng, const char* word_part, const char* word_rus);
//
int dict_load(const char* file_name);
//
int dict_save(const char* file_name);
//
void dict_clear();
//
void dict_entry_clear(DICT_ENTRY* entry);
//--------------------------------------------
extern const char* dict_file_name;
extern LIST1 dict;
#endif /*__DICT_H__*/

#ifdef __MENU_H__
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
#endif /*__MENU_H__*/

#ifdef __DICTIONARY_INPUT_DLG_H__
#define MAX_TITLE 50

#define TITLE   0
#define BUFFER  1
#define COLUMNS 2

typedef struct InputBox {
	HANDLE handle;
	SMALL_RECT rect;
	CHAR_INFO* bak;
	CHAR_INFO* wnd;
	COORD size; // width, height
	WORD edit_attr;
	int max_width;
	// буфер ввода
	//char* buffer;
	// 2-мерный массив строк: 1я колонка - надписи
	// 2я колонка - редактируемые поля (можно передавать начальные значения)
	char*** contents;
	int row_count; // количество полей (пар надпись/значение)
	int row; // текущее редактируемое поле
} InputBox;

int box_init(InputBox* box, HANDLE handle, SMALL_RECT rect, char*** contents, int row_count);
void box_clear(InputBox* box);
int box_save(InputBox* box);
int box_draw(InputBox* box);
int box_restore(InputBox* box);
#endif /*__DICTIONARY_INPUT_DLG_H__*/

#endif /*__MODULE_HEADER__*/
