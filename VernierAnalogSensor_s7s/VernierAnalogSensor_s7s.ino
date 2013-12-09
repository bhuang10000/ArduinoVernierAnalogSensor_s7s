/* Serial 7-Segment Display Example Code
 Serial Mode Stopwatch
 by: Jim Lindblom
 SparkFun Electronics
 date: November 27, 2012
 license: This code is public domain.
 
 This example code shows how you could use software serial
 Arduino library to interface with a Serial 7-Segment Display.
 
 There are example functions for setting the display's
 brightness, decimals and clearing the display.
 
 The print function is used with the SoftwareSerial library
 to send display data to the S7S.
 
 Circuit:
 Arduino -------------- Serial 7-Segment
 5V   --------------------  VCC
 GND  --------------------  GND
 8   --------------------  RX
 */
#include <SoftwareSerial.h>
#define DELIMITTER '\t'

// These are the Arduino pins required to create a software seiral
//  instance. We'll actually only use the TX pin.
const int softwareTx = 8;
const int softwareRx = 7;

SoftwareSerial s7s1(softwareRx, 12);
SoftwareSerial s7s2(softwareRx, 13);

unsigned int counter = 0;  // This variable will count up to 65k
char tempString[10];  // Will be used with sprintf to create strings
char tempString2[10];

unsigned long timeRef;
unsigned long currTime;
int ndx;
int refreshRate = 50;

void setup()
{
  // Must begin s7s software serial at the correct baud rate.
  //  The default of the s7s is 9600.
  String myString = "Sens1";
  Serial.begin(9600);
  s7s1.begin(9600);
  s7s2.begin(9600);

  // Clear the display, and then turn on all segments and decimals
  clearDisplay();  // Clears display, resets cursor
  setDecimals(0b000000);  // Turn on all decimals, colon, apos

  // Clear the display before jumping into loop
  clearDisplay();  
  timeRef = millis();
}

void loop()
{
  currTime = millis();
  if (currTime >= ndx*refreshRate + timeRef)
  {
    ndx++;
    // Magical sprintf creates a string for us to send to the s7s.
    //  The %4d option creates a 4-digit integer.
    Serial.print(currTime / 1000.0, 2);
    Serial.print(DELIMITTER);
    Serial.print(analogRead(A0));
    Serial.print(DELIMITTER);
    Serial.print(analogRead(A2));
    Serial.println();
    sprintf(tempString, "%4d", analogRead(A0));
    sprintf(tempString2, "%4d", analogRead(A2));

    // This will output the tempString to the S7S
    s7s1.print(tempString);
    s7s2.print(tempString2);

    setDecimals(0b00000000);  // Sets digit 3 decimal on
    counter++;
  }
}

// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplay()
{
  s7s1.write(0x76);  // Clear display command
  s7s2.write(0x76);
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightness(byte value)
{
  s7s1.write(0x7A);  // Set brightness command byte
  s7s1.write(value);  // brightness data byte
  s7s2.write(0x7A);  // Set brightness command byte
  s7s2.write(value);  // brightness data byte

}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal 
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimals(byte decimals)
{
  s7s1.write(0x77);
  s7s1.write(decimals);
  s7s2.write(0x77);
  s7s2.write(decimals);

}


