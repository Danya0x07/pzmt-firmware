#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <main.h>

typedef enum {
    CommandType_UNRECOGNIZABLE,
    CommandType_SET_VOLUME,
    CommandType_PLAY_FINITE_TONE,
    CommandType_PLAY_INFINITE_TONE,
    CommandType_STOP_PLAYING,

    NUM_OF_COMMAND_TYPES
} CommandType_t;

struct Command {
    CommandType_t type;
    union {
        struct {
            uint16_t frequency;
            uint16_t duration;
        };
        bool volumeRaised;
    } params;
};

typedef enum {
    ReplyCode_WRONG_CMD = '0',
    ReplyCode_OK = '1',
    ReplyCode_READY = '2',
    ReplyCode_BUSY = '3',

    NUM_OF_REPLY_CODES
} ReplyCode_t;

struct Reply {
    ReplyCode_t code;
};

void Protocol_BuildStartupMsg(char *buff);
void Protocol_ParseCommand(char *line, struct Command *cmd);
void Protocol_BuildReply(const struct Reply *reply, char *buff);

#endif // _PROTOCOL_H