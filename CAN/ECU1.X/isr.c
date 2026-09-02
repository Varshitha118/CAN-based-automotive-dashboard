#include <xc.h>
#include "isr.h"
extern unsigned int dutycycle;
extern unsigned int pwmcount;
void __interrupt() isr(void)
{
  if (TMR0IF)
  {
    TMR0 = TMR0 + 8;

    if (pwmcount++ >=100)
      pwmcount=0;
    TMR0IF = 0;
  }	    
}
