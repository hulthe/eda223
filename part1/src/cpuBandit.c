#include "TinyTimber.h"
#include "cpuBandit.h"

CPUBandit cpuBandit = { initObject(), 1000 };


void doBusyWork(CPUBandit* self, int _) {
    for(int i = 0; i < self->backgroundLoopRange; i++) {}
    ASYNC(self, doBusyWork, NULL);
}

void modifyLoad(CPUBandit* self, int delta) {
    self->backgroundLoopRange += delta;
    if(self->backgroundLoopRange < 0) {
        self->backgroundLoopRange = 0;
    }
}