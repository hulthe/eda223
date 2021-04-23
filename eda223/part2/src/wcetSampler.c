#include "wcetSampler.h"
#include "cli.h"
#include <stdio.h>

const int NUM_SAMPLES = 500;

void wcetBegin(WCETSampler* self) {
    self->start = CURRENT_OFFSET();
}

void wcetEnd(WCETSampler* self) {
    Time end = CURRENT_OFFSET();
    if(self->timeSamples < NUM_SAMPLES) {
        self->timeSamples += 1;

        Time elapsed = end - self->start;
        self->totalMeasuredTime += elapsed;
        if(elapsed > self->largestMeasuredTime) {
            self->largestMeasuredTime = elapsed;
        }
    } else {
        if (self->timeSamples == NUM_SAMPLES) {
            const int STRLEN = 100;
            char s[STRLEN];
            
            long worst = USEC_OF(self->largestMeasuredTime);
            long average = USEC_OF(self->totalMeasuredTime) / self->timeSamples;

            snprintf(s, STRLEN, "WCET %s:\nWorst: %ld\nAverage: %ld\nSamples: %d", self->label, worst, average, self->timeSamples);
            SYNC(&cliHandler, printLine, (int)s);

            self->timeSamples += 1;
        }
    }
}