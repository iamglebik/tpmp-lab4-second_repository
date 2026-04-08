#include <stdio.h>
#include <stdlib.h>
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
