static bool wifi_connected = false;
String WiFi_Status;
String WiFi_Ip;
String WiFi_hostname;

void setupWiFi()
{ 
  WiFi_hostname = getHostname("WiFi");
  WiFi.setHostname(string2Char(WiFi_hostname));
  WiFi.begin(ssid, password);
  WiFi.setSleep(false); // Do not sleep
  log("[WiFi] Connecting to WiFi...");
}

// Beep one and on LED and set ETH or WiFi to "Connected"
void setNetworkConnectedStatus(String type)
{
  onOffSpeaker(1, true); // OK
  onLED(); // OK

  if(type == "ETH")
  {
      eth_connected = true;
      ETH_Status = "Connected";
  }
  else if(type == "WiFi")
  {
      wifi_connected = true;
      WiFi_Status = "Connected";
  }
  else
  {
      log("[ETH_WIFI] Unknown NIC type");
  }
}

// Beep three times and off LED and set ETH or WiFi to "Disconnected"
void setNetworkDisconnectedStatus(String type)
{
  onOffSpeaker(3, true); // Error    
  offLED(); // Error  

  if(type == "ETH")
  {
      eth_connected = false;
      ETH_Status = "Disconnected";
  }
  else if(type == "WiFi")
  {
      wifi_connected = false;
      WiFi_Status ="Disconnected";
  }
  else
  {
      log("[ETH_WIFI] Unknown NIC type");
  }
}

void disconnectWiFi()
{
  log("[WiFi] Disconnecting WiFi...");
  WiFi.disconnect(true);  
}

// Shared between Ethernet and Wifi
// All network event is at https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino
void WiFiEvent(WiFiEvent_t event)
{  
  switch(event) {
    // ===============================
    // Ethernet
    // ===============================
    case SYSTEM_EVENT_ETH_START:
      log("[ETH] ETH Started");
      ETH_hostname = getHostname("ETH");
      ETH.setHostname(string2Char(ETH_hostname));
      log("[ETH] Hostname: " + ETH_hostname);      
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      log("[ETH] ETH Connected. No IP.");
      ETH_Status = "Connected. No IP Yet";
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      // Disconnect wifi if have active ethernet IP
      disconnectWiFi();
      setNetworkDisconnectedStatus("WiFi");   
         
      ETH_Ip = ETH.localIP().toString();
      log("[ETH] ETH connected. IP address: " + ETH_Ip);
      setNetworkConnectedStatus("ETH");
      
      // Reconnect MQTT when wifi is reconnected
      setupMQTT();
      // connectToMQTT();
      
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      log("[ETH] ETH Disconnected");
      setNetworkDisconnectedStatus("ETH");

      // Reconnect via Wifi as backup
      setupWiFi();
      break;
    case SYSTEM_EVENT_ETH_STOP:
      log("[ETH] ETH Stopped");
      setNetworkDisconnectedStatus("ETH");
      break;
    // ===============================
    // WiFi
    // ===============================
    case SYSTEM_EVENT_STA_START:
      log("[WiFi] WiFi station start");
      break;
    case SYSTEM_EVENT_STA_STOP:
      log("[WiFi] WiFi station stopped");
      break;  
    case SYSTEM_EVENT_STA_CONNECTED:
      log("[WiFi] WiFi station connected to AP");
      break;
    case SYSTEM_EVENT_AP_START:
      log("[WiFi] WiFi AP started");
      break;   
    case SYSTEM_EVENT_WIFI_READY:
      log("[WiFi] WiFi ready");
      break;     
    case SYSTEM_EVENT_STA_GOT_IP:
      // Disconnect if ETH is connected with IP
      if(eth_connected == true)
      {
        disconnectWiFi();
        setNetworkDisconnectedStatus("WiFi");
      }
      else
      {
        Serial.print("[WiFi] WiFi connected. ");
        WiFi_Ip = WiFi.localIP().toString();
        log("[WiFi] IP address: " + WiFi_Ip);        
        setNetworkConnectedStatus("WiFi");

        // Reconnect MQTT when wifi is reconnected
        setupMQTT();
        // connectToMQTT();
      }
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
    case SYSTEM_EVENT_STA_LOST_IP:
      log("[WiFi] WiFi lost connection. Reconnecting...");      
      setNetworkDisconnectedStatus("WiFi");
      WiFi.reconnect();
      break;
    default:      
      log("[ETH_WiFi] Unhandled ETH_WiFi event code: " + (String) event);      
      break;
  }
}
