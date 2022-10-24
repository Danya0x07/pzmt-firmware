#include <protocol.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUPPORTED_FREQUENCY 20000
#define MAX_SUPPORTED_DURATION 20000

static enum RequestType _ParseRequestType(char firstChar)
{
    if (firstChar >= '0' && firstChar <= '9') {
        return RequestType_PlayFiniteTone;
    }
    switch (firstChar) {
        case 'v':
        case 'V':
            return RequestType_SetVolume;
        default:
            return RequestType_Undefined;
    }
}

void Protocol_ParseRequest(char *line, struct Request *request)
{
    uint16_t frequency = 0, duration = 0;

    request->type = _ParseRequestType(line[0]);

    if (request->type == RequestType_SetVolume) {
        request->content.volumeRaised = line[0] == 'V';
    }
    else if (request->type == RequestType_PlayFiniteTone) {
        frequency = atoi(line);
        line = strchr(line, ',');
        if (line) {
            duration = atoi(++line);
        }
        if (frequency > MAX_SUPPORTED_FREQUENCY || duration > MAX_SUPPORTED_DURATION) {
            request->type = RequestType_Undefined;
        }
        else if (duration == 0) {
            request->type = frequency == 0 ? RequestType_StopPlaying : RequestType_PlayInfiniteTone;
        }
        request->content.frequency = frequency;
        request->content.duration = duration;
    }    
}

void Protocol_BuildResponse(struct Response *response, char *buff)
{
    buff[0] = response->type == ResponseType_Acknowledge ? '0' : '1';
    buff[1] = '\n';
    buff[2] = '\0';
}