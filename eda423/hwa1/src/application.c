#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "sioTinyTimber.h"
#include "canTinyTimber.h"
#include "toneGenerator.h"
#include "cli.h"
#include "player.h"
#include "song.h"
#include "candler.h"
#include "userButton.h"
#include "tempoButton.h"
#include "canMessSender.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    Object super;
} App;

App app = { initObject() };

void reader(App*, int);
void receiver(App*, int);

Serial sci0 = initSerial(SCI_PORT0, &cliHandler, read);

Can can0 = initCan(CAN_PORT0, &candler, recvCanMsg);

extern SysIO sio0;
UserButton userButton = newUserButton(&sio0);
SysIO sio0 = initSysIO(SIO_PORT0, &userButton, userButtonTrig);

TempoButton tempoButton = newTempoButton();
CanMessSender canMessSender = newCanMessSender(MSEC(500));

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);

    SYNC(&cliHandler, printLine, (int)"CAN-msg received:");
    SYNC(&cliHandler, printLine, (int)msg.buff);
}

void startApp(App *self, int arg) {
    CAN_INIT(&can0);

    SCI_INIT(&sci0);
    ASYNC(&cliHandler, initCLI, (int)&sci0);

    SIO_INIT(&sio0);

    ButtonConfig buttonConfig = emptyButtonConfig();

//#define PROBLEM_2
#ifndef PROBLEM_2
    buttonConfig.onExitPressMomentary = foreignMethod(&tempoButton, tempoButtonTap);
    buttonConfig.onEnterPressAndHold = foreignMethod(&tempoButton, tempoButtonReset);
#else
    buttonConfig.onEnterPressAndHold = foreignMethod(&canMessSender, canMessSenderStart);
    buttonConfig.onExitPressAndHold = foreignMethod(&canMessSender, canMessSenderStop);
#endif
    SYNC(&userButton, initUserButton, (int)&buttonConfig);

    ASYNC(&toneGenerator, toneGeneratorPulse, 0);
}

int main() {
    INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
    INSTALL(&can0, can_interrupt, CAN_IRQ0);
    INSTALL(&sio0, sio_interrupt, SIO_IRQ0);
    TINYTIMBER(&app, startApp, 0);
    return 0;
}
