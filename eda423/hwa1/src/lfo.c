#include "lfo.h"
#include "cli.h"
#include "player.h"
#include "toneGenerator.h"
#include <stdio.h>

//#define LFO_PRINT_SAMPLES

LFOObject lfOscillator = newLFOObject();

#define SIN_STEPS 16
#define SAW_STEPS 15

int SIN_WAVE_TABLE[SIN_STEPS];
int SAW_WAVE_TABLE[SAW_STEPS];

int lfoModulate(LFOSample* self, int value) {
    return value * (100 + self->mod) / 100;
}

void setLFOIntensity(LFOObject* self, int inten) {
    self->intensity = inten;
}

void setLFOWaveform(LFOObject* self, int wave) {
    switch (wave) {
        case 1:;
            self->waveform = SquareWave;
            break;
        case 2:;
            self->waveform = SawtoothWave;
            break;
        case 3:;
            self->waveform = SinusWave;
            break;
        default:;
            break;
    }
}

void setLFOPeriod(LFOObject* self, int period) {
    self->period = period;
}

void setLFOParameter(LFOObject* self, int param) {
    switch (param) {
        case 1:;
            self->param = Volume;
            break;
        case 2:;
            self->param = Tempo;
            break;
        case 3:;
            self->param = Period;
            break;
        default:;
            break;
    }
}

void sendLFOSample(LFOObject* self, LFOSample sample) {
    sample.param = self->param;
    sample.mod = sample.mod * self->intensity / 100;

#ifdef LFO_PRINT_SAMPLES
    char s[100];
    snprintf(s, 100, "LFO Sample. param=%d mod=%d%%", sample.param, sample.mod);
    SYNC(&cliHandler, printLine, (int)s);
#endif

    SYNC(&player, setPlayerLFO, (int)&sample);
    SYNC(&toneGenerator, setGeneratorLFO, (int)&sample);
}

void lfoSinWave(LFOObject* self, int progress) {
    if (progress < SIN_STEPS) {
        AFTER(self->period / SIN_STEPS, self, lfoSinWave, progress + 1);
    } else {
        AFTER(self->period / SIN_STEPS, self, lfoLoop, 0);
    }

    LFOSample out;
    out.mod = SIN_WAVE_TABLE[progress];

    sendLFOSample(self, out);
}

void lfoSawWave(LFOObject* self, int progress) {
    if (progress < SAW_STEPS) {
        AFTER(self->period / SAW_STEPS, self, lfoSawWave, progress + 1);
    } else {
        AFTER(self->period / SAW_STEPS, self, lfoLoop, 0);
    }

    LFOSample out;
    out.mod = SAW_WAVE_TABLE[progress];

    sendLFOSample(self, out);
}

void lfoSetSample(LFOObject* self, int mod) {
    LFOSample out;
    out.mod = mod;
    sendLFOSample(self, out);
}

void lfoLoop(LFOObject* self, int _) {
    switch (self->waveform) {
        case SquareWave:;
            ASYNC(self, lfoSetSample, -100);
            AFTER(self->period / 2, self, lfoSetSample, 100);
            AFTER(self->period, self, lfoLoop, 0);
            break;
        case SawtoothWave:;
            ASYNC(self, lfoSawWave, 0);
            break;
        case SinusWave:;
            ASYNC(self, lfoSinWave, 0);
            break;
        default:;
            ASYNC(&cliHandler, printLine, (int)"Unknown LFO param");
            AFTER(self->period, self, lfoLoop, 0);
            // do nothing
    }
}

int SIN_WAVE_TABLE[SIN_STEPS] = { 0, 40, 74, 95, 99, 86, 58, 20, -20, -58, -86, -99, -95, -74, -40 };
int SAW_WAVE_TABLE[SAW_STEPS] = { -100, -85, -71, -57, -42, -28, -14, 0, 14, 28, 42, 57, 71, 85, 100 };
