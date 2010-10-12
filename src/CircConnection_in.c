#include "CircConnection.h"
#include <stdio.h>

extern void out_pong(CircConnection*, const gchar*);

void in_parse_message(CircConnection* self, const gchar* raw_message)
{
    printf("%s\n", raw_message);
    gchar** raw_split = g_strsplit(raw_message, " ", 0);

    if(g_strcmp0(raw_split[0], "PING") == 0)out_pong(self, raw_split[1]);
}
