#include "CircConnection.h"
#include <stdlib.h>
#include <stdio.h>

extern void circ_connection_update_status(CircConnection*, CircConnectionStatus);
extern GHashTable* circ_connection_get_callbacks(CircConnection*);
extern void out_pong(CircConnection*, const gchar*);

void in_numeric_reply(CircConnection* self, int num_reply, const gchar* params);

void in_parse_message(CircConnection* self, const gchar* raw_message)
{
    printf("%s\n", raw_message);
    
    gchar** raw_split0 = g_strsplit(raw_message, " :", 0);
    
    
    if(g_strcmp0(raw_split0[0], "PING") == 0)
    {
        out_pong(self, raw_split0[1]);
        g_strfreev(raw_split0);
        return; //we don't need further parsing of the message
    }
    
    if(g_strcmp0(raw_split0[0], "ERROR") == 0){
        //do stuff
        g_strfreev(raw_split0);
        circ_connection_update_status(self, STATUS_DISCONNECTED);
        return;
    } 
    
    gchar** raw_split1 = g_strsplit(raw_split0[0], " ", 0);
    
    int num_reply = atoi(raw_split1[1]);
    if(num_reply != 0) in_numeric_reply(self, num_reply, raw_split0[1]);
    
    //no num_reply
    
    g_strfreev(raw_split0);
    g_strfreev(raw_split1);
}

void in_numeric_reply(CircConnection* self, int num_reply, const gchar* params)
{ 
    if(!params)return;
    
    if(num_reply == 1) circ_connection_update_status(self, STATUS_CONNECTED);
    GHashTable* event_callbacks = circ_connection_get_callbacks(self);
    CircEventCallback callback = g_hash_table_lookup(event_callbacks, "numeric-reply-received");
    if(callback) callback(self, num_reply, params);
}
