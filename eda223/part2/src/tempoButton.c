#include "tempoButton.h"
#include "cli.h"
#include "candler.h"
#include <stdlib.h>
#include <stdio.h>

TempoButton tempoButton = {initObject(), initTimer(), {0}, 0};

int timeToBPM(Time);

void tempoButtonPush(TempoButton* self, int _) {
    if(self->pushCount == 0) {
        T_RESET(&self->pushTimer);
        self->pushCount += 1;
        return;
    }
    
    Time now = T_SAMPLE(&self->pushTimer);

    // filter contact bounces
    Time sinceLast = now - self->samples[self->pushCount - 1];
    if (sinceLast < FILTER_THRESHOLD) {
        ASYNC(&cliHandler, printLine, (int)"Ignoring contact bounce");
        return;
    }

    // save sample
    self->samples[self->pushCount] = now;

    //char s[100];
    //snprintf(s, 100, "Button sample at %ld.", now);
    //SYNC(&cliHandler, printLine, (int)s);

    // if we have collected all the samples we need
    // compute the diff between each pair of button presses
    if (++self->pushCount >= TARGET_PUSH_COUNT) {
        self->pushCount = 0;

        Time average = 0;
        Time diffs[TARGET_PUSH_COUNT - 1];
        for (int i = 0; i < TARGET_PUSH_COUNT - 1; i++) {
            diffs[i] = self->samples[i + 1] - self->samples[i];
            average += diffs[i];

            // check that no diff varied more than FILTER_THRESHOLD
            for (int j = 0; j < i; j++) {
                if (abs(diffs[j] - diffs[i]) >= FILTER_THRESHOLD) {
                    ASYNC(&cliHandler, printLine, (int)"Tempo button presses varied by more than 100ms. Ignoring.");
                    return;
                }
            }
        }

        average /= (TARGET_PUSH_COUNT - 1);
        int bpm = timeToBPM(average);

        if (bpm >= 30 && bpm <= 300) {
            char s[100];
            snprintf(s, 100, "Nice beat. Setting BPM to %d.", bpm);
            SYNC(&cliHandler, printLine, (int)s);

            Command command = {CMD_SET_TEMPO, bpm};
            SYNC(&candler, sendCommand, (int)&command);
        } else {
            ASYNC(&cliHandler, printLine, (int)"BPM out of range [30..300]");
        }
    }
}

int timeToBPM(Time time) {
    return SEC(60) / time;
}