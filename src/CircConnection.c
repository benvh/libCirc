#include "CircConnection.h"
#include <stdlib.h>
#include <gio/gio.h>

struct _CircConnection
{
    CircConnectionStatus    status;
    CircIdentity*           identity;
    
    GSocketConnectable*     sock_addr;
    GSocketClient*          sock_clie;
    GSocketConnection*      sock_conn;
    
    GThread*                recv_thread;    
    
    /* event callbacks */
    circ_event_connected                    cb_connected;
    circ_event_disconnected                 cb_disconnected;
    circ_event_connection_status_changed    cb_connection_status_changed;
};


/* Private Functions */
CircConnection* circ_connection_init(CircConnection* self, CircIdentity* identity, const gchar* host, int port)
{
    self->status = STATUS_DISCONNECTED;
    self->identity = identity;
    
    self->sock_addr = g_network_address_new(host, port);
    self->sock_clie = g_socket_client_new();
    self->sock_conn = NULL;
    
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
    self->status = STATUS_CONNECTING;
    self->recv_thread = g_thread_create(circ_connection_recv, self, FALSE, NULL);
}
