#include "candler.h"
#include "canTinyTimber.h"
#include "song.h"
#include "player.h"
#include "cli.h"
#include <stdio.h>

Candler candler = {initObject(), 1};

const uchar CAN_NODE_ID = 1;

// Command command
void sendCommand(Candler* self, int command_ptr) {
    Command* command = (Command*)command_ptr;

    // send message on CAN-bus
    CANMsg msg;
    msg.msgId = command->kind;
    msg.nodeId = CAN_NODE_ID;
    msg.length = 1;
    msg.buff[0] = command->arg;
    CAN_SEND(&can0, &msg);

    // if we are the leader, execute this command directly
    if(self->leader) {
        recvCommand(self, command_ptr);
    }
}

// Command command
void recvCommand(Candler* self, int command_ptr) {
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
        ASYNC(&player, setPlayerKey, (int8_t)command->arg);
        break;
    }
}

void recvCanMsg(Candler* self, int _) {
    // parse the can-msg into a Command struct
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);

    char s[100];
    snprintf(s, 100, "CAN command received: %d", msg.msgId);
    SYNC(&cliHandler, printLine, (int)s);

    // if board is in slave-mode, execute the command
    if (!self->leader) {
        Command command;
        command.kind = msg.msgId;
        command.arg = msg.buff[0];

        recvCommand(self, (int)&command);
    }
}

int toggleLeaderMode(Candler* self, int _) {
    self->leader = !self->leader;
    return self->leader;
}