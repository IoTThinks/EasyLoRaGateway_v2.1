// Include required EasyLoraGateway libraries
#include "EasyLoRa.h"

// ===================================================
// Main Program
// ===================================================
// This runs once when the gateway boots. 
void setup() {
  // Enable debug and system information
  setupSerial();
  setChipID();

  // Enable notification
  setupLED();    
  setupSpeaker();  
  
  // Enable TCP/IP comunication
  setupEthernet();
  setupWiFi();
  
  // Enable 2 LoRa chips
  setupLoRa();  
  setupLoRa2(); 

  // Not in use
  // setupPreferences() is empty.
  // setupSerialBT(); Use to much memory heap
  // setupMQTT() is triggered by ETH / WiFi event
  // setupProcessor() is triggered by setupMQTT
  // setupWebServer();  
  // setupOTA();
  // setupButton();
   
  // Create background cron job task
  // xTaskCreatePinnedToCore(cronjobTask, "cronjobTask", 10240, NULL, CRONJOB_POLLING_NODE_PRIORITY, NULL, 1); 
  xTaskCreate(cronjobTask, "cronjobTask", 10240, NULL, CRONJOB_POLLING_NODE_PRIORITY, NULL);  
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
    // Task 1: Polling MQTT
    runCronjob_pollingMQTT();
    vTaskDelay(100); // Let other tasks to run

    // Task 2: Polling nodes
    runCronJob_pollingNodes();
    vTaskDelay(100); // Let other tasks to run
  }
}

// Task 1: Polling MQTT
void runCronjob_pollingMQTT()
{  
  CRONJOB_POLLING_MQTT_CURRENT_MILLIS = millis();
  if (CRONJOB_POLLING_MQTT_CURRENT_MILLIS - CRONJOB_POLLING_MQTT_START_MILLIS >= CRONJOB_POLLING_MQTT_PERIOD)
  {  
    // log("[MAIN] Polling MQTT data...");
    
    // Receive commands from Portal
    sendAndReceiveMQTT();

    // Reset timer
    CRONJOB_POLLING_MQTT_START_MILLIS = CRONJOB_POLLING_MQTT_CURRENT_MILLIS;
  }
}

// Task 2: Polling nodes
// TODO: To be replaced as dynamic code
void runCronJob_pollingNodes()
{    
  CRONJOB_POLLING_NODE_CURRENT_MILLIS = millis();
  if (CRONJOB_POLLING_NODE_CURRENT_MILLIS - CRONJOB_POLLING_NODE_START_MILLIS >= CRONJOB_POLLING_NODE_PERIOD)
  {    
    // To display node list before polling
    // displayNodeList();
      
    if(strlen(SYS_NODELIST[CRONJOB_POLLING_NODE_NEXTID]) > 0)
    {
      // To get to-be-polled node id
      byte currentNodeId = CRONJOB_POLLING_NODE_NEXTID;
      
      log("[MAIN] Polling node[", string2Char(String(currentNodeId)), "]: ", SYS_NODELIST[currentNodeId]);  
      pollTelemetryData(SYS_NODELIST[currentNodeId]);
      // vTaskDelay(SYS_NODEPOLLING_DELAY);
    }

    // To set for next polling node id
    if(CRONJOB_POLLING_NODE_NEXTID >= (SYS_MAXNODES -1))
    {
      // Reach last node, to back to first node
      CRONJOB_POLLING_NODE_NEXTID = 0;
    }
    else
    {
      // To poll next node id next time
      CRONJOB_POLLING_NODE_NEXTID++;
    }
    
    // Reset timer
    CRONJOB_POLLING_NODE_START_MILLIS = CRONJOB_POLLING_NODE_CURRENT_MILLIS;
  }
}
