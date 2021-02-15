#ifndef TONE_GENERATOR_H
#define TONE_GENERATOR_H

#include "TinyTimber.h"
#include "wcetSampler.h"
#include <stdint.h>

typedef struct {
    Object super;

    WCETSampler wcet;
    Time period;
    int enableDeadlines;
    uint8_t volume;
    uint8_t mVol;
} ToneGenerator;

ToneGenerator toneGenerator;

int setGeneratorTonePeriod(ToneGenerator* self, int period_us);

int getGeneratorVolume(ToneGenerator* self, int _);
int setGeneratorVolume(ToneGenerator* self, int volume);
int muteGenerator(ToneGenerator* self, int _);
int toneGeneratorDeadline(ToneGenerator* self, int _);

void toneGeneratorPulse(ToneGenerator* self, int high);

#endif