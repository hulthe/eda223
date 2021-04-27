#ifndef CAN_MESS_SENDER_H
#define CAN_MESS_SENDER_H

#include "TinyTimber.h"

typedef struct {
    Object super;
    Time period;
    int startId;
} CanMessSender;

#define newCanMessSender(period) \
    { initObject(), period, 0 }

void canMessSenderStart(CanMessSender*, int _);
void canMessSenderStop(CanMessSender*, int _);

#endif