#include <msp430g2553.h>


//Input/output defines
#define BUTTON BIT3
#define LEDR BIT0

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

//-----------------------------------------------------------------------------------------------------

void teethcounting(){
    if(current_count != old_count){
    if(current_count > nrofteeth){
        current_count = 0;

    }

    //P1OUT ^= LEDR;
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


  WDTCTL   = WDTPW + WDTHOLD;               // Stop watchdog timer
  BCSCTL1  = CALBC1_16MHZ;                  // Set range
  DCOCTL   = CALDCO_16MHZ;                  // Set DCO step and modulation
  BCSCTL2 |= DIVS_2;                        // Divide SMCLK by 8

  P1DIR |=  LEDR;                            // Set P1.0 to output direction
  P1IE  |=  BUTTON;                          // P1.3 interrupt enabled
  P1IES |=  BUTTON;                          // P1.3 Hi/lo edge
  P1REN |=  BUTTON;                          // Enable Pull Up on SW2 (P1.3)
  P1OUT |=  BUTTON;                          //TÜRAAAAAAAAAA!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  P1IFG &= ~BUTTON;                         // P1.3 IFG cleared
                                            //BIT3 on Port 1 can be used as Switch
  P1DIR |= BIT5;                            // P1.5 output
  P1SEL |= BIT5;                            // P1.5 TA1/2 options
  //P1SEL2 &= ~BIT6;
  TACCTL0 |= CCIE+OUTMOD_7;                          //Enable Interrupts on Timer
  TACCR0   = 30;
  //TACCR1 = 1;//Number of cycles in the timer
  TACTL   |= TASSEL_2;                        //Use SMCLK as source for timer
 // TACTL &= ~MC_1;                         //Use UP mode timer
  TACTL   |= MC_1;
  TACTL   |= ID_2;
 // TACCTL0 |= OUTMOD_4;
 // TACCR1 = 1;//Number of cycles in the timer
  // TACTL |= TAIE;


      __enable_interrupt();

      while(1)
          {
          if(TA0R==40000){
              timervalue = TA0R;
              P1OUT  ^= LEDR;
              timeraveragevalue();
          }else{
             // P1OUT  &= ~LEDR;
          }

          teethcounting();

      }
}


//-----------------------------------------------------------------------------------------------------


// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    //TACCR0 = 100;
    TACTL |= MC_1;
   current_count++;
  // TACTL |= TACLR ;
   P1IFG &= ~BUTTON;

}


//-----------------------------------------------------------------------------------------------------


// Timer_A interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
  // TACCR0 = 0;
    TACTL &= ~MC_1;
  //  P1OUT ^= LEDR;
    //timervalue = TA0R;
   TACTL |= TACLR ;
   //TACCTL0 &= ~CCIFG; //NEW TO TEST INTERRUPT FLAG RESET
}

