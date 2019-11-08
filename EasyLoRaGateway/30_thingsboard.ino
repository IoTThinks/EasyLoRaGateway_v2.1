
// ==========================
// TB Main process
// ==========================
void processDownlinkTBMessage(const String& topic, const String& mqttMessage)
{
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

    // TODO: Forward to node via LoRa instead
    if(methodName == "setP1")
    {
      // TODO: Need to get response from node
      publishAttribute(deviceName, "p1", params);
      
      // Response "success"
      publishRPCResponse(deviceName, request_id, R"=====({"success": false})=====");
      //publishRPCResponse(deviceName, request_id, "Done.");
      //publishRPCResponse(deviceName, request_id, R"=====({"done": "true", "data": [{"stdout": "Hello"}]})=====");
    }
    else if(methodName == "setP2")
    {
      // TODO: Need to get response from node
      publishAttribute(deviceName, "p2", params);
      
      // Response "success"
      publishRPCResponse(deviceName, request_id, R"=====({"success": false})=====");
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
void processUplinkTBMessage(const String& topic, const String& mqttMessage)
{
  log("[TB] <= Uplink message: " + mqttMessage);
  EasyJsonPacket easyJsonPacket = getEasyJsonPacket(mqttMessage);
  publishTelemetry(easyJsonPacket.src, easyJsonPacket.telemetry);
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
// For one device and one parameter only
// {"Device C":{"p1":"false"}}
void publishAttribute(const String& deviceName, const String& paramName, const String& paramValue)
{
  String returnStr((char *)0);  
  returnStr.reserve(64);
  returnStr += R"=====({")=====";
  returnStr += deviceName;
  returnStr += R"=====(":{")=====";
  returnStr += paramName;
  returnStr += R"=====(":")=====";
  returnStr += paramValue;
  returnStr += R"=====("}})=====";
  
  publishToMQTT(MQTT_API_ATTRIBUTE, returnStr);
}

// To send telemetry data
// For one device only
// Timestamp is default to 0. Will be populated by server
// telemetryData: "temperature": 42, "humidity": 80
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
  String returnStr((char *)0);  
  returnStr.reserve(128);
  returnStr += R"=====({")=====";
  returnStr += deviceName;
  returnStr += R"=====(":[{"ts": 0, "values":{)=====";
  returnStr += telemetryData;
  returnStr += "}}]}";
  
  publishToMQTT(MQTT_API_TELEMETRY, returnStr);
}

void testTBJsson()
{
  publishDeviceConnect("Device C");
  publishAttribute("Device C", "switch", "false");
  publishTelemetry("Device C", R"=====("temperature": 42, "humidity": 80)=====");   
}

// ==========================
// TB Data Converter
// ==========================
// TO DO: NEED IMPROVEMENT
// Get IoT message from Json
// Populate "src" and the remaining telemetry
// Format of input: src:"DeviceID","mb_temp":20,"s_smk":30
EasyJsonPacket getEasyJsonPacket(String easyJsonPacket){
  // Create Json document
  StaticJsonDocument<200> doc = toJsonDoc("{" + easyJsonPacket + "}");

  // Get attribute
  // Get "src" attribute
  String src = getJsonAttValue(doc, "src", "", "");
  
  // Remove "src" and return the rest as telemetry
  String telemetry = removeJsonAtt(doc, "src");

  // Remove { at the begining and } at the end of the json
  telemetry = telemetry.substring(1, telemetry.length() -1);
  
  // Create IoTMessage
  EasyJsonPacket easyPacket;
  easyPacket.src = src;
  easyPacket.telemetry = telemetry;

  //log("easyPacket.src=" + easyPacket.src);
  //log("easyPacket.telemetry=" + easyPacket.telemetry);
  return easyPacket;
}
