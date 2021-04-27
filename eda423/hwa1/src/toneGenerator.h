#ifndef TONE_GENERATOR_H
#define TONE_GENERATOR_H

#include "TinyTimber.h"
#include <stdint.h>

typedef struct {
    Object super;

    Time period;
    int enableDeadlines;
    uint8_t volume;
    uint8_t muted;
} ToneGenerator;

extern ToneGenerator toneGenerator;

int setGeneratorTonePeriod(ToneGenerator* self, int period_us);

int getGeneratorVolume(ToneGenerator* self, int _);
int setGeneratorVolume(ToneGenerator* self, int volume);
void muteGenerator(ToneGenerator* self, int _);
void unmuteGenerator(ToneGenerator* self, int _);
int toneGeneratorDeadline(ToneGenerator* self, int _);

void toneGeneratorPulse(ToneGenerator* self, int high);

#endif