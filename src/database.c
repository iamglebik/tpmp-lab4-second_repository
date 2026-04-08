#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
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

int update_car(Database *db, int id, Car *car) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "UPDATE cars SET car_number='%s', brand='%s', mileage_on_purchase=%.2f, capacity=%.2f WHERE id=%d;",
        car->number, car->brand, car->initial_mileage, car->capacity, id);
    return execute_query(db, sql);
}

int delete_car(Database *db, int id) {
    char sql[128];
    snprintf(sql, sizeof(sql), "DELETE FROM cars WHERE id=%d;", id);
    return execute_query(db, sql);
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

int update_driver(Database *db, int id, Driver *driver) {
    char sql[1024];
    snprintf(sql, sizeof(sql),
        "UPDATE drivers SET personnel_number=%d, last_name='%s', category='%s', experience=%d, address='%s', birth_year=%d WHERE id=%d;",
        driver->personal_id, driver->full_name, driver->category, driver->experience, driver->address, driver->birth_year, id);
    return execute_query(db, sql);
}

int delete_driver(Database *db, int id) {
    char sql[128];
    snprintf(sql, sizeof(sql), "DELETE FROM drivers WHERE id=%d;", id);
    return execute_query(db, sql);
}

int add_order(Database *db, Order *order) {
    char check_sql[256];
    double capacity = 0;
    char *err_msg = NULL;
    
    snprintf(check_sql, sizeof(check_sql), "SELECT capacity FROM cars WHERE id = %d;", order->car_id);
    db->rc = sqlite3_exec(db->db, check_sql, callback, &capacity, &err_msg);
    
    if (order->cargo_weight > capacity) {
        printf("Ошибка: Масса груза (%.2f) превышает грузоподъемность (%.2f)\n", order->cargo_weight, capacity);
        return 0;
    }
    
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

int update_order(Database *db, int id, Order *order) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "UPDATE orders SET order_date='%s', drivers_id=%d, cars_id=%d, mileage=%.2f, cargo_mass=%.2f, transport_cost=%.2f WHERE id=%d;",
        order->date, order->driver_id, order->car_id, order->distance, order->cargo_weight, order->total_cost, id);
    return execute_query(db, sql);
}

int delete_order(Database *db, int id) {
    char sql[128];
    snprintf(sql, sizeof(sql), "DELETE FROM orders WHERE id=%d;", id);
    return execute_query(db, sql);
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

void calculate_and_save_all_drivers_earnings(Database *db, const char *start_date, const char *end_date) {
    char sql[4096];
    char current_date[11];
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(current_date, sizeof(current_date), "%Y-%m-%d", t);
    
    snprintf(sql, sizeof(sql),
        "INSERT INTO salary_report (report_date, start_period, end_period, driver_id, driver_name, trips_count, total_cargo, earnings, report_type) "
        "SELECT "
        "'%s', '%s', '%s', d.id, d.last_name, "
        "COUNT(o.id), COALESCE(SUM(o.cargo_mass), 0), "
        "COALESCE(SUM(o.transport_cost) * 0.2, 0), "
        "'period_all' "
        "FROM drivers d "
        "LEFT JOIN orders o ON d.id = o.drivers_id AND o.order_date BETWEEN '%s' AND '%s' "
        "GROUP BY d.id;",
        current_date, start_date, end_date, start_date, end_date);
    
    printf("\n=== РАСЧЕТ ЗАРПЛАТ ВСЕХ ВОДИТЕЛЕЙ ЗА ПЕРИОД ===\n");
    printf("Период: %s - %s\n", start_date, end_date);
    
    if (execute_query(db, sql)) {
        printf("Результаты сохранены в таблицу salary_report\n");
        show_salary_report(db);
    } else {
        printf("Ошибка при сохранении отчета\n");
    }
}

void calculate_and_save_driver_earnings(Database *db, const char *start_date, const char *end_date, const char *driver_name) {
    char sql[2048];
    char current_date[11];
    char name_pattern[100];
    int driver_id = 0;
    
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(current_date, sizeof(current_date), "%Y-%m-%d", t);
    
    snprintf(name_pattern, sizeof(name_pattern), "%%%s%%", driver_name);
    
    char find_sql[512];
    snprintf(find_sql, sizeof(find_sql),
        "SELECT id FROM drivers WHERE last_name LIKE '%s' LIMIT 1;", name_pattern);
    
    sqlite3_stmt *stmt;
    db->rc = sqlite3_prepare_v2(db->db, find_sql, -1, &stmt, NULL);
    if (db->rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        driver_id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    
    if (driver_id == 0) {
        printf("Водитель с фамилией '%s' не найден\n", driver_name);
        return;
    }
    
    snprintf(sql, sizeof(sql),
        "INSERT INTO salary_report (report_date, start_period, end_period, driver_id, driver_name, trips_count, total_cargo, earnings, report_type) "
        "SELECT "
        "'%s', '%s', '%s', d.id, d.last_name, "
        "COUNT(o.id), COALESCE(SUM(o.cargo_mass), 0), "
        "COALESCE(SUM(o.transport_cost) * 0.2, 0), "
        "'period_driver' "
        "FROM drivers d "
        "LEFT JOIN orders o ON d.id = o.drivers_id AND o.order_date BETWEEN '%s' AND '%s' "
        "WHERE d.id = %d;",
        current_date, start_date, end_date, start_date, end_date, driver_id);
    
    printf("\n=== РАСЧЕТ ЗАРПЛАТЫ ВОДИТЕЛЯ ЗА ПЕРИОД ===\n");
    printf("Водитель: %s\n", driver_name);
    printf("Период: %s - %s\n", start_date, end_date);
    
    if (execute_query(db, sql)) {
        printf("Результат сохранен в таблицу salary_report\n");
        show_salary_report(db);
    } else {
        printf("Ошибка при сохранении отчета\n");
    }
}

void show_salary_report(Database *db) {
    printf("\n=== ИСТОРИЯ ОТЧЕТОВ ПО ЗАРПЛАТАМ ===\n");
    execute_query(db, 
        "SELECT id, report_date, start_period, end_period, driver_name, "
        "trips_count, total_cargo, earnings, report_type "
        "FROM salary_report ORDER BY id DESC LIMIT 10;");
}