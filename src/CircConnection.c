#include "CircConnection.h"
#include <stdlib.h>
#include <gio/gio.h>

#include <stdio.h>

struct _CircConnection
{
    CircConnectionStatus    status;
    CircIdentity*           identity;
    
    GSocketConnectable*     sock_addr;
    GSocketClient*          sock_clie;
    GSocketConnection*      sock_conn;
    
    GDataInputStream*       dinstream;
    GDataOutputStream*      doutstream;
    
    GThread*                recv_thread;    
    
    GHashTable*             event_callbacks;
};


/* Private Functions */
extern void in_parse_message(CircConnection* self, const gchar*);
extern void out_send_ident(CircConnection* self);

CircConnection* circ_connection_init(CircConnection* self, CircIdentity* identity, const gchar* host, int port)
{
    self->status = STATUS_DISCONNECTED;
    self->identity = identity;
    
    self->sock_addr = g_network_address_new(host, port);
    self->sock_clie = g_socket_client_new();
    
    self->dinstream = NULL;
    self->doutstream = NULL;
    
    self->event_callbacks = g_hash_table_new(g_str_hash, g_str_equal);
    
    return self;
}

void circ_connection_update_status(CircConnection* self, CircConnectionStatus status)
{
    self->status = status;
    
    CircEventCallback callback = g_hash_table_lookup(self->event_callbacks, "connection-status-changed");
    if(callback)callback(self, status);
}

/* We need this in CircConnection_in ... */
GHashTable* circ_connection_get_callbacks(CircConnection* self)
{
    return self->event_callbacks;
}

/* Main loop of the connection (this is threaded!!) */
gpointer circ_connection_recv(gpointer data)
{
    CircConnection* self = (CircConnection*)data;    
    circ_connection_update_status(self, STATUS_CONNECTING);
    
    self->sock_conn = g_socket_client_connect(self->sock_clie, self->sock_addr, NULL, NULL); //try and connect
    if(self->sock_conn == NULL)
    {
        printf("Error: Failed to connect to %s:%d\n", g_network_address_get_hostname(G_NETWORK_ADDRESS(self->sock_addr)), g_network_address_get_port(G_NETWORK_ADDRESS(self->sock_addr)));
        circ_connection_update_status(self, STATUS_DISCONNECTED);
        return;
    }
    
    circ_connection_update_status(self, STATUS_AUTH);
    
    if(!self->dinstream)self->dinstream = g_data_input_stream_new( g_io_stream_get_input_stream(G_IO_STREAM(self->sock_conn)) );
    if(!self->doutstream)self->doutstream = g_data_output_stream_new( g_io_stream_get_output_stream(G_IO_STREAM(self->sock_conn)) );
    
    out_send_ident(self);
    
    gchar* raw_in;
    gsize nraw_in;
    while(self->status > STATUS_CONNECTING)
    {   
            raw_in = g_data_input_stream_read_line(self->dinstream, &nraw_in, NULL, NULL);
            in_parse_message(self, raw_in);
            g_free(raw_in);
    }
    printf("Disconnected\n");
    //do some cleaning up after disconnect
}
/* Public Functions */
CircConnection* circ_connection_new(CircIdentity* identity, const gchar* host, int port)
{
    return circ_connection_init( (CircConnection*)malloc(sizeof(CircConnection)), identity, host, port);
}

void circ_connection_destroy(CircConnection* self)
{
    //implement here
}


void circ_connection_connect(CircConnection* self)
{
    if(self->status != STATUS_DISCONNECTED) return;
    self->recv_thread = g_thread_create(circ_connection_recv, self, FALSE, NULL);
}

void circ_connection_send_raw_message(CircConnection* self, const gchar* raw_message)
{
    if(self->status < STATUS_AUTH) return;
    g_data_output_stream_put_string(self->doutstream, raw_message, NULL, NULL);
    printf("%s", raw_message);
}


CircIdentity* circ_connection_get_identity(CircConnection* self)
{
    return self->identity;
}


void circ_connection_event_connect(CircConnection* self, const gchar* event, CircEventCallback callback)
{
    if(!g_hash_table_lookup(self->event_callbacks, event))
    {
        g_hash_table_insert(self->event_callbacks, g_strdup(event), callback);
    }
    else printf("ERROR: Event '%s' already connected. You need to disconnect the event before reconnecting it!\n", event);
}

void circ_connection_event_disconnect(CircConnection* self, const gchar* event)
{
    CircEventCallback callback;
    gchar** orig_key;
    
    if(g_hash_table_lookup_extended(self->event_callbacks, event, (gpointer*)(&orig_key), (gpointer*)(&callback)) == TRUE)
    {
        g_hash_table_remove(self->event_callbacks, event);
        g_free(orig_key);
    }
}
