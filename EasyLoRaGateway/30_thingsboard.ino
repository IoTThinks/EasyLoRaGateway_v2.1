int temp = 1;
int humid = 1;
  
void uploadTelemetryData()
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("portal.iotthinks.com", httpPort)) {
    Serial.println("connection failed");
    return;
  }

  temp++;
  humid++;
  String data = "{temp:" + (String) temp + ", humid:" + (String) humid + "}";

  Serial.print("Requesting POST: ");
  // Send request to the server:
  client.println("POST /api/v1/A1_TEST_TOKEN/telemetry HTTP/1.1");
  client.println("Host: portal.iotthinks.com");
  client.println("Accept: */*");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println();
  client.print(data);
  
  delay(500); // Can be changed
  
  if (client.connected()) { 
    client.stop();  // DISCONNECT FROM THE SERVER
  }
  Serial.println();
  Serial.println("closing connection");
  delay(5000);
}
