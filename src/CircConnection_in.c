#include "CircConnection.h"
#include "CircEventInterface.h"
#include <stdlib.h>
#include <stdio.h>

extern void circ_connection_update_status(CircConnection*, CircConnectionStatus);
extern GHashTable* circ_connection_get_callbacks(CircConnection*);
extern void out_pong(CircConnection*, const gchar*);

void in_numeric_reply(CircConnection *self, int num_reply, const gchar *params);
void in_message(CircConnection *self, const gchar *from, const gchar *channel, const gchar *message);
void in_notice(CircConnection *self, const gchar *from, const gchar *message);



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
    else if(g_strcmp0(raw_split0[0], "ERROR") == 0){
        //do stuff
        g_strfreev(raw_split0);
        circ_connection_update_status(self, STATUS_DISCONNECTED);
        return;
    } 
    
    gchar** raw_split1 = g_strsplit(raw_split0[0], " ", 0);
    
    int num_reply = atoi(raw_split1[1]);
    if(num_reply != 0) in_numeric_reply(self, num_reply, raw_split0[1]);
    
    //no num_reply
    gchar** nick_host = g_strsplit(raw_split1[0], "!", 0);
    
    if(g_strcmp0(raw_split1[1], "PRIVMSG") == 0)
    {
		in_message(self, nick_host[0]+1, raw_split1[2], raw_split0[1]);
    }
    else if(g_strcmp0(raw_split1[1], "NOTICE") == 0)
    {
 		in_notice(self, nick_host[0]+1, raw_split0[1]);
    }
    
    g_strfreev(nick_host);
    g_strfreev(raw_split0);
    g_strfreev(raw_split1);
}

void in_numeric_reply(CircConnection* self, int num_reply, const gchar* params)
{ 
    if(!params)return;
    
    if(num_reply == RPL_ENDOFMOTD) circ_connection_update_status(self, STATUS_CONNECTED);
    if(circ_connection_get_status(self) == STATUS_AUTH && num_reply == ERR_NICKNAMEINUSE)
    {
        CircIdentity* ident = circ_connection_get_identity(self);
        gchar* new_nick = g_strdup_printf("%s_", circ_identity_get_nick(ident));
        circ_connection_change_nick(self, new_nick);
        g_free(new_nick);
    }

	circ_call_numeric_reply_received(self, num_reply, params);
}

void in_message(CircConnection* self, const gchar* from, const gchar* channel, const gchar* message)
{
	//do some extra stuff here
	circ_call_message_received(self, from, channel, message);
}

void in_notice(CircConnection *self, const gchar *from, const gchar *message)
{
	//do some extra stuff here
	circ_call_notice_received(self, from, message);
}
