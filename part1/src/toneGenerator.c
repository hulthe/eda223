#include "TinyTimber.h"
#include "toneGenerator.h"

#define DAC_OUT (*((volatile uint8_t*) 0x4000741C))
#define MEASURE_WCET

//ToneGenerator toneGenerator = { initObject(), USEC(1136), 10, 0 };
ToneGenerator toneGenerator = { initObject(), newWCETSampler("toneGenerator"), USEC(1000), 0, 4, 0 };

int setGeneratorTonePeriod(ToneGenerator* self, int period_us) {
    if(period_us > 0) {
        self->period = USEC(period_us);
        return 0;
    } else {
        return -1;
    }
}

int getGeneratorVolume(ToneGenerator* self, int _) {
    return self->volume;
}

int muteGenerator(ToneGenerator* self, int _) {
    if (self->volume != 0) {
        self->mVol = self->volume;
        self->volume = 0;
    } else {
        self->volume = self->mVol;
        self->mVol = 0;
    }
    return 0;
}

int setGeneratorVolume(ToneGenerator* self, int volume) {
    if ((volume > 0) && (volume <= 10)) {
        self->volume = volume;
        return 0;
    } else {
        return -1;
    }
}

int toneGeneratorDeadline(ToneGenerator* self, int _) {
    self->enableDeadlines = !self->enableDeadlines;
    return self->enableDeadlines;
}

void toneGeneratorPulse(ToneGenerator* self, int high) {
    Time dl = NULL;
    if(self->enableDeadlines) {
        dl = USEC(100);
    }
    SEND(self->period, dl, self, toneGeneratorPulse, !high);

#ifdef MEASURE_WCET
    wcetBegin(&self->wcet);
#endif

    int dac_value;
    if (high) {
        dac_value = 0;
    } else {
        dac_value = self->volume;
    }

    DAC_OUT = dac_value;

#ifdef MEASURE_WCET
    wcetEnd(&self->wcet);
#endif
}