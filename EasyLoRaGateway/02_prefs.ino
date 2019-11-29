// ====================================
// PREFERENCES
// ====================================
// To save preferences to flash (without SD card)
// https://github.com/espressif/arduino-esp32/blob/master/libraries/Preferences/examples/StartCounter/StartCounter.ino

void setupPreference() {
  // Read-Write mode: set second parameter to false 
  // Note: Namespace name is limited to 15 chars
  // Open Preferences in on-demand get/set only?
}

void beginPreference(bool isReadonly)
{
  preferences.begin("easylora", isReadonly);
}

void setPreference(const char* key, const char* value)
{
  // Open Preferences in Read-Write mode
  beginPreference(false);
  
  String savedPrefs = preferences.getString(key, "");

  // If not exist saved value 
  // or saved value and to-save value is different
  // Then save new value
  if(savedPrefs == "" || strcmp(value, string2Char(savedPrefs)) != 0 )
  {
    preferences.putString(key, value);
    log("[Prefs] Preference saved. Key : ", key, " , value: ", value);
  }
  else
  {
    log("[Prefs] Skip saving same preference value. Key : ", key, " , value: ", value);
  }
  
  // Close Preferences
  closePreference();
}

String getPreference(const char* key)
{
  // Open Preferences in readonly mode
  beginPreference(true);
  String savedPrefs = preferences.getString(key, "");

  // Close Preferences
  closePreference();
  return savedPrefs;
}

// Close the Preferences
void closePreference()
{
  // To close after every set and get preference?
  // TODO: Is this thread-safe?
  preferences.end();  
}
