#ifndef CANDLER_H
#define CANDLER_H

#include "TinyTimber.h"
#include "canTinyTimber.h"

#define CMD_PLAY_START 1
#define CMD_PLAY_STOP 2
#define CMD_MUTE 3
#define CMD_UNMUTE 4
#define CMD_SET_VOLUME 5
#define CMD_SET_TEMPO 6
#define CMD_SET_KEY 7
#define CMD_HELLO 8

typedef struct {
    int kind;
    int arg;
} Command;

#define MIN_MESSAGE_PERIOD SEC(1)

#define CAN_BUFFER_CAPACITY 10

typedef struct {
    int currPlace;
    int length;
    CANMsg array[CAN_BUFFER_CAPACITY];
} CANBuffer;

typedef struct {
    Object super;
    Time lastMsgRecieved;
    CANBuffer buffer;
    uint8_t leader;
    uint8_t nextMsgId;
    int SeqNum;
} Candler;


extern Candler candler;

extern Can can0;

// Dispatch a command
// the command will be sent to the CAN-bus
// and also directly to the local machine if the board is in leader-mod
//
// Command command
void sendCommand(Candler* self, int command);

// Executes a command on the local board
//
// Command command
void execCommand(Candler* self, int command);

void recvCanMsg(Candler* self, int _);

// returns true if the candler is now in leader mode
int toggleLeaderMode(Candler* self, int _);

void toggleSeqNum(Candler*, int _);


#endif