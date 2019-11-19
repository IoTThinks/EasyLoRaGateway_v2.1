// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================
// Status
String LORA_Status = "Not Initialized";
//String LORA_Lastreceived_Msg ="--No data--";

void setActiveLoRa() {
  digitalWrite(LORA2_SS, HIGH);
  digitalWrite(LORA_SS, LOW);  
}

// Setup Lora
void setupLoRa() {
  log("[LoRa 1] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO012);
  
  while (!LoRa.begin(LORA_FREQ)) {
    log("[LoRa 1] Starting LoRa failed!");    
    LORA_Status="FAILED";
    delay(1000);
  }

  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setCodingRate4(LORA_CR);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa.enableCrc();

  // Set continuous receive
  LoRa.onReceive(onLoRa1ReceiveCallback);
  LoRa_rxMode();
  
  log("[LoRa 1] LoRa started OK!");
  LORA_Status = "OK";
}

void LoRa_txMode(){
  log("[LoRa 1] Set TX mode");
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}

void LoRa_rxMode(){
  log("[LoRa 1] Set RX mode");
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

// To send a single LoRa message
void sendLoRaMessage(const char* outgoing) {
  // Check heap mem
  logHeap();
  
  // If sending message from sensor is empty, ignore it.
  if(strlen(outgoing) == 0)
    return;
   
  // Set active LoRa chip
  setActiveLoRa();

  // Start sending
  log("[LoRa 1] => Sending packet: ", outgoing);
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  
  // Not working with invertIQ option
  // LoRa.endPacket(true); // true = async / non-blocking mode

  // Set back to receive mode
  LoRa_rxMode();                        // set rx mode
}

void loRa1ReadTask(void* pvParameters) {
  int packetSize = *((int*)pvParameters);
  
  // To receive message using char* to avoid heap fragmentation
  char incoming[256];
  
  for (int i = 0; i < packetSize; i++) {
    incoming[i] = (char)LoRa.read();
  }

  // To terminate the string
  incoming[packetSize] = '\0';

  // Added LoRa signal quality
  // ",meta":{"rssi":-60,"snr":9}
  strcat(incoming, R"=====(,"meta":{"rssi":)=====");
  strcat(incoming, string2Char(String(LoRa.packetRssi())));
  strcat(incoming, R"=====(,"snr":)=====");
  strcat(incoming, string2Char(String(LoRa.packetSnr())));
  strcat(incoming, "}");
  
  log("[LoRa 1] Receive and process downlink message: ", incoming);
  processUplinkTBMessage(incoming);
  
  //LORA_Lastreceived_Msg = incoming;
  
  // Cleanup task before exiting
  vTaskDelay(10);
  vTaskDelete(NULL);
}

void onLoRa1ReceiveCallback(int packetSize) {
  xTaskCreate(loRa1ReadTask, "loRa1ReadTask", 10240, (void*)&packetSize, CRONJOB_PRIORITY_READLORA, NULL);
}
