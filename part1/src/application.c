#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include "toneGenerator.h"
#include "cpuBandit.h"
#include "cli.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct {
    Object super;
    int count;
    char c;

    //Parser parser;
} App;

//App app = { initObject(), 0, 'X', initParser() };
App app = { initObject(), 0, 'X' };

void reader(App*, int);
void receiver(App*, int);

//Serial sci0 = initSerial(SCI_PORT0, &app, reader);
Serial sci0 = initSerial(SCI_PORT0, &testtesttest, read);

Can can0 = initCan(CAN_PORT0, &app, receiver);

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    //SCI_WRITE(&sci0, "Can msg received: ");
    //SCI_WRITE(&sci0, msg.buff);
}

void reader(App *self, int c) {
    SCI_WRITE(&sci0, "Rcv: \'");
    SCI_WRITECHAR(&sci0, c);
    SCI_WRITE(&sci0, "\'\n");

//    parserInput(&self->parser, &sci0, c);
}

void startApp(App *self, int arg) {
    CANMsg msg;

    ASYNC(&toneGenerator, toneGeneratorPulse, 0);
    ASYNC(&cpuBandit, doBusyWork, 0);

    CAN_INIT(&can0);

    SCI_INIT(&sci0);
    ASYNC(&testtesttest, initCLI, (int)&sci0);

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
