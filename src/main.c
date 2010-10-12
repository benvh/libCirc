#include "CircConnection.h"
#include <glib.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    g_type_init();
    
    CircIdentity* identity = circ_identity_new("Jitse De Wandel", "Jizzmeister");
    CircConnection* conn = circ_connection_new(identity, "localhost", 6667); 
    
    circ_connection_connect(conn);
    
    GMainLoop* main_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(main_loop);
            
    return 0;
}
