#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strdup, strcpy, strcat, snprintf—don't make me say it again
#include "../include/sea.h"

sqlite3 *db = NULL;  // Global for shutdown

void sea_model_init(const char *db_path) {
    if (db) {
        sqlite3_close(db);  // Close previous if open, avoid leaks
        db = NULL;
    }
    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        fprintf(stderr, "DB open failed: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    // Create users table
    const char *sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"
                      "INSERT OR IGNORE INTO users (name) VALUES ('Linus'), ('Torvalds'), ('Hater');";  // Fake data!
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, NULL, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

char *sea_model_fetch_users(void) {
    // Assumes db already init'd by sea_init; don't re-init here
    if (!db) {
        return strdup("<p>No DB, you forgot sea_init, moron.</p>");
    }
    sqlite3_stmt *stmt;
    char *result = malloc(2048);  // Bigger buffer, because why not
    if (!result) return strdup("<p>Out of memory, you hog.</p>");

    strcpy(result, "<html><body><h1>Users (C-Style)</h1><ul>");

    const char *sql = "SELECT name FROM users ORDER BY id;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        int row_count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && row_count < 100) {  // Cap it, moron
            const unsigned char *name = sqlite3_column_text(stmt, 0);
            char buf[512];
            snprintf(buf, sizeof(buf), "<li>%s</li>", name ? (const char *)name : "NULL");
            strcat(result, buf);
            row_count++;
        }
        sqlite3_finalize(stmt);
    } else {
        strcat(result, "<li>Query failed—check your SQL, noob</li>");
    }

    strcat(result, "</ul></body></html>");
    return result;
}
