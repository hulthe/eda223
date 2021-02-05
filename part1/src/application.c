#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>


#define BUF_CAPACITY 30

typedef struct {
    Object super;
    int count;
    char c;

    char buf[BUF_CAPACITY];
    int buflen;
    int sum;
} App;

App app = { initObject(), 0, 'X', {0}, 0, 0 };

void reader(App*, int);
void receiver(App*, int);

Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
}

void reader(App *self, int c) {
    SCI_WRITE(&sci0, "Rcv: \'");
    SCI_WRITECHAR(&sci0, c);
    SCI_WRITE(&sci0, "\'\n");

    if(self->buflen >= BUF_CAPACITY) {
        return;
    }

    switch(c) {
    case 'e':
        // Parse number
        self->buf[self->buflen] = '\0';
        int num = atoi(self->buf);
        self->sum += num;

        // Clear buffer
        self->buflen = 0;

        // Convert the sum to a string
        char print_buf[BUF_CAPACITY];
        snprintf(print_buf, BUF_CAPACITY, "%d", self->sum);

        // Print the number
        SCI_WRITE(&sci0, "sum: ");
        SCI_WRITE(&sci0, print_buf);
        SCI_WRITE(&sci0, "\n");

        break;
    case 'F':
        self->sum = 0;
        self->buflen = 0;
        break;
    default:
        self->buf[self->buflen] = c;
        self->buflen = self->buflen + 1;
        break;
    }
}

void startApp(App *self, int arg) {
    CANMsg msg;

    CAN_INIT(&can0);
    SCI_INIT(&sci0);
    SCI_WRITE(&sci0, "Hello, hello...\n");

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
