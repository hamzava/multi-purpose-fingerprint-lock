#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RELAY_PIN       4
#define ACCESS_DELAY    3000 // Keep lock unlocked for 3 seconds 

int LED_GREEN = 6;
int LED_RED = 7;

void setup()
{
  //sets led pins as output
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) 
  {
  } 
  else 
  {
    while (1) { delay(1); }
  }
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   //Switch off relay initially. Relay is LOW level triggered relay so we need to write HIGH.
}

void loop()
{
  if ( getFingerPrint() != -1)
  {
    //sets relay voltage to low to retract solinoid
    digitalWrite(RELAY_PIN, HIGH);
    //Sets led to show if fingerprint matches
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    //delays the code to allow door to be opened
    delay(ACCESS_DELAY);
    //relocks the solinoid
    digitalWrite(RELAY_PIN, LOW); 
    //show door is locked
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }  
  delay(50);            //Add some delay before next scan.
}

// returns -1 if failed, otherwise returns ID #
int getFingerPrint() 
{
  int p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  return finger.fingerID;
}