// Include required EasyLoraGateway libraries
#include "EasyLoRa.h"

// ===================================================
// Main Program
// ===================================================
// This runs once when the gateway boots. 
void setup() {
  setupSerial();
  setChipID();
  setupLED();
  
  // TODO: Check heap fragmentation
  // setupSerialBT();  
  setupSpeaker();
  //setupButton();
  //setupFileSystem();
  setupEthernet();
  setupWiFi();

  // No need. Triggered by ETH / WiFi event
  //setupMQTT();

  // TODO: To enable LoRa 2 for faster concurrency
  setupLoRa();  
  setupLoRa2(); 

  // Not in use
  //setupWebServer();  
  //setupOTA();
  //setupPreferences();  

  // Create background cron job task
  xTaskCreate(cronjobTask, "cronjobTask", 10240, NULL, CRONJOB_PRIORITY_POLLING_NODE, NULL);
}

// Do the real works here
void loop() {  
  // Check heap mem
  // logHeap();

  // TODO: Use multiple threads for faster speed
  // Receive commands from Portal
  // sendAndReceiveMQTT();

  // Not in use
  //runWebServer();
  //performUserAction();
  //waitingForOTA();  

  // Delay a bit for watch dog
  vTaskDelay(1);
}

void cronjobTask(void* pvParameters) {
  while(true)
  {
    runCronJobs();
    vTaskDelay(100);
  }
}

void runCronJobs()
{
  // Receive commands from Portal
  sendAndReceiveMQTT();
  
  CRONJOB_CURRENT_MILLIS = millis();
  if (CRONJOB_CURRENT_MILLIS - CRONJOB_START_MILLIS >= CRONJOB_POLLING_NODE_PERIOD)
  {      
    log("[MAIN] Polling telemetry data...");
    log("[MAIN] ============ Polling telemetry data...Node 0 ============");
    pollTelemetryData("BCDDC2C31684"); // 132
    vTaskDelay(1000);

    log("[MAIN] ============ Polling telemetry data...Node 1 ============");
    pollTelemetryData("4C11AE707F9C"); // 156
    vTaskDelay(1000);

    log("[MAIN] ============ Polling telemetry data...Node 4 ============");
    pollTelemetryData("BCDDC2C56C64"); // 100
    vTaskDelay(1000);

    log("[MAIN] ============ Polling telemetry data...Node 5 ============");
    pollTelemetryData("4C11AE71A0A8"); // 168
    vTaskDelay(1000);
    
    log("[MAIN] ============ Polling telemetry data...Node 6 ============");
    pollTelemetryData("240AC417E194"); // 148
    vTaskDelay(1000);

    log("[MAIN] ============ Polling telemetry data...Node 9 ============");
    pollTelemetryData("4C11AE6D3EA0"); // 160
    vTaskDelay(1000);

    log("[MAIN] ============ Polling telemetry data...Node 10 ============");
    pollTelemetryData("240AC417E1B8"); // 184
    vTaskDelay(1000);
    
    CRONJOB_START_MILLIS = CRONJOB_CURRENT_MILLIS;
  }
}
