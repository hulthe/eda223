#ifndef WCET_SAMPLER_H
#define WCET_SAMPLER_H

#include "TinyTimber.h"

typedef struct {
    const char* label;
    unsigned int requiredSamples;
    unsigned int timeSamples;
    Time start;
    Time totalMeasuredTime;
    Time largestMeasuredTime;
    Time smallestMeasuredTime;
} WCETSampler;

#define newWCETSampler(label, requiredSamples) { label, requiredSamples, 0, 0, 0, 0, 0 }

void wcetBegin(WCETSampler* self);
void wcetEnd(WCETSampler* self);

#endif