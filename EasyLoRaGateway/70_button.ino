void setupButton()
{
  Serial.println("[BTN] Setup button");
  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), buttonPressed, RISING);
}

void buttonPressed()          
{                   
   Serial.println("[BTN] Button is pressed");
   isBtnPressed = true;
}

void performUserAction()
{
  // Sound some alarm
  if(isBtnPressed)
  {
     Serial.println("[BTN] Perform user action");
    isBtnPressed = false;
    onOffSpeaker(1, true); // OK
    blinkOnLED();    
  }
}
