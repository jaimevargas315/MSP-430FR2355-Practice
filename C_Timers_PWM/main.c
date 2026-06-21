#include "intrinsics.h"
#include "msp430fr2355.h"
#include <driverlib.h>

int main(void) {
  // Stop watchdog timer
  WDT_A_hold(WDT_A_BASE);

  // setup ports
  P1DIR |= BIT0;        // set P1.0 to an output
  P1OUT |= BIT0;        // LED1 =1 initially
  PM5CTL0 &= ~LOCKLPM5; // turn on GPIO

  // setup timer
  TB0CTL |= TBCLR;        // reset timer
  TB0CTL |= TBSSEL__ACLK; // clock = AMCLK (32kHz)
  TB0CTL |= MC__UP;       // up mode for CCR0
  TB0CCR0 = 32768;        // set PWM period
  TB0CCR1 = 1638;         // set PWM duty cycle

  // setup compare interrupt requests
  TB0CCTL0 |= CCIE;     // local enable for CCR0
  TB0CCTL1 |= CCIE;     // local enable for CCR1
  __enable_interrupt(); // enable maskable interrupts
  TB0CCTL0 &= ~CCIFG;   // clear flag for CCR0
  TB0CCTL1 &= ~CCIFG;   // ckear fkag for CCR1

  while (1) {
    // loop forever
  }

  return 0;
}

// interrupt service routines
#pragma vector = TIMER0_B0_VECTOR // ISR for period
__interrupt void ISR_TB0_CCR0(void) {
  P1OUT |= BIT0;      // set LED1 ON
  TB0CCTL0 &= ~CCIFG; // clear flag for CCR0
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_CCR1(void) {
  P1OUT &= ~BIT0;     // turn LED1 off
  TB0CCTL1 &= ~CCIFG; // clear flag for CCR1
}