#include <msp430.h>

#define one 1000000 //one second, for delay

#define front 0x02 //P1.1 on, all others off
#define back 0x04 //P1.2 on, all others off
#define left 0x08 //P1.3 on, all others off
#define right 0x10 //P1.4 on, all others off

void init(void);
void HALT(void);
void wait_a_bit(void);
void go_forward(void);
void turn_right(void);
void turn_left(void);
void reverse(void);
void reverse_left(void);

int main (void)
{
  init();
  // speed is on 0% at the moment
  while(1)
  {

    wait_a_bit();     /* wait for 3 seconds */
    go_forward();     /* move forward ..*/
    wait_a_bit();     /*..for 3 seconds */

    HALT();           /* stop the motors */

    wait_a_bit();     /* wait for 3 seconds */
    turn_right();     /* do a clockwise arc movement ..*/
    wait_a_bit();     /*.. for 3 seconds */

    HALT();           /* stop the motors */

    wait_a_bit();     /* wait for 3 seconds */
    reverse();        /* do a reverse movement.. */
    wait_a_bit();     /*.. for 3 seconds */

    HALT();           /* stop the motors */

    wait_a_bit();     /* wait for 3 seconds */
    go_forward();     /* move forward ..*/
    wait_a_bit();     /*..for 3 seconds */

    HALT();           /* stop the motors */

    wait_a_bit();     /* wait for 3 seconds */
    reverse_left();   /* do a complex route ..*/
    wait_a_bit();     /* ..for 3 seconds */

    HALT();           /* stop the motors */
    //do it forever
  }
}
/////////////////////////////////////////////////////////////////
void init(void)
{
  P1DIR |= BIT1 + BIT2 + BIT3 + BIT4; // P1.1 = forward, P1.2 = reverse, P1.3 = turn left, P1.4 = turn right
  P1OUT &= 0x00; // init pins to zero
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  P2DIR |= BIT1 + BIT4;  // P2.1 and P2.4 output
  P2SEL |= BIT1 + BIT4; // P2.1 and P2.4 TA1/2 options
  P2OUT &= 0x00; // Set P2.1 and P2.4 to off
  TA1CCR0 = 512-1;  // PWM Period
  TA1CCTL1 = OUTMOD_7; // CCR1 reset/set
  TA1CCR1 = 0; // CCR1 PWM duty cycle on P2.1 (0%)
  TA1CCTL2 = OUTMOD_7;
  TA1CCR2 = 256; // CCR2 PWM duty cycle on P2.4 (50%)
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode
}
/////////////////////////////////////////////////////////////////
void wait_a_bit(void)
{
  int i = 0;
  for(i = 0; i<= 3; i++)
  {
    __delay_cycles(one);
  }
  i = 0;
}
/////////////////////////////////////////////////////////////////
void go_forward(void)
{
  P1OUT &= 0x00; // don't enable any directions yet
  P1OUT |= front; //turn on front direction
  TA1CCR1 = 256; // go forward on half speed
}
/////////////////////////////////////////////////////////////////
void turn_right(void)
{
  P1OUT &= 0x00; // don't enable any directions yet
  P1OUT |= right; //enable right steering
  TA1CCR1 = 128; // move on a right-arc slowly
}
/////////////////////////////////////////////////////////////////
void turn_left(void)
{
  P1OUT &= 0x00; // don't enable any directions yet
  P1OUT |= left; //enable left steering
  TA1CCR1 = 128; //move on a left-arc slowly
}
/////////////////////////////////////////////////////////////////
void reverse(void)
{
  P1OUT &= 0x00; // don't enable any directions yet
  P1OUT |= back; //turn on reverse
  TA1CCR1 = 512; //move back on full speed
}
/////////////////////////////////////////////////////////////////
void reverse_left(void)
{
  P1OUT &= 0x00; // don't enable any directions yet
  P1OUT |= back + left; //turn on reverse with a left turn
  TA1CCR1 = 128; //move on a counter-clockwise left arc slowly
}
/////////////////////////////////////////////////////////////////
void HALT(void)
{
  TA1CCR1 = 1; // stop motors
}
