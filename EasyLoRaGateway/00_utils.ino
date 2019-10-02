
char* string2Char(String str){
    if(str.length()!=0){
        char *p = const_cast<char*>(str.c_str());
        return p;
    }
}

// Mac to string without ":"
String macToStr(const uint8_t* mac, bool isWithColon)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
  
  if (isWithColon && i < 5)
    result += ':';
  }
  
  return result;
}

String getHostname(String type)
{
  uint8_t mac[6];

  if(type == "ETH")
  {
    esp_err_t err = esp_read_mac(mac, ESP_MAC_ETH);
  }
  else // type == WiFi
  {
   esp_err_t err = esp_read_mac(mac, ESP_MAC_WIFI_STA);
  }
  
  return "EG-" + macToStr(mac, false);
}

// ====================
// JSON
// ====================
// Print json to string for printing
String jsonToString(StaticJsonDocument<200> doc)
{
  String output;
  serializeJson(doc, output);
  return output;
}

// Convert to Json Doc from string
StaticJsonDocument<200> toJsonDoc(String jsonStr)
{  
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }

  return doc;
}

// Get Json Attribute value
String getJsonAttValue(String attName, StaticJsonDocument<200> doc)
{
  return doc[attName];
}

String removeJsonAtt(String attName, StaticJsonDocument<200> doc)
{
  doc.remove(attName);
  return jsonToString(doc);
}

// Get IoT message from Json
// Populate "src" and the remaining telemetry
IoTMessage getIoTMessage(String jsonStr){
  // Create Json document
  StaticJsonDocument<200> doc = toJsonDoc(jsonStr);

  // Get attribute
  // Get "src" attribute
  String src = getJsonAttValue("src", doc);
  // Remove "src" and return the rest as telemetry
  String telemetry = removeJsonAtt("src", doc); 
  
  // Create IoTMessage
  IoTMessage iotMsg;
  iotMsg.src = src;
  iotMsg.telemetry = telemetry;
  return iotMsg;
}
