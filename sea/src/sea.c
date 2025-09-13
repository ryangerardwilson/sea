#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strdup if needed, but mostly for strlen in prints
#include "../include/sea.h"

// Statics for config—framework caches 'em, but you print in main anyway
static int g_port = 3000;
static char *g_app_name = NULL;

void sea_init(const char *db_path, int port, const char *app_name) {
    g_port = port;
    if (g_app_name) free(g_app_name); // Re-init? Free old
    g_app_name = strdup(app_name ? app_name : "MyApp");
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
    // Free cached app_name—don't leak
    if (g_app_name) {
        free(g_app_name);
        g_app_name = NULL;
    }
    printf("Sea shutdown—good riddance.\n");
}
