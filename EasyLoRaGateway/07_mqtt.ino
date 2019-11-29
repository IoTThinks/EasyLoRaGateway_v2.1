// https://pubsubclient.knolleary.net/
// https://github.com/knolleary/pubsubclient
// =====================
// MQTT
// =====================
PubSubClient mqttClient;
String MQTT_Status = "Not Initialized";
//String MQTT_Lastsent_Msg = "--No data--";

void setupMQTT() {
  // Should be connected to internet
  mqttClient.setClient(netClient);
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  // TODO: Set MQTT_CLEAN_SESSION to false to keep existing session?
  // Default: MQTT_KEEPALIVE=15s, MQTT_CLEAN_SESSION=???, MQTT_SOCKET_TIMEOUT=15s

  // MQTT listener
  mqttClient.setCallback(mqttMessageReceived);

  // Only connect to MQTT when there is a message to MQTT server
  // The default is to subscribe everything
  connectToMQTT();  
}

void connectToMQTT() {
  // check network
  if(!eth_connected && !wifi_connected) {
    log("[MQTT] ETH and WiFi not connected. Skip connecting to MQTT...");    
  }
  else
  {
    log("[MQTT] Connecting to MQTT...");
    // NULL for not using password
    mqttClient.connect(SYS_ChipID, MQTT_USERNAME, NULL);
  }

  if(mqttClient.connected())
  {
    log("[MQTT] Connected to MQTT.");
    onOffSpeaker(1, true); // OK
    MQTT_Status = "Connected";   

    // Initiate processor when MQTT is connected
    setupProcessor();
  }
  else
  {    
    log("MQTT] Connect to MQTT failed.");
    printMQTTErrors();
    MQTT_Status = "Not connected";
  }
}

// ====================
// Basic commands
// ====================
// Public status of MQTT
bool isMQTTConnected()
{
  return mqttClient.connected();
}

void mqttMessageReceived(char* topic, byte* payload, unsigned int length) 
{
  char payloadStr[MQTT_BUFFER_SIZE];
  
  for (int i = 0; i < length; i++) {
    payloadStr[i] = (char)payload[i];
  }

  payloadStr[length] = '\0';
  
  /// log("[MQTT] => Received from MQTT: ", string2Char(payload), " .Topic: ", string2Char(topic));
  log("[MQTT] => Received from MQTT: ", payloadStr, " .Topic: ", topic);
  
  // To process MQTT in TB module
  processDownlinkTBMessage(topic, payloadStr);
}

void subscribeToMQTT(const char* topic)
{
  log("[MQTT] <= Subscribing to topic: ", topic);

  // Try one more time
  if (!mqttClient.connected()) {    
    log("MQTT] ERROR!!! MQTT FAILED. SKIP SUBSCRIBING.");
    printMQTTErrors();
    onOffSpeaker(6, true); // Error
    connectToMQTT();
  }

  // Subscribe for topic
  // mqttClient.subscribe(topic);
  mqttClient.subscribe(topic, MQTT_QOS); // QOS 1
}

void publishToMQTT(const char* topic, const char* message) {
  log("[MQTT] <= Send to MQTT: ", message, ". Topic: ", string2Char(topic));  

  // Try one more time
  if (!mqttClient.connected()) {
    log("[MQTT] ERROR!!! MQTT FAILED. SKIP PUBLISHING.");
    printMQTTErrors();
    onOffSpeaker(6, true); // Error
    connectToMQTT();
  }
  
  // TODO: To set QoS 1?
  mqttClient.publish(topic, message, MQTT_RETAINED);
  
  //MQTT_Lastsent_Msg = message;
}

// Read the MQTT receive and send buffers and process any messages it finds.
void sendAndReceiveMQTT() {  
  mqttClient.loop(); // Sends and receives packets
  vTaskDelay(1);  
}

void printMQTTErrors() {
  log("[MQTT] Last state = ", getMQTTState());
}

char* getMQTTState()
{
  int mqttErr = mqttClient.state();
  switch(mqttErr)
  {
    case -4:
      return "MQTT_CONNECTION_TIMEOUT";
    case -3:
      return "MQTT_CONNECTION_LOST";
    case -2:
      return "MQTT_CONNECT_FAILED";
    case -1:
      return "MQTT_DISCONNECTED";
    case 0:
      return "MQTT_CONNECTED";
    case 1:
      return "MQTT_CONNECT_BAD_PROTOCOL";
    case 2:
      return "MQTT_CONNECT_BAD_CLIENT_ID";
    case 3:
      return "MQTT_CONNECT_UNAVAILABLE";
    case 4:
      return "MQTT_CONNECT_BAD_CREDENTIALS";
    case 5:
      return " MQTT_CONNECT_UNAUTHORIZED";
    default:
      return "MQTT_UNKNOWN_ERROR";    
  }
}
