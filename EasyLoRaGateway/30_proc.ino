// ==========================
// PROCESSOR
// ==========================
void setupProcessor()
{
  log("[PROC] Setup processor.");
  // Subscribed by default?
  // For attributes of the devices / nodes
  // subscribeToMQTT(MQTT_API_RPC);
  // subscribeToMQTT(MQTT_API_ATTRIBUTE);
  
  
  // Subscribe for attibutes from device MQTT API
  // For attributes of the gateway itself
  subscribeToMQTT(MQTT_API_GATEWAY_ATTRIBUTE_RESPONSE);    
  // For attribute updates of the gateway itself
  subscribeToMQTT(MQTT_API_GATEWAY_ATTRIBUTE_SUBSCRIBE);

  // Publish node information request
  updateNodeListing();   
}

// ==========================
// Process TB Downlink
// ==========================
void processDownlinkTBMessage(const char* topic, const char* mqttMessage)
{
  log("[PROC] => Process downlink message: ", mqttMessage, ". Topic: ", topic);
  
  StaticJsonDocument<UTILS_JSON_MAXLENGTH> doc = toJsonDoc(mqttMessage);
  if(doc.isNull())
  {
    log("[PROC] Invalid downlink message");
    return;
  }
  
  if(strcmp(topic, MQTT_API_RPC) == 0)
  {
    String deviceID = getJsonAttValue(doc, "device", "", "");
    String request_id = getJsonAttValue(doc, "data", "id", "");
    String methodName = getJsonAttValue(doc, "data", "method", "");
    String params = getJsonAttValue(doc, "data", "params", "");
    
    // To forward to LoRa node
    if(methodName == "setP2")
    {      
      // Gateway to ask node to execute command
      // "dst":"Device C","id":251,"method":"setP1","params":"false"
      sendDownlinkLoRaCommand(string2Char(deviceID), request_id, methodName, params);
      
      // Process for attribute update and RPC response is done at Uplink process
      // Do nothing here
    }   
    else if(methodName == "getTermInfo")
    {
      // {"device":"Device C","data":{"id":318,"method":"getTermInfo"}}
      // publishRPCResponse(deviceName, request_id, R"=====({ok: true, platform: "Easy LoRa", type: "ENI", release: "1.2"})=====");      
      // publishRPCResponse(deviceName, request_id, R"=====({ok: true, platform: os.platform(), type: os.type(), release: os.release()})====="); 
    }

    log("[PROC] => Process downlink message: MQTT_API_RPC. Device=", string2Char(deviceID),
      ", id=", string2Char(request_id), ", method=", string2Char(methodName),
      ", params=", string2Char(params));
  }
  // else if(strlen(strchr(topic, MQTT_API_GATEWAY_ATTRIBUTE_RESPONSE_BASE)) > 0)
  else if(String(topic).indexOf(MQTT_API_GATEWAY_ATTRIBUTE_RESPONSE_BASE) >= 0) // TODO: To avoid using String
  {
    // Topic: v1/devices/me/attributes/response/1    
    // {"shared":{"Node1":"BCDDC2C31684"}}    

    // TODO: To make the count dynamic
    // MQTT_API_GATEWAY_ATTRIBUTE_RESPONSE_BASE has 34 characters
    // Get id at the back of topic from 34th character
    const char* id = &topic[34];
    
    // Node[id] up to 999 nodes
    char* nodeAttName = (char*) malloc(8);
    strcpy(nodeAttName, "Node");
    strcat(nodeAttName, id);

    // Get actual Device Id from node[id]
    String updatedNodeId = getJsonAttValue(doc, "shared", nodeAttName, "");

    // Ignore node without information (aka. Node in not the list in portal)
    if(updatedNodeId != "null")
    {        
      // To process and update one node at a time
      // id is typeof byte
      updateNodeInfo(charPtr2Int(id), updatedNodeId);
    }

    // Free memory
    free(nodeAttName);
  }
  else if(strcmp(topic, MQTT_API_GATEWAY_ATTRIBUTE_SUBSCRIBE) == 0)
  {
    log("[PROC] To process new node updates / delete");

    // Check for deleted nodes
    // {"deleted":["Node20","Node21"]}
    // deletedNodeIds should be in ["Node20","Node21"]
    String deletedNodeIds = getJsonAttValue(doc, "deleted", "", "");
    
    if(deletedNodeIds != "null") // Delete nodes
    {
      // To stop publishing updates for this particular device to this Gateway.
      // The device is not deleted.
      // {"deleted":["Node20","Node21"]}. Topic: v1/devices/me/attributes
      // Create json object with ["Node20","Node21"] only
      StaticJsonDocument<UTILS_JSON_MAXLENGTH> deletedDoc;
      deserializeJson(deletedDoc, deletedNodeIds);
       
      JsonArray deletedArray = deletedDoc.as<JsonArray>();
      for(JsonVariant item : deletedArray) 
      {
        // Node20
        const char* deletedId = item.as<char*>();
        
        // 20
        int deletedInt = charPtr2Int(&deletedId[4]);

        // Remove node id 20
        removeNodeNodeInfo(deletedInt);
      }           
    }
    
    // Add or update nodes
    // New nodes or updated nodes
    // {"Node19":"222222222222"}
    JsonObject jsonObject = doc.as<JsonObject>();    
    for (JsonPair keyValue : jsonObject) 
    {
      const char* nodeId = keyValue.key().c_str(); 

      // Ignore "deleted nodes"
      if(strcmp(nodeId, "deleted") == 0)
        continue;
      
      // Remove prefix "Node"
      int nodeIdInt = charPtr2Int(&nodeId[4]);
      const char* nodeUpdate = keyValue.value().as<char*>();

      // Update node information
      updateNodeInfo(nodeIdInt, nodeUpdate);
    }
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
  StaticJsonDocument<UTILS_JSON_MAXLENGTH> doc = toJsonDoc(parsingMessage);

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
  else if(act == "tmt")
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
void publishDeviceConnect(const char* deviceID)
{
  char *outgoing = (char *) malloc (64);
  strcpy(outgoing, R"=====({"device":")=====");
  strcat(outgoing, string2Char(deviceID));
  strcat(outgoing, R"=====("})=====");

  publishToMQTT(MQTT_API_CONNECT, outgoing);

  // Free memory
  vTaskDelay(1);
  free(outgoing);
}

// To stop publishing updates for this particular device to this Gateway.
// The device is not deleted.
// {"device":"Device A"}
void publishDeviceDisonnect(const char* deviceID)
{
  char *outgoing = (char *) malloc (64);
  strcpy(outgoing, R"=====({"device":")=====");
  strcat(outgoing, string2Char(deviceID));
  strcat(outgoing, R"=====("})=====");

  publishToMQTT(MQTT_API_DISCONNECT, outgoing);

  // Free memory
  vTaskDelay(1);
  free(outgoing);
}

// TODO: To change to char* and free after use
// Publish RPC response
// {"device": "Device C", "id": $request_id, "data": {"success": true}}
// Data is in json format with proper { }
void publishRPCResponse(const String& deviceID, const String& requestId, const String& data)
{
  if(deviceID == "null")
  {
    log("[PROC] Null device id. Skip publishing RPC.");
    return;
  }

  char *outgoing = (char *) malloc (256);
  strcpy(outgoing, R"=====({"device": ")=====");
  strcat(outgoing, string2Char(deviceID));
  strcat(outgoing, R"=====(", "id": )=====");
  strcat(outgoing, string2Char(requestId));
  strcat(outgoing, R"=====(, "data": )=====");
  strcat(outgoing, string2Char(data));
  strcat(outgoing, "}");  

  publishToMQTT(MQTT_API_RPC, outgoing);

  // Free memory
  vTaskDelay(1);
  free(outgoing);
}

// TODO: To change to char* and free after use
// Set device attribute
// attributeData is in format {"p1":"false","p2":"true"}
// {"Device C":{"p1":"false","p2":"true"}}
void publishAttribute(const String& deviceID, const String& attributeData)
{
  if(deviceID == "null")
  {
    log("[PROC] Null device id. Skip publishing attribute.");
    return;
  }
  
  char *outgoing = (char *) malloc (256);
  strcpy(outgoing, R"=====({")=====");
  strcat(outgoing, string2Char(deviceID));
  strcat(outgoing, R"=====(":)=====");
  strcat(outgoing, string2Char(attributeData));
  strcat(outgoing, R"=====(})=====");
  
  publishToMQTT(MQTT_API_ATTRIBUTE, outgoing);

  // Free memory
  vTaskDelay(1);
  free(outgoing);
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
void publishTelemetry(const String& deviceID, const String& telemetryData)
{
  if(deviceID == "null")
  {
    log("[PROC] Null device ID. Skip publishing telemetry.");
    return;
  }

  char *outgoing = (char *) malloc (256);
  strcpy(outgoing, R"=====({")=====");
  strcat(outgoing, string2Char(deviceID));
  strcat(outgoing, R"=====(":[{"ts": 0, "values":)=====");
  strcat(outgoing, string2Char(telemetryData));
  strcat(outgoing, "}]}");
   
  publishToMQTT(MQTT_API_TELEMETRY, outgoing);

  // Free memory
  vTaskDelay(1);
  free(outgoing);
}

// ==========================
// TB Node provisioning
// ==========================
// To request portal for all node infmration
void updateNodeListing()
{
  // Request for active node listing
  for(int i=0; i < SYS_MAXNODES; i++)
  {        
    // Clear the node id
    SYS_NODELIST[i][0] = '\0';
    
    // Ask for update node id
    // Request number i for node i
    requestGatewayAttribute(i, string2Char("Node" + String(i)));
  }
}

// To ask portal for stored node information
// RequestId is integer
// attributeData is {"sharedKeys":"Node1"
void requestGatewayAttribute(int requestId, const char* sharedKey)
{
  char* outgoing = (char *) malloc (128);
      
  // Gateway to ask for its shared attribute value
  // {"sharedKeys":"TotalNodes"}
  strcpy(outgoing, R"=====({"sharedKeys":")=====");
  strcat(outgoing, sharedKey);  
  strcat(outgoing, "\"}");
  
  publishToMQTT(string2Char(MQTT_API_GATEWAY_ATTRIBUTE_REQUEST + String(requestId)), outgoing);

  // Free memory
  vTaskDelay(1);
  free(outgoing);
}

// To update each node information
// Id is the order of node. Eg: 1, 2, 3...
// Node id is MAC-based id
void updateNodeInfo(byte id, const String& updatedNodeId)
{
  log("[PROC] Update Node Info, node[", string2Char(String(id)), 
    "], updated node id: ", string2Char(updatedNodeId));

  if(id >= SYS_MAXNODES)
  {
    log("[PROC] INVALID NODE ID.");
    return;
  }
  else if(updatedNodeId.length() != SYS_NODEID_LENGTH)
  {
    log("[PROC] INVALID LENGTH OF NODE ID.");
    return;
  }
  
  // Update to SYS_NODELIST
  strcpy(SYS_NODELIST[id], string2Char(updatedNodeId));

  // TODO: Update to flash
  // Save to flash

  // Update to portal
  publishDeviceConnect(SYS_NODELIST[id]);  
}

// To remove node from TB and stop polling
void removeNodeNodeInfo(byte id)
{
  log("[PROC] Remove Node Info, node[", string2Char(String(id)), 
    "], updated node id: ", SYS_NODELIST[id]);

  // Update to portal
  publishDeviceDisonnect(SYS_NODELIST[id]); 

  // Set SYS_NODELIST[id] to empty to stop polling
  SYS_NODELIST[id][0] = '\0';
}

// To display all nodes in the list
void displayNodeList()
{ 
  // Display node listing
  log("[PROC] Displaying node list...");
  
  for(int i=0; i < SYS_MAXNODES; i++)
  {  
    if(strlen(SYS_NODELIST[i]) > 0)
    {
      log("[PROC] Node[", string2Char(String(i)), "]: ", SYS_NODELIST[i]);
    }
  }  
}

// ==========================
// LoRa downlink commands
// ==========================
// Gateway to ask node to execute command via LoRa message
// "dst":"Device C","id":251,"method":"setP1","params":"false"
void sendDownlinkLoRaCommand(const char* deviceID, const String& request_id, const String& methodName, 
  const String& params)
{ 
  Serial.println("sendDownlinkLoRaCommand");
  logHeap();
  char *outgoing = (char *) malloc (256);
      
  // Gateway to ask node to execute command
  // "dst":"Device C","id":251,"method":"setP1","params":"false"
  strcpy(outgoing, R"=====("dst":")=====");
  strcat(outgoing, deviceID);
  strcat(outgoing, R"=====(","id":)=====");
  strcat(outgoing, string2Char(request_id));
  strcat(outgoing, R"=====(,"method":")=====");
  strcat(outgoing, string2Char(methodName));
  strcat(outgoing, R"=====(","params":")=====");
  strcat(outgoing, string2Char(params));
  strcat(outgoing, "\"");

  // Send LoRa command
  sendDownlinkLoRaMessage(deviceID, outgoing);
  
  // Free memory
  vTaskDelay(1);
  free(outgoing);
  
  logHeap();
}

// TODO: To change to char* and free after use
// Gateway to send LoRa request to LoRa node for telemetry data
// Message format: "dst":"Device C","id":251,"method":"poll","params":"tmt"
void pollTelemetryData(const char* deviceID)
{
  // To check memory heap
  log("[PROC] Started polling node: ", deviceID);
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
  vTaskDelay(1);
  free(outgoing);

  // To check memory heap
  log("[PROC] Ended polling node: ", deviceID);
  logHeap();
}
// ==========================
// Physical LoRa transmission
// ==========================
void sendDownlinkLoRaMessage(const char* deviceID, const char* outgoing)
{
  // Get customed frequencty for each device id
  long customedLoRaFreq = getCustomFreqForDeviceID(deviceID);

  // Send via LoRa 2
  sendDynamicLoRa2Message(customedLoRaFreq, outgoing); 
}
