#include <msp430g2553.h>

#define LEDR BIT0
#define LEDG BIT6
#define BUTTON BIT3



int main(void) {

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P1REN |= BUTTON;
    P1DIR |= LEDR;
    P1DIR |= LEDG; // make pin 0 and 6 output
    P1OUT &= ~LEDR;        // make pin 0 and 6 high
   // P1SEL |= LEDG;
    P1OUT &= ~LEDG;
    P1REN |= BUTTON;
    P1OUT |= BUTTON;




    TACCTL0 |= CCIE;    //Enable Interrupts on Timer
    TACCR0 = 65000;     //Number of cycles in the timer
    TACTL |= TASSEL_2;  //Use main clock as source for timer
    TACTL |= ID_2;      //Devide
    TACTL |= MC_0;      //Mode select 0-stop, 1-up, 2-ramp
    TACTL |= TACLR;     //register
    //TA0R - register itself

    __enable_interrupt();   // intrinsic function

    while(1)
    {
        if(TA0R < 10000){
            P1OUT |= LEDG;
        }else{
            P1OUT &= ~LEDG;
        }
        if ((BUTTON & P1IN)){
       //     P1OUT |= LEDG;       // if P1.3 set, set P1.0
        }
            else{
                P1OUT |= LEDR;
                TACTL |= TACLR;
               // P1OUT &= ~LEDG;
                TACTL |= MC_1;

            }


}


}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    P1OUT &= ~LEDR;
    TACTL &= ~MC_1;
    TACTL |= TACLR ;
}

