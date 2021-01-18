void setupWiFi()
{
   // delete old config
   WiFi.disconnect(true);
   delay(1000);
   
   // WiFi.onEvent(WiFiEvent);

   // Reduce TX Power
   // https://github.com/espressif/arduino-esp32/issues/2792
   // https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiGeneric.h#L52
   // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-station-general-scenario
   // https://blog.voneicken.com/projects/low-power-wifi-intro/
   // WiFi.setTxPower(WIFI_POWER_MINUS_1dBm); // Worked. To reduce brownout power
   
   // Set false for better ping latency
   // WiFi.setSleep(false);

   WiFi.begin(WIFI_NAME, WIFI_PWD);
   // delay(5000); // To wait for DHCP IP
   // Serial.println("[WiFi] If you can see this, WiFi is PASSED.");
}

 void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("[ETH] ETH Started");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("[ETH] ETH Connected");
      ETH_Status = "Connected. No IP Yet";
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("[ETH] ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      ETH_Ip = ETH.localIP().toString();
      Serial.print(ETH_Ip);
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");  
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("[ETH] ETH Disconnected");
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("[ETH] ETH Stopped");
      break;
    // ==================== WIFI ====================
    case SYSTEM_EVENT_STA_GOT_IP:      
      Serial.print("[WiFi] IP address: ");
      Serial.println(WiFi.localIP());
      WiFi_IP = WiFi.localIP().toString();
      // WiFi.mode(WIFI_OFF); // Still brownout
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      // Serial.println("[WiFi] WiFi lost connection.  Attempting to reconnect...");
      WiFi.reconnect();
      break;
    case SYSTEM_EVENT_STA_START:
      // Serial.println("[WiFi] WiFi station start");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      // Serial.println("[WiFi] WiFi station connected to AP");
      break;
    case SYSTEM_EVENT_AP_START:
      // Serial.println("[WiFi] WiFi AP start");
      break;
    default:      
      // Serial.printf("[WiFi] Unhandled WiFi Event %d\n", event);       
      break;
  }
}

void testWiFi()
{
  if (WiFi_IP != "" && WiFi_IP != "0.0.0.0")
  {
    Serial.print("[WiFi] WiFi IP: ");
    Serial.print(WiFi_IP);
    Serial.println(". Passed.");
  }
  else
  {
    Serial.println("[WiFi] No WiFi yet! Check WiFi SSID and password in EasyLoRaGateway.h!");
  }    
}
