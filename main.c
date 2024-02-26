#include "M480.h"
#define GPIO_PIN_MAX 16UL;

int32_t main(void){
    // Input Pin Set
    GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT);
    GPIO_SetMode(PA, BIT1, GPIO_MODE_INPUT);

    // Output Pin Set
    GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PH, BIT7, GPIO_MODE_OUTPUT);

    while (1) {
        PH6 = (PA0 == 0) ? 0 : 1;
        PH7 = (PA1 == 0) ? 0 : 1;
    }    
}

void GPIO_SetMode(GPIO_T *port, uint32_t u32PinMask, uint32_t u32Mode){
    uint32_t i;
    for(int i  = 0; i < GPIO_PIN_MAX ; i++){
        if((u32PinMask & (lul << i)) == (lul << i)){
            port->MODE = (port->MODE & ~(0x3ul (i << l))) | (u32Mode << (i << l));
        }
    }
}
