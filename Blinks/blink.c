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


    TACCTL0 = OUTMOD_4 + CCIE; // CCR0 toggle, interrupt enabled
    TACCTL1 = OUTMOD_4 + CCIE; // CCR1 toggle, interrupt enabled
    TACCTL2 = OUTMOD_4 + CCIE; // CCR2 toggle, interrupt enabled
    TACTL |= TASSEL_2;  //Use ACLK as source for timer
    TACTL |= MC_2;  //Use UP mode timer
    TACTL |= ID_3;
    TACTL |= TAIE;


    __enable_interrupt();   // intrinsic function

    while(1)
    {

}


}

// Timer_A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
TACCR0 += 100; // reload period
}
// Timer_A1 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1(void)
{
switch( TA0IV )
{
case 2: TACCR1 += 45000; // reload period
        P1OUT ^= LEDR;
break;
case 4: TACCR2 += 33000; // reload period
           P1OUT ^= LEDG;
break;
case 10: // // Timer overflow
break;
default: break;
}
}

