#include "candler.h"
#include "canTinyTimber.h"
#include "song.h"
#include "player.h"
#include "cli.h"
#include <stdio.h>

Candler candler = {initObject(), 0, {0, 0, {}}, 1, 0};

#define ENABLE_CAN
#define REGULATOR

CANBuffer canbuffer;

const uchar CAN_NODE_ID = 1;

// Command command
void sendCommand(Candler* self, int command_ptr) {
    Command* command = (Command*)command_ptr;

#ifdef ENABLE_CAN
    // send message on CAN-bus
    CANMsg msg;
    msg.msgId = self->nextMsgId++;
    msg.buff[0] = command->kind;
    msg.nodeId = CAN_NODE_ID;
    msg.length = 5;

    uint32_t n = command->arg;
    msg.buff[1] = (n >> 24) & 0xFF;
    msg.buff[2] = (n >> 16) & 0xFF;
    msg.buff[3] = (n >>  8) & 0xFF;
    msg.buff[4] =  n        & 0xFF;

    CAN_SEND(&can0, &msg);
#endif

    // if we are the leader, execute this command directly
    if(self->leader) {
        execCommand(self, command_ptr);
    }
}

// Command command
void execCommand(Candler* self, int command_ptr) {
    // execute the command, calling the relevant methods on Player
    Command* command = (Command*)command_ptr;

    switch (command->kind) {
    case CMD_PLAY_START:;
        if(command->arg >= 0 && command->arg < SONG_LIST_LEN) {
            Song* song = SONG_LIST[command->arg];
            ASYNC(&player, playerPlay, song);
        }
        break;
    case CMD_PLAY_STOP:;
        ASYNC(&player, playerStop, NULL);
        break;

    case CMD_MUTE:;
        ASYNC(&player, mutePlayer, NULL);
        break;

    case CMD_UNMUTE:;
        ASYNC(&player, unmutePlayer, NULL);
        break;

    case CMD_SET_VOLUME:;
        ASYNC(&player, setPlayerVolume, command->arg);
        break;

    case CMD_SET_TEMPO:;
        ASYNC(&player, setPlayerTempo, command->arg);
        break;
        
    case CMD_SET_KEY:;
        ASYNC(&player, setPlayerKey, command->arg);
        break;

    case CMD_HELLO:;
        /* dummy message, doesn't do anything */
        ASYNC(&cliHandler, printLine, (int)"Hello!");
        break;
    }
}

void deliverCanMsg(Candler* self, CANMsg msg) {
    Command command;
    command.kind = msg.buff[0];

    uint32_t n = 0;
    n |= ((uint32_t)msg.buff[1]) << 24;
    n |= ((uint32_t)msg.buff[2]) << 16;
    n |= ((uint32_t)msg.buff[3]) << 8;
    n |= ((uint32_t)msg.buff[4]);
    command.arg = (int)n;

    execCommand(self, (int)&command);
}

void popBuffer(Candler* self, int slot) {
    SYNC(&cliHandler, printLine, (int)"Buffer will be popped");
    CANMsg msg = self->buffer.array[slot];
    self->buffer.length--;

    deliverCanMsg(self, msg);
    SYNC(&cliHandler, printLine, (int)"Buffer have been popped");
}

void recvCanMsg(Candler* self, int _) {
    // parse the can-msg into a Command struct
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);

    char s[100];
    snprintf(s, 100, "CAN command received. msgId=%d kind=%d", msg.msgId, msg.buff[0]);
    SYNC(&cliHandler, printLine, (int)s);

    // if board is in slave-mode, execute the command
    if (!self->leader) {
        Timer baseline0 = initTimer();
        Time now = T_SAMPLE(&baseline0);
        Time target = self->lastMsgRecieved + MIN_MESSAGE_PERIOD;

        char s[100];
        snprintf(s, 100, "now=%d.%03d target=%d.%03d", SEC_OF(now), MSEC_OF(now), SEC_OF(target), MSEC_OF(target));
        SYNC(&cliHandler, printLine, (int)s);

        if (now >= target) {
            SYNC(&cliHandler, printLine, (int)"Message will be handled now");
            deliverCanMsg(self, msg);
            self->lastMsgRecieved = now;
            SYNC(&cliHandler, printLine, (int)"Message has been handled");
        } else if (self->buffer.length >= CAN_BUFFER_CAPACITY) {
            // no more space in buffer
        } else {
            SYNC(&cliHandler, printLine, (int)"Messaged will be added to buffer");
            int index = self->buffer.currPlace;
            self->buffer.currPlace = (index + 1) % CAN_BUFFER_CAPACITY;
            self->buffer.length += 1;
            self->buffer.array[index] = msg;
            self->lastMsgRecieved = target;
            SYNC(&cliHandler, printLine, (int)"Message added");
            AFTER(target - now, self, popBuffer, index);
            SYNC(&cliHandler, printLine, (int)"Popbuffer scheduled");
        }
    }
}

int toggleLeaderMode(Candler* self, int _) {
    self->leader = !self->leader;
    return self->leader;
}