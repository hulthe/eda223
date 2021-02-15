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
            
            Time retWTime = wcetGetWorst(self);
            Time retATime = wcetGetAverage(self);
            snprintf(s, STRLEN, "WCET:\nWorst: %d\nAverage: %d", USEC_OF(retWTime), USEC_OF(retATime));
            SYNC(&testtesttest, printLine, (int)s);
            //SYNC(&testtesttest, printLine, (int)self->label);

            self->timeSamples += 1;
        }
    }
    
}

int wcetGetWorst(WCETSampler* self) {
    return self->largestMeasuredTime;
}

int wcetGetAverage(WCETSampler* self) {
    if(self->timeSamples == 0) {
        return 0;
    } else {
        return (int)((double)self->totalMeasuredTime / (double)self->timeSamples);
    }
}