#ifndef CAN_H
#define CAN_H

#include "canTinyTimber.h"


// Protocol:
//
// | msgid      | byte0 |
// | ---------- | ----- | 
// | PLAY_START | xxxxx | 
// | PLAY_STOP  | xxxxx |
// | MUTE       | xxxxx |
// | UNMUT      | xxxxx |
// | SET_VOLUME | value |
// | SET_TEMPO  | value |
// | SET_KEY    | value |

const uchar CAN_NODE_ID = 1;

const uchar CAN_MSG_PLAY_START = 1;
const uchar CAN_MSG_PLAY_STOP = 2;
const uchar CAN_MSG_MUTE = 3;
const uchar CAN_MSG_UNMUTE = 4;
const uchar CAN_MSG_SET_VOLUME = 5;
const uchar CAN_MSG_SET_TEMPO = 6;
const uchar CAN_MSG_SET_KEY = 7;

#endif