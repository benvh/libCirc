#include "CircConnection.h"
#include <gtk/gtk.h>
#include <stdio.h>

GtkTextBuffer *text_buffer;

void conn_connection_status_changed(CircConnection* conn, CircConnectionStatus status)
{
    if(status == STATUS_CONNECTED) gtk_text_buffer_insert_at_cursor(text_buffer, "I R READY TO ROLL!", -1);
}

void conn_numeric_reply_received(CircConnection* conn, IrcReplyCode num_reply, const gchar* params)
{
    gtk_text_buffer_insert_at_cursor(text_buffer, params, -1);
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *scrolled_window;
    
    GtkWidget *text_view;
    
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
        
    text_buffer = gtk_text_buffer_new(NULL);
    text_view = gtk_text_view_new_with_buffer(text_buffer);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    
    gtk_container_add(GTK_CONTAINER(window), text_view);    
    
    CircIdentity* ident = circ_identity_new("Trololo", "Ben");
    CircConnection* conn = circ_connection_new(ident, "localhost", 6667);
    
    circ_connection_event_connect(conn, "connection-status-changed", (CircEventCallback)conn_connection_status_changed);
    circ_connection_event_connect(conn, "numeric-reply-received", (CircEventCallback)conn_numeric_reply_received);
    
    circ_connection_connect(conn);
    
    gtk_widget_show_all(window);
    gtk_main();   
            
    return 0;
}   
