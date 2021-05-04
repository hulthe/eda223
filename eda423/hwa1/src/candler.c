#include "candler.h"
#include "canTinyTimber.h"
#include "song.h"
#include "player.h"
#include "cli.h"
#include <stdio.h>

Candler candler = {initObject(), newWCETSampler("Candler", 30), 0, {0, 0, {}}, 1, 0};

#define ENABLE_CAN
#define REGULATOR
//#define MEASURE_REGULATOR_WCET
//#define MEASURE_POPBUFFER_WCET

CANBuffer canbuffer;

const uchar CAN_NODE_ID = 1;

// Command command
void sendCommand(Candler* self, int command_ptr) {
    Command* command = (Command*)command_ptr;

#ifdef ENABLE_CAN
    // send message on CAN-bus
    CANMsg msg;
    msg.msgId = self->nextMsgId++;
    msg.buff[0] = (uchar)command->kind;
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
    command.kind = (int)msg.buff[0];

    uint32_t n = 0;
    n |= ((uint32_t)msg.buff[1]) << 24;
    n |= ((uint32_t)msg.buff[2]) << 16;
    n |= ((uint32_t)msg.buff[3]) << 8;
    n |= ((uint32_t)msg.buff[4]);
    command.arg = (int)n;

    if(self->canId == 1) {
        char s[100];
        snprintf(s, 100, "CAN command delivered. msgId=%d kind=%d", msg.msgId, msg.buff[0]);
        SYNC(&cliHandler, printLine, (int)s);
    }

    execCommand(self, (int)&command);
}

void popBuffer(Candler* self, int slot) {
#ifdef MEASURE_POPBUFFER_WCET
    wcetBegin(&self->wcet);
#endif

    CANMsg msg = self->buffer.array[slot];
    self->buffer.length -= 1;

    deliverCanMsg(self, msg);

    if (self->buffer.length > 0) {
        AFTER(MIN_MESSAGE_PERIOD, self, popBuffer, (slot + 1) % CAN_BUFFER_CAPACITY);
    }

#ifdef MEASURE_POPBUFFER_WCET
    wcetEnd(&self->wcet);
#endif
}

void toggleCanId(Candler* self, int _) {
    self->canId =  !self->canId;
    if(self->canId) {
      ASYNC(&cliHandler, printLine, (int)"CAN ID enabled");
    }
    else {
      ASYNC(&cliHandler, printLine, (int)"CAN ID disabled");}
}

void recvCanMsg(Candler* self, int _) {
#ifdef MEASURE_REGULATOR_WCET
    wcetBegin(&self->wcet);
#endif

    // parse the can-msg into a Command struct
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);

    if(self->canId == 1) {
        char s[100];
        snprintf(s, 100, "CAN command arrived. msgId=%d kind=%d", msg.msgId, msg.buff[0]);
        SYNC(&cliHandler, printLine, (int)s);
    }

    // if board is in slave-mode, execute the command
    if (!self->leader) {
        Timer baseline0 = initTimer();
        Time now = T_SAMPLE(&baseline0);
        Time target = self->lastMsgRecieved + MIN_MESSAGE_PERIOD;

        if (now >= target) {
            deliverCanMsg(self, msg);
            self->lastMsgRecieved = now;
        } else if (self->buffer.length >= CAN_BUFFER_CAPACITY) {
            // no more space in buffer
            SYNC(&cliHandler, printLine, (int)"Message buffer is full");
        } else {
            int index = self->buffer.currPlace;
            self->buffer.currPlace = (index + 1) % CAN_BUFFER_CAPACITY;
            self->buffer.array[index] = msg;
            self->lastMsgRecieved = target;

            if (self->buffer.length++ == 0) {
                AFTER(target - now, self, popBuffer, index);
            }
        }
    }

#ifdef MEASURE_REGULATOR_WCET
    wcetEnd(&self->wcet);
#endif
}

int toggleLeaderMode(Candler* self, int _) {
    self->leader = !self->leader;
    return self->leader;
}