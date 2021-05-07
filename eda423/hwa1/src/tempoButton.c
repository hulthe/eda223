#include "tempoButton.h"
#include "cli.h"
#include "candler.h"
#include <stdio.h>


int timeToBPM(Time time) {
    return SEC(60) / time;
}

void tempoButtonTap(TempoButton* self, int _) {
    SYNC(&cliHandler, printLine, (int)"tempoButtonTap");

    if(self->firstPress) {
        T_RESET(&self->tapTimer);
        self->firstPress = 0;
        return;
    }

    self->firstPress = 1;

    Time sinceLast = T_SAMPLE(&self->tapTimer);

    if (sinceLast >= MSEC(200) && sinceLast <= MSEC(2000)) {
        int bpm = timeToBPM(sinceLast);

        char s[100];
        snprintf(s, 100, "Inter-arrival time: %d.%03ds. Setting BPM to %d.", SEC_OF(sinceLast), MSEC_OF(sinceLast), bpm);
        SYNC(&cliHandler, printLine, (int)s);

        Command command = {CMD_SET_TEMPO, bpm};
        SYNC(&candler, sendCommand, (int)&command);
    } else {
        char s[100];
        snprintf(s, 100, "Inter-arrival time: %d.%03ds. Out of range [0.2, 2]s", SEC_OF(sinceLast), MSEC_OF(sinceLast));
        SYNC(&cliHandler, printLine, (int)s);
    }
}

void tempoButtonReset(TempoButton* self, int _) {
    Command command = {CMD_SET_TEMPO, 120};
    SYNC(&candler, sendCommand, (int)&command);
    SYNC(&cliHandler, printLine, (int)"tempoButtonReset");
}
