/*
Name : Varshitha H K
Date : 11-july-2026
Project Name : Can Based Automotive Dashboard
Description ECU3 : Receives data from ECU1 and ECU2 and display vehicle information on CLCD
*/

#include <xc.h>
#include "matrix_keypad.h"
#include "clcd.h"
#include "timer0.h"
#include "adc.h"
#include "main.h"
#include "can.h"

#define _XTAL_FREQ 20000000
#define MAXSPEED  249

unsigned int speed=0;
unsigned int val=0;
unsigned int pwmcount=0;
unsigned int count = 0;
unsigned char blink_state = LED_OFF;

volatile unsigned char led_state = e_ind_off;
volatile IndicatorStatus prev_ind_status = e_ind_off;
volatile IndicatorStatus cur_ind_status = e_ind_off;

char arr[10];
unsigned char cangear;
char canspd[10];
char canrpm_arr[10];
char cantemp_arr[10];

void main(void)
{
  init_can();
  init_timer2();
  init_adc();
  init_matrix_keypad();
  init_clcd();
  GIE=1;
  PEIE=1;
  TMR2IF = 0;
  TRISA5=1; //POT


  clcd_print("SPD G RPM  T  ID", LINE1(0));
  while (1)
  {
    //receive
    if (can_receive())
    {
      unsigned int msgid;
      msgid=(((unsigned int ) can_payload[SIDH] << 3 ) | (can_payload[SIDL]) >> 5);
      if (msgid == 0x35E)//ecu1 sends speed in D2 and gear in D3
      {
//        sprintf(canspd, "%3u", can_payload[D2]);
        speed_to_str(can_payload[D2],canspd);
        cangear=can_payload[D3];
        clcd_print(canspd, LINE2(0));
        clcd_putch(cangear, LINE2(4));
      } else if (msgid == 0x30)//ecu2 msg id 0x30 sends only rpm
      {
        unsigned int canrpm=0;
        canrpm=( (unsigned int ) (can_payload[D0] << 8) | can_payload[D1] ); //msb in D1
//        sprintf(canrpm_arr, "%4u", canrpm);
        rpm_to_str(canrpm,canrpm_arr);
        clcd_print(canrpm_arr, LINE2(6));
      }
      else if (msgid == 0x40)//ecu2 msg id 0x40 sends temperature
      {
        unsigned int cantemp=0;
        cantemp=( (unsigned int ) (can_payload[D1])); //lsb in D1
//        sprintf(cantemp_arr, "%2u", cantemp);
        temp_to_str(cantemp,cantemp_arr);
        clcd_print(cantemp_arr, LINE2(11));
      } else if (msgid == 0x50)// ecu2 msg id 0x50 sends indicator
      {
        led_state = ((unsigned int) (can_payload[D1]));

        if (led_state == e_ind_left)
          clcd_putch('L', LINE2(14));
        else if (led_state == e_ind_right)
          clcd_putch('R', LINE2(14));
        else if (led_state == e_ind_hazard)
          clcd_putch('H', LINE2(14));
        else
          clcd_putch(' ', LINE2(14));
      }
    }
  }
  return;
}

void __interrupt() isr(void)
{
  if (TMR2IF)
  {
    TMR2IF=0;
    count++;

    if (count == 10000)
    {
      count = 0;

      if (led_state == e_ind_off)
      {
        LEFT_IND_OFF();
        RIGHT_IND_OFF();
      } else
      {
        if (blink_state == LED_OFF)
        {
          blink_state = LED_ON;
          if (led_state == e_ind_left || led_state == e_ind_hazard)
            LEFT_IND_ON();
          if (led_state == e_ind_right || led_state == e_ind_hazard)
            RIGHT_IND_ON();
        } else
        {
          blink_state = LED_OFF;
          LEFT_IND_OFF();
          RIGHT_IND_OFF();
        }
      }
    }
  }
}



void speed_to_str(unsigned int speed, char *str)
{
    if(speed >= 100)
        str[0] = (speed / 100) + '0';
    else
        str[0] = ' ';

    if(speed >= 10)
        str[1] = ((speed / 10) % 10) + '0';
    else
        str[1] = ' ';

    str[2] = (speed % 10) + '0';
    str[3] = '\0';
}

void rpm_to_str(unsigned int rpm, char *str)
{
    unsigned char thousands;
    unsigned char hundreds;
    unsigned char tens;
    unsigned char ones;

    thousands = rpm / 1000;
    hundreds  = (rpm / 100) % 10;
    tens      = (rpm / 10) % 10;
    ones      = rpm % 10;

    if(thousands == 0)
        str[0] = ' ';
    else
        str[0] = thousands + '0';

    if(thousands == 0 && hundreds == 0)
        str[1] = ' ';
    else
        str[1] = hundreds + '0';

    if(thousands == 0 && hundreds == 0 && tens == 0)
        str[2] = ' ';
    else
        str[2] = tens + '0';

    str[3] = ones + '0';

    str[4] = '\0';
}

void temp_to_str(unsigned int temp, char *str)
{
    unsigned char tens;
    unsigned char ones;

    tens = temp / 10;
    ones = temp % 10;

    if(tens == 0)
        str[0] = ' ';
    else
        str[0] = tens + '0';

    str[1] = ones + '0';

    str[2] = '\0';
}