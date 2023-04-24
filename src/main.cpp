/*
Auteur      : Bilel Belhadj
Titre       : The Things Network MKRWAN1310
Description : Connect to the things network end device with the mkr wan 1310 and send the data collected with the two sensors
Date        : 22-04-2023
Version     : 0.0.1
Source      : https://docs.arduino.cc/tutorials/mkr-wan-1310/the-things-network
              https://www.hackster.io/ZoLuSs/first-step-with-mkr-wan-1310-and-the-things-networks-9f6065
*/

#include <Arduino.h>
#include <MKRWAN.h>

LoRaModem modem;


//constantes
const int mode = 1;
const int connSensor = 6;
const int lightSensor = A3;


//variables
int connected, err, light = 0, connectivity = 0;
String appEui = "0000000000000000";                 //application EUI
String appKey = "C0608C705A53CFA8E338B09CDE5B844B"; //Application key
String data = "";

void setup() {

  Serial.begin(9600);         //demarrer le moniteur serie
  pinMode(connSensor, INPUT); //pinmode broche connectivity

  while (!Serial){}
  
  Serial.println("Welcome to MKR WAN 1300/1310 first configuration sketch");
  Serial.println("Register to your favourite LoRa network and we are ready to go!");
  
  //demarrage du modem
  if (!modem.begin(US915)) {                    //regional canada band
    Serial.println("Failed to start module");
    while (1) {}
  };

  Serial.print("Your module version is: ");     //print modem version
  Serial.println(modem.version());

  if (modem.version() != ARDUINO_FW_VERSION) {  //verifier la version
    Serial.println("Please make sure that the latest modem firmware is installed.");
    Serial.println("To update the firmware upload the 'MKRWANFWUpdate_standalone.ino' sketch.");
  }
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());      //afficher le 1310 EUI sur le moniteur serie

  appKey.trim();
  appEui.trim();
  Serial.print("trim done...!");
  connected = modem.joinOTAA(appEui, appKey); //essayer de connecter
  Serial.println("first connection tried");
  

  while (!connected) {    //en cas ou il ya pas de connection il essaye de se connecter encore jusqua ca se connecte
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    connected = modem.joinOTAA(appEui, appKey);
    Serial.print(".");
    delay(50);
  }

  if (connected) {                   //affiche si c est connecter
    Serial.println("Connected...");
    delay(500);
  }
}

void loop() {

  //collecter les donnees du capteur
  light = analogRead(lightSensor);
  connectivity = digitalRead(connSensor);

  //construire la chaine
  data = "{ light: " + String(light) + ", conductivity: " + String(connectivity) + "}";
  Serial.println(data);

  //send data
  modem.setPort(3);     //port have to be the same as the ttn end_device
  modem.beginPacket();  //start packet
  modem.print(data);    //payload
  err = modem.endPacket(true);

  //afficher message d'erreur
  if (err > 0) {
    Serial.println("Message N1 sent correctly!");
  } else {
    Serial.println("Error sending message N1 :(");
  }

  
  while (modem.available()) {
    Serial.write(modem.read());
  }
  
  modem.poll();   //keep the connection alive
  delay(12000);   //send data every 12 seconds
}
