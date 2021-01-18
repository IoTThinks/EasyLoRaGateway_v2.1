// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================

// Setup Lora
void setupLoRa() {
  Serial.println("[LoRa 1] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  //SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  while (!LoRa.begin(433E6)) {
    Serial.println("[LoRa 1] Starting LoRa failed!");
    delay(1000);
  }

  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setCodingRate4(LORA_CR);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa.enableCrc();

  Serial.println("[LoRa 1] LoRa started OK!");
}

String receiveLoRaMessage() {  
  int packetSize = LoRa.parsePacket();

  if (packetSize == 0) return "";          // if there's no packet, return
 
  String incoming = "";                 // payload of packet

  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

   // Added LoRa signal quality
  incoming = "RSSI:" + String(LoRa.packetRssi()) +
             " | SNR:" + String(LoRa.packetSnr()) + 
             " | " + incoming;

  return incoming;
}

void sendLoRaMessage(String outgoing) {
  Serial.println("[LoRa 1]=> Sending packet: " + outgoing);
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it  
}
