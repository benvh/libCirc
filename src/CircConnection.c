#include "CircConnection.h"
#include <stdlib.h>

struct _CircConnection
{
    ConnectionStatus        status;
    
    GSocketConnectable*     sock_addr;
    GSocketClient*          sock_clie;
    GSocketConnection*      sock_conn;
    
};


/* Private Functions */
CircConnection* circ_connection_init(CircConnection* self, const gchar* host, int port)
{
    self->status = STATUS_DISCONNECTED;
    
    self->sock_addr = g_network_address_new(host, port);
    self->sock_clie = g_socket_client_new();
    self->sock_conn = NULL;
    
    return self;
}

/* Public Functions */
CircConnection* circ_connection_new(const gchar* host, int port)
{
    return circ_connection_init( (CircConnection*)malloc(sizeof(CircConnetion)), host, port);
}
