/*
 * CircIdenity.h
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

#ifndef __CIRC_IDENTITY_H__
#define __CIRC_IDENTITY_H__

typedef struct _CircIdentity CircIdentity;

#include <glib.h>

CircIdentity*       circ_identity_new(const gchar* realname, const gchar* nick);
void                circ_identity_destroy(CircIdentity* identity);

void                circ_identity_set_name(CircIdentity* identity, const gchar* realname);
void                circ_identity_set_nick(CircIdentity* identity, const gchar* nick);
const gchar*        circ_identity_get_name(CircIdentity* identity);
const gchar*        circ_identity_get_nick(CircIdentity* identity);


#endif /* __CIRC_IDENTITY_H__ */

