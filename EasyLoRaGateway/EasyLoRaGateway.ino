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
  setupButton();
  //setupFileSystem();
  setupEthernet();
  setupWiFi();
  delay(5000);
  setupMQTT();
  setupLoRa();  
  setupLoRa2(); 
  setupWebServer();
  //delay(10000);
  //setupOTA();
  //delay(1000);
  //setupPreferences();
}

// Do the real works here
void loop() {
  // Working
  receiveAndForwardLoRaMessage();
  processMQTTMessages();
  runWebServer();

  // User presses button for action
  //performUserAction();
  
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
    Serial.println("[LoRa 1] Received message: " + message);
    // Send to MQTT
    forwardNodeMessageToMQTT(message);

    // Send data to ThingsBoard
    IoTMessage tbMessage = getIoTMessage("{" + message + "}");
    uploadTelemetryData(tbMessage.src, tbMessage.telemetry);  
    blinkOffLED();
  }
  
  delay(50);

  // For LoRa 2
  String message2 = receiveLoRa2Message();
  if(message2 != ""){
    Serial.println("[LoRa 2] Received message: " + message2);
    
    // Send to MQTT
    forwardNodeMessageToMQTT(message2);

    // Send data to ThingsBoard
    IoTMessage tbMessage2 = getIoTMessage("{" + message2 + "}");
    uploadTelemetryData(tbMessage2.src, tbMessage2.telemetry);  
    blinkOffLED();
  }

  delay(50);  
}
