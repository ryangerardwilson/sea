#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../include/sea.h" // Path to sea headers
// Custom handler example
static SeaResponse my_app_users(SeaRequest *req) {
    (void)req;
    char *body = sea_model_fetch_users(); // Framework model
    return (SeaResponse){body, 200, strlen(body)};
}
// Root handler—because apparently you need me to hold your hand for the homepage
static SeaResponse my_app_root(SeaRequest *req) {
    (void)req;
    char *body = sea_view_render("welcome", NULL); // Use the view stub, or just hardcode if it sucks
    if (!body) {
        body = strdup("<html><body><h1>Sea App—Welcome, You Kernel Newbie</h1><p>Links: <a href='/users'>Users</a> | <a href='/pages'>Static Pages</a></p></body></html>");
    }
    return (SeaResponse){body, 200, strlen(body)};
}
static enum MHD_Result my_app_callback(void *cls,
                                       struct MHD_Connection *conn,
                                       const char *url,
                                       const char *method,
                                       const char *version,
                                       const char *upload_data,
                                       size_t *upload_data_size,
                                       void **con_cls) {
    (void)cls; (void)version; (void)upload_data; (void)upload_data_size; (void)con_cls;
    SeaRequest req = {0};
    sea_parse_http_request(method, url, NULL, &req);
    SeaResponse res = sea_handle_request(&req);
    struct MHD_Response *mhd_res = MHD_create_response_from_buffer(res.body_len, res.body, MHD_RESPMEM_MUST_COPY);
    if (!mhd_res) {
        free(req.method); free(req.uri); free(req.query_string);
        free(res.body);
        return MHD_NO;
    }
    MHD_add_response_header(mhd_res, "Content-Type", "text/html");
    enum MHD_Result ret = MHD_queue_response(conn, res.status, mhd_res);
    MHD_destroy_response(mhd_res);
    free(req.method); free(req.uri); free(req.query_string);
    free(res.body);
    return ret;
}
int main(int argc, char **argv) {
    // Args: [db_path] [port] [app_name] — defaults: app.db, 7000, MyApp
    const char *db_path = (argc > 1) ? argv[1] : "app.db";
    int port = (argc > 2) ? atoi(argv[2]) : 7000;
    const char *app_name = (argc > 3) ? argv[3] : "app";
    sea_init(db_path, port, app_name); // Framework up—pass the damn params
    sea_add_route("/", "GET", my_app_root); // Root route, you forgetful twit
    sea_add_route("/users", "GET", my_app_users); // Your routes here
    sea_add_static_dir("/pages", "./pages"); // Serve static crap from /pages -> ./pages dir
    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port,
                                                 NULL, NULL,
                                                 my_app_callback, NULL,
                                                 MHD_OPTION_END);
    if (!daemon) {
        perror("MHD_start_daemon failed: ");
        sea_shutdown();
        return 1;
    }
    printf("%s on Sea: http://localhost:%d/ (DB: %s)\n", app_name, port, db_path);
    printf("Try: /users (dynamic) | /pages/index.html (static)\n");
    getchar();
    MHD_stop_daemon(daemon);
    sea_shutdown();
    return 0;
}
