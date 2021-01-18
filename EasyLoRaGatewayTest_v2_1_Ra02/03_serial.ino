// ====================================
// Serial
// ====================================
void setupSerial() {
  Serial.begin(BAUD_RATE);
  Serial.println("[SERIAL] Setup Serial. Done.");
}

void testSerial() {
  Serial.println("[Serial] If you see this, serial is PASSED.");
}
