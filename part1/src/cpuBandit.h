#ifndef CPU_BANDIT_H
#define CPU_BANDIT_H

#include "TinyTimber.h"
#include <stdint.h>

typedef struct {
    Object super;

    int backgroundLoopRange;
} CPUBandit;

CPUBandit cpuBandit;

void doBusyWork(CPUBandit* self, int _);
void modifyLoad(CPUBandit* self, int delta);

#endif