
// lcd display 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
//---------------------

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

// defines pins numbers
const int buzzer = 10;

const int led1 = 23;
const int led2 = 24;
const int led3 = 25;
const int trigPin1 = 3;
const int echoPin1 = 2;
const int trigPin2 = 5;
const int echoPin2 = 4;
const int trigPin3 = 7;
const int echoPin3 = 6;
const int trigPin4 = 9;
const int echoPin4 = 8;
// defines variables
long duration1;
int distance1;
long duration2;
int distance2;
long duration3;
int distance3;
long duration4;
int distance4;
void setup() {
  // -------------------------------------------------------- lcd
    lcd.begin();
    lcd.backlight();
  //--------------------------------------------------------- ultrasonic sensors 
pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
pinMode(trigPin4, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin4, INPUT); // Sets the echoPin as an Input

/// ----------------------------------------------------------buzzer
pinMode(buzzer, OUTPUT);

/// ---------------------------------------------------------- LEDS
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
 
Serial.begin(9600); // Starts the serial communication
}
void loop() {
// ------------------------------------------------------------------------- ultrasonic sensors
// ------------------------------- senzor 1 
// Clears the trigPin
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration1 = pulseIn(echoPin1, HIGH);
// Calculating the distance
distance1 = duration1*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance1: ");
Serial.println(distance1);

// ------------------------------ senzor 2 
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration2= pulseIn(echoPin2, HIGH);
// Calculating the distance
distance2 = duration2*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance2: ");
Serial.println(distance2);

// ------------------------------ senzor 3 
digitalWrite(trigPin3, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin3, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin3, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration3= pulseIn(echoPin3, HIGH);
// Calculating the distance
distance3 = duration3*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance3: ");
Serial.println(distance3);

//-------------------------------senzor 4
digitalWrite(trigPin4, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin4, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin4, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration4= pulseIn(echoPin4, HIGH);
// Calculating the distance
distance4 = duration4*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance4: ");
Serial.println(distance4);

// ------------------------------------------------------------------------- LCD
  lcd.clear();
  //-------------------distance 1 
  lcd.setCursor(0,0);
  lcd.print("D1 ");
  lcd.setCursor(3,0);
  lcd.print(distance1);
   //-------------------distance 3 
  lcd.print("  ");
  lcd.print("D3 ");
  lcd.print(distance3);
  // ------------------distance 2
  lcd.setCursor(0,1);
  lcd.print("D2 ");
  lcd.setCursor(3,1);
  lcd.print(distance2); 
 
  //------------------distance 4 
  lcd.print("  D4 ");
  lcd.print(distance4);

// ------------------------------------------------------------------------- LEDS
  

  if (distance1 < 4)
  {
    digitalWrite(led1, HIGH);
    int thisNote=2;
    int noteDuration = 1000 ;
    tone(buzzer, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzer);
  }
  else 
  {
    digitalWrite(led1, LOW); 
  }
  
  if (distance1 >4 & distance1 < 8)
  {
     digitalWrite(led2, HIGH);
  }
  else
  {
     digitalWrite(led2, LOW);
  }

  if (distance1 >10 & distance1 < 15)
  {
     digitalWrite(led3, HIGH);
  }
  else
  {
     digitalWrite(led3, LOW);
  }
  

 
}
