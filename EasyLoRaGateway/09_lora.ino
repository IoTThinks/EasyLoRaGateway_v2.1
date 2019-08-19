// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================
// Status
String LORA_Status = "Not Initialized";
String LORA_Lastreceived_Msg ="--No data--";

void setActiveLoRa() {
  digitalWrite(LORA2_SS, HIGH);
  digitalWrite(LORA_SS, LOW);  
}

// Setup Lora
void setupLoRa() {
  Serial.println("[LoRa 1] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO012);
  
  while (!LoRa.begin(LORA_FEQ)) {
    Serial.println("[LoRa 1] Starting LoRa failed!");    
    LORA_Status="FAILED";
    delay(1000);
  }

  LoRa.setSpreadingFactor(LORA_SF);
  LoRa.setCodingRate4(LORA_CR);
  LoRa.setSignalBandwidth(LORA_BW);
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  LoRa.enableCrc();

  Serial.println("[LoRa 1] LoRa started OK!");
  LORA_Status = "OK";
}

String receiveLoRaMessage() {
  // Set active LoRa chip
  setActiveLoRa();
  
  int packetSize = LoRa.parsePacket();
  if (packetSize == 0) return "";          // if there's no packet, return

  // read packet header bytes:
  /*
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  */
  
  String incoming = "";                 // payload of packet

  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  /*
  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:   
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);  
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  */

  // Added LoRa signal quality
  incoming = "RSSI:" + String(LoRa.packetRssi()) +
             " | SNR:" + String(LoRa.packetSnr()) + 
             " | " + incoming;
  
  LORA_Lastreceived_Msg = incoming;
  return incoming;
}

// NOT tested yet
void sendLoRaMessage(String outgoing) {
  // Set active LoRa chip
  setActiveLoRa();
  
  Serial.println("[LoRa 1]=> Sending packet: " + outgoing);
  LoRa.beginPacket();                   // start packet
  /*
  msgCount++;                           // increment message ID
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  */
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it  
}
