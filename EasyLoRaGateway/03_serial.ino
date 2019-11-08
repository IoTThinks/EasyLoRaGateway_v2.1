// ====================================
// Serial
// ====================================
void setupSerial() {
  Serial.begin(BAUD_RATE);
}

// If Bluetooth client is connected, print debug log to BlueTooth and serial
void log(const String& message)
{
  Serial.println(message);
  if(isBTConnected)
  {
    SerialBT.println(message);
  }
}
