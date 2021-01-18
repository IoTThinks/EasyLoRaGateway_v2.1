void setupButton()
{
  pinMode(BTN, INPUT_PULLUP);
  // LOW, HIGH, CHANGE, RISING, FALLING
  // attachInterrupt(digitalPinToInterrupt(BTN), buttonPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(BTN), buttonPressed, FALLING);
  // Serial.println("[BUTTON] Setup Button. Done.");
}

void testButton()
{
  Serial.println("[BUTTON] Please PRESS button...");
  delay(5000);
}

// In ISR context
// Can not have any log() or delay in this method
void buttonPressed()          
{
  Serial.println("[BUTTON] Button is pressed. PASSED.");
}
