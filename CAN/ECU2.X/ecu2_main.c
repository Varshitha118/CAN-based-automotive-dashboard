/*
Name : Varshitha H K
Date : 11-july-2026
Project Name : Can Based Automotive Dashboard
Description ECU2 : Reads RPM,engine temperature and indicators and sends via CAN 
*/
#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "clcd.h"

#define _XTAL_FREQ 20000000

unsigned int count = 0;
unsigned char blink_state = LED_OFF;
unsigned char collison_flag = 0;

void init_config()
{   
    init_adc();
    init_matrix_keypad();
    //init_timer2();
    init_can();
    init_clcd();
    
    GIE = 1;
    PEIE = 1;
}

int main()
{
    unsigned int rpm = 0;
    unsigned char temperature = 0;
    init_config();
    clcd_print("NORMAL MODE", LINE1(0));
    while(1)
    {
        if(can_receive())
        {
            unsigned int msg_id = (((unsigned int)can_payload[SIDH] << 3) | ((unsigned int)can_payload[SIDL] >> 5));
            
            if(msg_id == COLLISION_MSG_ID)
            {
                collison_flag = 1;
                CLEAR_DISP_SCREEN;
            }
        }
        
        if(!collison_flag)
        {
            rpm = get_rpm();
            led_state = process_indicator();
        }
        else
        {
            clcd_print("COLLISION MODE", LINE1(0)); 
            rpm = 0;
            led_state = e_ind_hazard;
        }
        
        temperature = get_engine_temp();
        
        while(TXB0REQ);
        can_transmit(RPM_MSG_ID, rpm);
        __delay_ms(10);
        
        while(TXB0REQ)
        {
          //clcd_print("STUCK2", LINE2(0));
        }
        can_transmit(ENG_TEMP_MSG_ID, temperature);
        __delay_ms(10);
        
        while(TXB0REQ)
        {
          //clcd_print("STUCK3", LINE2(0));
        }
        can_transmit(INDICATOR_MSG_ID, led_state);
        __delay_ms(10);        
    }
}


