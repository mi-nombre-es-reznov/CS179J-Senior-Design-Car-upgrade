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

// Global variables
int r = 0;  // Initialize value
int CANCEL_BUTTON = 41;  // Used to send menu back on Raspberry Pi: Button #1
int ADD_USER = 37;       // Add User: Button #3
int DEL_DATABASE = 39;   // Delete database: Button #2 (used w/ button 1 && 2)
int GREEN_LED = 2;
int RED_LED = 4;
int TX = 11;
int RX  = 10;
int current_finger = -1;  // Initialize with a non-existent value.
int curr_num_fingers = -1;  // Initialize with a non-existent value.

// Call functions
uint8_t getFingerprintEnroll(int id);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial

// For UNO and others without hardware serial, we must use software serial...
// pin #11 is IN from sensor (GREEN wire) - TX
// pin #10 is OUT from arduino  (WHITE wire) - RX
// comment these two lines if using hardware serial
SoftwareSerial mySerial(TX, RX);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  // Initializing same pins as output signal LEDs. Nicholas - 6 May 2019
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(CANCEL_BUTTON, INPUT);
  pinMode(ADD_USER, INPUT);
  pinMode(DEL_DATABASE, INPUT);
  
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
    digitalWrite(GREEN_LED, HIGH);  // On #1
    delay(50);  // wait
    digitalWrite(GREEN_LED, LOW); // Off #1
    delay(50);  // wait
    digitalWrite(GREEN_LED, HIGH);  // On #2
    delay(50);  // wait
    digitalWrite(GREEN_LED, LOW);  // Off #2
    delay(50);  // wait
    digitalWrite(GREEN_LED, HIGH);  // On #3
    delay(50);  // wait
    digitalWrite(GREEN_LED, LOW);  // Off #3
    delay(50);  // wait
  } else {
    Serial.println("Did not find fingerprint sensor :(");

    // Blinks on and off 3 times w/a 50ms wait time. (RED LED)
    // Nicholas - 6 May 2019
    digitalWrite(RED_LED, HIGH);  // On #1
    delay(50);  // wait
    digitalWrite(RED_LED, LOW); // Off #1
    delay(50);  // wait
    digitalWrite(RED_LED, HIGH);  // On #2
    delay(50);  // wait
    digitalWrite(RED_LED, LOW);  // Off #2
    delay(50);  // wait
    digitalWrite(RED_LED, HIGH);  // On #3
    delay(50);  // wait
    digitalWrite(RED_LED, LOW);  // Off #3
    delay(50);  // wait
    
    while (1) { delay(1); }
  }

  // Figure out how to find out how many fingerprints are in the database and print it.
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{ 
  getFingerprintID();
  recieve_input();

  // Decide what area to go: Scan fingerprint || Exit to Menu || Create new fingerprint || Delete database
  if(digitalRead(CANCEL_BUTTON) == HIGH)
  {
    Exit_to_Menu();         // Check for input to send flag to exit menu
  }
  else if(digitalRead(ADD_USER) == HIGH)
  {
    // Use LEDs to indicate change of state. Turn on for two seconds. - Nicholas: 16 May 2019
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    
    // Stuff needed to call function
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
  else if(digitalRead(DEL_DATABASE) == HIGH)
  {
    // Figure out how to find out how many fingerprints are in the database and print it.
    //finger.getTemplateCount();
    curr_num_fingers = finger.templateCount;
    Serial.println(finger.templateCount);
    
    // Verify there are fingerprints in the database.
    if(curr_num_fingers > 0)
    {
      // Ask for owner's OG fingerprint. If true, delete.
      Serial.println("Deleting database. Please scan the owner's first fingerprint.");
      getFingerprintID();
      if(current_finger == 0)
      {
            // Call to empty database
            if (finger.verifyPassword()) {
              Serial.println("Found fingerprint sensor!");
            } else {
              Serial.println("Did not find fingerprint sensor :(");
              while (1);
            }
            
            // LED flashing sequence to let user know database is being deleted
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, HIGH);
            delay(500);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, LOW);
            delay(500);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, HIGH);
            delay(500);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, LOW);
            delay(500);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, HIGH);
            delay(500);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, LOW);
            delay(500);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, HIGH);
            delay(2000);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, LOW);
            delay(1000);
            digitalWrite(GREEN_LED, HIGH);
            delay(50);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
            delay(50);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            delay(50);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
            delay(50);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            delay(50);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
            delay(50);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            delay(50);
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, HIGH);
            delay(50);
            digitalWrite(GREEN_LED, HIGH);
            digitalWrite(RED_LED, LOW);
            delay(50);
            
            // Turn off both LEDs after execution - Nicholas: 16 May 2019
            digitalWrite(GREEN_LED, LOW);
            digitalWrite(RED_LED, LOW);
            
            finger.emptyDatabase();
            Serial.println("Now database is empty :)");

            // Reset values
            current_finger = -1;
            curr_num_fingers = -1;
      }
    }
    else
    {
      Serial.println("Database is already empty :("); // Let user know there are no fingerprints. - Nicholas: 16 May 2019
    }
  }

  // End of options - loop through process again
  delay(100);            //don't ned to run this at full speed.
}

                                            /*  -----------------------------------------
                                             *            Fingerprint Code
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
   This is the code for the fingerprint checker! This will scan the fingerprint and compare with the images in the internal database.
   ----------------------------------------------------------------------------------------------------------------------------------- */
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

    digitalWrite(GREEN_LED, HIGH);
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {    
    //Serial.println("Did not find a match");
    Serial.println("-1"); // Nicholas - 15 May 2019
    digitalWrite(RED_LED, HIGH);
    delay(2000);
    digitalWrite(RED_LED, LOW);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  //Serial.println("1");   Holding out for now. Trying to pass in user-specific fingerprint to Raspberry Pi.
  //Serial.print("Found ID #"); 
  Serial.println(finger.fingerID); // Unlocking to track finger ID number for 2-factor authentication.
  //Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  current_finger = finger.fingerID;   // Use this to assign the most recent value of the most recent scanned fingerprint. Used for deleting database. - Nicholas: 16 May 2019
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


/* ------------------------------------------------------------------------------------------------------------------------------------------------------
    Try and get write-back from Raspberry Pi. This is a readback in the event that steps are executed out of order. (e.g. Barcode scanner is used first)
   ------------------------------------------------------------------------------------------------------------------------------------------------------ */
void recieve_input()
{
    if(Serial.available()){         //From RPi to Arduino
    r = (Serial.read() - '0');  //conveting the value of chars to integer
    Serial.println(r);
  }

  if(r == 3)
  {
    // Turn off both LEDs just in case - Nicholas: 16 May 2019
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    // Flash LEDS on/off w/opposite lights - Nicholas: 16 May 2019
    digitalWrite(GREEN_LED, HIGH);
    delay(50);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(50);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(50);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(50);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(50);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(50);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(50);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(50);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(50);

    // Turn off both LEDs after execution - Nicholas: 16 May 2019
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    r = 0;  // Reset value of 'r' so that execution of lights doesn't constantly execute.
  }
}

/* ---------------------------------------------------------------------------------------------
    Check if user wants to exit menu. If pushed, send flag of '-3' to Raspberry Pi to Main Menu
   --------------------------------------------------------------------------------------------- */
void Exit_to_Menu() // Nicholas - 16 May 2019
{
  // Turn the LED on/off 3 times to let user know command was sent. - Nicholas 16 May 2019  
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(1000);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(1000);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(1000);
  
  Serial.println(-2); // Send '-2' value to the Raspberry Pi. - Nicholas 16 May 2019
}


                                            /*  -----------------------------------------
                                             *                 Add New User
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
     This is the code for adding new fingerprints! This will scan the fingerprint and enter it with an ID into the internal database.
   ----------------------------------------------------------------------------------------------------------------------------------- */
uint8_t getFingerprintEnroll(int id) {
  uint8_t p = -1;
  Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      digitalWrite(GREEN_LED, HIGH); // Nicholas - 6 May 2019
      delay(2000);
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
      
    // Check for button press for canceling fingerprint entry.
    if(digitalRead(CANCEL_BUTTON) == HIGH)
    {
      break;  // Break out of infinite loop.
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
    digitalWrite(GREEN_LED, LOW); // Nicholas - 6 May 2019
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
      
      // Check for button press for canceling fingerprint entry.
      if(digitalRead(CANCEL_BUTTON) == HIGH)
      {
        break;  // Break out of infinite loop.
      }
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
    digitalWrite(GREEN_LED, HIGH); // Nicholas - 6 May 2019
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    digitalWrite(RED_LED, HIGH); // Nicholas - 6 May 2019
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


  // Turn off LEDS
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  
    if(digitalRead(CANCEL_BUTTON) == HIGH)
    {
      Serial.println("New fingerprint canceled.\nPlease scan your finger.");
      // Use LEDs to indicate change of state. Turn on for two seconds. - Nicholas: 16 May 2019
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
      delay(2000);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
    }
}


























                                            /*  -----------------------------------------
                                             *                 Delete Database
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
     This is the code for deleting the database. All three buttons must be activated and fingerprint value 0 (owner) must be scanned.
   ----------------------------------------------------------------------------------------------------------------------------------- */
