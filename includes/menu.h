#ifndef MENU_H
#define MENU_H

#include "database.h"

void show_main_menu(void);
int handle_menu_choice(Database *db, int choice);

#endif
