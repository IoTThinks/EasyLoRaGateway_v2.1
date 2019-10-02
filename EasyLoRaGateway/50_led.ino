// ====================================
// LED
// ====================================
// LED is active high
// So pin set to high will on the LED
String LED_Status = "Not initalized";

void setupLED() {
  pinMode(LED, OUTPUT);
  // Default to off
  digitalWrite(LED, LOW); 
  LED_Status = "OK";
}

void onLED() {
  digitalWrite(LED, HIGH);
  delay(50);
}

void offLED() {  
  digitalWrite(LED, LOW);
  delay(50);
}

void blinkOffLED() {
  onLED();
  offLED();
}

void blinkOnLED() {
  offLED();
  onLED();  
}
