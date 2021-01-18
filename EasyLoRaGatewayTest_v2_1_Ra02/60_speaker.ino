// https://techtutorialsx.com/2017/07/01/esp32-arduino-controlling-a-buzzer-with-pwm/
// Setup speakers with pin and settings
void setupSpeaker()
{
  // speaker
  ledcSetup(SPK_CHANNEL, SPK_FREQ, SPK_RESOLUTION);
  ledcAttachPin(SPK, SPK_CHANNEL);
  ledcWrite(SPK_CHANNEL, 0);
}

// On speaker
void testSpeaker()
{
  Serial.println("[SPK] On and off speaker...");
  ledcWrite(SPK_CHANNEL, SPK_FREQ);
  delay(50);
  ledcWrite(SPK_CHANNEL, 0);
  delay(50);
}
