// ====================================
// PREFERENCES
// ====================================
// To save preferences to flash (without SD card)
// https://github.com/espressif/arduino-esp32/blob/master/libraries/Preferences/examples/StartCounter/StartCounter.ino
// http://www.iotsharing.com/2017/06/how-to-use-preferences-to-backup-data-in-main-flash-when-power-off.html?m=1
// https://github.com/espressif/arduino-esp32/blob/master/libraries/Preferences/src/Preferences.h
// http://www.iotsharing.com/2017/06/how-to-use-preferences-to-backup-data-in-main-flash-when-power-off.html?m=1

void setupPreferences() {
  /* Start a namespace "iotsharing"
  in Read-Write mode: set second parameter to false 
  Note: Namespace name is limited to 15 chars */
  preferences.begin("easylora", false);

  /* get value of key "reset_times", if key not exist return default value 0 in second argument
  Note: Key name is limited to 15 chars too */
  reset_times = preferences.getUInt("reset_times", 0);

  /* we have just reset ESP then increase reset_times */
  reset_times++;

  Serial.printf("Number of restart times: %d\n", reset_times);
  
  /* Store reset_times to the Preferences */
  preferences.putUInt("reset_times", reset_times);

  /* Close the Preferences */
  preferences.end();  
}

