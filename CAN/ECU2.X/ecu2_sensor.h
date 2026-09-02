#ifndef ECU2_SENSOR_H
#define	ECU2_SENSOR_H

#include <stdint.h>
#include "matrix_keypad.h"
#include <xc.h>

#define RPM_ADC_CHANNEL 0x04
#define ENG_TEMP_ADC_CHANNEL 0x06

#define LED_OFF 0
#define LED_ON 1

#define RIGHT_IND_ON()  (PORTBbits.RB4 = 1)
#define RIGHT_IND_OFF() (PORTBbits.RB4 = 0)
#define LEFT_IND_ON()   (PORTBbits.RB0 = 1)
#define LEFT_IND_OFF()  (PORTBbits.RB0 = 0)

typedef enum 
{
    e_ind_off,
    e_ind_left,
    e_ind_right,
    e_ind_hazard
} IndicatorStatus;

extern volatile IndicatorStatus prev_ind_status, cur_ind_status;
extern volatile unsigned char led_state;

uint16_t get_rpm();
uint16_t get_engine_temp();
IndicatorStatus process_indicator();
void init_timer2();


#endif	/* ECU1_SENSOR_H */