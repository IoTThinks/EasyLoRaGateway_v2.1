// Include required EasyLoraGateway libraries
#include "EasyLoRa.h"

// ===================================================
// Main Program
// ===================================================
// This runs once when the gateway boots. 
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

  // Initial start time
  startMillis = millis();
}

// Do the real works here
void loop() {  
  // Check heap mem
  //log("esp_free_heap: " + String(esp_get_free_heap_size()) + 
  //    ", free_min_heap: " + String(esp_get_minimum_free_heap_size()));

  // TODO: Use multiple threads for faster speed
  // Working 
  // Receive commands from Portal
  sendAndReceiveMQTT();

  // Forward the commands to LoRa node
  receiveAndForwardLoRaMessage();  

  // Run cron jobs
  runCronJobs();
  
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
    log("[MAIN] <= Received message: " + message);
    // Send data to ThingsBoard
    processUplinkTBMessage(message);
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


void runCronJobs()
{
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    log("[MAIN] Poll telemetry data");
    pollTelemetryData("BCDDC2C31684");
    startMillis = currentMillis;
  }
}
