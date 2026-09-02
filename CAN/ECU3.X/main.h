#ifndef MAIN_H
#define MAIN_H

#include <xc.h>

#define _XTAL_FREQ 20000000

/* Defines the data */
#define TRUE			1
#define FALSE			0

#define LED_OFF 0
#define LED_ON 1

#define RIGHT_IND_ON()  (PORTBbits.RB4 = 1)
#define RIGHT_IND_OFF() (PORTBbits.RB4 = 0)
#define LEFT_IND_ON()   (PORTBbits.RB0 = 1)
#define LEFT_IND_OFF()  (PORTBbits.RB0 = 0)


//extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
extern volatile unsigned char led_state;

typedef enum 
{
    e_ind_off,
    e_ind_left,
    e_ind_right,
    e_ind_hazard
} IndicatorStatus;

void speed_to_str(unsigned int speed, char *str);
void rpm_to_str(unsigned int rpm, char *str);
void temp_to_str(unsigned int temp, char *str);


#endif