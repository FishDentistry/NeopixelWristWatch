// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      12

const int buttonPin = 2;  
int buttonState = 0; 

long prev = 0;
int DURATION_IN_MILLIS = 1000;

int currmin = 0; 

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  // End of trinket special code
  pinMode(buttonPin, INPUT);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(64);
  pixels.show();

  Serial.begin(9600);
  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
     //rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  int hourpick = hourSetup();
  currmin = minSetup(); //Assume this is from watch setup
  rtc.adjust(DateTime(2017, 1, 27, hourpick, currmin*5, 0));
}

void loop() {

  buttonState = digitalRead(buttonPin);
  DateTime now = rtc.now();
  int currhour = now.hour();
  int newmin = now.minute();
  if (newmin % 5 == 0){
    currmin = newmin/5;
  }

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if (currhour >= 12){
      if(currhour == 24){
        currhour = 0;
        Serial.print(currhour);
      }else{
        currhour = currhour - 12;
        Serial.print(currhour);
      }
    }
    //if (newmin % 5 == 0){
      //currmin = newmin/5;
      pixels.setPixelColor(currhour, pixels.Color(255,0,0)); // Moderately bright green color.
    pixels.setPixelColor(currmin, pixels.Color(255,0,255)); // Moderately bright green color.
    pixels.show();
    delay(1000);
    pixels.setPixelColor(currmin, pixels.Color(0,0,0));
    pixels.show();
    delay(1000);
    pixels.setPixelColor(currmin, pixels.Color(255,0,255));
    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(2000); // Delay for a period of time (in milliseconds).
    pixels.setPixelColor(currmin, pixels.Color(0,0,0));
    pixels.setPixelColor(currhour, pixels.Color(0,0,0));
    pixels.show();
    //}
    //else{
     // pixels.setPixelColor(currhour, pixels.Color(255,0,0)); // Moderately bright green color.
    //pixels.setPixelColor(currmin, pixels.Color(255,0,255)); // Moderately bright green color.
   // pixels.show();
   // delay(1000);
   // pixels.setPixelColor(currmin, pixels.Color(0,0,0));
   // pixels.show();
   // delay(1000);
   // pixels.setPixelColor(currmin, pixels.Color(255,0,255));
   // pixels.show(); // This sends the updated pixel color to the hardware.

   // delay(2000); // Delay for a period of time (in milliseconds).
   // pixels.setPixelColor(currmin, pixels.Color(0,0,0));
   // pixels.setPixelColor(currhour, pixels.Color(0,0,0));
   // pixels.show();
   // }
   // Delay for a period of time (in milliseconds).

  } 

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

}

int hourSetup(){
  int hold = 1;
  int test = 0;
  pixels.setPixelColor(0, pixels.Color(255,0,0));
  pixels.show();
  while(hold == 1){
    
  buttonState = 0;
  if(digitalRead(buttonPin)){
    prev = millis();
    buttonState = 1;
    while((millis()-prev)<=DURATION_IN_MILLIS){
      if(!(digitalRead(buttonPin))){
        buttonState = 2;
        break;
      }
    }
  }
 
  if(!buttonState){
    // TODO nothing is pressed
  }else if(buttonState == 1){
    // TODO button is pressed long
    hold = 2;
    Serial.print("Exiting setup");
    pixels.setPixelColor(test, pixels.Color(255,0,0));
    pixels.show();
    return test;
  }else if(buttonState ==2){
    //TODO button is pressed short
    pixels.setPixelColor(test, pixels.Color(0,0,0));
    pixels.show();
    test = test + 1;
    if (test == 12){
      test = 0;
    }
    pixels.setPixelColor(test, pixels.Color(255,0,0));
    pixels.show();
    Serial.print(test);
  }
  }
}


int minSetup(){
  int hold = 1;
  int test = 0;
  pixels.setPixelColor(0, pixels.Color(0,255,0));
  pixels.show();
  while(hold == 1){
    
  buttonState = 0;
  if(digitalRead(buttonPin)){
    prev = millis();
    buttonState = 1;
    while((millis()-prev)<=DURATION_IN_MILLIS){
      if(!(digitalRead(buttonPin))){
        buttonState = 2;
        break;
      }
    }
  }
 
  if(!buttonState){
    // TODO nothing is pressed
  }else if(buttonState == 1){
    // TODO button is pressed long
    hold = 2;
    Serial.print("Exiting setup");
    pixels.setPixelColor(test, pixels.Color(0,255,0));
    pixels.show();
    return test;
  }else if(buttonState ==2){
    //TODO button is pressed short
    pixels.setPixelColor(test, pixels.Color(0,0,0));
    pixels.show();
    test = test + 1;
    if (test == 12){
      test = 0;
    }
    pixels.setPixelColor(test, pixels.Color(0,255,0));
    pixels.show();
    Serial.print(test);
  }
  }
}
