#include <stdlib.h>
#include <string.h>  // For strdup, strcmp, and all that string voodoo, idiot
#include "../include/sea.h"

void sea_parse_http_request(const char *method_str, const char *url_str, const char *query_str, SeaRequest *req) {
    req->method = strdup(method_str ? method_str : "GET");
    req->uri = strdup(url_str ? url_str : "/");
    req->query_string = strdup(query_str ? query_str : "");

    // Later: Parse query params into a map, headers, POST body. For now, fuck off.
    // TODO: URL-decode, path splitting for params like /users/:id
}
