/*! 
    @file     Final_Project_Stem_2023.pde
    @author   Andrew Talpins
*/
#define REST 0
#define  C3  131
#define  CS3 139
#define  D3  147
#define  DS3 156
#define  E3  165
#define  F3  175
#define  F S3 185
#define  G3  196
#define  GS3 208
#define  A3  220
#define  AS3 233
#define  B3  247
#define  C4  262
#define  CS4 277
#define  D4  294
#define  DS4 311
#define  E4  330
#define  F4  349
#define  FS4 370
#define  G4  392
#define  GS4 415
#define  A4  440
#define  AS4 466
#define  B4  494
#define  C5  523
#define  CS5 554
#define  D5  587
#define  DS5 622
#define  E5  659
#define  F5  698
#define  FS5 740
#define  G5  784
#define  GS5 831
#define  A5  880
#define  AS5 932
#define  B5  988
#define  C6  1047
#define  CS6 1109


#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

#define RST_PIN         5          
#define SS_PIN          10         

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


LiquidCrystal_I2C lcd(0x27,16,2); // Set up LCD

int speakerPin = 7;

int start = 4; // pin of Button to start name process
#define slider A1 
int buttonstatus; // variable for button status 
char name[] = "____________"; // Blank array for name

char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz -"; // Array for possible characters

int notes[] = {E5,G5,A5,C6,B5,A5,G5,F5,E5,C5,D5,E5,F5,E5,D5,G4,A4,B4,C5};
int delays[] = {400,200,200,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100};
int songLen;

void setup(void) {
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); //Set cursor to the top left of LCD
  pinMode(speakerPin, OUTPUT);
  Serial.begin(115200); // Set up Serial Monitor
  while (!Serial) delay(10); 

  Serial.println("Hello!");

  SPI.begin();
  mfrc522.PCD_Init();		// Init MFRC522

}


void loop(void) {
	if ( ! mfrc522.PICC_IsNewCardPresent()) {

lcd.setCursor(0,0); //Set cursor to the top left of LCD
lcd.clear();
lcd.print("WAITING...");
buttonstatus = digitalRead(start); //read the button status

  if (buttonstatus == 1)
{
  Serial.println("clicked");
  lcd.clear();
  lcd.print(name);
  int i = 0;
  while (i < 100) // set a while loop to go through and set the name
  {
    int slideInput = analogRead(slider); // input of the potentiometer
    int numofLetter = slideInput / 18; // divide by 18, because there are 1024 possible values, and 54 possible letters
	//Serial.println(letters[numofLetter]);    
    name[i] = letters[numofLetter]; // set the ith element of the name array to the letter chosen
    lcd.setCursor(0,0);
    lcd.print(name); // LCD out the name so the person can see what letter it is
    int butInput = digitalRead(start); 
    if (butInput == 1) // if button is pressd while in the loop
    {
	  delay(300);
      if (digitalRead(start) == 1){ //if button is still held
        lcd.clear();
        lcd.print("HOLD TO EXIT"); 
        delay(1500);
        if (digitalRead(start) == 1){ // wait 1.5 seconds. If still held exit the loop
    	  lcd.clear();
          lcd.print("LET GO");
          i = 400;
          delay (1000);
        }
      }
      name[i] = letters[numofLetter]; // if button isn't held down, set the character chosen to the name array 
      lcd.clear();
      lcd.print(name);
      i +=1; // add one to i to go through the loop again for the next element in the array
    
    }
  
}
  }

if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  lcd.clear();
  lcd.setCursor(0,0); //Set cursor to the top left of LCD
  lcd.print("WELCOME BACK ");
  lcd.setCursor(0, 1);
  lcd.print(name);
  song();
  delay(8000);
  }
}

void song()
{
  int songLen = sizeof(notes);
  songLen = songLen/2;
  for (int i = 0;i<songLen; i=i+1){
    Serial.println(notes[i]);
    float pause = delays[i] * 1.2;
    tone(7, notes[i], delays[i]);
    delay(pause);
}
}
