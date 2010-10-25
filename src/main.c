#include "lib/CircConnection.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

/* THIS PROGRAM IS NOT PART OF LIBCIRC. IT IS FOR DEMONSTRATION PURPOSES ONLY! */

CircConnection *conn;
GtkWidget *window, *scroll_chatview, *txt_chatview, *txt_send;
GtkTextBuffer *chat_buffer;

void conn_connection_status_changed(CircConnection* conn, CircConnectionStatus status)
{
    if(status == STATUS_CONNECTED)
    {
    	circ_connection_send_raw_message(conn, "JOIN #opers\n");
    }
}

void conn_numeric_reply_received(CircConnection* conn, IrcReplyCode reply, const gchar* params)
{
	if(reply != RPL_ISUPPORT)
	{
		gtk_text_buffer_insert_at_cursor(chat_buffer, g_strdup(params), -1);
	}
}

void conn_message_received(CircConnection* conn, const gchar* from, const gchar* channel, const gchar* message)
{
	gchar* msg = g_strdup_printf("<%s %s> %s", from, channel, message);
	gtk_text_buffer_insert_at_cursor(chat_buffer, msg, -1);
	g_free(msg);
}

void conn_notice_received(CircConnection *conn, const gchar *from, const gchar *text)
{
	gchar *msg = g_strdup_printf("Notice from %s: %s\n", from, text);
	gtk_text_buffer_insert_at_cursor(chat_buffer, msg, -1);
	g_free(msg);
}

void conn_user_joined_channel(CircConnection *conn, const gchar *user, const gchar *message)
{
	gchar *msg = g_strdup_printf("%s joined %s", user, message);
	gtk_text_buffer_insert_at_cursor(chat_buffer, msg, -1);
	g_free(msg);
}

void conn_user_disconnected(CircConnection *conn, const gchar *user, const gchar *message)
{
	gchar *msg = g_strdup_printf("%s has lef irc (%s)\n", user, message);
	gtk_text_buffer_insert_at_cursor(chat_buffer, msg, -1);
	g_free(msg);
}

void send_message(GtkWidget *widget, gpointer data)
{
	gchar* msg = g_strdup_printf("<me> %s\n", gtk_entry_get_text(GTK_ENTRY(txt_send)));
	
	circ_connection_send_message(conn, "#opers", gtk_entry_get_text(GTK_ENTRY(txt_send)));
	gtk_text_buffer_insert_at_cursor(chat_buffer, msg, -1);
	gtk_entry_set_text(GTK_ENTRY(txt_send), "");
	
	g_free(msg);
}

int main (int argc, char *argv[])
{
	//these two are important because libCirc is threaded!!!
	g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();
	
	gtk_init(&argc, &argv);
	
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "interface.glade", NULL);
	
	window = GTK_WIDGET( gtk_builder_get_object(builder, "main_window") );
	scroll_chatview = GTK_WIDGET( gtk_builder_get_object(builder, "scroll_chatview") );
	txt_chatview = GTK_WIDGET( gtk_builder_get_object(builder, "txt_chatview") );
	txt_send = GTK_WIDGET( gtk_builder_get_object(builder, "txt_send") );
	
	g_signal_connect(G_OBJECT(txt_send), "activate", G_CALLBACK(send_message), NULL);
	
	chat_buffer = gtk_text_buffer_new(NULL);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(txt_chatview), chat_buffer);
	
	
    
    CircIdentity *ident = circ_identity_new("Trololo", "libCirc");
    conn = circ_connection_new(ident, "localhost", 6667);
    
    circ_connection_event_connect(conn, "connection-status-changed", (CircEventCallback)conn_connection_status_changed);
    circ_connection_event_connect(conn, "numeric-reply-received", (CircEventCallback)conn_numeric_reply_received);
    circ_connection_event_connect(conn, "message-received", (CircEventCallback)conn_message_received);
    circ_connection_event_connect(conn, "notice-received", (CircEventCallback)conn_notice_received);
    circ_connection_event_connect(conn, "user-joined-channel", (CircEventCallback)conn_user_joined_channel);
    circ_connection_event_connect(conn, "user-disconnected", (CircEventCallback)conn_user_disconnected);
    
	circ_connection_connect(conn);
    
    gtk_widget_show_all(window);
   
	gtk_main();	
	gdk_threads_leave();
            
    return 0;
}   
