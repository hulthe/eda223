#ifndef TONE_GENERATOR_H
#define TONE_GENERATOR_H

#include "TinyTimber.h"
#include "wcetSampler.h"
#include "lfo.h"
#include <stdint.h>

typedef struct {
    Object super;
    WCETSampler wcet;
    LFO lfo;
    Time period;
    int enableDeadlines;
    uint8_t volume;
    uint8_t muted;
} ToneGenerator;

#define newToneGenerator() \
    { initObject(), newWCETSampler("Tone Generator", 1000), newLFO(), USEC(1000), 1, 0, 3 };

extern ToneGenerator toneGenerator;

int setGeneratorTonePeriod(ToneGenerator* self, int period_us);

int getGeneratorVolume(ToneGenerator* self, int _);
int setGeneratorVolume(ToneGenerator* self, int volume);
void muteGenerator(ToneGenerator* self, int _);
void unmuteGenerator(ToneGenerator* self, int _);
int toneGeneratorDeadline(ToneGenerator* self, int _);

// LFO* lfo;
void setGeneratorLFO(ToneGenerator* self, int lfo);

void toneGeneratorPulse(ToneGenerator* self, int high);

#endif