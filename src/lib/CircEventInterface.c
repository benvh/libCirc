#include "CircEventInterface.h"
#include <stdio.h>

extern void* circ_connection_get_callback(CircConnection*, const gchar*);

typedef void (*tCircConnectionStatusChanged)(CircConnection*, CircConnectionStatus);
typedef void (*tCircNumericReplyReceived)(CircConnection*, IrcReplyCode, const gchar*);
typedef void (*tCircMessageReceived)(CircConnection*, const gchar*, const gchar*, const gchar*);
typedef void (*tCircNoticeReceived)(CircConnection*, const gchar*, const gchar*);
typedef void (*tCircUserJoinedChannel)(CircConnection*, const gchar*, const gchar*);
typedef void (*tCircUserDisconnected)(CircConnection*, const gchar*, const gchar*);

void circ_call_connection_status_changed(CircConnection *conn, CircConnectionStatus status)
{
	void* callback = circ_connection_get_callback(conn, "connection-status-changed");
	if(callback)
		((tCircConnectionStatusChanged)callback)(conn, status);
}

void circ_call_numeric_reply_received(CircConnection* conn, IrcReplyCode code, const gchar* message)
{
	void* callback = circ_connection_get_callback(conn, "numeric-reply-received");
	if(callback)
		((tCircNumericReplyReceived)callback)(conn, code, message);
}

void circ_call_message_received(CircConnection *conn, const gchar *from, const gchar *channel, const gchar *message)
{
	void* callback = circ_connection_get_callback(conn, "message-received");
	if(callback)
		((tCircMessageReceived)callback)(conn, from, channel, message);
}

void circ_call_notice_received(CircConnection *conn, const gchar *from, const gchar *message)
{
	void* callback = circ_connection_get_callback(conn, "notice-received");
	if(callback)
		((tCircNoticeReceived)callback)(conn, from, message);
}

void circ_call_user_joined_channel(CircConnection *conn, const gchar *user, const gchar *channel)
{
	void* callback = circ_connection_get_callback(conn, "user-joined-channel");
	if(callback)
		((tCircUserJoinedChannel)callback)(conn, user, channel);
}

void circ_call_user_disconnected(CircConnection *conn, const gchar *user, const gchar *message)
{
	void* callback = circ_connection_get_callback(conn, "user-disconnected");
	if(callback)
		((tCircUserDisconnected)callback)(conn, user, message);
}

