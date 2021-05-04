#ifndef LFO_H
#define LFO_H

#include "TinyTimber.h"

typedef enum {
    SquareWave=1, SawtoothWave, SinusWave
} Waveform;

typedef enum {
    Volume=1, Tempo, Period
} LFOParam;

typedef struct {
    Timer start;
    Time period;
    int intensity;
    Waveform waveform;
    LFOParam param;
} LFO;

#define newLFO() \
    { initTimer(), SEC(1), 0, SinusWave, Volume }

typedef struct {
    Object super;
    LFO lfo;
} LFOObject;

#define newLFOObject() \
    { initObject(), newLFO() }

extern LFOObject lfOscillator;

void setLFOIntensity(LFOObject* self, int inten);
void setLFOWaveform(LFOObject* self, int wave);
void setLFOPeriod(LFOObject* self, int period);
void setLFOParameter(LFOObject* self, int param);

// LFO* out
//
// Copy the current LFO to *out
void copyLFO(LFOObject* self, int out);

// Not an object method, can't be called through TinyTimber
int lfoModulate(LFO* self, int value);

#endif