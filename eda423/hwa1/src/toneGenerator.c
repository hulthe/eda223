#include "TinyTimber.h"
#include "toneGenerator.h"

#define DAC_OUT (*((volatile uint8_t*) 0x4000741C))

// uncomment to measure execution time
//#define MEASURE_WCET

ToneGenerator toneGenerator = { initObject(), USEC(1000), 1, 0, 3 };

int setGeneratorTonePeriod(ToneGenerator* self, int period_us) {
    if(period_us > 0) {
        self->period = USEC(period_us);
        return 0;
    } else {
        return -1;
    }
}

int getGeneratorVolume(ToneGenerator* self, int _) {
    if (self->muted) {
        return 0;
    } else {
        return self->volume;
    }
}

void muteGenerator(ToneGenerator* self, int _) {
    self->muted = 1;    
}

void unmuteGenerator(ToneGenerator* self, int _) {
    self->muted = 0;
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
    // This function runs too fast to measure, so we use the
    // loop to to scale up the execution by a factor of 1000
    //for(int i = 0; i < 1000; i++) {
#endif

    int dac_value;
    if (high) {
        dac_value = 0;
    } else {
        dac_value = getGeneratorVolume(self, 0);
    }

    DAC_OUT = dac_value;

#ifdef MEASURE_WCET
    //}
    wcetEnd(&self->wcet);
#endif
}