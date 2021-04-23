#include "TinyTimber.h"
#include "sioTinyTimber.h"

void sio_init(SysIO *self, int unused) {

    GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction) 0); // Green LED On

	NVIC_SetPriority( EXTI9_5_IRQn, __IRQ_PRIORITY);
	NVIC_EnableIRQ( EXTI9_5_IRQn );
}

int sio_read(SysIO *self, int unused) {
    return GPIO_ReadInputDataBit(self->port, GPIO_Pin_7);
}

void sio_write(SysIO *self, int val) {
    GPIO_WriteBit(self->port, GPIO_Pin_0, (BitAction) val);
}

void sio_toggle(SysIO *self, int unused) {
    GPIO_ToggleBits(self->port, GPIO_Pin_0);
}

void DUMP(char *s);

int sio_interrupt(SysIO *self, int unused) {
    if (EXTI_GetITStatus(EXTI_Line7) == SET) {
//        DUMP("\n\rYey! A GPIOB bit7 IRQ!\n\r");

        EXTI_ClearITPendingBit(EXTI_Line7); // remove interrupt request
    
        if (self->obj) {
            ASYNC(self->obj, self->meth, 0);
            doIRQSchedule = 1;
        }
    } else {
        DUMP("\n\rStrange: Not a GPIOB bit7 IRQ!\n\r");
    }

	return 0;
}
