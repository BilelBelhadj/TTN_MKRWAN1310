/*
  First Configuration
  This sketch demonstrates the usage of MKR WAN 1300/1310 LoRa module.
  This example code is in the public domain.
*/
#include <Arduino.h>
#include <MKRWAN.h>

LoRaModem modem;

// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

String appEui = "0000000000000000";
String appKey = "C0608C705A53CFA8E338B09CDE5B844B";


//constantes
const int mode = 1;
const int connSensor = 6;
const int lightSensor = A3;

//variables
int connected, err, light = 0, connectivity = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(connSensor, INPUT);

  while (!Serial);
  Serial.println("Welcome to MKR WAN 1300/1310 first configuration sketch");
  Serial.println("Register to your favourite LoRa network and we are ready to go!");
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  if (modem.version() != ARDUINO_FW_VERSION) {
    Serial.println("Please make sure that the latest modem firmware is installed.");
    Serial.println("To update the firmware upload the 'MKRWANFWUpdate_standalone.ino' sketch.");
  }
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  appKey.trim();
  appEui.trim();
  Serial.print("trim done...!");
  connected = modem.joinOTAA(appEui, appKey);
  Serial.println("first connection tried");
  

  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    //while (1) {}
    connected = modem.joinOTAA(appEui, appKey);
    Serial.print(".");
    delay(50);
  }

  if (connected) {
    Serial.println("Connected...");
    delay(500);
  }
}

void loop() {

  light = analogRead(lightSensor);
  connectivity = digitalRead(connSensor);

  modem.setPort(3);
  modem.beginPacket();
  modem.print(String(light));
  modem.print(String(connectivity));

  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }
  
  while (modem.available()) {
    Serial.write(modem.read());
  }
  modem.poll();
  
}
