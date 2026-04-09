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

void calculate_and_save_all_drivers_earnings(Database* db, const char* start_date, const char* end_date);
void calculate_and_save_driver_earnings(Database* db, const char* start_date, const char* end_date, const char* driver_name);
void show_salary_report(Database* db);
#endif
