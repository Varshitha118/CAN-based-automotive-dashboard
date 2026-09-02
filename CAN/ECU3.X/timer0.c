#include <xc.h>
#include "timer0.h"

//void init_timer0(void)
//{
//	/*
//	 * Setting instruction cycle clock (Fosc / 4) as the source of
//	 * timer0
//	 */
//
///*Setting 8 bit timer register*/
//	T08BIT = 1;
//
///* Selecting internal clock source */
//	T0CS = 0;
//
///* Enabling timer0*/
//	TMR0ON = 1;
//
///* disabling prescaler*/
//	PSA = 1;
//
//	TMR0 = 6;
//
//	/* Clearing timer0 overflow interrupt flag bit */
//	TMR0IF = 0;
//
//	/* Enabling timer0 overflow interrupt */
//	TMR0IE = 1;
//}

void init_timer2()
{
    TMR2ON = 1;     //config for timer2
    TMR2IE = 1;
    TMR2IF = 0;
    PR2 = 249;
}