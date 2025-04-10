#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SCK  2
#define PN532_MISO 5
#define PN532_MOSI 3
#define PN532_SS   4

#define SOLENOID_PIN 13

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN532 board");
    while (1);
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");

  pinMode(SOLENOID_PIN, OUTPUT);
}

void loop() {
  byte scannedUID[7];
  uint8_t scannedUIDLength;

  if (readRFID(scannedUID, &scannedUIDLength)) {
    Serial.print("Scanned UID: ");
    printHex(scannedUID, scannedUIDLength);

    Serial.println("Activating Solenoid.");
    digitalWrite(SOLENOID_PIN, HIGH);
    delay(500); // Adjust the delay as needed
    digitalWrite(SOLENOID_PIN, LOW);
  }
  delay(500); // Delay to prevent rapid triggering
}

bool readRFID(byte *uid, uint8_t *uidLength) {
  return nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength);
}

void printHex(byte *buffer, uint8_t bufferLength) {
  for (int i = 0; i < bufferLength; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}
