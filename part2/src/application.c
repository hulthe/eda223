#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include "toneGenerator.h"
#include "cli.h"
#include "player.h"
#include "song.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    Object super;
} App;

App app = { initObject() };

void reader(App*, int);
void receiver(App*, int);

Serial sci0 = initSerial(SCI_PORT0, &cliHandler, read);

Can can0 = initCan(CAN_PORT0, &app, receiver);

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);

    SYNC(&cliHandler, printLine, (int)"CAN-msg received:");
    SYNC(&cliHandler, printLine, (int)msg.buff);
}

void startApp(App *self, int arg) {
    CANMsg msg;

    CAN_INIT(&can0);

    SCI_INIT(&sci0);
    ASYNC(&cliHandler, initCLI, (int)&sci0);

    ASYNC(&toneGenerator, toneGeneratorPulse, 0);
    
    msg.msgId = 1;
    msg.nodeId = 1;
    msg.length = 6;
    msg.buff[0] = 'H';
    msg.buff[1] = 'e';
    msg.buff[2] = 'l';
    msg.buff[3] = 'l';
    msg.buff[4] = 'o';
    msg.buff[5] = 0;
    CAN_SEND(&can0, &msg);
}

int main() {
    INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
    INSTALL(&can0, can_interrupt, CAN_IRQ0);
    TINYTIMBER(&app, startApp, 0);
    return 0;
}
