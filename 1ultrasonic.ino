
// lcd display 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#include <SoftwareSerial.h>
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
long duration2;
long duration3;
long duration4;

int distance1;
int distance2;
int distance3;
int distance4;



int a[11]={0};
int generatedMatrix[12]={0};
int maxim = 0;
int indexMax = 99;

//-----------------------bluetooth-------------------------------

SoftwareSerial BTSerial(50,51); // RX | TX

char byte_serial;
char byte_bt;
char bt[100] = "";
char pin[4] ="";
char receivedPin[20]="";
bool readyC = false;

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
 
  Serial.begin(38400); // Starts the serial communication
  BTSerial.begin(38400); 
}
void loop() {

   bluetoothComm();
   readSensors();

  if (readyC)
  {
    int d = digit();
    if ( d != -1)
    {   
        Serial.print("D = ");
        Serial.println(generatedMatrix[d]);
        int temp = 0; 
        for (int i =0 ;i < 30; i++)
        { 
          delay(5);

          if (d == digit())
          {
            temp ++;
          }
        }
        if (temp > 25)
        {
          digitalWrite(led1, HIGH);
          int thisNote=2;
          int noteDuration = 1000 ;
          tone(buzzer, melody[thisNote], noteDuration);
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          noTone(buzzer);

          if(generatedMatrix[d] != 11 )
          {
            if (generatedMatrix[d] != 10)
            {
            char digit[4] = "";
            itoa(generatedMatrix[d],digit,10);
            strncat(pin,digit,strlen(digit));
            }
            else
            {
              if (strlen(pin) == 4)
              {
                Serial.println(strcmp(pin,receivedPin));
                if(strcmp(pin,receivedPin) == 0)
                {
                  Serial.print("Pinul este corect = ");
                  Serial.println(pin);
                }
                readyC = false;
                strcpy(pin,"");
              }
          }
          }
          else
          { 
            
            char elim[5]="";
            for(int j = 0; j < strlen(pin)-1; j++)
            {
              elim[j] = pin[j];  
            }

            strcpy(pin,elim);
            
          }
          
          

          Serial.print("Pin = ");
          Serial.println(pin);
          if (strlen(pin) > 4)
          {
            Serial.println(strcmp(pin,receivedPin));
            if(strcmp(pin,receivedPin) == 0)
            {
              Serial.print("Pinul este corect = ");
              Serial.println(pin);
            }
            readyC = false;
            strcpy(pin,"");
          }
        }
  
    }
    delay(800);
  }
  
  
}

void bluetoothComm()
{
   while (BTSerial.available())
                {
                  byte_bt = BTSerial.read();

                  char str1[2] = {byte_bt , '\0'};
                  strcat(bt,str1);
                }
                
                if(strcmp(bt,"") !=0)
                {
                  if(strncmp(bt,"ready",5) ==0)
                  { 

                    strncpy(receivedPin,(bt+5),4);
                    char user[20]="";
                    strcpy(user,(bt+9));
                    Serial.println(receivedPin);
                    Serial.println(user);
                    BTSerial.println(generateDigitMatrix());
                    readyC = true;
                  }

                } 
                strcpy(bt,"");
                
                byte_bt = 0;    
                if (Serial.available())
                {
                   byte_serial = Serial.read();
                   BTSerial.println(byte_serial);
                }
}

String generateDigitMatrix()
{

        char result[100] = "";
        char digit[10];
        int a[13]={0};
        
                    for(int i = 0; i<=11;i++)
                    {
                      
                      bool flag = true;
                      int r = 7;
                      r = random(0,12);
                   
                         do{
                        flag = true;
                        r = random(0,12);
                        for(int j = 0; j<i & i > 1; j++)
                        {
                          
                          if(a[j]==r)
                          {
                            flag = false;
                          }
                        }
                        
                      }while(!flag);
                      a[i]=r;
                      itoa(r,digit,10);
            
                      if(i!=11)
                      {
                        strncat(digit,"|",1);
                      }
                 
                      strncat(result,digit,strlen(digit));
                       generatedMatrix[i] = r;
                    }
                   
  
  return result;
}

int digit()
{
  if (distance1 < 25 & distance1 == min(min(distance1,distance2),min(distance3,distance4)))
  {
      if (distance1 < 4)
      {
        return 9;
      }
      
      if (distance1 > 8 & distance1 < 12)
      { 
         return 10;
      }
    
      if (distance1 > 16 & distance1 < 22)
      {
         return 11;
      }
  }

  if (distance2 < 25 & distance2 == min(min(distance1,distance2),min(distance3,distance4)))
  {
      if (distance2 < 4)
      {
        return 6;
      }
      
      if (distance2 > 8 & distance2 < 12)
      { 
         return 7;
      }
    
      if (distance2 > 16 & distance2 < 22)
      {
         return 8;
      }
  }

  if (distance3 < 25 & distance3 == min(min(distance1,distance2),min(distance3,distance4)))
  {
      if (distance3 < 4)
      {
        return 3;
      }
    
      if (distance3 > 8 & distance3 < 12)
      { 
         return 4;
      }
    
    
      if (distance3 >16 & distance3 < 22)
      {
           return 5;
      }
  }

  if (distance4 < 25 & distance4 == min(min(distance1,distance2),min(distance3,distance4)))
  {
      if (distance4 < 4)
      {
        return 0;
      }
      if (distance4 >8 & distance4 < 12)
      { 
         return 1;
      }
    
      if (distance4 >16 & distance4 < 22)
      {
           return 2;
      }
  }
  
  return -1;
}




void readSensors()
{
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
//Serial.print("Distance1: ");
//Serial.println(distance1);

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
//Serial.print("Distance2: ");
//Serial.println(distance2);

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
//Serial.print("Distance3: ");
//Serial.println(distance3);

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
//Serial.print("Distance4: ");
//Serial.println(distance4);

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

// ------------------------------------------------------------------------- First Row

}
