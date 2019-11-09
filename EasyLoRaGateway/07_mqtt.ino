// https://github.com/256dpi/arduino-mqtt
// =====================
// MQTT
// =====================
MQTTClient mqttClient(1024);

unsigned long lastMillis = 0;

String MQTT_Status = "Not Initialized";
String MQTT_Lastsent_Msg = "--No data--";

void setupMQTT() {
  // Should be connected to internet  
  mqttClient.begin(MQTT_SERVER, MQTT_PORT, netClient);
  // int keepAlive, bool cleanSession, int timeout
  // cleanSession = false => resume to use old session
  mqttClient.setOptions(MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION, MQTT_TIMEOUT);

  // MQTT listener
  mqttClient.onMessage(mqttMessageReceived);

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
    //mqttClient.connect(string2Char(getChipID()), MQTT_USERNAME, MQTT_SECRET);
    mqttClient.connect(string2Char(getChipID()), MQTT_USERNAME);
  }

  if(mqttClient.connected())
  {
    log("[MQTT] Connected to MQTT.");
    MQTT_Status = "Connected";   

     // Subscribe for attibutes
    //subscribeToMQTT(MQTT_API_RPC);
    //subscribeToMQTT(MQTT_API_ATTRIBUTE);
    
    // TODO: Need to reconnect to device at start and periodically
    publishDeviceConnect("BCDDC2C31684");
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
void mqttMessageReceived(String &topic, String &payload) {
  log("[MQTT] => Received message from MQTT: " + payload + " [Topic: " + topic + "]");
  
  // To process MQTT in TB module
  processDownlinkTBMessage(topic, payload);
}

void subscribeToMQTT(String topic)
{
  log("[MQTT] <= Subscribing to topic: " + topic);  

  // Try one more time
  if (!mqttClient.connected()) {    
    log("MQTT] MQTT failed. Skip subcribe command.");
    printMQTTErrors();
    connectToMQTT();
  }
  else
  {  
    // Subscribe for topic
    mqttClient.subscribe(topic);
  }
}

void publishToMQTT(String topic, String message) {
  log("[MQTT] <= Send message to MQTT: " + message + " [Topic: " + topic + "]");  

  // Try one more time
  if (!mqttClient.connected()) {
    log("[MQTT] MQTT failed. Skip publísh command.");
    printMQTTErrors();
    connectToMQTT();
  }
  else
  {
    mqttClient.publish(topic, message);
    MQTT_Lastsent_Msg = message;
  }
}

// Read the MQTT receive and send buffers and process any messages it finds.
void sendAndReceiveMQTT() {  
  mqttClient.loop(); // Sends and receives packets
  delay(10);  
}

void printMQTTErrors() {
  log("[MQTT] Last error = " + getMQTTLastError() + ", Return Code = " + getMQTTReturnCode());
}

String getMQTTLastError()
{
  int lwmqtt_err_t = mqttClient.lastError();
  switch(lwmqtt_err_t)
  {
    case 0:
      return "LWMQTT_SUCCESS";
    case -1:
      return "LWMQTT_BUFFER_TOO_SHORT";
    case -2:
      return "LWMQTT_VARNUM_OVERFLOW";
    case -3:
      return "LWMQTT_NETWORK_FAILED_CONNECT";
    case -4:
      return "LWMQTT_NETWORK_TIMEOUT";
    case -5:
      return "LWMQTT_NETWORK_FAILED_READ";
    case -6:
      return "LWMQTT_NETWORK_FAILED_WRITE";
    case -7:
      return "LWMQTT_REMAINING_LENGTH_OVERFLOW";
    case -8:
      return "LWMQTT_REMAINING_LENGTH_MISMATCH";
    case -9:
      return "LWMQTT_MISSING_OR_WRONG_PACKET";
    case -10:
      return "LWMQTT_CONNECTION_DENIED";
    case -11:
      return "LWMQTT_FAILED_SUBSCRIPTION";
    case -12:
      return "LWMQTT_SUBACK_ARRAY_OVERFLOW";
    case -13:
      return "LWMQTT_PONG_TIMEOUT";
    default:
      return "UNKNOWN_MQTT_LAST_ERROR";    
  }
}

String getMQTTReturnCode()
{
  int lwmqtt_return_code_t = mqttClient.returnCode();
  switch(lwmqtt_return_code_t)
  {
    case 0:
      return "LWMQTT_CONNECTION_ACCEPTED";
    case 1:
      return "LWMQTT_UNACCEPTABLE_PROTOCOL";
    case 2:
      return "LWMQTT_IDENTIFIER_REJECTED";
    case 3:
      return "LWMQTT_SERVER_UNAVAILABLE";
    case 4:
      return "LWMQTT_BAD_USERNAME_OR_PASSWORD";
    case 5:
      return "LWMQTT_NOT_AUTHORIZED";
    case 6:
      return "LWMQTT_UNKNOWN_RETURN_CODE";
    default:
      return "UNKNOWN_MQTT_RETURN_CODE";
  }
}
