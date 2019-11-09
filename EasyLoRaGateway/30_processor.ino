
// ==========================
// TB Main process
// ==========================
void processDownlinkTBMessage(const String& topic, const String& mqttMessage)
{
  log("[TB] => Process downlink message: : " + mqttMessage + " [Topic: " + topic + "]");
  
  StaticJsonDocument<200> doc = toJsonDoc(mqttMessage);
  if(doc.isNull())
  {
    log("[TB] Invalid MQTT Format. Skip processing message.");
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
      String outgoing((char *)0);
      
      // Gateway to ask not to execute command
      // "dst":"Device C","id":251,"method":"setP1","params":"false"
      outgoing += R"=====("dst":")=====";
      outgoing += deviceName;
      outgoing += R"=====(","id":)=====";
      outgoing += request_id;
      outgoing += R"=====(,"method":")=====";
      outgoing += methodName;
      outgoing += R"=====(","params":")=====";
      outgoing += params;
      outgoing += "\"";
      sendLoRaMessage(outgoing);
      
      // Process for attribute update and RPC response is done at Uplink process     
    }   
    else if(methodName == "getTermInfo")
    {
      // {"device":"Device C","data":{"id":318,"method":"getTermInfo"}}
      // publishRPCResponse(deviceName, request_id, R"=====({ok: true, platform: "Easy LoRa", type: "ENI", release: "1.2"})=====");      
      // publishRPCResponse(deviceName, request_id, R"=====({ok: true, platform: os.platform(), type: os.type(), release: os.release()})====="); 
    }

    log("[TB] => Downlink message: MQTT_API_RPC. Device=" + deviceName + ", id=" + request_id + ", method=" + methodName + ", params=" + params);
  }
  else
  {
    log("[TB] => Downlink message: Unhandled message topic.");
  }
}

// Process uplink message to TB
// Uplink message will be in below format
// "src":"BCDDC2C31684","id":366, "act":"att", "data":{"p1":true}, "meta":{"rssi":-55,"snr":9.25}
void processUplinkTBMessage(const String& message)
{
  log("[TB] <= Process uplink message: " + message);

  // Create Json document
  StaticJsonDocument<200> doc = toJsonDoc("{" + message + "}");
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
void publishDeviceConnect(const String& deviceName)
{
  String returnStr((char *)0);
  returnStr.reserve(64);
  returnStr += R"=====({"device":")=====";
  returnStr += deviceName;
  returnStr += R"=====("})====="; 

  publishToMQTT(MQTT_API_CONNECT, returnStr);
}

// Publish RPC response
// {"device": "Device C", "id": $request_id, "data": {"success": true}}
// Data is in json format with proper { }
void publishRPCResponse(const String& deviceName, const String& requestId, const String& data)
{
  if(deviceName == "null")
  {
    log("[TB] Null device name. Skip publishing RPC.");
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

// Set device attribute
// attributeData is in format {"p1":"false","p2":"true"}
// {"Device C":{"p1":"false","p2":"true"}}
void publishAttribute(const String& deviceName, const String& attributeData)
{
  if(deviceName == "null")
  {
    log("[TB] Null device name. Skip publishing attribute.");
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
    log("[TB] Null device name. Skip publishing telemetry.");
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

void testTBJsson()
{
  publishDeviceConnect("Device C");
  publishAttribute("Device C", R"=====("switch":"false")=====");
  publishTelemetry("Device C", R"=====("temperature": 42, "humidity": 80)=====");   
}

// ==========================
// CRON JOBS
// ==========================
// Gateway to send LoRa request to LoRa node for telemetry data
// Message format: "dst":"Device C","id":251,"method":"poll","params":"tmt"
void pollTelemetryData(String deviceID)
{
  String outgoing((char *)0);  
  outgoing.reserve(64);
  
  outgoing += R"=====("dst":")=====";
  outgoing += deviceID;
  outgoing += R"=====(","id":)=====";
  outgoing += (++gatewayReqID);
  outgoing += R"=====(,"method":"poll","params":"tmt")=====";

  sendLoRaMessage(outgoing);
}
