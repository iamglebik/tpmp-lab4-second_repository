#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct {
    sqlite3 *db;
    char *err_msg;
    int rc;
} Database;

typedef struct {
    int id;
    char number[20];
    char brand[50];
    double initial_mileage;
    double capacity;
} Car;

typedef struct {
    int id;
    int personal_id;
    char full_name[100];
    char category[10];
    int experience;
    char address[200];
    int birth_year;
} Driver;

typedef struct {
    int id;
    char date[11];
    int driver_id;
    int car_id;
    double distance;
    double cargo_weight;
    double total_cost;
} Order;

Database* init_database(const char *path);
void close_database(Database *db);
int execute_query(Database *db, const char *sql);
int callback(void *NotUsed, int argc, char **argv, char **azColName);

int add_car(Database *db, Car *car);
void get_all_cars(Database *db);
int update_car(Database *db, int id, Car *car);
int delete_car(Database *db, int id);

int add_driver(Database *db, Driver *driver);
void get_all_drivers(Database *db);
int update_driver(Database *db, int id, Driver *driver);
int delete_driver(Database *db, int id);

int add_order(Database *db, Order *order);
void get_all_orders(Database *db);
int update_order(Database *db, int id, Order *order);
int delete_order(Database *db, int id);

void report_all_drivers_summary(Database *db);
double calc_driver_earnings_period(Database *db, int driver_id, const char *start, const char *end);
void find_min_trips_driver(Database *db);
void find_max_mileage_car(Database *db);

#endif
