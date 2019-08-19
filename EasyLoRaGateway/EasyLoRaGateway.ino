// Include required EasyLoraGateway libraries
#include "EasyLoRaGateway.h"

// ===================================================
// Main Program
// ===================================================
// This runs once when the gateway boots. 
// The delay is not neccessary and just to avoid component re-initizalized by other compopents.
// setupXXX functions are placed in module files
void setup() {      
  setupLED();  
  setupSerial();
  setupSpeaker();
  //setupFileSystem();
  setupEthernet();
  //delay(1000);
  setupMQTT();
  setupLoRa();  
  setupLoRa2(); 
  setupWebServer();
  //delay(10000);
  //setupOTA();
  //delay(1000);
  //setupPreferences();   

  // Done setup
  displayLEDErrorCode();
  onLED();
  onOffSpeaker(1, false);
}

// Do the real works here
void loop() {  
  // Working
  receiveAndForwardLoRaMessage();
  processMQTTMessages();
  runWebServer();
  
  // TODO: When to accept OTA update?
  //waitingForOTA();
}

// Need to flush buffer to send or receive MQTT messages
void processMQTTMessages() {
  // Send / receive from MQTT buffer
  flushMQTTBuffer();
}

// Receive LoRa message and send to MQTT Server
void receiveAndForwardLoRaMessage(){
  // For LoRa 1
  String message = receiveLoRaMessage();
  if(message != ""){    
    forwardNodeMessageToMQTT(message);
    dataReceivedLED();
  }

  delay(50);
  
  // For LoRa 2
  String message2 = receiveLoRa2Message();
  if(message2 != ""){    
    forwardNodeMessageToMQTT(message2);
    dataReceivedLED();
  }

  delay(50);
}
