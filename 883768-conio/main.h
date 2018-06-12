#ifndef __DICTIONARY_MAIN_H__
#define __DICTIONARY_MAIN_H__

#include "menu.h"

// Описание функций, которые подключаются к пунктам меню
int Edit(MENU* menu, ITEM*);
int Delete(MENU* menu);
int Search(MENU* menu, ITEM*);
int Sort(MENU* menu, ITEM*);
int Save(MENU* menu, ITEM*);
int Help(MENU* menu, ITEM*);
int Exit(MENU* menu, ITEM*);

#endif
