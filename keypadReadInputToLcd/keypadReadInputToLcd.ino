#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN A0
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode card_status;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows

String newBalance = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("PCD Ready!"));

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.print("Scan card:");
  lcd.setCursor(0, 1);

  // Initialize the key (default key)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  // Check if a new card is present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Read and display the balance from the card
    Serial.println(F("\n*** Balance on the PICC ***\n"));
    String currentBalanceStr = readBytesFromBlock(4);
    int currentBalance = currentBalanceStr.toInt(); // Convert balance string to integer
    lcd.clear();
    lcd.print("Balance: ");
    lcd.print(currentBalanceStr);
    lcd.setCursor(0, 1);
    Serial.println(currentBalanceStr);
    Serial.println(F("\n***************************\n"));

    // Allow user to enter new balance
    newBalance = "";
    char key;
    while (true) {
      key = keypad.getKey();
      if (key) {
        if (key == '#') {
          // Clear the input
          lcd.clear();
          lcd.print("Enter data:");
          lcd.setCursor(0, 1);
          newBalance = "";
        } else if (key == '*') {
          // Calculate new balance
          int newBalanceInt = newBalance.toInt(); // Convert new balance string to integer
          int updatedBalance = currentBalance + newBalanceInt;

          // Write the updated balance to the card
          writeBytesToBlock(4, String(updatedBalance));

          // Display the updated balance on the card
          Serial.println(F("\n*** Updated Balance on the PICC ***\n"));
          String updatedBalanceStr = String(updatedBalance);
          lcd.clear();
          lcd.print("Balance: ");
          lcd.print(updatedBalanceStr);
          lcd.setCursor(0, 1);
          Serial.println(updatedBalanceStr);
          Serial.println(F("\n***************************\n"));
          break;  // Exit the inner loop after writing the new balance
        } else {
          // Append key to the new balance and display
          newBalance += key;
          lcd.clear();
          lcd.print("Add Money:");
          lcd.setCursor(0, 1);
          lcd.print(newBalance);
        }
      }
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

String readBytesFromBlock(byte blockNumber) {
  card_status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(mfrc522.uid));
  if (card_status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(card_status));
    return "";
  }

  byte arrayAddress[18];
  byte bufferSize = sizeof(arrayAddress);
  card_status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &bufferSize);
  if (card_status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(card_status));
    return "";
  }

  String value = "";
  for (uint8_t i = 0; i < 16; i++) {
    value += (char)arrayAddress[i];
  }
  value.trim();
  return value;
}

void writeBytesToBlock(byte blockNumber, String data) {
  card_status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNumber, &key, &(mfrc522.uid));
  if (card_status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(card_status));
    return;
  }

  byte buffer[16];
  data.getBytes(buffer, sizeof(buffer));

  // Ensure the data is exactly 16 bytes long
  for (uint8_t i = data.length(); i < 16; i++) {
    buffer[i] = 0;
  }

  card_status = mfrc522.MIFARE_Write(blockNumber, buffer, 16);
  if (card_status != MFRC522::STATUS_OK) {
    Serial.print(F("Writing failed: "));
    Serial.println(mfrc522.GetStatusCodeName(card_status));
  } else {
    Serial.println(F("Writing succeeded."));
  }
}
