

void DumpFreeRAM(){ 
  Serial.print(" free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.print("/");
  Serial.print(ESP.getHeapSize());
  Serial.print(" min free heap: ");
  Serial.print(ESP.getMinFreeHeap());
  Serial.print(" free PSRAM: ");
  Serial.println(ESP.getFreePsram());
} 



void connectToWifi() {
  Serial.print("Client ");
  Serial.print(MQTT_PUB_DEV_PREFIX);
  Serial.print(" Connecting to Wi-Fi...");
  Serial.println(WIFI_SSID);
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}


void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}


#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
// ARDUINO_EVENT_WIFI_STA_GOT_IP
// 
// ARDUINO_EVENT_WIFI_STA_DISCONNECTED
void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("Connected to Wi-Fi - ");
  Serial.print(" Signal strength:");
  Serial.println(WiFi.RSSI());
  //DumpFreeRAM();
  connectToMqtt();
}

void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from Wi-Fi.");
  // mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  // wifiReconnectTimer.once(2, connectToWifi);
  uiWifiDisconnects++;
}
#else
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.print("Connected to Wi-Fi - ");
  Serial.print(" Signal strength:");
  Serial.println(WiFi.RSSI());
  // connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  // mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  // wifiReconnectTimer.once(2, connectToWifi);
}
#endif


