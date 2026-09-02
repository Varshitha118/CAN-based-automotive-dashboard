/*
Name : Varshitha H K
Date : 11-july-2026
Project Name : Can Based Automotive Dashboard
Description ECU1 : Reads vehicle speed and gear and sends data via CAN  
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
char arr[10];
unsigned char cangear;
char canspd[10];

void main(void)
{
  init_can();
  init_timer0();
  init_adc();
  init_matrix_keypad();
  init_clcd();
  GIE=1;
  PEIE=1;
  TMR0IF = 0;
  TRISA5=1; //POT
  //  clcd_print("SPD G RPM  T  ID", LINE1(0));////for checking
  unsigned char col=0;
  arr[4]='N';
  clcd_print("NORMAL MODE", LINE1(0));
//        CLEAR_DISP_SCREEN;

  while (1)
  {
  unsigned int msgid=0x35E;
    //speed 
    unsigned long sum=0;
    for (int i=0; i < 20; i++)
    {
      sum=sum + read_adc(4);
    }
    val = sum / 20;
    unsigned char key =read_switches(STATE_CHANGE);
    speed = (((unsigned long) val * MAXSPEED) / 1023);
    speed_to_str(speed, arr);
    //    clcd_print(arr, LINE2(0)); ////for checking

    //gear 
    if (key == MK_SW1)
    {
//      clcd_print("COLLISION MODE", LINE2(0));
      if (arr[4] == 'N')
        arr[4]='1';
      else if (arr[4] >= '1' && arr[4] < '6')
        arr[4]++;
      else if (arr[4] == '6')
        arr[4]='R';
      key=0;
    } 
    else if (key == MK_SW2)
    {
      if (arr[4] == 'R')
        arr[4]='6';
      else if (arr[4] > '1' && arr[4] <= '6')
        arr[4]--;
      else if (arr[4] == '1')
        arr[4]='N';
      key=0;
    } else if (key == MK_SW3)
    {
      col=1;
            msgid=0x70;
      //      while(TXB0REQ);
      //      speed=0;
      //      arr[4]='N';
      //      can_transmit(msgid,speed,arr[4]);
//      CLEAR_DISP_SCREEN;
      //msgid = 0x70;
      speed=0;
      arr[4]='N';
      clcd_print("COLLISION MODE", LINE1(0)); //for debugging
//      while (TXB0REQ);
//      can_transmit(0x70, 0, 'N');
//      __delay_ms(50);
    }
    //    clcd_putch(arr[4], LINE2(4)); ////for checking
    if (col)
    {
      speed=0;
      arr[4]='N';
    }
     // transmit
//    while (TXB0REQ);
    can_transmit(msgid, speed, arr[4]);
//    __delay_ms(10);

  }
  return;
}

void speed_to_str(unsigned int speed, char *str)
{
  unsigned char hundreds;
  unsigned char tens;
  unsigned char ones;

  hundreds = speed / 100;
  tens = (speed / 10) % 10;
  ones = speed % 10;

  if (hundreds == 0)
    str[0] = ' ';
  else
    str[0] = hundreds + '0';

  if (hundreds == 0 && tens == 0)
    str[1] = ' ';
  else
    str[1] = tens + '0';

  str[2] = ones + '0';
  str[3] = '\0';
}
