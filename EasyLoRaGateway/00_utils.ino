// ====================================
// Hostname
// ====================================
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

// ====================================
// Chip ID
// ====================================
// To get chip ID, ESP32 will return mac address
String getChipID()
{
  // Get default mac address
  uint8_t chipid[6];
  esp_efuse_mac_get_default (chipid);

  // Format into uppercase mac address without :
  // Eg: BCDDC2C31684
  char returnStr[15];
  snprintf(returnStr, 15, "%02X%02X%02X%02X%02X%02X",chipid[0], chipid[1], chipid[2], 
            chipid[3], chipid[4], chipid[5]);
  return returnStr;
}

// To get chip ID, ESP32 will return last 3 octets of mac address
String getShortChipID()
{  
  // Get default mac address
  uint8_t chipid[6];
  esp_efuse_mac_get_default (chipid);

  // Format into uppercase mac address without :
  // Eg: BCDDC2C31684
  char returnStr[9];
  snprintf(returnStr, 9, "%02X%02X%02X", chipid[3], chipid[4], chipid[5]);
  return returnStr;
}

void printChipID()
{
  log("[UTIL] Chip ID: " + String(getChipID()));
}

// ====================================
// JSON
// ====================================
// Print json to string for printing
String jsonToString(StaticJsonDocument<200> doc)
{
  String output;
  serializeJson(doc, output);
  return output;
}

// Convert to Json Doc from string
StaticJsonDocument<200> toJsonDoc(const String& jsonStr)
{  
  //log("[UTIL] Parsing json string=" + jsonStr);
  
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);

  // Test if parsing succeeds.
  if (error) {
    log("[UTIL] Json parse failed.");
  }
    
  return doc;
}

// Get Json Attribute value
String getJsonAttValue(StaticJsonDocument<200> doc, const String& attNameLevel1, const String& attNameLevel2, 
                       const String& attNameLevel3)
{
  if(attNameLevel3 != "")
    doc[attNameLevel1][attNameLevel2][attNameLevel2];
  else if(attNameLevel2 != "")
    return doc[attNameLevel1][attNameLevel2];
  else
    return doc[attNameLevel1];
}

String removeJsonAtt(StaticJsonDocument<200> doc, const String& attName)
{
  doc.remove(attName);
  return jsonToString(doc);
}

void testJson()
{
  StaticJsonDocument<200> doc;
  String controlWidgetMsg = R"=====({"device":"Device C","data":{"id":207,"method":"setP1","params":false}})=====";
  doc = toJsonDoc(controlWidgetMsg);  
  Serial.println(getJsonAttValue(doc, "device", "", ""));
  Serial.println(getJsonAttValue(doc, "data", "id", ""));
  Serial.println(getJsonAttValue(doc, "data", "method", ""));
  Serial.println(getJsonAttValue(doc, "data", "params", ""));
}
