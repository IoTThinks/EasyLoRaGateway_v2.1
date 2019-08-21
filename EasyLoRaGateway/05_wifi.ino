static bool wifi_connected = false;
String WiFi_Status;
String WiFi_Ip;

void setupWiFi()
{    
  //WiFi.onEvent(WiFiEvent);
  
  WiFi.begin(ssid, password);
  Serial.println("[WiFi] Connecting to WiFi...");
}

void reconnectWiFi()
{
  Serial.println("[WiFi] Reconnecting WiFi...");
  setupWiFi();
  delay(1000);
}

void disconnectWiFi()
{
  Serial.println("[WiFi] Disconnecting WiFi...");
  // delete old config
  WiFi.disconnect(true);
  WiFi_Status ="Disconnected";
  delay(1000);
}

// Shared between Ethernet and Wifi
void WiFiEvent(WiFiEvent_t event)
{  
  switch(event) {
    // Ethernet
    case SYSTEM_EVENT_ETH_START:
      Serial.println("[ETH] ETH Started");
      ETH_hostname = getHostname();
      ETH.setHostname(string2Char(ETH_hostname));
      Serial.println("[ETH] Hostname = " + ETH_hostname);      
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("[ETH] ETH Connected");
      ETH_Status = "Connected. No IP Yet";
      //onOffSpeaker(3); // Error
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
      eth_connected = true;
      ETH_Status = "Connected";
      onOffSpeaker(1, false); // OK

      // Disconnect wifi if have active ethernet IP
      disconnectWiFi();
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("[ETH] ETH Disconnected");
      eth_connected = false;
      ETH_Status = "Disconnected";
      onOffSpeaker(3, true); // Error

      // Reconnect via Wifi as backup
      reconnectWiFi();
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("[ETH] ETH Stopped");
      eth_connected = false;
      ETH_Status = "Stopped";
      onOffSpeaker(3, true); // Error
      break;
    // WiFi
    case SYSTEM_EVENT_STA_GOT_IP:
      // Disconnect if ETH is connected with IP
      if(eth_connected == true)
      {
        disconnectWiFi();        
      }
      else
      {
        Serial.print("[WiFi] WiFi connected. ");
        WiFi_Status = "Connected";
        WiFi_Ip = WiFi.localIP().toString();
        Serial.print("IP address: ");
        Serial.println(WiFi_Ip);        
        wifi_connected = true;
        onOffSpeaker(1, false); // OK
      }
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("[WiFi] WiFi lost connection.  Attempting to reconnect...");
      WiFi.reconnect();
      wifi_connected = false;
      WiFi_Status ="Disconnected";
      onOffSpeaker(3, true); // Error
      break;
    case SYSTEM_EVENT_STA_START:
      Serial.println("[WiFi] WiFi station start");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("[WiFi] WiFi station connected to AP");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("[WiFi] WiFi AP start");
      break;    
    default:      
      Serial.println("[ETH_WiFi] Unhandled ETH_WiFi Event");
      //Serial.printf("[WiFi] Unhandled WiFi Event %d\n", event);       
      break;
  }
}
