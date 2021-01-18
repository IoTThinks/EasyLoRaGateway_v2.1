// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================

//SPIClass SPI2(VSPI);
LoRaClass LoRa2;

// Setup Lora
void setupLoRa2() {
  // Serial.println("[LoRa 2] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  SPI.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI);
  LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO0);  
  
  while (!LoRa2.begin(LORA2_FREQ)) {
    Serial.println("[LoRa 2] Starting LoRa failed!");
    delay(1000);
  }

  LoRa2.setSpreadingFactor(LORA2_SF);
  LoRa2.setCodingRate4(LORA2_CR);
  LoRa2.setSignalBandwidth(LORA2_BW);
  LoRa2.setPreambleLength(LORA2_PREAMBLE_LENGTH);
  LoRa2.enableCrc();
  LoRa2.setGain(LORA2_LNA_GAIN);

  // Has effect on sent RSSI
  LoRa2.setTxPower(LORA2_TXPOWER, PA_OUTPUT_PA_BOOST_PIN);  
  
  Serial.println("[LoRa 2] LoRa started OK!");
}

void sendLoRa2Message(String outgoing) {
  Serial.println("[LoRa 2]=> Sending packet: " + outgoing);
  LoRa2.beginPacket();                   // start packet
  LoRa2.print(outgoing);                 // add payload
  LoRa2.endPacket();                     // finish packet and send it  
}
