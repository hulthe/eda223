#ifndef CPU_BANDIT_H
#define CPU_BANDIT_H

#include "TinyTimber.h"
#include "wcetSampler.h"
#include <stdint.h>

typedef struct {
    Object super;

    int backgroundLoopRange;
    int enableDeadlines;
    WCETSampler wcet;
} CPUBandit;

extern CPUBandit cpuBandit;

void doBusyWork(CPUBandit* self, int _);
void setLoad(CPUBandit* self, int newLoad);
int getLoad(CPUBandit* self, int _);
int toggleBanditDeadline(CPUBandit* self, int _);

#endif