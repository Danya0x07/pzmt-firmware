#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <main.h>

enum RequestType {
    RequestType_Undefined,
    RequestType_SetVolume,
    RequestType_PlayFiniteTone,
    RequestType_PlayInfiniteTone,
    RequestType_StopPlaying
};

struct Request {
    enum RequestType type;
    union {
        struct {
            uint16_t frequency;
            uint16_t duration;
        };
        bool volumeRaised;
    } content;
};

enum ResponseType {
    ResponseType_BadRequest,
    ResponseType_Acknowledge
};

struct Response {
    enum ResponseType type;
};

void Protocol_ParseRequest(char *line, struct Request *request);
void Protocol_BuildResponse(struct Response *response, char *buff);

#endif // _PROTOCOL_H