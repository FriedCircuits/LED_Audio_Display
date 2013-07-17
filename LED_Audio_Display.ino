/*
LED Audio Display v.1
Created: 07/05/2013
Updated: 07/05/2013
by: William Garrido
2x 8x8 LED Maxtrix Control
https://www.tindie.com/products/FriedCircuits/led-matrix-link/
Used example from http://tronixstuff.wordpress.com/tag/msgeq7-2/

*/
#include <Font.h>
#include <LedControl.h>

int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5 
int left[7]; // store band values in these arrays
int right[7]; 
int band;
const int devCount = 2;
const int ledVCount = 8;
const int ledHCount = devCount * 8;
const int colCount = 8 * devCount;
int binLED[colCount*8];


// pin 6 is connected to the MAX7219\AMS1107 pin 1
// pin 7 is connected to the CLK pin 13
// pin 3 is connected to LOAD pin 12

LedControl lc=LedControl(6,7,3,devCount); 
int brightness = 15;


void setup()
{
  Serial.begin(115200);
  pinMode(res, OUTPUT); // reset
  pinMode(strobe, OUTPUT); // strobe
  digitalWrite(res,LOW); // reset low
  digitalWrite(strobe,HIGH); //pin 5 is RESET on the shield
  
  for(int address=0;address<devCount;address++) {
    /*The MAX72XX is in power-saving mode on startup*/
    lc.shutdown(address,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(address,brightness);
    /* and clear the display */
    lc.clearDisplay(address);
  }
    
}



void loop()
{
  int val = 0;
  readMSGEQ7();
  int leftBandCount = 0;
  int rightBandCount = 0;
  for(int x=0; x<colCount; x++)
  {

    if (x < 7){
      val = map(left[leftBandCount], 0, 1023, 0, 8);
      //val = constrain(val, 0, 7);
      loadBin(x, val);
      leftBandCount++;

    }
    else if (x >= colCount-7){
      int val = map(right[rightBandCount], 0, 1023, 0, 8);
      //val = constrain(val, 0, 7);
      loadBin(x, val);
      rightBandCount++;
    }
  }
  
 setDisplay();
  
}

void readMSGEQ7()
// Function to read 7 band equalizers
{
  digitalWrite(res, HIGH);
  digitalWrite(res, LOW);
  for(band=0; band <7; band++)
  {
    digitalWrite(strobe,LOW); // strobe pin on the shield - kicks the IC up to the next band 
    delayMicroseconds(30); // 
    left[band] = analogRead(0); // store left band reading
    right[band] = analogRead(1); // ... and the right
    digitalWrite(strobe,HIGH); 
 }
}

void setDisplay()
{
  int currPos = 0;
  for (int d=devCount-1;d>=0;d--) // Loop through displays
  {
    for (int r=0;r<8;r++) // Set up rows on current display
    {  
      lc.setRow(d,r,binLED[currPos]);
      currPos++;
    }
            
  } 
}

void loadBin(int x, int val)
{
        switch (val){
        case 0:
            binLED[x] = B00000000;
            break;
        case 1:
            binLED[x] = B10000000;
            break;
        case 2:
            binLED[x] = B11000000;
            break;
        case 3:
            binLED[x] = B11100000;
            break;
        case 4:
            binLED[x] = B11110000;
            break;
        case 5:
            binLED[x] = B11111000;
            break;
        case 6:
            binLED[x] = B11111100;
            break;
        case 7:
            binLED[x] = B11111110;
            break;
        case 8:
            binLED[x] = B11111111;
            break;
        default:
            binLED[x] = B11111111;
      }
      
}
     


