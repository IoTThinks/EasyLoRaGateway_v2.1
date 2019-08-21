// https://github.com/256dpi/arduino-mqtt
// =====================
// MQTT
// =====================
// Definitions
#define mqttBrokerServer "broker.shiftr.io"
#define mqttNamespace "vinhdat82~easyloragateway"
#define mqttUsername "iotthinks"
#define mqttSecret "easyloragateway"
#define mqttTopic_FromNodes "/iotthinks/easyloragateway/nodes/from/messages"

MQTTClient mqttClient;

unsigned long lastMillis = 0;

String MQTT_Status = "Not Initialized";
String MQTT_Lastsent_Msg = "--No data--";

void setupMQTT() {
  // Should be connected to internet
  mqttClient.begin(mqttBrokerServer, netClient);
  mqttClient.onMessage(mqttMessageReceived);

  // Only connect to MQTT when there is a message to MQTT server
  connectToMQTT();
}

void connectToMQTT() {
  // check network
  if(!eth_connected && !wifi_connected) {
    Serial.println("[MQTT] ETH and WiFi not connected. Reconnecting...");    
  }
  else
  {
    Serial.println("[MQTT] Connecting to MQTT...");
    mqttClient.connect(mqttNamespace, mqttUsername, mqttSecret);  
    delay(1000);
  }

  if(mqttClient.connected())
  {
    Serial.println("[MQTT] Connected to MQTT.");
    MQTT_Status = "Connected";
    mqttClient.subscribe("/iotthinks/easyloragateway/nodes/to/ota");
  }
  else
  {
    Serial.println("[MQTT] Not connected to MQTT.");
    MQTT_Status = "Not connected";
  }
}

void mqttMessageReceived(String &topic, String &payload) {
  Serial.println("[MQTT]<= Received message from MQTT: " + payload + " [Topic: " + topic + "]");
}

void publishToMQTT(String topic, String message) {
  Serial.println("[MQTT]=> Send message to MQTT: " + message + " [Topic: " + topic + "]");  
  
  if (!mqttClient.connected()) {
    connectToMQTT();
  }

  mqttClient.publish(topic, message);
  MQTT_Lastsent_Msg = message;
}

// Read the MQTT receive and send buffers and process any messages it finds.
void flushMQTTBuffer() {
  mqttClient.loop(); // Why need this?
  delay(10);
}

void forwardNodeMessageToMQTT(String message)
{
  publishToMQTT(mqttTopic_FromNodes, message);  
}

void testMQTT() {
  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    publishToMQTT("/hello", "Easy Lora Gateway");
  }
}
