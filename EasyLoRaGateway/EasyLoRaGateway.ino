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
  
  //setupSerialBT();
  printChipID();
  
  setupSpeaker();
  setupButton();
  //setupFileSystem();
  setupEthernet();
  setupWiFi();
  //delay(5000);  
  //setupMQTT(); <====== Triggered by ETH / WiFi event
  setupLoRa();  
  //setupLoRa2(); 
  //setupWebServer();
  //delay(10000);
  //setupOTA();
  //delay(1000);
  //setupPreferences();  
  //publishDeviceConnect("BCDDC2C31684");
  //testTBJsson();

  //testJson();
  //getEasyJsonPacket(R"=====(src:"BCDDC2C31684","mb_temp":"error","s_smk":"error")=====");
}

// Do the real works here
void loop() {  
  // Check heap mem
  //log("esp_free_heap: " + String(esp_get_free_heap_size()) + 
  //    ", free_min_heap: " + String(esp_get_minimum_free_heap_size()));
  
  // Working 
  receiveAndForwardLoRaMessage();
  sendAndReceiveMQTT();
  //runWebServer();

  // User presses button for action
  //performUserAction();
  
  // TODO: When to accept OTA update?
  //waitingForOTA();  
}

// Receive LoRa message and send to MQTT Server
void receiveAndForwardLoRaMessage(){
  // For LoRa 1
  String message = receiveLoRaMessage();
  if(message != ""){   
    log("[LoRa 1] <= Received message: " + message);
    // Send data to ThingsBoard
    //publishToMQTT(MQTT_API_TELEMETRY, message);
    processUplinkTBMessage(MQTT_API_TELEMETRY, message);
    blinkOffLED();
  }

  /*
  // For LoRa 2
  String message2 = receiveLoRa2Message();
  if(message2 != ""){
    log("[LoRa 2] <= Received message: " + message2);
    
    // Send to MQTT
    forwardNodeMessageToMQTT(message2);

    // Send data to ThingsBoard
    IoTMessage tbMessage2 = getIoTMessage("{" + message2 + "}");
    uploadTelemetryData(tbMessage2.src, tbMessage2.telemetry);
    blinkOffLED();
  }
  */
}
