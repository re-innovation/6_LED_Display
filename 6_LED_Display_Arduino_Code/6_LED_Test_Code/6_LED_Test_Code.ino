/********************************************************
/****** 6 x LED Unit with Controller ********************
/****** by Matt Little **********************************
/****** Date: 19/12/13 **********************************
/****** info@re-innovation.co.uk ************************
/****** www.re-innovation.co.uk *************************
/********************************************************

Utilising example code from:
http://playground.arduino.cc/Main/LedControl

You will have to download that library and install before using this code.

This test code will show you how to:
Display numbers
Display simple text (only a few characters available)
Display scrolling numbers/text
Fade the display up and down

The display uses an AS11007 8 x 7 segment LED driver.
There are 6 digits available (LHS: 0-5 :RHS).

see www.re-innovation.co.uk for more details
 
*/


//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 pin 13 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD
 LedControl(MOSI,CLK,~CS,Number of units)
 
 */
LedControl lc=LedControl(11,13,10,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=250;


// This string is comprised of binary numbers which make digits.
// The 7 seg display comprises:
//
//  --a--
// f     b
// |     |
//  --g--
// e     c
// |     |
//  --d--   DP
//
// Where the binary number is: DP a b c d e f g
// This code says "7 SEg tESt HEllO !" 
char testString[30] = {
B00000000,B00000000,B00000000,B00000000,B00000000,B01110000,
B00000000,B01011011,B01001111,B01111011,
B00000000,B00001111,B01001111,B01011011,B00001111,B00000000,
B00110111,B01001111,B00001110,B00001110,B01111110,B10100000,B10100000,
B00000000,B00000000,B00000000,B00000000,B00000000
};

void setup() {
  // initialize the digital pin as an output.
  pinMode(10, OUTPUT);  
  digitalWrite(10, LOW);   // This is the LOAD or ~CS (chip select) line
  
  /*
   The MAX72XX/AS11007 is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop() { 
  
  // Scroll some text on the disply:
  scrollTestOn7Segment();

  // Fade in?out a message:
  fadeDisplay();

  // Count up from 0 to 999
  countNumbers();
  
  delay(1000);  // Pause for a short time
  
}


/*
 This method will display the characters for the
 words "7 SEG TEST HELLO! " one after the other on digit 0. 
 */
void scrollTestOn7Segment() {
  
  for(int i=0;i<25;i++)
  {
    lc.setRow(0,0,testString[i]);
    lc.setRow(0,1,testString[i+1]);
    lc.setRow(0,2,testString[i+2]);
    lc.setRow(0,3,testString[i+3]);
    lc.setRow(0,4,testString[i+4]);
    lc.setRow(0,5,testString[i+5]);
    delay(delaytime);
  }
}

// This function will print a number (from -999 to 999)
// onto digits 2,3,4,5
void printNumber(int v) {
    int ones;
    int tens;
    int hundreds;
    boolean negative = false;	

    if(v < -999 || v > 999) 
    {
       return;
    }
    if(v<0) 
    {
        negative=true;
        v=v*-1;
    }
    
    ones=v%10;
    v=v/10;
    tens=v%10;
    v=v/10;
    hundreds=v;			
    if(negative) {
       //print character '-' in the leftmost column	
       lc.setChar(0,2,'-',false);
    }
    else {
       //print a blank in the sign column
       lc.setChar(0,2,' ',false);
    }
    //Now print the number digit by digit
    lc.setDigit(0,3,(byte)hundreds,false);
    lc.setDigit(0,4,(byte)tens,false);
    lc.setDigit(0,5,(byte)ones,false);
}


/*
 This method will count up very fast from 
  0-1000 on digits 2-5.
 */
void countNumbers() {
  
  for(int i=0;i<=999;i++)
  {
    printNumber(i);
    delay(10);
  }
  
}


/*
 This method will put some data on the display and fade 
 up and down.
 */
void fadeDisplay() {
    lc.setRow(0,0,testString[16]);
    lc.setRow(0,1,testString[17]);
    lc.setRow(0,2,testString[18]);
    lc.setRow(0,3,testString[19]);
    lc.setRow(0,4,testString[20]);
    lc.setRow(0,5,testString[21]);
    
    for(int i=0;i<16;i++)
    {
      lc.setIntensity(0,i);
      delay(delaytime);
    }    
    
    for(int i=0;i<16;i++)
    {
      lc.setIntensity(0,15-i);
      delay(delaytime);
    }
    // Set intensity back to a medium value
    lc.setIntensity(0,8);    
    /* and clear the display */
    lc.clearDisplay(0);
}


