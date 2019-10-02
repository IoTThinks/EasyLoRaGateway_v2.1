// ====================================
// Lora
// https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md
// ====================================
// Status
String LORA2_Status = "Not Initialized";
String LORA2_Lastreceived_Msg ="--No data--";

LoRaClass LoRa2;

void setActiveLoRa2() {
  digitalWrite(LORA_SS, HIGH);
  digitalWrite(LORA2_SS, LOW);
}

// Setup Lora
void setupLoRa2() {
  Serial.println("[LoRa 2] Setting up LoRa");

  // Set SS to high
  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);  

  pinMode(LORA2_SS, OUTPUT);
  digitalWrite(LORA2_SS, HIGH); 
  
  SPI.begin(LORA2_SCK, LORA2_MISO, LORA2_MOSI);
  LoRa2.setPins(LORA2_SS, LORA2_RST, LORA2_DIO012);
  
  while (!LoRa2.begin(LORA2_FEQ)) {
    Serial.println("[LoRa 2] Starting LoRa failed!");    
    LORA2_Status="FAILED";
    delay(1000);
  }

  LoRa2.setSpreadingFactor(LORA2_SF);
  LoRa2.setCodingRate4(LORA2_CR);
  LoRa2.setSignalBandwidth(LORA2_BW);
  LoRa2.setPreambleLength(LORA2_PREAMBLE_LENGTH);
  LoRa2.enableCrc();

  Serial.println("[LoRa 2] LoRa started OK!");
  LORA2_Status = "OK";
}

String receiveLoRa2Message() {
  // Set active LoRa chip
  setActiveLoRa2();
  
  int packetSize = LoRa2.parsePacket();
  if (packetSize == 0) return "";          // if there's no packet, return

  // read packet header bytes:
  /*
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  */
  
  String incoming = "";                 // payload of packet

  while (LoRa2.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa2.read();      // add bytes one by one
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
  incoming = "\"rssi\":" + String(LoRa2.packetRssi()) +
             ",\"snr\":" + String(LoRa2.packetSnr()) + 
             "," + incoming;
  
  LORA2_Lastreceived_Msg = incoming;
  return incoming;
}

// NOT tested yet
void sendLoRa2Message(String outgoing) {
  // Set active LoRa chip
  setActiveLoRa2();
  
  Serial.println("[LoRa 2]=> Sending packet: " + outgoing);
  LoRa2.beginPacket();                   // start packet
  /*
  msgCount++;                           // increment message ID
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  */
  LoRa2.print(outgoing);                 // add payload
  LoRa2.endPacket();                     // finish packet and send it  
}
