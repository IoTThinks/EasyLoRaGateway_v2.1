// ====================================
// LED
// ====================================

void setupLED() {
  // Serial.println("[LED] Setup LED");
  pinMode(LED, OUTPUT);

  // On led by default
  blinkLEDOn();
}

void testLED() {
  Serial.println("[LED] Check if LED is off and on?...");
  
  // One
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);

  // Two
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);

  // Three
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
}

void blinkLEDOn()
{
   digitalWrite(LED, LOW);
   delay(500);
   digitalWrite(LED, HIGH);
   delay(500);    
}
