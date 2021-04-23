#ifndef TEMPO_BUTTON_H
#define TEMPO_BUTTON_H

#include "TinyTimber.h"
#include "sioTinyTimber.h"

#define FILTER_THRESHOLD MSEC(100)

#define PRESSED 0
#define RELEASED 1

typedef struct {
    Object super;
    SysIO* sio;
    int trigger;
    int firstPress;
    int pressId;
    Timer clickTimer;
    Timer pressedTimer;
    Time pushDelay;
} TempoButton;


#define newTempoButton(sio) \
    {initObject(), (SysIO*)sio, PRESSED, 1, 0, initTimer(), initTimer(), 0}

//extern TempoButton tempoButton;

void initTempoButton(TempoButton*, int _);
void tempoButtonTrig(TempoButton*, int _);

#endif