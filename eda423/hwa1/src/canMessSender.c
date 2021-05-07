#include "canMessSender.h"
#include "candler.h"
#include "cli.h"

void canMessSenderLoop(CanMessSender* self, int startId) {
    if (startId != self->startId) {
        return;
    }

    AFTER(self->period, self, canMessSenderLoop, startId);

    Command cmd = {CMD_HELLO, 0};
    SYNC(&candler, sendCommand, (int)&cmd);
}

void canMessSenderStart(CanMessSender* self, int _) {
    ASYNC(self, canMessSenderLoop, self->startId);
}
void canMessSenderStop(CanMessSender* self, int _) {
    self->startId += 1;
}

void canMessOnce(CanMessSender* self, int _) {
    Command cmd = {CMD_HELLO, 0};
    SYNC(&candler, sendCommand, (int)&cmd);
}