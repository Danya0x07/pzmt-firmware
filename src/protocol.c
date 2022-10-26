#include <protocol.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUPPORTED_FREQUENCY 20000
#define MAX_SUPPORTED_DURATION 20000

static CommandType_t ParseCommandType(char firstChar)
{
    if (firstChar >= '0' && firstChar <= '9') {
        return CommandType_PLAY_FINITE_TONE;
    }
    switch (firstChar) {
        case 'v':
        case 'V':
            return CommandType_SET_VOLUME;
        default:
            return CommandType_UNRECOGNIZABLE;
    }
}

void Protocol_ParseCommand(char *line, struct Command *cmd)
{
    uint16_t frequency = 0, duration = 0;

    cmd->type = ParseCommandType(line[0]);

    if (cmd->type == CommandType_SET_VOLUME) {
        cmd->params.volumeRaised = line[0] == 'V';
    }
    else if (cmd->type == CommandType_PLAY_FINITE_TONE) {
        frequency = atoi(line);
        line = strchr(line, ',');
        if (line) {
            duration = atoi(++line);
        }
        if (frequency > MAX_SUPPORTED_FREQUENCY || duration > MAX_SUPPORTED_DURATION) {
            cmd->type = CommandType_UNRECOGNIZABLE;
        }
        else if (duration == 0) {
            cmd->type = frequency == 0 ? 
                CommandType_STOP_PLAYING : CommandType_PLAY_INFINITE_TONE;
        }
        cmd->params.frequency = frequency;
        cmd->params.duration = duration;
    }    
}

void Protocol_BuildReply(struct Reply *response, char *buff)
{
    buff[0] = response->code == ReplyCode_OK ? '0' : '1';
    buff[1] = '\n';
    buff[2] = '\0';
}