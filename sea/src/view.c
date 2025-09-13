#include <stdlib.h>
#include <string.h>  // strdup, strcmp
#include <stdio.h>   // snprintf, because apparently you can't read
#include "../include/sea.h"

char *sea_view_render(const char *template, char **vars) {
    (void)vars;  // Unused stub

    if (!template) {
        template = "default";
    }

    // For now, just static HTML. Later: Parse {{var}} mustache-style without Ruby's ERB diarrhea.
    if (strcmp(template, "users") == 0) {
        return strdup("<div>Rendered users template—vars ignored for now.</div>");
    }

    const char *basic = "<html><body><h1>Sea View Rendered: %s</h1><p>No fancy shit yet.</p></body></html>";
    char *rendered = malloc(1024);
    if (rendered) {
        snprintf(rendered, 1024, basic, template);
    } else {
        return strdup("<p>Render failed—malloc hates you.</p>");
    }
    return rendered;
}
