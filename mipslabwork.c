/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "Seesaw.h"

int mytime = 0x5957;
int prime = 1234567;
int timeoutcount;
volatile int * trise = (volatile int *) 0xbf886100;
volatile int * porte = (volatile int *) 0xbf886110;
int count;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  
  IFS(0) = IFS(0) & ~(0x1<<8); //clear flag
  /*
  if (timeoutcount == 10) {
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    timeoutcount = 0;
  }
  timeoutcount++;
  return;
  */
  return;
}

void setup_ports()
{
    // Turn off all analog inputs, make everything digital
    AD1PCFGSET = 0xFF;
    
    // Make all ports outputs initially
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISF = 0;
    TRISG = 0;     
    
    // Set all port values low initially
    /*
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    LATF = 0;
    LATG = 0;
    */
}

/* Lab-specific initialization goes here */
void labinit( void )
{ 
  setup_ports();
  init();
  // Enable i2c connection and init seesaw
  /*
  int i = init();
  if (!i) {
    display_string(0, "Wrong HW code");
    display_update();
    while(1);
  }
  uint8_t c;
  beginTrans(0);
  read8(&c);
  endTrans();
  int a = (int) c;
  while (1);
  */
  display_string(0, "test");
  display_update();
  I2C1CON = 1 << 15;
  I2C1CONSET = 0x1; //PIC32_IRQ_I2C1M master event IRQS
  while (I2C1CON & 0x1);
  I2C1TRN = 0x36 << 1;
  while(I2C1STAT & (1 << 14));
  I2C1TRN = 0x00;
  while(I2C1STAT & (1 << 14));
  I2C1TRN = 0x04;
  while(I2C1STAT & (1 << 14));
  I2C1CONSET = 0x2;
  while (I2C1CON & 0x2);
  I2C1TRN = (0x36 << 1) | 1;
  while(I2C1STAT & (1 << 14));
  I2C1CONSET = 0x8;               // Receive enable
  while (I2C1CON & 0x8) {
    display_string(0, "first here");
    display_update();
  } 
  while (!(I2C1STAT & 0x2)){
    display_string(0, "Still here");
    display_update();
  }
  int t = I2C1RCV;

  display_string(0, "got Past");
  display_string(1, itoaconv(t));
  display_update();

  
  


}

/* This function is called repetitively from the main program */
void labwork( void ) {
  /*
  float tempC = tempRead();
  uint16_t capread = moistureRead();

  display_string(0, itoaconv(capread));
  //display_string(1, itoaconv(tempC));
  display_update();
  delay(200);
  */
}
