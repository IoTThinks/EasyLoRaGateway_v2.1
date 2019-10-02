// =====================
// ThingsBoard
// =====================
  
void uploadTelemetryData(String src, String telemetry)
{
  WiFiClient client;  
  if (!client.connect(TB_HOST, TB_PORT)) {
    Serial.println("[TB] Connection to ThingsBoard failed");
    return;
  }

  Serial.print("[TB] Sending POST request. ");
  
  // Send request to the server with "src" as secret key and "telemetry" as data
  client.println("POST /api/v1/" + src + "/telemetry HTTP/1.1");
  client.println("Host: " + String(TB_HOST));
  client.println("Accept: */*");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(telemetry.length());
  client.println();
  client.print(telemetry);
  
  delay(500); // Can be changed
  
  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }
  Serial.println();
  Serial.println("[TB] Closed connection to ThingsBoard");
  delay(5000);
}
