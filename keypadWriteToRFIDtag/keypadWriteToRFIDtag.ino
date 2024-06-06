#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode card_status;

// Constants for row and column sizes for the keypad
const byte ROWS = 4;
const byte COLS = 4;

// Array to represent keys on keypad
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connections to Arduino for the keypad
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Create keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Create LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables to store key values
String inputString = "";  // String to hold the sequence of key presses

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialize the LCD
  lcd.backlight();
  lcd.init();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter data and");
  lcd.setCursor(0, 1);
  lcd.print("press #");

  // Initialize the key for RFID
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("Enter data on the keypad, and press '#' to save to RFID tag"));
}

void loop() {
  // Get key value if pressed
  char customKey = customKeypad.getKey();

  if (customKey) {
    if (customKey == '#') {
      // Write the input string to the RFID tag
      writeToRFIDTag(inputString);
      inputString = ""; // Clear the input string after writing
      // Reset the LCD message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter data and");
      lcd.setCursor(0, 1);
      lcd.print("press #");
    } else {
      // Append the pressed key to the input string
      inputString += customKey;
    }
  }
}

void writeToRFIDTag(String data) {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println(F("[No card detected. Bring card closer.]"));
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println(F("[Failed to read card serial.]"));
    return;
  }

  Serial.println(F("[Card detected]"));

  byte buffr[16];
  byte block = 4;

  // Copy the input data to the buffer, padding with spaces if necessary
  for (byte i = 0; i < 16; i++) {
    if (i < data.length()) {
      buffr[i] = data[i];
    } else {
      buffr[i] = ' ';
    }
  }

  // Authenticate
  card_status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (card_status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(card_status));
    return;
  }
  Serial.println(F("PCD_Authenticate() success"));

  // Write block
  card_status = mfrc522.MIFARE_Write(block, buffr, 16);
  if (card_status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(card_status));
    return;
  }
  Serial.println(F("Data saved to RFID tag"));

  // Success message on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Successfully");
  lcd.setCursor(0, 1);
  lcd.print("written to tag");

  // Halt PICC
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void displayError(MFRC522::StatusCode status) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error: ");
  lcd.print(mfrc522.GetStatusCodeName(status));
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter data and");
  lcd.setCursor(0, 1);
  lcd.print("press #");
}
