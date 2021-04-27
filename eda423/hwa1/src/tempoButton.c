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

    //char s[100];
    //snprintf(s, 100, "PRESS_MOMENTARY for %d.%03ds", SEC_OF(sinceLast), MSEC_OF(sinceLast));
    //SYNC(&cliHandler, printLine, (int)s);

    if (sinceLast >= MSEC(200) && sinceLast <= MSEC(2000)) {
        int bpm = timeToBPM(sinceLast);

        char s[100];
        snprintf(s, 100, "Nice beat. Setting BPM to %d.", bpm);
        SYNC(&cliHandler, printLine, (int)s);

        Command command = {CMD_SET_TEMPO, bpm};
        SYNC(&candler, sendCommand, (int)&command);
    } else {
        ASYNC(&cliHandler, printLine, (int)"Tempo out of range [200..2000]ms");
    }
}

void tempoButtonReset(TempoButton* self, int _) {
    SYNC(&cliHandler, printLine, (int)"tempoButtonReset");
}