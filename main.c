    #include "M480.h";
    #define GPIO_PIN_MAX 16UL

    int32_t main(void){
        // Input Pin Set
        GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT); // SW1
        GPIO_SetMode(PA, BIT1, GPIO_MODE_INPUT); // SW2

        GPIO_SetMode(PG, BIT2, GPIO_MODE_INPUT); // Up
        GPIO_SetMode(PC, BIT10, GPIO_MODE_INPUT); //Down
        GPIO_SetMode(PG, BIT3, GPIO_MODE_INPUT); // Center

        // Output Pin Set
        GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT); // LEDR1
        GPIO_SetMode(PH, BIT7, GPIO_MODE_OUTPUT); // LEDG1

        while (1) {
            if(PG2 && !PC10){
                PH6 = 1;
                PH7 = 0;
            }
            else if(!PG2 && PC10){
                PH6 = 0;
                PH7 = 1;
            }
            else if(!PG2 && !PC10){
                PH6 = 1;
                PH7 = 1;
            }
            else{
                PH6 = (PA0 == 0) ? 0 : 1;
                PH7 = (PA1 == 0) ? 0 : 1;
            }        
        }    
    }

    void GPIO_SetMode(GPIO_T *port, uint32_t u32PinMask, uint32_t u32Mode){
        uint32_t i;
        for(i = 0ul; i < GPIO_PIN_MAX ; i++){
            if((u32PinMask & (1ul << i)) == (1ul << i)){
                port->MODE = (port->MODE & ~(0x3ul << (i << 1))) | (u32Mode << (i << 1));
            }
        }
    }
