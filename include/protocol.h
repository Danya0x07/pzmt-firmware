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
    ReplyCode_WRONG_CMD,
    ReplyCode_OK,
    ReplyCode_READY,
    ReplyCode_BUSY,

    NUM_OF_REPLY_CODES
} ReplyCode_t;

struct Reply {
    ReplyCode_t code;
};

void Protocol_ParseCommand(char *line, struct Command *cmd);
void Protocol_BuildReply(struct Reply *reply, char *buff);

#endif // _PROTOCOL_H