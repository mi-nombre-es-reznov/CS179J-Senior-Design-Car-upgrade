  /*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
int r = 0;

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(11, 10);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  // Initializing same pins as output signal LEDs. Nicholas - 6 May 2019
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");

    // Blinks on and off 3 times w/a 50ms wait time. (GREEN LED)
    // Nicholas - 6 May 2019
    digitalWrite(2, HIGH);  // On #1
    delay(50);  // wait
    digitalWrite(2,LOW); // Off #1
    delay(50);  // wait
    digitalWrite(2, HIGH);  // On #2
    delay(50);  // wait
    digitalWrite(2,LOW);  // Off #2
    delay(50);  // wait
    digitalWrite(2, HIGH);  // On #3
    delay(50);  // wait
    digitalWrite(2,LOW);  // Off #3
    delay(50);  // wait
  } else {
    Serial.println("Did not find fingerprint sensor :(");

    // Blinks on and off 3 times w/a 50ms wait time. (RED LED)
    // Nicholas - 6 May 2019
    digitalWrite(4, HIGH);  // On #1
    delay(50);  // wait
    digitalWrite(4,LOW); // Off #1
    delay(50);  // wait
    digitalWrite(4, HIGH);  // On #2
    delay(50);  // wait
    digitalWrite(4,LOW);  // Off #2
    delay(50);  // wait
    digitalWrite(4, HIGH);  // On #3
    delay(50);  // wait
    digitalWrite(4,LOW);  // Off #3
    delay(50);  // wait
    
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{  
  getFingerprintID();
  recieve_input();
  delay(100);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() { 
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");

    digitalWrite(2,HIGH);
    delay(2000);
    digitalWrite(2,LOW);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {    
    //Serial.println("Did not find a match");
    //Serial.println("0");

    digitalWrite(4,HIGH);
    delay(2000);
    digitalWrite(4,LOW);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.println("1");
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!  
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
























// Try and get write-back from Raspberry Pi
void recieve_input()
{
    if(Serial.available()){         //From RPi to Arduino
    r = (Serial.read() - '0');  //conveting the value of chars to integer
    Serial.println(r);
  }

  if(r == 3)
  {
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    digitalWrite(2, LOW);
    delay(1000);

    r = 0;
  }
}
