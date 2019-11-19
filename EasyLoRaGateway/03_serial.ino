// ====================================
// Serial
// ====================================
void setupSerial() {
  Serial.begin(BAUD_RATE);
}

// Log free heap
void logHeap()
{
  log("esp_free_heap: ", string2Char(String(esp_get_free_heap_size())), 
      ", free_min_heap: ", string2Char(String(esp_get_minimum_free_heap_size())));
}

// Optimize log with char*
void log(const char* content)
{
  log(content, true);
}

// Optimize log with char*
void log(const char* content, bool isNewLine)
{
  Serial.print(content);
  if(isNewLine)
  {
    Serial.println();
  }
  
  if(isBTConnected)
  {
    SerialBT.print(content);
    if(isNewLine)
    {
      SerialBT.println();
    } 
  }
}

// Optimize log with char*
void log(const char* prefix1, const char* dynamicContent1)
{
  log(prefix1, false);
  log(dynamicContent1, true);
}

// Optimize log with char*
void log(const char* prefix1, const char* dynamicContent1, 
         const char* prefix2, const char* dynamicContent2)
{
  log(prefix1, false);
  log(dynamicContent1, false);
  log(prefix2, false);
  log(dynamicContent2, true);
}

// Optimize log with char*
void log(const char* prefix1, const char* dynamicContent1, 
         const char* prefix2, const char* dynamicContent2,
         const char* prefix3, const char* dynamicContent3)
{
  log(prefix1, false);
  log(dynamicContent1, false);
  log(prefix2, false);
  log(dynamicContent2, false);
  log(prefix3, false);
  log(dynamicContent3, true);
}

// Optimize log with char*
void log(const char* prefix1, const char* dynamicContent1, 
         const char* prefix2, const char* dynamicContent2,
         const char* prefix3, const char* dynamicContent3,
         const char* prefix4, const char* dynamicContent4)
{
  log(prefix1, false);
  log(dynamicContent1, false);
  log(prefix2, false);
  log(dynamicContent2, false);
  log(prefix3, false);
  log(dynamicContent3, false);
  log(prefix4, false);
  log(dynamicContent4, true);
}
