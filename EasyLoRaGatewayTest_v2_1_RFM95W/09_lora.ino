// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================
// Setup Lora
void setupLoRa() {
  // Serial.println("[LoRa 1] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  //SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  while (!LoRa.begin(LORA_FREQ)) {
    Serial.println("[LoRa 1] Starting LoRa failed!");
    delay(1000);
  }

  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setCodingRate4(LORA_CR);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa.enableCrc();
  LoRa.setGain(LORA_LNA_GAIN);

  // register the receive callback
  LoRa.onReceive(onReceive);
  // LoRa.onReceive(onLoRa1ReceiveCallback);

  // put the radio into receive mode
  LoRa.receive();
  
  Serial.println("[LoRa 1] LoRa started OK!");
}

void onReceive(int packetSize) {
  Serial.print("[LoRa 1] Noise floor: ");
  Serial.println(LoRa.rssi());
  
  // received a packet
  Serial.print("[LoRa 1] Received packet '");
  
  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }
  
  // print RSSI of packet
  Serial.print("' with RSSI = ");
  Serial.print(LoRa.packetRssi());

  // print SNR of packet
  Serial.print(", SNR = ");
  Serial.println(LoRa.packetSnr());

  // This line will crash
  // print FreqError of packet
  // Serial.print(", FreqError = ");
  // Serial.println(LoRa.packetFrequencyError());

  // Set flag
  isLoRaPacketReceived = true;
}

void sendLoRaMessage(String outgoing) {
  Serial.println("[LoRa 1]=> Sending packet: " + outgoing);
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it  
}

void loRa1ReadTask(void* pvParameters) {
  // Blink LED when receiving a packet
  blinkLEDOn();
      
  // received a packet
  Serial.print("Received packet '");

  int packetSize = *((int*)pvParameters);
  
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI = ");
  Serial.print(LoRa.packetRssi());

  // print SNR of packet
  Serial.print(", SNR = ");
  Serial.print(LoRa.packetSnr());

  // print FreqError of packet
  Serial.print(", FreqError = ");
  Serial.println(LoRa.packetFrequencyError());

  // Set flag
  isLoRaPacketReceived = true;    
  // End of receiving
  
  // Cleanup task before exiting  
  yield();
  vTaskDelete(NULL);
}

void onLoRa1ReceiveCallback(int packetSize) {
  // Have heap leaking issue when running on two cores quickly
  // xTaskCreate(loRa1ReadTask, "loRa1ReadTask", 10240, (void*)&packetSize, 
  //  CRONJOB_READLORA_PRIORITY, NULL);
  xTaskCreatePinnedToCore(loRa1ReadTask, "loRa1ReadTask", 10240, (void*)&packetSize, 
    /*CRONJOB_READLORA_PRIORITY*/ 2, NULL, 0);
}
