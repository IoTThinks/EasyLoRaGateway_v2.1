char* string2Char(String str){
    if(str.length()!=0){
        char *p = const_cast<char*>(str.c_str());
        return p;
    }
}

// Mac to string without ":"
String macToStr(const uint8_t* mac, bool isWithColon)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
  
  if (isWithColon && i < 5)
    result += ':';
  }
  
  return result;
}

String getHostname()
{
  uint8_t mac[6];
   esp_err_t err = esp_read_mac(mac, ESP_MAC_ETH);
   return "EG-" + macToStr(mac, false);
}
