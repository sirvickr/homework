#ifndef __DICTIONARY_MAIN_H__
#define __DICTIONARY_MAIN_H__

#include "menu.h"

// Описание функций, которые подключаются к пунктам меню
int Edit(MENU* menu);
int Delete(MENU* menu);
int Search(MENU* menu);
int Sort(MENU* menu);
int Save(MENU* menu);
int Help(MENU* menu);
int Exit(MENU* menu);

#endif
