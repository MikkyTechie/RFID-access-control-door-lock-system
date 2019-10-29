
/* 16-04-2019
   ProtyTech_ solutions
   checkout "Protytech_"  on instagram for pics and videos
   by Oluwanifemi Ogunjemilua

    This code is an extended version of RFID access control door lock. everything you will need for this project is on my instagram page "Protytech_"
 */

//Include the needed libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <AddicoreRFID.h>
#include <SPI.h>
#include <Servo.h>
Servo myServo;

//here all the varriable that will be needed for this project is defined.

#define uchar   unsigned char
#define uint    unsigned int

uchar fifobytes;
uchar fifoValue;
 
// creat LiquidCrystal object to control the LCD
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
// create AddicoreRFID object to control the RFID module
AddicoreRFID myRFID;
 
 
//set the pins for RGB LED, RFID and RELAY modules
int red=8;
int green=7;
const int chipSelectPin = 10;
const int NRSTPD = 5;
//Maximum length of the array
#define MAX_LEN 16
 
void setup() {
  // RFID reader SOUT pin connected to Serial RX pin at 9600bps (check to make sure com port is set to same baud rate)
  Serial.begin(9600);
  myServo.attach (3);
  myServo.write (0);
  // start the SPI library:
  SPI.begin();
 
  // Declaring our pins as outputs/inputs and then also writing them low/high upon turning on
  
  pinMode(chipSelectPin, OUTPUT);             // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin
  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD, OUTPUT);                    // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(NRSTPD, HIGH);
  // starts the RFID and LCD module
  myRFID.AddicoreRFID_Init();
  lcd.begin(16,2);
 lcd.print("ProtyTech RFID");
  lcd.setCursor(0,1);
lcd.print(" Access control");
  delay(1000);
 
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
}}
 
void loop()
{
  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;  //Selection operation block address 0 to 63
  String mynum = "";
 
  str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
  if (status == MI_OK)
  {
    Serial.println("RFID tag detected");
    Serial.print("Tag Type:\t\t");
    uint tagType = str[0] << 8;
    tagType = tagType + str[1];
    // All of the different types of cards the module can read. Will tell you which type you have when scanned
    switch (tagType) {
      case 0x4400:
        Serial.println("Mifare UltraLight");
        break;
      case 0x400:
        Serial.println("Mifare One (S50)");
        break;
      case 0x200:
        Serial.println("Mifare One (S70)");
        break;
      case 0x800:
        Serial.println("Mifare Pro (X)");
        break;
      case 0x4403:
        Serial.println("Mifare DESFire");
        break;
      default:
        Serial.println("Unknown");
        break;
    }
  }
 
  //Anti-collision, return tag serial number 4 bytes
  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
    //What we will print to serial monitor. Change the words inside "..." to whatever you want to say. currently gives some info on card.
    checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
    Serial.print("The tag's number is:\t");
    Serial.print(str[0]);
    Serial.print(" , ");
    Serial.print(str[1]);
    Serial.print(" , ");
    Serial.print(str[2]);
    Serial.print(" , ");
    Serial.println(str[3]);
 
    Serial.print("Read Checksum:\t\t");
    Serial.println(str[4]);
    Serial.print("Calculated Checksum:\t");
    Serial.println(checksum1);
 
    // Should really check all pairs, but for now we'll just use the first
    //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor. Whatever your first number is, put here.
    if (str[0] == 224)
    {
      
      
      myServo.write (0);
      delay(1000);
      myServo.write(0);                                                  // sets the cursor to bottom left, lcd.setCursor needs to arguments (Coloumn, Row).
      
      
      
      //Serial.print "..." and lcd.print "..." can be changed to what you desire. keep in mind when lcd.setCursor is used, it is on a 16x2 LCD. So one row can only display 16 chars.
      Serial.println("\nHello Michael!\n");
      // setting cursor to top left corner
      lcd.setCursor(0,0);
      // quotation marks send characters to LCD
      lcd.print("Hello Michael");
      digitalWrite (green,HIGH);
     digitalWrite (red,LOW);  
     lcd.setCursor(0, 1);
      lcd.print("access granted!");
      delay (2000);
      lcd.clear();
      myServo.write (100);
      delay(1000);
      myServo.write(0);                                                  // sets the cursor to bottom left, lcd.setCursor needs to arguments (Coloumn, Row).
      
 
    } else if (str[0] == 86) {            // Run any other cards you have and create a new else statement for each ones number, then change what you want to happen for them.
      Serial.println("\nHello Junelle!!\n");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello Mary");
      digitalWrite (red,HIGH);
      digitalWrite (green,LOW);
      lcd.setCursor(0, 1);
      // in my example, i have denied my partners tag, not turning the relay, which would eventually open an electromagnetic door lock
      
      lcd.print("Access denied!");
      
      delay(2000);
      lcd.clear();
    }
    //Gives us a gap between scans, and a delay between readings.
    Serial.println();
    delay(1000);
  }
 
  myRFID.AddicoreRFID_Halt();          //Command tag into hibernation
 
}
