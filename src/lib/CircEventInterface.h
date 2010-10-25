#ifndef __CIRC_EVENT_INTERFACE_H__
#define __CIRC_EVENT_INTERFACE_H__

#include <glib.h>
#include "CircConnection.h"
#include "irc.h"

void circ_call_connection_status_changed(CircConnection *conn, CircConnectionStatus status);
void circ_call_numeric_reply_received(CircConnection *conn, IrcReplyCode replycode, const gchar *message);
void circ_call_message_received(CircConnection *conn, const gchar *from, const gchar *channel, const gchar *message);
void circ_call_notice_received(CircConnection *conn, const gchar *from, const gchar *message);
void circ_call_user_joined_channel(CircConnection *conn, const gchar *user, const gchar *channel);
void circ_call_user_parted_channel(CircConnection *conn, const gchar *user, const gchar *channel);
void circ_call_user_disconnected(CircConnection *conn, const gchar *user, const gchar *message);
void circ_call_channel_flags_changed(CircConnection *conn, const gchar *from, const gchar *channel, const gchar *flags);
void circ_call_user_flags_changed(CircConnection *conn, const gchar *from, const gchar* channel, const gchar *user, const gchar *flags);


#endif /* __CIRC_EVENT_INTERFACE_H__ */

