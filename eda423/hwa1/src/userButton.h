#ifndef USER_BUTTON_H
#define USER_BUTTON_H

#include "TinyTimber.h"
#include "sioTinyTimber.h"

#define FILTER_THRESHOLD MSEC(100)

#define PRESSED 0
#define RELEASED 1

typedef struct {
    Object* obj;
    Method meth;
} ForeignMethod;

typedef struct {
    // argument: none
    ForeignMethod onEnterPressAndHold;

    // argument: Time duration
    ForeignMethod onExitPressAndHold;

    // argument: none
    ForeignMethod onEnterPressMomentary;
    
    // argument: none
    ForeignMethod onExitPressMomentary;
} ButtonConfig;

typedef struct {
    Object super;
    SysIO* sio;
    int trigger;
    int pressId;
    Timer clickTimer;
    Timer pressedTimer;
    Time pushDelay;
    ButtonConfig buttonConfig;
} UserButton;



#define foreignMethod(obj, meth) \
    (ForeignMethod){(Object*)obj, (Method)meth}

#define emptyButtonConfig() {foreignMethod(0, 0)}

#define newUserButton(sio) \
    {initObject(), (SysIO*)sio, PRESSED, 0, initTimer(), initTimer(), 0, emptyButtonConfig()}

// ButtonConfig* config
void initUserButton(UserButton*, int config);
void userButtonTrig(UserButton*, int _);

#endif