
                                            /// Proiect de laborator "Touchless payment method"
                                            //  Balanean Dan, grupa 30238

#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#include <SoftwareSerial.h>
// Setam aderesa LCD ului: 0x27 
LiquidCrystal_I2C lcd(0x27, 16, 2);


int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};



// defines pins numbers
const int buzzer = 10;

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
// variabilele utilizate in comunicarea cu modulul bluetooth

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

 
  Serial.begin(38400); // Incepem comunicarea seriala (utilizata in timpul dezvoltarii proiectului)
  BTSerial.begin(38400); // Incepem comunicarea cu modulul BT la un baud rate de 38400
  
}

// in bucla loop, am apelat functiile necesare comunicarii si activarii senzorilor, si am scris logica de functionare a sistemului
void loop() {

   bluetoothComm();
   readSensors();

  if(!readyC)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Introduceti pin!");
  }
  if (readyC)
  {
    
    int d = digit();
                                                    // daca distanta citita de un senzor se incadreaza in cerintele stabilite in functia digit(), o vom putea interpreta ca tasta
    if ( d != -1)    
    {   
        Serial.print("D = ");
        Serial.println(generatedMatrix[d]);
        int temp = 0; 
        
                                                    // pentru confirmarea alegerii unei taste, sistemul va face mai multe masuratori consecutive si va compara rezultatul acestora 
        
        for (int i =0 ;i < 30; i++)           
        { 
          delay(5);

          if (d == digit())
          {
            temp ++;
          }
        }
        if (temp > 25)
        {                                           // pentru a indica utilizatorului ca tasta a fost acceptata, se va emite un semnal sonor folosind buzzerul  
          
          int thisNote=2;
          int noteDuration = 1000 ;
          tone(buzzer, melody[thisNote], noteDuration);
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          noTone(buzzer);

          if(generatedMatrix[d] != 11 ) // !delete   --- cat timp tasta e diferita de delete
          {
            if (generatedMatrix[d] != 10) //! ok     --- cat timp tasta e diferita de ok 
            {
            char digit[4] = "";
            itoa(generatedMatrix[d],digit,10);        // se incepe compunerea noului pin folosind cifra nou introdusa
            strncat(pin,digit,strlen(digit));
            }
            else                                      // atunci cand tasta ok este apasata si pinul are lungimea potrivita, se va face comparatia si se va da un raspuns corespunzator (acceptare/respingere plata )
            {
              if (strlen(pin) == 4)
              {
                Serial.println(strcmp(pin,receivedPin));
                if(strcmp(pin,receivedPin) == 0)
                {
                  Serial.print("Pinul este corect = ");
                  Serial.println(pin);
                  confirmSound();
                  readyC = false;
                  strcpy(pin,"");
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Plata acceptata!");
                  delay(1000);
                  
                }
                else
                {
                  Serial.print("Pinul nu este corect. Reincercati! ");
                  failSound();
                  lcd.clear();
                  lcd.setCursor(0,0);
                  lcd.print("Reincercati !");
                  delay(1000);
                }
                //readyC = false;
                strcpy(pin,"");             // se reinitializeaza pinul introdus la ""
              }
          }
          }
          else                              // in caz ca tasta delete e apasata, se va sterge ultima cifra introdusa din pin
          { 
            
            char elim[5]="";
            for(int j = 0; j < strlen(pin)-1; j++)
            {
              elim[j] = pin[j];  
            }

            strcpy(pin,elim);
            
          }
          
        }
  
    }
    delay(800);
  }
  
  
}

// generarea sunetelor de confirmare
void confirmSound()
{
  for(int i=0; i<4;i++)
  {
      int thisNote=2;
      int noteDuration = 1000 ;
      tone(buzzer, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(buzzer);
  }
  
}
//generarea unui sunet mai lung care sugereaza esecul actiunii
void failSound()
{

      int thisNote=3;
      int noteDuration = 2000 ;
      tone(buzzer, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(buzzer);
  
}
// functia care citeste si scrie date, comunicare cu modul BT
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
                  if(strncmp(bt,"ready",5) ==0)   // la primirea mesajului ready, sistemul se pregateste de o noua plata, se primesc datele personale: nume, pin
                  { 

                    strncpy(receivedPin,(bt+5),4);  //se citeste pinul
                    char user[20]="";
                    strcpy(user,(bt+9));            // se citeste numele
                    Serial.println(receivedPin);  
                    Serial.println(user);
                    BTSerial.println(generateDigitMatrix());  // se trimite spre telefon, prin BT, matricea de taste asezate aleator
                    readyC = true;                            // variabila care semnaleaza inceperea platii
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

// aceasta functie va returna sub forma unui string, indexul fiecarei taste intr-o ordine specifica, separat prin caracterul "|"
String generateDigitMatrix()
{

        char result[100] = "";
        char digit[10];
        int a[13]={0};
        
        //// generarea a valorii pentru fiecare pozitie din matrice. valoarea va reprezenta valoarea tastei de la 0 la 9, nr 10 va reprezenta tasta 0k, iar 11 tasta delete
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

// aceasta functie va returna pozitia din matrice pe care senzorii utrasonici o detecteaza.
// fiecare senzor ultrasonic va putea identifica 3 intervale in care se poate afla degetul utilizatorului
// aceste intervale sunt stabilite astfel incat, sa nu se suprapuna, iar utilizatorul sa formeze tasta dorita de el
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



// in aceasta functie am calculat distantele citite de fiecare senzor ultrasonic, variabile globale care vor fi folosite ulterior
void readSensors()
{
  // ------------------------------------------------------------------------- ultrasonic sensors
// ------------------------------- senzor 1 
// Clears the trigPin
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
// setam trig pin pe HIGH 
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
// functia pulseIn returneaza timpul in care unda de sunet a ajuns inapoi la senzor
duration1 = pulseIn(echoPin1, HIGH);
// calculam distanta, tinand cont de viteza sunetului exprimata in cm/micro secunde   (340m/s = 0.034cm/10^-6 s)
distance1 = duration1*0.034/2;


// ------------------------------ senzor 2 
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
duration2= pulseIn(echoPin2, HIGH);
distance2 = duration2*0.034/2;


// ------------------------------ senzor 3 
digitalWrite(trigPin3, LOW);
delayMicroseconds(2);
digitalWrite(trigPin3, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin3, LOW);
duration3= pulseIn(echoPin3, HIGH);

distance3 = duration3*0.034/2;


//-------------------------------senzor 4
digitalWrite(trigPin4, LOW);
delayMicroseconds(2);
// setam trig pin pe HIGH 
digitalWrite(trigPin4, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin4, LOW);
// functia pulseIn returneaza timpul in care unda de sunet a ajuns inapoi la senzor
duration4= pulseIn(echoPin4, HIGH);
// calculam distanta, tinand cont de viteza sunetului exprimata in cm/micro secunde   (340m/s = 0.034cm/10^-6 s)
distance4 = duration4*0.034/2;


}
