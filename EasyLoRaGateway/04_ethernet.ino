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
  // Shared between Ethernet and Wifi
  // WiFi event is shared object between Ethernet and WiFi
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
}

void httpGet(const char * host, uint16_t port)
{
  Serial.print("\n[ETH] Connecting to ");
  Serial.println(host);

  //WiFiClient client;
  if (!netClient.connect(host, port)) {
    Serial.println("[ETH] Connection failed");
    return;
  }
  
  netClient.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (netClient.connected() && !netClient.available());
  while (netClient.available()) {
    Serial.write(netClient.read());
  }

  Serial.println("closing connection\n");
  netClient.stop();
}

void testHttpGet()
{
  httpGet("iotthinks.com", 80);
}
