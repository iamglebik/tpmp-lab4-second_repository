#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "database.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

Database* init_database(const char *path) {
    Database *db = malloc(sizeof(Database));
    if (!db) return NULL;
    
    db->err_msg = NULL;
    db->rc = sqlite3_open(path, &db->db);
    
    if (db->rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db->db));
        db->db = NULL;
    }
    return db;
}

void close_database(Database *db) {
    if (db) {
        sqlite3_close(db->db);
        free(db);
    }
}

int execute_query(Database *db, const char *sql) {
    db->err_msg = NULL;
    db->rc = sqlite3_exec(db->db, sql, callback, NULL, &db->err_msg);
    if (db->rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", db->err_msg);
        sqlite3_free(db->err_msg);
        return 0;
    }
    return 1;
}

int add_car(Database *db, Car *car) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO cars (car_number, brand, mileage_on_purchase, capacity) "
        "VALUES ('%s', '%s', %.2f, %.2f);",
        car->number, car->brand, car->initial_mileage, car->capacity);
    return execute_query(db, sql);
}

void get_all_cars(Database *db) {
    execute_query(db, "SELECT id, car_number, brand, mileage_on_purchase, capacity FROM cars;");
}

int add_driver(Database *db, Driver *driver) {
    char sql[1024];
    snprintf(sql, sizeof(sql),
        "INSERT INTO drivers (personnel_number, last_name, category, experience, address, birth_year) "
        "VALUES (%d, '%s', '%s', %d, '%s', %d);",
        driver->personal_id, driver->full_name, driver->category, 
        driver->experience, driver->address, driver->birth_year);
    return execute_query(db, sql);
}

void get_all_drivers(Database *db) {
    execute_query(db, "SELECT id, personnel_number, last_name, category, experience, address, birth_year FROM drivers;");
}

int add_order(Database *db, Order *order) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO orders (order_date, drivers_id, cars_id, mileage, cargo_mass, transport_cost) "
        "VALUES ('%s', %d, %d, %.2f, %.2f, %.2f);",
        order->date, order->driver_id, order->car_id, 
        order->distance, order->cargo_weight, order->total_cost);
    return execute_query(db, sql);
}

void get_all_orders(Database *db) {
    execute_query(db, "SELECT id, order_date, drivers_id, cars_id, mileage, cargo_mass, transport_cost FROM orders;");
}

void report_all_drivers_summary(Database *db) {
    printf("\n--- ОТЧЕТ ПО ВОДИТЕЛЯМ ---\n");
    printf("Водитель | Поездки | Масса груза | Заработано (20%%)\n");
    execute_query(db, 
        "SELECT d.last_name, COUNT(o.id) as trips, SUM(o.cargo_mass) as total_cargo, "
        "SUM(o.transport_cost) * 0.2 as earnings "
        "FROM drivers d "
        "LEFT JOIN orders o ON d.id = o.drivers_id "
        "GROUP BY d.id;");
}

double calc_driver_earnings_period(Database *db, int driver_id, const char *start, const char *end) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "SELECT SUM(transport_cost) * 0.2 as earnings FROM orders "
        "WHERE drivers_id = %d AND order_date BETWEEN '%s' AND '%s';",
        driver_id, start, end);
    execute_query(db, sql);
    return 0;
}

void find_min_trips_driver(Database *db) {
    printf("\n--- ВОДИТЕЛЬ С НАИМЕНЬШИМ КОЛИЧЕСТВОМ ПОЕЗДОК ---\n");
    execute_query(db, 
        "SELECT d.last_name, COUNT(o.id) as trips "
        "FROM drivers d "
        "LEFT JOIN orders o ON d.id = o.drivers_id "
        "GROUP BY d.id "
        "ORDER BY trips ASC LIMIT 1;");
}

void find_max_mileage_car(Database *db) {
    printf("\n--- АВТОМОБИЛЬ С МАКСИМАЛЬНЫМ ПРОБЕГОМ ---\n");
    execute_query(db,
        "SELECT c.car_number, c.brand, SUM(o.mileage) as total_mileage "
        "FROM cars c "
        "JOIN orders o ON c.id = o.cars_id "
        "GROUP BY c.id "
        "ORDER BY total_mileage DESC LIMIT 1;");
}
