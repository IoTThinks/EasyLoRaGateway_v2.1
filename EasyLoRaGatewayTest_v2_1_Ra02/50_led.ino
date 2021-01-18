// ====================================
// LED
// ====================================

void setupLED() {
  Serial.println("[LED] Setup LED");
  pinMode(LED, OUTPUT);
}

void testLED() {
  Serial.println("[LED] On and off LED...");
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
