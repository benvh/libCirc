#ifndef __IRC_H__
#define __IRC_H__

/*
 * SERVER NUMERIC REPLIES
 * (for more info check: http://www.irchelp.org/irchelp/rfc/chapter6.html or RFC 1459)
 */
typedef enum
{
    RPL_ISUPPORT = 5,
    RPL_ENDOFMOTD = 376,
    ERR_NOSUCHCHANNEL = 403,
    ERR_TOOMANYCHANNELS = 405,
    ERR_NICKNAMEINUSE = 433,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL
    
}IrcReplyCode;

#endif /* __IRC_H__ */

