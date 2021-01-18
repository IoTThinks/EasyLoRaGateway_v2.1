// Include required EasyLoraGateway libraries
#include "EasyLoRaGateway.h"

// ===================================================
// Main Program
// ===================================================
void setActiveLoRa() {
  digitalWrite(LORA2_SS, HIGH);
  digitalWrite(LORA_SS, LOW);  
}

void setActiveLoRa2() {
  digitalWrite(LORA_SS, HIGH);
  digitalWrite(LORA2_SS, LOW);
}

void setup() {   
  setupSerial();
  setupLED();
  setupSpeaker();
  setupGPIOs();
  setupEthernet();  
  setupLoRa();
  setupLoRa2();
  delay(5000);
}

// Do the real works here
void loop() {    
  Serial.println("======= BEGIN TEST ========");
  testSerial();  
  testLED(); 
  testSpeaker(); 
  testGPIOs();
  testHttpGet();

  Serial.println("Testing LoRa for a few times");
  for(int i=0; i < 10000; i++)
  {
    // LoRa 1
    setActiveLoRa(); 
    String loraMessage = receiveLoRaMessage();
    if(loraMessage != "") 
      Serial.println("[LoRa 1] " + loraMessage);
    delay(50);
   
    // LoRa 2    
    setActiveLoRa2();
    String lora2Message = receiveLoRa2Message();
    if(lora2Message != "") 
      Serial.println("[LoRa  2] " + lora2Message); 
    delay(50);
  }
   //sendLoRa2Message("Hello from gateway");
  Serial.println("======= END TEST ========");
}
