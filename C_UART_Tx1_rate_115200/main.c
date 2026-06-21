#include "intrinsics.h"
#include "msp430fr2355.h"
#include <driverlib.h>

int main(void) {
  // Stop watchdog timer
  WDT_A_hold(WDT_A_BASE);

  // setup UART
  UCA1CTLW0 |= UCSWRST;       // put UART A1 into SW reset
  UCA1CTLW0 |= UCSSEL__SMCLK; // choose SMCLK for UART A1
  UCA1BRW = 8;                // set prescaler to 8
  UCA1MCTLW = 0XD600;         // config modulation settings & low freq

  // setup ports
  P4SEL1 &= ~BIT3; // P4SEL1.3 : P4SEL0.3 = 01
  P4SEL0 |= BIT3;  // put UART A1 Tx on P4.3

  PM5CTL0 &= ~LOCKLPM5; // turn on I/O

  UCA1CTLW0 &= ~UCSWRST; // remove UART A1 SW reset

  // main loop
  int i;
  while (1) {
    UCA1TXBUF = 0X4D; // sebd x4D over UART A1
    for (i = 0; i < 100; i++) {
      // delay
    }
  }

  return 0;
}
