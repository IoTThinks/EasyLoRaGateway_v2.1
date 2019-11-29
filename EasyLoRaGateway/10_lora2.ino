// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================
// Status
String LORA2_Status = "Not Initialized";
String LORA2_Lastreceived_Msg ="--No data--";

void setActiveLoRa2() {
  digitalWrite(LORA_SS, HIGH);
  digitalWrite(LORA2_SS, LOW);
}

// Setup Lora
void setupLoRa2() {
  log("[LoRa 2] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  SPI.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI);
  LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO012);
  
  while (!LoRa2.begin(LORA2_FREQ_BASE)) {
    log("[LoRa 2] Starting LoRa failed!");    
    LORA2_Status="FAILED";
    delay(1000);
  }

  LoRa2.setSpreadingFactor(LORA2_SF);
  LoRa2.setCodingRate4(LORA2_CR);
  LoRa2.setSignalBandwidth(LORA2_BW);
  LoRa2.setPreambleLength(LORA2_PREAMBLE_LENGTH);
  LoRa2.enableCrc();

  LoRa2_txMode();
  
  log("[LoRa 2] LoRa started OK!");
  LORA2_Status = "OK";
}

void LoRa2_txMode(){
  log("[LoRa 2] Set TX mode");
  LoRa2.idle();                          // set standby mode
  LoRa2.enableInvertIQ();                // active invert I and Q signals
}

// Send LoRa message at customed LoRa frequency
void sendDynamicLoRa2Message(long customedLoRaFreq, const char* outgoing)
{
  LoRa2.setFrequency(customedLoRaFreq);
  sendLoRa2Message(outgoing);
}

// Send LoRa 2 message
void sendLoRa2Message(const char* outgoing) {
  // Set active LoRa chip
  taskDISABLE_INTERRUPTS();
  setActiveLoRa2();
  taskENABLE_INTERRUPTS();
  
  log("[LoRa 2]=> Sending packet: ", outgoing);
  // LoRa2_txMode();                        // set tx mode
  LoRa2.beginPacket();                   // start packet
  LoRa2.print(outgoing);                 // add payload
  // LoRa2.endPacket();                     // finish packet and send it  
  LoRa2.endPacket(true);                 // More reliable for RPC, may corrupt packet
  
  // Set active LoRa chip
  taskDISABLE_INTERRUPTS();
  setActiveLoRa();
  LoRa_rxMode();
  taskENABLE_INTERRUPTS();
}
