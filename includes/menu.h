#ifndef MENU_H
#define MENU_H

#include "database.h"

typedef enum {
    ROLE_ADMIN = 0,
    ROLE_DRIVER = 1
} UserRole;

typedef struct {
    int id;
    char username[50];
    UserRole role;
    int driver_id; 
} Session;

void show_main_menu(Session *session, Database *db);
int handle_menu_choice(Session *session, Database *db, int choice);

void show_admin_menu(void);
void show_driver_menu(void);

void input_car_form(Car *car);
void input_driver_form(Driver *driver);
void input_order_form(Order *order);

#endif
