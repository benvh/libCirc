#include "CircConnection_out.h"

void out_send_ident(CircConnection* self)
{
    const gchar* name = circ_identity_get_name(circ_connection_get_identity(self));
    const gchar* nick = circ_identity_get_nick(circ_connection_get_identity(self));
    
    gchar* user_msg = g_strdup_printf("USER %s dummy dummy :%s\n", nick, name);
    circ_connection_send_raw_message(self, user_msg);
    out_send_nick(self);
    
    g_free(user_msg);
}

void out_send_nick(CircConnection* self)
{
    const gchar* nick = circ_identity_get_nick(circ_connection_get_identity(self));
    
    gchar* nick_msg = g_strdup_printf("NICK %s\n", nick);
    circ_connection_send_raw_message(self, nick_msg);
    
    g_free(nick_msg);
}
