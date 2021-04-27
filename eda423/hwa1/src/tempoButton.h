#ifndef TEMPO_BUTTON_H
#define TEMPO_BUTTON_H

#include "TinyTimber.h"

typedef struct {
    Object super;
    int firstPress;
    int pressId;
    Timer tapTimer;
    Time pushDelay;
} TempoButton;

#define newTempoButton() \
    (TempoButton){ initObject(), 1, 0, initTimer(), 0 }

void tempoButtonTap(TempoButton*, int _);
void tempoButtonReset(TempoButton*, int _);


#endif