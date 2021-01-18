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

  setupWiFi();

  setupButton();  
  setupEthernet();  
  setupLoRa();
  setupLoRa2();
  // TODO: Test DIO1
  delay(5000);
}

// Do the real works here
void loop() {
  // Signal if a packet is received
  if(isLoRaPacketReceived)
  {
    // testSpeaker();
    blinkLEDOn();
    isLoRaPacketReceived = false;
  }

  Serial.println("======= BEGIN TEST ========");
  testSerial();  
  testSpeaker();
  testLED();   
  testButton();   

  // To test WiFi
  testWiFi();

  // Test Internet from LAN Port
  testEthernet();
  delay(2000);

  // To test LoRa
  Serial.println("LoRa 2 sends message to LoRa 1...");

  for(int i=0; i<3; i++)
  {
    // LoRa 2 to send to LoRa 1
    sendLoRa2Message("Hello from gateway");
    delay(2000);

    // Beep if a packet is received
    if(isLoRaPacketReceived)
    {
      // testSpeaker();
      isLoRaPacketReceived = false;
    }
  }

  // To receive LoRa messages in 10 seconds
  Serial.println("[LoRa 1] To check for LoRa messages in 10 seconds...");
  delay(5000);  
  Serial.println("======= END TEST ========");
}
