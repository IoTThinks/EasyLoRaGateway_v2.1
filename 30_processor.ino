// ==========================
// Process TB Downlink
// ==========================
void processDownlinkTBMessage(const String& topic, const String& mqttMessage)
{
  log("[PROC] => Process downlink message: : ", string2Char(mqttMessage), ". Topic: ", string2Char(topic));
  
  StaticJsonDocument<200> doc = toJsonDoc(string2Char(mqttMessage));
  if(doc.isNull())
  {
    log("[PROC] Invalid downlink message");
    return;
  }
  
  if(topic == MQTT_API_RPC)
  {
    String deviceName = getJsonAttValue(doc, "device", "", "");   
    String request_id = getJsonAttValue(doc, "data", "id", "");  
    String methodName = getJsonAttValue(doc, "data", "method", "");   
    String params = getJsonAttValue(doc, "data", "params", "");
    
    // To forward to LoRa node
    if(methodName == "setP2")
    {      
      // Gateway to ask node to execute command
      // "dst":"Device C","id":251,"method":"setP1","params":"false"
      sendDownlinkLoRaCommand(deviceName, request_id, methodName, params);
      
      // Process for attribute update and RPC response is done at Uplink process
      // Do nothing here
    }   
    else if(methodName == "getTermInfo")
    {
      // {"device":"Device C","data":{"id":318,"method":"getTermInfo"}}
      // publishRPCResponse(deviceName, request_id, R"=====({ok: true, platform: "Easy LoRa", type: "ENI", release: "1.2"})=====");      
      // publishRPCResponse(deviceName, request_id, R"=====({ok: true, platform: os.platform(), type: os.type(), release: os.release()})====="); 
    }

    log("[PROC] => Process downlink message: MQTT_API_RPC. Device=", string2Char(deviceName),
      ", id=", string2Char(request_id), ", method=", string2Char(methodName),
      ", params=", string2Char(params));
  }
  else
  {
    log("[PROC] => Unhandled downlink message topic.");
  }
}

// ==========================
// Process TB Uplink
// ==========================
// Process uplink message to TB
// Uplink message will be in below format
// "src":"BCDDC2C31684","id":366, "act":"att", "data":{"p1":true}, "meta":{"rssi":-55,"snr":9.25}
void processUplinkTBMessage(const char* message)
{
  log("[PROC] <= Process uplink message: ", message);

  // Added { } to be a json
  char parsingMessage[128];
  strcpy(parsingMessage, "{");
  strcat(parsingMessage, message);
  strcat(parsingMessage, "}");
  
  // Create Json document
  StaticJsonDocument<200> doc = toJsonDoc(parsingMessage);

  // TODO: To convert to char* for faster speed and lower heap fragmentation
  String src = getJsonAttValue(doc, "src", "", "");
  String id = getJsonAttValue(doc, "id", "", "");  
  String act = getJsonAttValue(doc, "act", "", "");  
  String data = getJsonAttValue(doc, "data", "", "");  
  String meta = getJsonAttValue(doc, "meta", "", "");

  if(act == "att")
  {
    // Main data
    publishAttribute(src, data);

    // Meta data such as RSSI, SNR
    publishTelemetry(src, meta);

    // Attribute request is normally from RPC
    publishRPCResponse(src, id, "Done.");
  }
  else if(act = "tmt")
  {
    // Main data
    publishTelemetry(src, data);

    // Meta data such as RSSI, SNR
    publishTelemetry(src, meta);
  }
}

// ==========================
// TB MQTT API
// ==========================
// To inform TB that device is connected to gateway
//  {"device":"Device A"}
// TODO: To change to char* and free after use
void publishDeviceConnect(const String& deviceName)
{
  String returnStr((char *)0);
  returnStr.reserve(64);
  returnStr += R"=====({"device":")=====";
  returnStr += deviceName;
  returnStr += R"=====("})====="; 

  publishToMQTT(MQTT_API_CONNECT, returnStr);
}

// TODO: To change to char* and free after use
// Publish RPC response
// {"device": "Device C", "id": $request_id, "data": {"success": true}}
// Data is in json format with proper { }
void publishRPCResponse(const String& deviceName, const String& requestId, const String& data)
{
  if(deviceName == "null")
  {
    log("[PROC] Null device name. Skip publishing RPC.");
    return;
  }
  
  String returnStr((char *)0);
  returnStr.reserve(64);
  returnStr += R"=====({"device": ")=====";
  returnStr += deviceName;
  returnStr += R"=====(", "id": )=====";
  returnStr += requestId;
  returnStr += R"=====(, "data": )=====";
  returnStr += data;
  returnStr += "}";

  publishToMQTT(MQTT_API_RPC, returnStr);
}

// TODO: To change to char* and free after use
// Set device attribute
// attributeData is in format {"p1":"false","p2":"true"}
// {"Device C":{"p1":"false","p2":"true"}}
void publishAttribute(const String& deviceName, const String& attributeData)
{
  if(deviceName == "null")
  {
    log("[PROC] Null device name. Skip publishing attribute.");
    return;
  }
  
  String returnStr((char *)0);  
  returnStr.reserve(64);  
  returnStr += R"=====({")=====";
  returnStr += deviceName;
  returnStr += R"=====(":)=====";
  returnStr += attributeData;
  returnStr += R"=====(})=====";
  
  publishToMQTT(MQTT_API_ATTRIBUTE, returnStr);
}

// TODO: To change to char* and free after use
// To send telemetry data
// For one device only
// Timestamp is default to 0. Will be populated by server
// telemetryData: {"temperature": 42, "humidity": 80}
/*
{
"Device A": 
  [
    {
      "ts": 1483228800111,
      "values": {"temperature": 42, "humidity": 80}
    }
  ]
}
*/
void publishTelemetry(const String& deviceName, const String& telemetryData)
{
  if(deviceName == "null")
  {
    log("[PROC] Null device name. Skip publishing telemetry.");
    return;
  }
  
  String returnStr((char *)0);  
  returnStr.reserve(128);
  returnStr += R"=====({")=====";
  returnStr += deviceName;
  returnStr += R"=====(":[{"ts": 0, "values":)=====";
  returnStr += telemetryData;
  returnStr += "}]}";
  
  publishToMQTT(MQTT_API_TELEMETRY, returnStr);
}

// ==========================
// LoRa downlink commands
// ==========================
// Gateway to ask node to execute command via LoRa message
// "dst":"Device C","id":251,"method":"setP1","params":"false"
void sendDownlinkLoRaCommand(const String& deviceName, const String& request_id, const String& methodName, 
  const String& params)
{ 
  Serial.println("sendDownlinkLoRaCommand");
  logHeap();
  char *outgoing = (char *) malloc (256);
      
  // Gateway to ask node to execute command
  // "dst":"Device C","id":251,"method":"setP1","params":"false"
  strcpy(outgoing, R"=====("dst":")=====");
  strcat(outgoing, string2Char(deviceName));
  strcat(outgoing, R"=====(","id":)=====");
  strcat(outgoing, string2Char(request_id));
  strcat(outgoing, R"=====(,"method":")=====");
  strcat(outgoing, string2Char(methodName));
  strcat(outgoing, R"=====(","params":")=====");
  strcat(outgoing, string2Char(params));
  strcat(outgoing, "\"");

  // Send LoRa command
  sendDownlinkLoRaMessage(deviceName, outgoing);
  
  // Free memory
  free(outgoing);
  
  logHeap();
}

// TODO: To change to char* and free after use
// Gateway to send LoRa request to LoRa node for telemetry data
// Message format: "dst":"Device C","id":251,"method":"poll","params":"tmt"
void pollTelemetryData(const String& deviceID)
{
  Serial.println("pollTelemetryData");
  logHeap();
  
  char *outgoing = (char *) malloc (256);
  strcpy(outgoing, R"=====("dst":")=====");
  strcat(outgoing, string2Char(deviceID));
  strcat(outgoing, R"=====(","id":)=====");
  strcat(outgoing, string2Char(String(++SYS_GatewayReqID)));
  strcat(outgoing, R"=====(,"method":"poll","params":"tmt")=====");

  // Send LoRa command
  sendDownlinkLoRaMessage(deviceID, outgoing);
  
  // Free memory
  free(outgoing);
  
  logHeap();
}
// ==========================
// Physical LoRa transmission
// ==========================
void sendDownlinkLoRaMessage(const String& deviceName, const char* outgoing)
{
  // Get customed frequencty for each device id
  long customedLoRaFreq = getCustomFreqFromDeviceName(deviceName);

  // Send via LoRa 2
  sendDynamicLoRa2Message(customedLoRaFreq, outgoing); 
}
