#ifndef SEA_H
#define SEA_H
#include <microhttpd.h>
#include <sqlite3.h>
#include <stddef.h> // For size_t, you forgetful twit

// Request struct
typedef struct {
    char *method;
    char *uri;
    char *query_string;
    // Add headers/body later, but keep it simple for now
} SeaRequest;

// Response struct
typedef struct {
    char *body;
    int status;
    size_t body_len;
} SeaResponse;

// Route handler callback
typedef SeaResponse (*SeaHandler)(SeaRequest *req);

// Router table entry
typedef struct {
    char *path;
    char *method;
    SeaHandler handler;
} SeaRoute;

// Globals for framework (yeah, globals suck, but shutdown needs 'em—fix with structs later)
extern SeaRoute routes[];
extern int route_count;
extern sqlite3 *db;

void sea_init_router(void);
void sea_add_route(const char *path, const char *method, SeaHandler handler);
SeaResponse sea_handle_request(SeaRequest *req);
void sea_model_init(const char *db_path);
char *sea_model_fetch_users(void);
char *sea_view_render(const char *template, char **vars); // Stub

// HTTP utils
void sea_parse_http_request(const char *method_str, const char *url_str, const char *query_str, SeaRequest *req);

// Framework bootstrap
void sea_init(const char *db_path, int port, const char *app_name); // Pass the params, moron—no more .env bullshit
void sea_shutdown(void); // Cleanup: free routes, close DB

#endif /* SEA_H */
