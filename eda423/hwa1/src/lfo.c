#include "lfo.h"
#include "cli.h"

LFOObject lfOscillator = newLFOObject();

int SIN_WAVE_TABLE[100];

int lfoMod(LFO* self) {
    Time now = T_SAMPLE(&self->start);

    Time inWave = now % self->period;
    int waveProgress = inWave * 100 / self->period;

    switch (self->waveform) {
    case SquareWave:;
        if (waveProgress < 50) {
            return -100;
        } else {
            return 100;
        }
        break;

    case SawtoothWave:;
        return -100 + 2 * waveProgress;

    case SinusWave:;
        return SIN_WAVE_TABLE[waveProgress];
    
    default:;
        ASYNC(&cliHandler, printLine, (int)"ERROR: invalid LFO waveform specified.");
        return 0;
    }
}

int lfoModulate(LFO* self, int volume) {
    int intesityMod = lfoMod(self) * self->intensity / 100;
    return volume * (100 + intesityMod) / 100;
}

void setLFOIntensity(LFOObject* self, int inten) {
    self->lfo.intensity = inten;
}

void setLFOWaveform(LFOObject* self, int wave) {
    switch (wave) {
        case 1:;
            self->lfo.waveform = SquareWave;
            break;
        case 2:;
            self->lfo.waveform = SawtoothWave;
            break;
        case 3:;
            self->lfo.waveform = SinusWave;
            break;
        default:;
            break;
    }
}

void setLFOPeriod(LFOObject* self, int period) {
    self->lfo.period = period;
}

void setLFOParameter(LFOObject* self, int param) {
    switch (param) {
        case 1:;
            self->lfo.param = Volume;
            break;
        case 2:;
            self->lfo.param = Tempo;
            break;
        case 3:;
            self->lfo.param = Period;
            break;
        default:;
            break;
    }
}

void copyLFO(LFOObject* self, int out_ptr) {
    LFO* out = (LFO*)out_ptr;
    *out = self->lfo;
}

int SIN_WAVE_TABLE[100] = {
    0,
    6,
    12,
    18,
    24,
    30,
    36,
    42,
    48,
    53,
    58,
    63,
    68,
    72,
    77,
    80,
    84,
    87,
    90,
    92,
    95,
    96,
    98,
    99,
    99,
    100,
    99,
    99,
    98,
    96,
    95,
    92,
    90,
    87,
    84,
    80,
    77,
    72,
    68,
    63,
    58,
    53,
    48,
    42,
    36,
    30,
    24,
    18,
    12,
    6,
    0,
    -6,
    -12,
    -18,
    -24,
    -30,
    -36,
    -42,
    -48,
    -53,
    -58,
    -63,
    -68,
    -72,
    -77,
    -80,
    -84,
    -87,
    -90,
    -92,
    -95,
    -96,
    -98,
    -99,
    -99,
    -100,
    -99,
    -99,
    -98,
    -96,
    -95,
    -92,
    -90,
    -87,
    -84,
    -80,
    -77,
    -72,
    -68,
    -63,
    -58,
    -53,
    -48,
    -42,
    -36,
    -30,
    -24,
    -18,
    -12,
    -6,
};
