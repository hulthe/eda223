#ifndef WCET_SAMPLER_H
#define WCET_SAMPLER_H

#include "TinyTimber.h"

typedef struct {
    const char* label;
    Time start;
    Time totalMeasuredTime;
    Time largestMeasuredTime;
    int timeSamples;
} WCETSampler;

#define newWCETSampler(label) { label, 0, 0, 0, 0 }

void wcetBegin(WCETSampler* self);
void wcetEnd(WCETSampler* self);

#endif