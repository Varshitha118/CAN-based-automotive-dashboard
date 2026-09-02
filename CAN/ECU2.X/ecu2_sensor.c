#include "ecu2_sensor.h"
#include "adc.h"
//#include "can.h"
#include "msg_id.h"

volatile unsigned char led_state = e_ind_off;
volatile IndicatorStatus prev_ind_status = e_ind_off;
volatile IndicatorStatus cur_ind_status = e_ind_off;

uint16_t get_rpm()
{
//    unsigned int adc_val = read_adc(RPM_ADC_CHANNEL);
    unsigned long sum = 0;
    
    for(unsigned char i = 0; i<50; i++)
        sum += read_adc(RPM_ADC_CHANNEL);
    
    return ((float)(sum / 50)/1023)*9999;
}

uint16_t get_engine_temp()
{
    unsigned int temp = read_adc(ENG_TEMP_ADC_CHANNEL); 
    
    unsigned char val = (float)temp * 0.4887;
    
    if(val >=99)
        val = 99;
    
    return val;
}

IndicatorStatus process_indicator()
{
   unsigned char sw = read_switches(STATE_CHANGE);

    prev_ind_status = cur_ind_status;

    if(prev_ind_status == e_ind_hazard && sw != MK_SW3)
        return cur_ind_status;

    if(sw == MK_SW1)
    {
        if(prev_ind_status == e_ind_left)
            cur_ind_status = e_ind_off;
        else
            cur_ind_status = e_ind_left;
    }
    else if(sw == MK_SW2)
    {
        if(prev_ind_status == e_ind_right)
            cur_ind_status = e_ind_off;
        else
            cur_ind_status = e_ind_right;
    }
    else if(sw == MK_SW3)
    {
        if(prev_ind_status == e_ind_hazard)
            cur_ind_status = e_ind_off;
        else
            cur_ind_status = e_ind_hazard;
    }

    return cur_ind_status;
    
}

void init_timer2()
{
    TMR2ON = 1;     //config for timer2
    TMR2IE = 1;
    TMR2IF = 0;
    PR2 = 249;
}