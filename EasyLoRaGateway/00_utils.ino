// ====================================
// Chip ID
// ====================================
// To get chip ID, ESP32 will return mac address
void setChipID()
{
  // ========
  // Chip ID
  // ========
  // Get default mac address
  uint8_t chipid[6];
  esp_efuse_mac_get_default (chipid);

  // Format into uppercase mac address without :
  // Eg: BCDDC2C31684
  snprintf(SYS_ChipID, 15, "%02X%02X%02X%02X%02X%02X",chipid[0], chipid[1], chipid[2], 
            chipid[3], chipid[4], chipid[5]);

  log("[UTILS] ChipID: ", SYS_ChipID);

  // ========
  // Hostname
  // ========  
  strcpy(SYS_HostName, "EG-");
  strcat(SYS_HostName, SYS_ChipID);
  log("[UTILS] Host Name: ", SYS_HostName);
}

void printChipID()
{
  log("[UTIL] Chip ID: ", SYS_ChipID);
}

// ====================================
// Converter
// ====================================
char* string2Char(String str){
    if(str.length()!=0){
        char *p = const_cast<char*>(str.c_str());
        return p;
    }
}

// Char to string
String char2String(char* ch)
{
  return String(ch);
}

// String to int
int string2Int(String str)
{
  return str.toInt();
}

// From char* to int
int charPtr2Int(const char* charPtr)
{
  return atoi(charPtr);
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

 // To convert a hex value in string to decimal
int hexChar2Decimal(const char* hexInput)
{
  return strtol(hexInput, NULL, 16);
}

// Device name is in format BCDDC2C56C64
long getCustomFreqForDeviceID(const char* deviceID)
{
  // Last octet is from 10th character. 64 in this case
  int deviceNetworkID = hexChar2Decimal(&deviceID[10]);
  int deviceCustomChannel = deviceNetworkID % LORA2_FREQ_NUM_CHANNELS + 1;  
  return LORA2_FREQ_BASE + deviceCustomChannel * LORA2_FREQ_STEPS;
}

// ====================================
// JSON
// ====================================
// Print json to string for printing
String jsonToString(StaticJsonDocument<UTILS_JSON_MAXLENGTH> doc)
{
  String output;
  serializeJson(doc, output);
  return output;
}

// Convert to Json Doc from string
StaticJsonDocument<UTILS_JSON_MAXLENGTH> toJsonDoc(const char* jsonStr)
{ 
  // log("[UTIL] Parsing json string=", jsonStr);
  StaticJsonDocument<UTILS_JSON_MAXLENGTH> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);

  // Test if parsing succeeds.
  if (error) {
    log("[UTIL] ERROR!!! JSON PARSE FAILED. FAILED JSON: ", jsonStr);
  }
    
  return doc;
}

// Get Json Attribute value
String getJsonAttValue(StaticJsonDocument<UTILS_JSON_MAXLENGTH> doc, const String& attNameLevel1, 
                            const String& attNameLevel2, const String& attNameLevel3)
{  
  if(attNameLevel3 != "")
    return doc[attNameLevel1][attNameLevel2][attNameLevel2];
  else if(attNameLevel2 != "")
    return doc[attNameLevel1][attNameLevel2];
  else
    return doc[attNameLevel1];
}

String removeJsonAtt(StaticJsonDocument<UTILS_JSON_MAXLENGTH> doc, const String& attName)
{
  doc.remove(attName);
  return jsonToString(doc);
}
