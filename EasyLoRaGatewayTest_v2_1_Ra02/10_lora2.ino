// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================

//SPIClass SPI2(VSPI);
LoRaClass LoRa2;

// Setup Lora
void setupLoRa2() {
  Serial.println("[LoRa 2] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  SPI.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI);
  LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO0);  
  
  while (!LoRa2.begin(433E6)) {
    Serial.println("[LoRa 2] Starting LoRa failed!");
    delay(1000);
  }

  LoRa2.setSpreadingFactor(LORA_SF);
  LoRa2.setCodingRate4(LORA_CR);
  LoRa2.setSignalBandwidth(LORA_BW);
  LoRa2.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa2.enableCrc();

  Serial.println("[LoRa 2] LoRa started OK!");
}

String receiveLoRa2Message() {
  int packetSize = LoRa2.parsePacket();

  if (packetSize == 0) return "";          // if there's no packet, return
 
  String incoming = "";                 // payload of packet

  while (LoRa2.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa2.read();      // add bytes one by one
  }

   // Added LoRa signal quality
  incoming = "RSSI:" + String(LoRa2.packetRssi()) +
             " | SNR:" + String(LoRa2.packetSnr()) + 
             " | " + incoming;

  return incoming;
}

void sendLoRa2Message(String outgoing) {
  Serial.println("[LoRa 2]=> Sending packet: " + outgoing);
  LoRa2.beginPacket();                   // start packet
  LoRa2.print(outgoing);                 // add payload
  LoRa2.endPacket();                     // finish packet and send it  
}
