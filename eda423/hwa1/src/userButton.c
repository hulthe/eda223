#include "userButton.h"
#include "cli.h"
#include "candler.h"
#include "sioTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

void pressAndHoldTrigger(UserButton*, int pressIndex);
void sendMessBurst(UserButton*, int pressId);

void initUserButton(UserButton* self, int config_ptr) {
    ButtonConfig* config = (ButtonConfig*)config_ptr;
    self->buttonConfig = *config;
    SIO_TRIG(self->sio, PRESSED);
    self->trigger = PRESSED;
}

void userButtonTrig(UserButton* self, int _) {
    switch (self->trigger)
    {
    case PRESSED:;
        Time sinceLast = T_SAMPLE(&self->clickTimer);

        // filter contact bounces
        if (sinceLast < FILTER_THRESHOLD) {
            ASYNC(&cliHandler, printLine, (int)"Ignoring contact bounce");
            return;
        }

        T_RESET(&self->clickTimer);


        // reconfigure interrupt trigger to falling
        // TODO: figure out if this is a race condition (since we might miss a button event that happens before we get here)
        SIO_TRIG(self->sio, RELEASED);
        self->trigger = RELEASED;
        T_RESET(&self->pressedTimer);

        AFTER(SEC(2), self, pressAndHoldTrigger, self->pressId);

        ForeignMethod fm = self->buttonConfig.onEnterPressMomentary;
        if(fm.obj) {
            SYNC(fm.obj, fm.meth, 0);
        }
        break;
    case RELEASED:;
        // reconfigure interrupt to trigger on another mode
        // TODO: figure out if this is a race condition (since we might miss a button event that happens before we get here)
        SIO_TRIG(self->sio, PRESSED);
        self->trigger = PRESSED;
        int tmp = T_SAMPLE(&self->pressedTimer);
        self->pressId++;

        if (tmp <= MSEC(2000)) {
            ForeignMethod fm = self->buttonConfig.onExitPressMomentary;
            if(fm.obj) {
                SYNC(fm.obj, fm.meth, 0);
            }
        } else {
            char s[100];
            snprintf(s, 100, "PRESS_AND_HOLD for %d.%03ds", SEC_OF(tmp), MSEC_OF(tmp));
            SYNC(&cliHandler, printLine, (int)s);
            //self->firstPress = 1;

            ForeignMethod fm = self->buttonConfig.onExitPressAndHold;
            if(fm.obj) {
                SYNC(fm.obj, fm.meth, 0);
            }

            //Command command = {CMD_SET_TEMPO, 120};
            //SYNC(&candler, sendCommand, (int)&command);
        }
        break;
    }
}

void pressAndHoldTrigger(UserButton* self, int pressId) {
    if (self->pressId == pressId) {
        SYNC(&cliHandler, printLine, (int)"PRESS-AND-HOLD");

        ForeignMethod fm = self->buttonConfig.onEnterPressAndHold;
        if(fm.obj) {
            ASYNC(fm.obj, fm.meth, 0);
        }
    }
}

void sendMessBurst(UserButton* self, int pressId) {
    if(self->pressId == pressId) {
        // send message every 500 msec if button is pressed and held
        AFTER(MSEC(500), self, sendMessBurst, pressId);

        Command command = {CMD_HELLO};
        SYNC(&candler, sendCommand, (int)&command);
    }
}