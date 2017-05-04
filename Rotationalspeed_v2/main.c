#include <msp430g2553.h>


//Input/output defines
#define BUTTON BIT3
#define LEDR BIT0
#define VRIN BIT0
#define COILOUT BIT5

//Timer values
unsigned int timervalue =0;

//Teethcounting values
volatile unsigned char current_count = 0;
volatile char old_count =0;
const char nrofteeth = 60;
const char missingteeth = 2;

//Averaging values
unsigned int averagearray[8];
unsigned long timeraverage=0;
unsigned int tobeaveragedpointer =7;

//System flags
char readtime = 0;

//-----------------------------------------------------------------------------------------------------

void teethcounting(){
    if(current_count != old_count){
    if(current_count == 10){
        TA0CTL |= MC_1;
    }
    if(current_count == 25){
        TA0CTL |= MC_1;
    }
    if(current_count == 40){
        TA0CTL |= MC_1;
    }
    if(current_count == 55){
        TA0CTL |= MC_1;
    }
    if(current_count > nrofteeth){
        current_count = 0;
    }
    //P1OUT ^= LEDR;
    P1OUT ^= LEDR;
    old_count = current_count;
    }
}

//-----------------------------------------------------------------------------------------------------

void timeraveragevalue(){
    char i;
    averagearray[tobeaveragedpointer]=timervalue;
    tobeaveragedpointer++;
    if(tobeaveragedpointer>7){
    tobeaveragedpointer=0;
    }
    for (i = 7; i>0; i--){
        timeraverage = timeraverage+ averagearray[i];}
    timeraverage>>=3;
}



//-----------------------------------------------------------------------------------------------------


int main(void)
{

//Processor timings and watchdog
  WDTCTL   = WDTPW + WDTHOLD;               // Stop watchdog timer
  BCSCTL1  = CALBC1_16MHZ;                  // Set range
  DCOCTL   = CALDCO_16MHZ;                  // Set DCO step and modulation
  BCSCTL2 |= DIVS_2;                        // Divide SMCLK by 4

  __delay_cycles(200);

//I/O port configuration
  P1DIR |= LEDR;                            // Set P1.0 to output direction
  P1IE  |= BUTTON;                          // P1.3 interrupt enabled
  P1IES |= BUTTON;                          // P1.3 Hi/lo edge
  P1REN |= BUTTON;                          // Enable Pull Up on SW2 (P1.3)
  P1OUT |= BUTTON;                          // TÜRAAAAAAAAAA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  P1IFG &= ~BUTTON;                         // P1.3 IFG cleared
                                            // BIT3 on Port 1 can be used as Switch

//Timer 0 configuration
   P1DIR    |= COILOUT;                     // P1.5 output
   P1SEL    |= COILOUT;                     // P1.5 TA1/2 options
   TA0CCTL0 |= CCIE;                        // Capture/compare interrupt enable
   TA0CCTL0 |= OUTMOD_7;                    // Output mode: 7 - PWM reset/set
   TA0CCR0  =  300;                          // Cycle lenght
   TA0CTL   |= TASSEL_2;                    // Use SMCLK as source for timer
   TA0CTL   |= MC_1;                        // Use UP mode timer
   TA0CTL   |= ID_2;                        // Timer divider

//Timer 1 configutation
  P2DIR     &= ~VRIN;                        // Port 2 to inputs
  P2SEL     |= VRIN;                         // P2.0 TA options
  TA1CCTL0  |= CM_2;                         // Capture mode
  TA1CCTL0  |= CCIS_0;                       // Capture input select
  TA1CCTL0  |= SCS;                          // Capture sychronize
  TA1CCTL0  |= CAP;                          // Capture mode select
  TA1CCTL0  |= CCI;                          // Capture input signal
  TA1CCTL0  |= CCIE;                         // Capture/compare interrupt enable
  TA1CTL    |= TASSEL_2;                     // Use SMCLK as source for timer
  TA1CTL    |= MC_2;                         // Use continuous mode timer
  TA1CTL    |= ID_2;                         // Timer divider


      __enable_interrupt();

      while(1)
          {
        //  teethcounting();
         // TA1CCTL0 |= CCIS_2;
         if(readtime!=0){
             readtime = 0;
             current_count++;
             if(timervalue>timeraverage*2){
                 P1OUT &= ~LEDR;
                 current_count = 0;
             }
             timeraveragevalue();
         }
        //  TA1CCTL0 |= CCIS_3;
        //  timervalue = TA1R;
        //  P1OUT ^= LEDR;
         teethcounting();
      }
}


//-----------------------------------------------------------------------------------------------------


// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    //TACCR0 = 100;
    //TA1CTL |= MC_1;
   current_count++;
  // TACTL |= TACLR ;
   P1IFG &= ~BUTTON;

}


//-----------------------------------------------------------------------------------------------------COIL INTERRUPT

// Timer0_A interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
  // TACCR0 = 0;
    TA0CTL &= ~MC_1;
  //  P1OUT ^= LEDR;
    //timervalue = TA0R;
  // TA0CTL |= TACLR ;
   //TACCTL0 &= ~CCIFG; //NEW TO TEST INTERRUPT FLAG RESET
}

//-----------------------------------------------------------------------------------------------------VR INTERRUPT

// Timer1_A interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
{
    //TACTL &= ~MC_1;
    timervalue = TA1R;
    TA1R = 0;
  //  P1OUT ^= LEDR;
    readtime = 10;
    //TACTL |= TACLR ;



}

