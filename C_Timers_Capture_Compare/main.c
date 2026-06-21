#include "intrinsics.h"
#include "msp430fr2355.h"
#include <driverlib.h>

int Captured = 0;

int main(void) {
  // Stop watchdog timer
  WDT_A_hold(WDT_A_BASE);

  // setup ports
  P1DIR |= BIT0;        // set P1.0 to an output
  P1OUT |= BIT0;        // LED1 =1 initially
  PM5CTL0 &= ~LOCKLPM5; // turn on GPIO

  P4DIR &= ~BIT1; // set P4.1 to input (SW1)
  P4REN |= BIT1;  // enable resistor of p4.1
  P4OUT |= BIT1;  // set resistor to pull-up
  P4IES |= BIT1;  // set IRQ sensitivity to high-to-low

  PM5CTL0 &= ~PM5CTL0; // turn on digital I/O

  // setup port IRQ
  P4IE |= BIT1;         // local enable for P4.1
  __enable_interrupt(); // enable maskable IRQs
  P4IFG &= ~BIT1;       // clear flag

  // setup timer
  TB0CTL |= TBCLR;          // reset timer
  TB0CTL |= MC__CONTINUOUS; // put into continuous mode
  TB0CTL |= TBSSEL__ACLK;   // choose ACLK
  TB0CTL |= ID__8;          // divide by 8 in pre-scaler

  // setup capture
  TB0CCTL0 |= CAP;       // put CCR0 into capture mode
  TB0CCTL0 |= CM__BOTH;  // sensitive to both edges
  TB0CCTL0 |= CCIS__GND; // put capture input level at GND

  while (1) {
    // loop forever
  }

  return 0;
}

// interrupt service routines
#pragma vector = PORT4_VECTOR
__interrupt void ISR_Port4_SW1(void) {
  P1OUT ^= BIT0;      // toggle LED
  TB0CCTL0 ^= CCIS0;  // switch between GND and VCC
  Captured = TB0CCR0; // store captured value
  P4IFG &= ~BIT1;     // clear flag
}