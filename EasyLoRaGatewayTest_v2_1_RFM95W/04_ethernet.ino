// ====================================
// Ethernet
// ====================================
// https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples

void setupEthernet()
{
    WiFi.onEvent(WiFiEvent);
    ETH.begin();
}

void testEthernet()
{
  if (ETH_Ip != "" && ETH_Ip != "0.0.0.0")
  {
    Serial.print("[ETH] Ethernet LAN IP: ");
    Serial.print(ETH_Ip);
    Serial.println(". Passed.");
  }
  else
  {
    Serial.println("[ETH] No Ethernet LAN yet! Plug in cable to test!");
  }    
}
