#include <stdio.h>
#include "../include/sea.h"
void sea_init(const char *db_path) {
    sea_init_router(); // Your existing router init
    sea_model_init(db_path ? db_path : ":memory:"); // Global DB init—use passed path or default
    printf("Sea framework initialized—don't fuck it up.\n");
}
void sea_shutdown(void) {
    // Free routes: loop and free paths/methods
    for (int i = 0; i < route_count; i++) {
        free(routes[i].path);
        free(routes[i].method);
    }
    // Close DB if global
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
    printf("Sea shutdown—good riddance.\n");
}
