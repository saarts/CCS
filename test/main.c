#include <msp430g2553.h>

#define LEDR BIT0
#define LEDG BIT6
#define BUTTON2 BIT3

//volatile int g = 0;

int main(void) {

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
  //  P1REN |= BUTTON2;
    P1DIR |= LEDR;
    P1DIR |= LEDG; // make pin 0 and 6 output
    P1OUT &= ~LEDR;        // make pin 0 and 6 high
    P1OUT &= ~LEDG;




    TACCTL0 |= CCIE;    //Enable Interrupts on Timer
    TACCR0 = 50000; //Number of cycles in the timer
    TACTL |= TASSEL_2;  //Use ACLK as source for timer
    TACTL |= MC_1;  //Use UP mode timer
    TACTL |= ID_2;

    __enable_interrupt();   // intrinsic function

    while(1)
    {

        if(TACCR0 == 25000){
            P1OUT ^= LEDG;
        }
}


}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1OUT ^= LEDR;

}
