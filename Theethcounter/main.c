#include <msp430g2553.h>
#define BUTTON BIT3
#define LEDR BIT0

volatile int current_count = 0;
volatile int old_count =0;
const int teeth = 60;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= LEDR;                            // Set P1.0 to output direction
  P1IE |=  BIT3;                            // P1.3 interrupt enabled
  P1IES |= BIT3;                            // P1.3 Hi/lo edge
  P1REN |= BIT3;                            // Enable Pull Up on SW2 (P1.3)
  P1OUT |= BUTTON;                          //TÜRAAAAAAAAAA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  P1IFG &= ~BIT3;                           // P1.3 IFG cleared
                                            //BIT3 on Port 1 can be used as Switch2
  P1DIR |= BIT6;            //P1.6 output
  P1SEL |= BIT6;            //P1.6 TA1/2 options
  TACCTL0 |= CCIE;          //Enable Interrupts on Timer
  TACCR0 = 30;              //Number of cycles in the timer
  TACTL |= TASSEL_2;        //Use SMCLK as source for timer
  TACTL &= ~MC_1;
  TACTL |= MC_0;
  TACTL |= ID_2;            //Divider
  TACCTL1 = OUTMOD_2;
  TACCR1 = 1;
    //  TACTL |= TAIE;
      __enable_interrupt();

      while(1)
          {
          if(current_count != old_count){
          if(current_count > teeth){
              current_count = 0;
              P1OUT ^= LEDR;

          }

          old_count = current_count;
          }

      }
}

// Port 1 interrupt service routine
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
//#else
//#error Compiler not supported!
//#endif
{
   TACTL |= MC_1;
   current_count++;
   P1IFG &= ~BIT3;

}


// Timer_A interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    TACTL &= ~MC_1;
    TACTL |= TACLR ;
  //  TACCTL0 &= ~CCIFG; //NEW TO TEST INTERRUPT FLAG RESET
}

