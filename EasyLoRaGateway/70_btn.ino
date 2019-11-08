void setupButton()
{
  log("[BTN] Setup button");
  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), buttonPressed, RISING);
}

void buttonPressed()          
{                   
   log("[BTN] Button is pressed");
   isBtnPressed = true;
}

void performUserAction()
{
  // Sound some alarm
  if(isBtnPressed)
  {
     log("[BTN] Perform user action");
    isBtnPressed = false;
    onOffSpeaker(1, true); // OK
    blinkOnLED();    
  }
}
