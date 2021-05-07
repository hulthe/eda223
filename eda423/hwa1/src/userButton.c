#include "userButton.h"
#include "cli.h"
#include "candler.h"
#include "sioTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

#define PRESS_AND_HOLD_THRESHOLD SEC(2)

void pressAndHoldTrigger(UserButton*, int pressIndex);

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

        AFTER(PRESS_AND_HOLD_THRESHOLD, self, pressAndHoldTrigger, self->pressId);

        ForeignMethod fm = self->buttonConfig.onEnterPressMomentary;
        if(fm.obj) {
            ASYNC(fm.obj, fm.meth, 0);
        }
        break;
    case RELEASED:;
        // reconfigure interrupt to trigger on another mode
        // TODO: figure out if this is a race condition (since we might miss a button event that happens before we get here)
        SIO_TRIG(self->sio, PRESSED);
        self->trigger = PRESSED;
        int pressDuration = T_SAMPLE(&self->pressedTimer);
        self->pressId++;

        if (pressDuration <= PRESS_AND_HOLD_THRESHOLD) {
            ForeignMethod fm = self->buttonConfig.onExitPressMomentary;
            if(fm.obj) {
                ASYNC(fm.obj, fm.meth, 0);
            }
        } else {
            ForeignMethod fm = self->buttonConfig.onExitPressAndHold;
            if(fm.obj) {
                ASYNC(fm.obj, fm.meth, 0);
            }

            char s[100];
            snprintf(s, 100, "exit PRESS_AND_HOLD after %d.%03ds", SEC_OF(pressDuration), MSEC_OF(pressDuration));
            SYNC(&cliHandler, printLine, (int)s);
        }
        break;
    }
}

void pressAndHoldTrigger(UserButton* self, int pressId) {
    if (self->pressId == pressId) {
        ASYNC(&cliHandler, printLine, (int)"enter PRESS-AND-HOLD");
 
        ForeignMethod fm = self->buttonConfig.onEnterPressAndHold;
        if(fm.obj) {
            ASYNC(fm.obj, fm.meth, 0);
        }
    }
}   