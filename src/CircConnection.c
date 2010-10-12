#include "CircConnection.h"
//#include "CircConnection_out.h"
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
    
    /* event callbacks */
    circ_event_connected                    cb_connected;
    circ_event_disconnected                 cb_disconnected;
    circ_event_connection_status_changed    cb_connection_status_changed;
};


/* Private Functions */
extern void out_send_ident(CircConnection* self);

CircConnection* circ_connection_init(CircConnection* self, CircIdentity* identity, const gchar* host, int port)
{
    self->status = STATUS_DISCONNECTED;
    self->identity = identity;
    
    self->sock_addr = g_network_address_new(host, port);
    self->sock_clie = g_socket_client_new();
    
    return self;
}

void circ_connection_update_status(CircConnection* self, CircConnectionStatus status)
{
    self->status = status;
    if(self->cb_connection_status_changed) self->cb_connection_status_changed(self, status);
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
    while((raw_in = g_data_input_stream_read_line(self->dinstream, &nraw_in, NULL, NULL)) != NULL)
    {
        printf("%s\n", raw_in);
        g_free(raw_in);
    }
    
    circ_connection_update_status(self, STATUS_DISCONNECTED);
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
}


CircIdentity* circ_connection_get_identity(CircConnection* self)
{
    return self->identity;
}
