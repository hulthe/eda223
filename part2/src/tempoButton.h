#ifndef TEMPO_BUTTON_H
#define TEMPO_BUTTON_H

#include "TinyTimber.h"

#define FILTER_THRESHOLD MSEC(100)
#define TARGET_PUSH_COUNT 4

typedef struct {
    Object super;
    Timer pushTimer;
    Time samples[TARGET_PUSH_COUNT];
    uint8_t pushCount;
} TempoButton;

extern TempoButton tempoButton;

void tempoButtonPush(TempoButton*, int _);

#endif