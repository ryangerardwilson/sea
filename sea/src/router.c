#include <stdlib.h>
#include <string.h>  // strdup, strlen, strcmp—basics, you hack
#include <stdio.h>   // fprintf, stderr—already there, but verify
#include "../include/sea.h"

#define MAX_ROUTES 100
SeaRoute routes[MAX_ROUTES];  // Global now, for shutdown access
int route_count = 0;          // Ditto

void sea_init_router(void) {
    route_count = 0;
    memset(routes, 0, sizeof(routes));  // Clear the slate, newbie
}

void sea_add_route(const char *path, const char *method, SeaHandler handler) {
    if (route_count >= MAX_ROUTES) {
        fprintf(stderr, "Too many routes, you route-hoarding bastard.\n");
        return;
    }
    routes[route_count].path = strdup(path ? path : "/");
    routes[route_count].method = strdup(method ? method : "GET");
    routes[route_count].handler = handler;
    route_count++;
}

SeaResponse sea_handle_request(SeaRequest *req) {
    for (int i = 0; i < route_count; i++) {
        if (strcmp(req->method, routes[i].method) == 0 &&
            strcmp(req->uri, routes[i].path) == 0) {
            return routes[i].handler(req);
        }
    }
    // Not found? Serve 404 like a proper failure
    const char *not_found = "<html><body><h1>Route Not Found—Try /users, Idiot</h1></body></html>";
    char *body_copy = strdup(not_found);  // Dup it properly, no int-to-ptr crap
    size_t len = strlen(not_found);
    SeaResponse res = {body_copy, 404, len};
    return res;
}
