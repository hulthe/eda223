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
    int mod;
    LFOParam param;
} LFOSample;

#define newLFOSample() { 0 }

typedef struct {
    Object super;
    Timer start;
    Time period;
    int intensity;
    Waveform waveform;
    LFOParam param;
} LFOObject;

#define newLFOObject() \
    { initObject(), initTimer(), SEC(1), 0, SinusWave, Volume }

extern LFOObject lfOscillator;

void setLFOIntensity(LFOObject* self, int inten);
void setLFOWaveform(LFOObject* self, int wave);
void setLFOPeriod(LFOObject* self, int period);
void setLFOParameter(LFOObject* self, int param);

void lfoLoop(LFOObject* self, int _);

// Not an object method, can't be called through TinyTimber
int lfoModulate(LFOSample* self, int value);

#endif