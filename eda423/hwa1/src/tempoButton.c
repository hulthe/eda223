#include "tempoButton.h"
#include "cli.h"
#include "candler.h"
#include "sioTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

int timeToBPM(Time);
void tempoButtonClick(TempoButton*, int _);
void pressAndHoldTrigger(TempoButton*, int pressIndex);
void sendMessBurst(TempoButton*, int pressId);

void initTempoButton(TempoButton* self, int _) {
    SIO_TRIG(self->sio, PRESSED);
    self->trigger = PRESSED;
}

void tempoButtonTrig(TempoButton* self, int _) {
    switch (self->trigger)
    {
    case PRESSED:
        // reconfigure interrupt trigger to falling
        // TODO: figure out if this is a race condition (since we might miss a button event that happens before we get here)
        SIO_TRIG(self->sio, RELEASED);
        self->trigger = RELEASED;
        T_RESET(&self->pressedTimer);
        AFTER(SEC(2), self, pressAndHoldTrigger, self->pressId);
        break;
    case RELEASED:
        // reconfigure interrupt to trigger on another mode
        // TODO: figure out if this is a race condition (since we might miss a button event that happens before we get here)
        SIO_TRIG(self->sio, PRESSED);
        self->trigger = PRESSED;
        int tmp = T_SAMPLE(&self->pressedTimer);
        self->pressId++;

        if (tmp <= MSEC(2000)) {
            tempoButtonClick(self, 0);
            
        } else {
            char s[100];
            snprintf(s, 100, "PRESS_AND_HOLD for %d.%03ds", SEC_OF(tmp), MSEC_OF(tmp));
            SYNC(&cliHandler, printLine, (int)s);
            self->firstPress = 1;

            Command command = {CMD_SET_TEMPO, 120};
            SYNC(&candler, sendCommand, (int)&command);
        }
        break;
    }
}

void tempoButtonClick(TempoButton* self, int _) {
    if(self->firstPress) {
        T_RESET(&self->clickTimer);
        self->firstPress = 0;
        return;
    }
    
    Time sinceLast = T_SAMPLE(&self->clickTimer);

    // filter contact bounces
    if (sinceLast < FILTER_THRESHOLD) {
        ASYNC(&cliHandler, printLine, (int)"Ignoring contact bounce");
        return;
    }

    self->firstPress = 1;

    char s[100];
    snprintf(s, 100, "PRESS_MOMENTARY for %d.%03ds", SEC_OF(sinceLast), MSEC_OF(sinceLast));
    SYNC(&cliHandler, printLine, (int)s);

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

void pressAndHoldTrigger(TempoButton* self, int pressId) {
    if (self->pressId == pressId) {
        ASYNC(&cliHandler, printLine, (int)"PRESS-AND-HOLD");

        ASYNC(self, sendMessBurst, pressId);
    }
}

int timeToBPM(Time time) {
    return SEC(60) / time;
}

void sendMessBurst(TempoButton* self, int pressId) {
    if(self->pressId == pressId) {
        // send message every 500 msec if button is pressed and held
        AFTER(MSEC(500), self, sendMessBurst, pressId);

        Command command = {CMD_HELLO};
        SYNC(&candler, sendCommand, (int)&command);
    }
}