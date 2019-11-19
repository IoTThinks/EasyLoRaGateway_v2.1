// ====================================
// Ethernet
// ====================================
// https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/examples
static bool eth_connected = false;
WiFiClient netClient;
String ETH_Status;
String ETH_Ip;

void setupEthernet()
{
  // Shared between Ethernet and Wifi
  // WiFi event is shared object between Ethernet and WiFi
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
}

void httpGet(char* host, uint16_t port)
{
  log("\n[ETH] Connecting to ", host);

  //WiFiClient client;
  if (!netClient.connect(host, port)) {
    log("[ETH] Connection failed");
    return;
  }
  
  netClient.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (netClient.connected() && !netClient.available());
  while (netClient.available()) {
    Serial.write(netClient.read());
  }

  log("closing connection");
  netClient.stop();
}

void testHttpGet()
{
  httpGet("iotthinks.com", 80);
}
