#include <stdio.h>
#include <assert.h>
#include <sqlite3.h>

sqlite3* init_test_db() {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    const char *sql = "CREATE TABLE cars (id INTEGER PRIMARY KEY, car_number TEXT, capacity REAL CHECK(capacity > 0));"
                      "INSERT INTO cars (car_number, capacity) VALUES ('A123BC', 20.0);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    return db;
}

void test_car_exists() {
    sqlite3 *db = init_test_db();
    sqlite3_stmt *stmt;
    const char *sql = "SELECT capacity FROM cars WHERE car_number = 'A123BC'";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    assert(sqlite3_step(stmt) == SQLITE_ROW);
    assert(sqlite3_column_double(stmt, 0) == 20.0);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    printf("test_car_exists passed\n");
}

void test_car_not_exists() {
    sqlite3 *db = init_test_db();
    sqlite3_stmt *stmt;
    const char *sql = "SELECT capacity FROM cars WHERE car_number = 'NOTEXIST'";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    assert(sqlite3_step(stmt) != SQLITE_ROW);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    printf("test_car_not_exists passed\n");
}

void test_capacity_positive() {
    sqlite3 *db = init_test_db();
    char *err_msg = NULL;
    const char *sql = "INSERT INTO cars (car_number, capacity) VALUES ('TEST', -5.0);";
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("test_capacity_positive passed (insert rejected)\n");
    } else {
        printf("test_capacity_positive failed (insert accepted)\n");
        assert(0);
    }
    sqlite3_free(err_msg);
    sqlite3_close(db);
}

int main() {
    test_car_exists();
    test_car_not_exists();
    test_capacity_positive();
    return 0;
}
