#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct {
    sqlite3 *db;
    char *err_msg;
    int rc;
} Database;

Database* init_database(const char *path);
void close_database(Database *db);
int execute_query(Database *db, const char *sql);
int callback(void *NotUsed, int argc, char **argv, char **azColName);

#endif
