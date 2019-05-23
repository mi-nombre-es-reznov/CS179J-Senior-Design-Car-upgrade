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
int r = 0;  // Initialize value - Main Menu
int s = 0;  // Initialize value - Loop break
int CANCEL_BUTTON = 41;  // Used to send menu back on Raspberry Pi: Button #1
int ADD_USER = 37;       // Add User: Button #3
int DEL_USER = 39;   // Delete database: Button #2 (used w/ button 1 && 2)
int GREEN_LED = 2;
int RED_LED = 4;
int TX = 11;
int RX  = 10;
int current_finger = -1;  // Initialize with a non-existent value.
int curr_num_fingers = -1;  // Initialize with a non-existent value.
int getFingerprintIDez();
// Make an array for all values read
int in_system[128] = {0}; // Array of ID locations that have fingerprints
int count = 0;  // To iterate through array
long RandomNumber;  // Used to create a random number
int random_num; // Assign value of generated number
bool retry = false; // Flag that tells if random_num is already in database.
bool flag = false;  // Used to determine when to break out of loop and continue searching menu.

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

                                            /*  -----------------------------------------
                                             *                 System Setup
                                             *  ----------------------------------------- */

void setup()  
{
  // Initializing same pins as output signal LEDs. Nicholas - 6 May 2019
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(CANCEL_BUTTON, INPUT);
  pinMode(ADD_USER, INPUT);
  pinMode(DEL_USER, INPUT);
  
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  //Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");

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
  //Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  //Serial.println("Waiting for valid finger...");
}

                                            /*  -----------------------------------------
                                             *                 System loop
                                             *  ----------------------------------------- */

void loop()                     // run over and over again
{ 
  // Turn off LEDs every cycle
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  /* --------------------------------
   *               LEGEND
   *   1) Read/Verify fingerprint
   *   2) Add New User
   *   3) Delete Individual User
   *   4) Wrong Order
   *   5) Delete Database
   *   default) SHOULD NEVER ENTER
     -------------------------------- */

    if(Serial.available())         //From RPi to Arduino
    {
      r = (Serial.read() - '0');  //converting the value of chars to integer
      // Serial.println(r);
    }


  // Test input with Serial terminal

  switch(r)
  {
    case 1:
    {
      while(1)
      {
        getFingerprintID(); // Used to read/verify ID

        flag = check_state(); // Check if we need to break out of loop
        //Serial.println(flag);
        
        // Cause a break of the inner while loop - Turn off sensor and wait for input again
        if(flag == true)
        {
          //Serial.println("State inner: ");
          //Serial.println(flag);
          flag = false;
          break;
        }

        // flag = false;
      }
      
      break;
    }
    case 2:
    {
      // Use LEDs to indicate change of state. Turn on for two seconds. - Nicholas: 16 May 2019
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
      delay(2000);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      delay(1000);
  
      finger.getTemplateCount();
      curr_num_fingers = finger.templateCount;
      Serial.print("curr num fingers: "); Serial.println(curr_num_fingers);
  
      if(curr_num_fingers > 0)
      {
        if(current_finger == 0)
        {
          // Grant access and flash
          digitalWrite(GREEN_LED, HIGH);
          delay(100);
          digitalWrite(GREEN_LED, LOW);
          delay(100);
          digitalWrite(GREEN_LED, HIGH);
          delay(100);
          digitalWrite(GREEN_LED, LOW);
          delay(100);
          digitalWrite(GREEN_LED, HIGH);
          delay(100);
          digitalWrite(GREEN_LED, LOW);
          delay(100);
          
          // Create an array of all fingeprint ID values.
          for (int finger = 0; finger < 128; finger++) {
            downloadFingerprintTemplate(finger);
          }
      
          // Do-while loop to ensure number is unique
          do
          {
            retry = false;  // Reset to false everytime so that true doesn't cause an infinite loop.
            
            random_num = gen_rand(); // Call function to generate random number
        
            // Verify the random number is unique. Otherwise, repeat until random # is unique.
            for(int index = 0; index < 128; index++)
            {
              if(in_system[index] == random_num)
              {
                retry = true;
                break;
              }
            }
          }while(retry == true);
        
          // Save ID in database
          Serial.println("Before while loop");
          while (!  getFingerprintEnroll(random_num) )
          {
            flag = check_state;

            if(flag == true)
            {
              flag = false;
              break;
            }

            // flag = false;
          }
          Serial.println("After while loop");
  
          count = 0;  // Reset count to 0 to cause an override of values in the array so that only the unique numbers appear w/o its duplicates every iteration.
          Serial.println("Already reset value to 0");
        }
        else
        {
          // Deny access and flash red LED
          digitalWrite(RED_LED, HIGH);
          delay(100);
          digitalWrite(RED_LED, LOW);
          delay(100);
          digitalWrite(RED_LED, HIGH);
          delay(100);
          digitalWrite(RED_LED, LOW);
          delay(100);
          digitalWrite(RED_LED, HIGH);
          delay(100);
          digitalWrite(RED_LED, LOW);
          delay(100);
        }
    
      }
      else
      {
        while(!getFingerprintEnroll(0));
      }

      Serial.println("Right before break");
      break;
    }
    case 3:
    {
      //Serial.println("Option delete a user");
      curr_num_fingers = finger.templateCount;
      //Serial.println(finger.templateCount); // Use to verify that the function is working, then comment out.
  
      if(curr_num_fingers > 0)
      {
        //Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
        //uint8_t id = readnumber();
        if (current_finger == 0) {// ID #0 not allowed, try again!
          // Deny access and flash red LED
          digitalWrite(RED_LED, HIGH);
          delay(100);
          digitalWrite(RED_LED, LOW);
          delay(100);
          digitalWrite(RED_LED, HIGH);
          delay(100);
          digitalWrite(RED_LED, LOW);
          delay(100);
          digitalWrite(RED_LED, HIGH);
          delay(100);
          digitalWrite(RED_LED, LOW);
          delay(100);
          
          return; // Do not execute deletion of fingerprint 0
        }
  
        // Inform user of what is finger is being deleted.
        //Serial.print("Deleting ID #");
        //Serial.println(current_finger);
        
        deleteFingerprint(current_finger);
      }

      break;
    }
    case 4:
    {
      Wrong_order();  // Used when the user tries to access things out of order

      break;
    }
    case 5:
    {
      Delete_database();  // Used to delete entire database

      break;
    }
    default:
    {
      // Serial.println("Invalid input");
      break;
    }
  }

  // End of options - loop through process again
  delay(100);            //don't ned to run this at full speed.
  r = -1; // Reset value to 0 every iteration to keep it within the loop awaiting instructions of what state to enter.
  s = -1; // Reset value to turn off sensor
  // current_finger = -1;  // Reset the current finger ID
  flag = false;
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
    Serial.println(128); // Nicholas - 15 May 2019
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

                                            /*  -----------------------------------------
                                             *                 Wrong Order
                                             *  ----------------------------------------- */

/* ------------------------------------------------------------------------------------------------------------------------------------------------------
    Try and get write-back from Raspberry Pi. This is a readback in the event that steps are executed out of order. (e.g. Barcode scanner is used first)
   ------------------------------------------------------------------------------------------------------------------------------------------------------ */
void Wrong_order()
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
}

                                            /*  -----------------------------------------
                                             *                  Exit to Menu
                                             *  ----------------------------------------- */

/* ---------------------------------------------------------------------------------------------
    Check if user wants to exit menu. If pushed, send flag of '-128' to Raspberry Pi to Main Menu
   --------------------------------------------------------------------------------------------- */
//void Exit_to_Menu() // Nicholas - 16 May 2019
//{
//  // Turn the LED on/off 3 times to let user know command was sent. - Nicholas 16 May 2019  
//  digitalWrite(RED_LED, LOW);
//  digitalWrite(GREEN_LED, LOW);
//  delay(1000);
//  digitalWrite(RED_LED, HIGH);
//  digitalWrite(GREEN_LED, HIGH);
//  delay(500);
//  digitalWrite(RED_LED, LOW);
//  digitalWrite(GREEN_LED, LOW);
//  delay(1000);
//  digitalWrite(RED_LED, HIGH);
//  digitalWrite(GREEN_LED, HIGH);
//  delay(500);
//  digitalWrite(RED_LED, LOW);
//  digitalWrite(GREEN_LED, LOW);
//  delay(1000);
//  
//  Serial.println(-128); // Send '-128' value to the Raspberry Pi. - Nicholas 16 May 2019
//}

                                            /*  -----------------------------------------
                                             *                 Add New User
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
     This is the code for adding new fingerprints! This will scan the fingerprint and enter it with an ID into the internal database.
   ----------------------------------------------------------------------------------------------------------------------------------- */
uint8_t getFingerprintEnroll(int id) {
  uint8_t p = -1;
  //Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      digitalWrite(GREEN_LED, HIGH); // Nicholas - 6 May 2019
      delay(2000);
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
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
//    if(digitalRead(CANCEL_BUTTON) == HIGH)
//    {
//      //Serial.println(-128);
//      loop();
//      break;  // Break out of infinite loop.
//    }
  }

  // OK success!

  p = finger.image2Tz(1);
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
  
  //Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
    digitalWrite(GREEN_LED, LOW); // Nicholas - 6 May 2019
    digitalWrite(RED_LED, LOW);   // Nicholas 19 May 2019

//    if(CANCEL_BUTTON == HIGH)
//    {
//      break;
//    }
  }

  p = -1;
  //Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
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
//      if(digitalRead(CANCEL_BUTTON) == HIGH)
//      {
//        break;  // Break out of infinite loop.
//      }
    }
  }

  // OK success!

  p = finger.image2Tz(2);
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
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
    digitalWrite(GREEN_LED, HIGH); // Nicholas - 6 May 2019
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    Serial.println(128);
    digitalWrite(RED_LED, HIGH); // Nicholas - 6 May 2019
    delay(2000);
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
    Serial.println(random_num); // Write value to system. This verifies that the number was saved correctly and is ready to send to RPi.
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
  
//    if(digitalRead(CANCEL_BUTTON) == HIGH)
//    {
//      Serial.println(-128);
//      //Serial.println("New fingerprint canceled.\nPlease scan your finger.");
//      // Use LEDs to indicate change of state. Turn on for two seconds. - Nicholas: 16 May 2019
//      digitalWrite(GREEN_LED, HIGH);
//      digitalWrite(RED_LED, HIGH);
//      delay(2000);
//      digitalWrite(GREEN_LED, LOW);
//      digitalWrite(RED_LED, LOW);
//    }
}

                                            /*  -----------------------------------------
                                             *                 Delete Database
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
     This is the code for deleting the database. A code of '2' must be sent from the Raspberry Pi to the Elegoo in order to delete.
   ----------------------------------------------------------------------------------------------------------------------------------- */
void Delete_database()
{
  // Figure out how to find out how many fingerprints are in the database and print it.
  curr_num_fingers = finger.templateCount;
  
  // Verify there are fingerprints in the database.
  if(curr_num_fingers > 0)
  { 
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
    //Serial.println("Now database is empty :)");
    Serial.println(-200);

    // Reset values
    current_finger = -1;
    curr_num_fingers = -1;
  }
  else
  {
    // Serial.println("Database is already empty :("); // Let user know there are no fingerprints. - Nicholas: 16 May 2019
    Serial.println(200);
  }
}

                                            /*  -----------------------------------------
                                             *         Delete Individual Fingerprint
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
     This is the code for deleting individual fingerprints. The owner (fingerprint ID 0) cannot be deleted using this method.
   ----------------------------------------------------------------------------------------------------------------------------------- */
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);
  int delete_finger_val = 0;

  if (p == FINGERPRINT_OK) {
    //Serial.println("Deleted!");

    // Flash LEDs to give confirmation that the most recent scanned fingerprint was deleted.
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
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    // Send data to RPi to delete from its local database
    delete_finger_val = (id * -1);
    Serial.println(delete_finger_val);
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } 
  else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not delete in that location");
    Serial.println(-200);
    // Light LED red
    digitalWrite(RED_LED, HIGH);
    delay(2000);
    digitalWrite(RED_LED, LOW);
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } 
  else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   

  current_finger = -1;
  delete_finger_val = 0;
}

                                            /*  -----------------------------------------
                                             *            Random Number Generator
                                             *  ----------------------------------------- */

/* -----------------------------------------------------------------------------------------------------------------------------------
     This is the code to generate a random number. It will be compared against what ID values are in the system to ensure uniquness.
   ----------------------------------------------------------------------------------------------------------------------------------- */
int gen_rand()
{
      RandomNumber = random(1,128);
      //Serial.print("The Random number is: ");
      //Serial.println(RandomNumber);

      // Random seed
      randomSeed(analogRead(A0));
      
      return RandomNumber;
}

                                            /*  -----------------------------------------
                                             *         Fingerpint database scanner
                                             *  ----------------------------------------- */

/* --------------------------------------------------------------------------------------------------------------------------------------
   This is the code for the fingerprint checker! This will scan internal database and return values to generate an array of fingerprints.
   -------------------------------------------------------------------------------------------------------------------------------------- */
uint8_t downloadFingerprintTemplate(uint16_t id)
{ 
  //Serial.println("------------------------------------");
  //Serial.print("Attempting to load #"); Serial.println(id);
  uint8_t p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.print("Template "); Serial.print(id); Serial.println(" loaded");
      // Store value in array for later comparison
      in_system[count] = id;
      count++;
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    default:
      //Serial.print("Unknown error "); Serial.println(p);
      return p;
  }
}

bool check_state()
{
  if(Serial.available())         //From RPi to Arduino
  {
    s = (Serial.read() - '0');  //converting the value of chars to integer
    // Serial.println(s);
  }

  if(s == 1)
  {
    //Serial.println("Entered return true");
    return true;
  }

  return false;
}
