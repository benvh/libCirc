#include "CircIdentity.h"
#include <stdlib.h>

struct _CircIdentity
{
    gchar* realname;
    gchar* nick;
};


/* Private Functions */
CircIdentity* circ_identity_init(CircIdentity* self, const gchar* realname, const gchar* nick)
{
    self->realname = g_strdup(realname);
    self->nick = g_strdup(nick);
    
    return self;
}

/* Public Functions */
CircIdentity* circ_identity_new(const gchar* realname, const gchar* nick)
{
    return circ_identity_init( (CircIdentity*)malloc(sizeof(CircIdentity)), realname, nick);
}

void circ_identity_destroy(CircIdentity* self)
{
    g_free(self->realname);
    g_free(self->nick);
    g_free(self);
}

void circ_identity_set_name(CircIdentity* self, const gchar* realname)
{
    g_free(self->realname);
    self->realname = g_strdup(realname);
}

void circ_identity_set_nick(CircIdentity* self, const gchar* nick)
{
    g_free(self->nick);
    self->nick = g_strdup(nick);
}

const gchar* circ_identity_get_name(CircIdentity* self)
{
    return self->realname;
}

const gchar* circ_identity_get_nick(CircIdentity* self)
{
    return self->nick;
}
