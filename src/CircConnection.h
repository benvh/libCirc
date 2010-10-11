/*
 * circConnection.h
 * This file is part of libCirc 
 *
 * Copyright (C) 2010 - Ben Van Houtven
 *
 * libCirc is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * libCirc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __CIRC_CONNECTION_H__
#define __CIRC_CONNECTION_H__

typedef struct _CircConnection CircConnection;

#include <glib.h>
#include "CircIdentity.h"

typedef enum
{
    STATUS_DISCONNECTED,
    STATUS_CONNECTING,
    STATUS_AUTH,
    STATUS_CONNECTED
}CircConnectionStatus;

/* Events */
typedef void (*circ_event_connected)(CircConnection* connection);
typedef void (*circ_event_disconnected)(CircConnection* connection);
typedef void (*circ_event_connection_status_changed)(CircConnection* connection, CircConnectionStatus status);
/* ------ */

CircConnection*     circ_connection_new(CircIdentity* identity, const gchar* host, int port);
void                circ_connection_destroy(CircConnection* connection);

void                circ_connection_connect(CircConnection* connection);
void                circ_connection_send_raw_message(CircConnection* connection, const gchar* raw_message);

CircIdentity*       circ_connection_get_identity(CircConnection* connection);

#endif /* __CIRC_CONNECTION_H__ */

