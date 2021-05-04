#include "wcetSampler.h"
#include "cli.h"
#include <stdio.h>

void wcetBegin(WCETSampler* self) {
    self->start = CURRENT_OFFSET();
}

void wcetEnd(WCETSampler* self) {
    Time end = CURRENT_OFFSET();
    if(self->timeSamples < self->requiredSamples) {

        Time elapsed = end - self->start;
        self->totalMeasuredTime += elapsed;

        if(self->timeSamples == 0) {
            self->smallestMeasuredTime = elapsed;
            self->largestMeasuredTime = elapsed;
        } else if(elapsed > self->largestMeasuredTime) {
            self->largestMeasuredTime = elapsed;
        } else if(elapsed < self->smallestMeasuredTime) {
            self->smallestMeasuredTime = elapsed;
        }

        self->timeSamples += 1;
    } else if (self->timeSamples == self->requiredSamples) {
        const int STRLEN = 200;
        char s[STRLEN];
        
        long best = USEC_OF(self->smallestMeasuredTime);
        long worst = USEC_OF(self->largestMeasuredTime);
        long average = USEC_OF(self->totalMeasuredTime) / self->timeSamples;

        snprintf(s, STRLEN, "WCET %s:\n- Best: %ldus\n- Worst: %ldµs\n- Average: %ldµs\n- Samples: %d\n- Resolution: 10µs", self->label, best, worst, average, self->timeSamples);
        SYNC(&cliHandler, printLine, (int)s);

        self->timeSamples += 1;
    }
}