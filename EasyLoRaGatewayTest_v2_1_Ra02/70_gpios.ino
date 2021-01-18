// ====================================
// GPIOS
// ====================================
void setupGPIOs() {
  pinMode(IO2, OUTPUT);
  pinMode(IO4, OUTPUT);
  Serial.println("[GPIO] Setup GPIOs. Done.");
}

void testGPIOs() {  
  // On  
  Serial.println("[GPIO] Check if GPIO 2 is in 3.3 volt?");
  digitalWrite(IO2, HIGH);
  delay(5000);
  Serial.println("[GPIO] Check if GPIO 4 is in 3.3 volt?");
  digitalWrite(IO4, HIGH);  
  delay(5000);
  
  // Off  
  Serial.println("[GPIO] Check if GPIO 2 is in 0 volt?");
  digitalWrite(IO2, LOW);
  delay(5000);
  Serial.println("[GPIO] Check if GPIO 4 is in 0 volt?");
  digitalWrite(IO4, LOW);  
  delay(5000);  
}
