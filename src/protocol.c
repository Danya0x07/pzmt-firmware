#include <stdlib.h>
#include <string.h>

#include <protocol.h>

#ifndef PIO_UNIT_TESTING
#   include <avr/pgmspace.h>
#else
#   define strcpy_P(dst, src) strcpy(dst, src)
#   define PROGMEM
#endif // PIO_UNIT_TESTING

static const PROGMEM char STARTUP_MSG[] = "PzMt Firmware v" FIRMWARE_VERSION "\n";

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

void Protocol_BuildStartupMsg(char *buff)
{
    strcpy_P(buff, STARTUP_MSG);
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

void Protocol_BuildReply(const struct Reply *reply, char *buff)
{
    buff[0] = (char)reply->code;
    buff[1] = '\n';
    buff[2] = '\0';
}