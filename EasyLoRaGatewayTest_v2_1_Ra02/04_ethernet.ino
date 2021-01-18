// ====================================
// Ethernet
// ====================================
// https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples
static bool eth_connected = false;
WiFiClient netClient;
String ETH_Status;
String ETH_Ip;
String ETH_hostname;

void setupEthernet()
{
    WiFi.onEvent(WiFiEvent);
    ETH.begin();
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
    default:
      break;
  }
}

void httpGet(const char * host, uint16_t port)
{
  Serial.print("[ETH] Connecting to ");
  Serial.print(host);

  //WiFiClient client;
  if (!netClient.connect(host, port)) {
    Serial.println(" failed!!!");
    return;
  }
  
  netClient.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (netClient.connected() && !netClient.available());

  //while (netClient.available()) {
  //  Serial.write(netClient.read());
  //}

  if (netClient.available()) {
    Serial.println(" OK. PASSED.");
  }

  netClient.stop();
}

void testHttpGet()
{
  httpGet("iotthinks.com", 80);
}
