#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sea/include/sea.h" // Path to sea headers

// Custom handler example
static SeaResponse my_app_users(SeaRequest *req) {
    (void)req;
    char *body = sea_model_fetch_users(); // Framework model
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
    (void)cls; (void)version; (void)upload_data; (void)upload_data_size; (void)con_cls;  // Shush unused warnings, lazy.

    SeaRequest req = {0};  // Zero it to avoid garbage.
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
    free(res.body);  // Don't leak, moron.

    return ret;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Usage: %s <db_path>\n", argv[0]); return 1; }
    sea_init(argv[1]); // Framework up, now with DB path
    sea_add_route("/users", "GET", my_app_users); // Your routes here
    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, 3000,
                                             NULL, NULL,  // apc, apc_cls—skip the policy crap
                                             my_app_callback, NULL,  // dh, dh_cls
                                             MHD_OPTION_END);
    if (!daemon) { 
        perror("MHD_start_daemon failed: ");
        sea_shutdown(); return 1;
    }
    printf("MyApp on Sea: http://localhost:3000/users (DB: %s)\n", argv[1]);
    getchar();
    MHD_stop_daemon(daemon);
    sea_shutdown();
    return 0;
}
