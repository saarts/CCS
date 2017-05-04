#include <msp430g2553.h>
#include <stdbool.h>

#define LEDR BIT0
#define LEDG BIT6
#define BUTTON BIT3

bool inc;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT6;                            // P1.6 output
    P1SEL |= BIT6;                            // P1.6 TA1/2 options
    TA0CCR0 = 512-1;                             // PWM Period
    TA0CCTL1 = OUTMOD_7;                         // CCR1 reset/set
    TA0CCR1 = 0;                               // CCR1 PWM duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TAIE;                  // SMCLK, up mode, Timer_A interrupt enabled, Timer_A interrupt pending


  while(1){
      TA0CCR1++;
      if(TA0CCR1>500){
          TA0CCR1 = 1;
      }
      __delay_cycles(5000);
  }

}

// Timer_A interrupt service routine
#pragma vector=TIMERA1_VECTOR
__interrupt void TIMERA(void)
{
    //if CCR1 is at 1 or at CCR0 minus 1, toggle the value of inc
    if ((CCR1 == (CCR0 - 1)) || (CCR1 == 1)) {
        inc = !inc;
    }
    // if inc is 1 increment CCR1, else decrement CCR1
    if (inc) {
        CCR1++;
    }
    else {
        CCR1--;
    }
    TACTL &= ~TAIFG;                           // Timer A IFG cleared
}

