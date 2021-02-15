#include "TinyTimber.h"
#include "cpuBandit.h"

#define MEASURE_WCET

//CPUBandit cpuBandit = { initObject(), 1000, 0 };
CPUBandit cpuBandit = { initObject(), 13000, 0, newWCETSampler("cpuBandit") };

const Time PERIOD = USEC(1300);

void doBusyWork(CPUBandit* self, int _) {
    Time dl = NULL;
    if(self->enableDeadlines) {
        dl = PERIOD;
    }
    SEND(PERIOD, dl, self, doBusyWork, NULL);

#ifdef MEASURE_WCET
    wcetBegin(&self->wcet);
#endif

    for(int i = 0; i < self->backgroundLoopRange; i++) {}

#ifdef MEASURE_WCET
    wcetEnd(&self->wcet);
#endif
}

void setLoad(CPUBandit* self, int load) {
    self->backgroundLoopRange = load;
    if(self->backgroundLoopRange < 0) {
        self->backgroundLoopRange = 0;
    }
}

int getLoad(CPUBandit* self, int _) {
    return self->backgroundLoopRange;
}

int toggleBanditDeadline(CPUBandit* self, int _) {
    self->enableDeadlines = !self->enableDeadlines;
    return self->enableDeadlines;
}