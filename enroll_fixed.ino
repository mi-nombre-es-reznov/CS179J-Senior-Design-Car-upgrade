/************************************************* 
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

uint8_t getFingerprintEnroll(int id);


// pin #11 is IN from sensor (GREEN wire)
// pin #10 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(11,10);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  // Set up Pin number 2 so that I can blink the LED 3 times in rapid succession if fingerprint scanner was found (color green). Else, solid red (3 seconds).
  pinMode(2,OUTPUT);  // Nicholas - 6 May 2019
  pinMode(4,OUTPUT);  // Nicholas - 6 May 2019

  Serial.begin(9600);
  Serial.println("fingertest");

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
    //Serial.println("We past the blinks!!!!!!!!!!!!! :("); - Testing why it wasn't blinking. Nicholas - 6 May 2019

    
    while (1);
  }
}

void loop()                     // run over and over again
{
  digitalWrite(2,LOW); // Nicholas - 6 May 2019
  digitalWrite(4,LOW); // Nicholas - 6 May 2019
  Serial.println("Type in the ID # you want to save this finger as...");
  int id = 0;
  while (true) {
    while (! Serial.available());
    char c = Serial.read();
    if (! isdigit(c)) break;
    id *= 10;
    id += c - '0';
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll(id) );
}

uint8_t getFingerprintEnroll(int id) {
  uint8_t p = -1;
  Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      digitalWrite(2,HIGH); // Nicholas - 6 May 2019
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
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
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
    digitalWrite(2,LOW); // Nicholas - 6 May 2019
  }

  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
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
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
    digitalWrite(2,HIGH); // Nicholas - 6 May 2019
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    digitalWrite(4,HIGH); // Nicholas - 6 May 2019
    delay(2000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
