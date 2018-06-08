#ifndef dictH
#define dictH
//---------------------------------------------------------------------------
struct MENU;
// Описание функций, которые подключаются к пунктам меню
int Edit(MENU* menu);
int Search(MENU* menu);
int Sort(MENU* menu);
int Save(MENU* menu);
int Help(MENU* menu);
int Exit(MENU* menu);

//---------------------------------------------------------------------------
#endif
