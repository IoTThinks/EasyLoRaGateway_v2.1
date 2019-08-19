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
void onSpeaker()
{
  Serial.println("[SPK] On");
  ledcWrite(SPK_CHANNEL, SPK_FREQ);
}

// Off Speaker
void offSpeaker()
{
  Serial.println("[SPK] Off");
  ledcWrite(SPK_CHANNEL, 0);
}

// On and off speaker multiple times for different sound codes
void onOffSpeaker(int times, bool isLongBeep)
{
  // Long beep in 200ms and short beep in 50ms
  int soundTime = isLongBeep? 100 : 50; 
    
  for(int i=0; i< times; i++)
  {
    onSpeaker();
    delay(soundTime);
    offSpeaker();  
    delay(soundTime);
  }
}

