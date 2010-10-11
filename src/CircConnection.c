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

/* Public Functions */
CircConnection* circ_connection_new(CircIdentity* identity, const gchar* host, int port)
{
    return circ_connection_init( (CircConnection*)malloc(sizeof(CircConnection)), identity, host, port);
}
