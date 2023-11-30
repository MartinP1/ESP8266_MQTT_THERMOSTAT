
void connectToWifi() {
  Serial.print("Connecting to Wi-Fi...");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}


void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}


#ifdef ARDUINO_D1_MINI32
// ARDUINO_EVENT_WIFI_STA_GOT_IP
// 
// ARDUINO_EVENT_WIFI_STA_DISCONNECTED
void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("Connected to Wi-Fi - ");
  Serial.print(" Signal strength:");
  Serial.println(WiFi.RSSI());
  connectToMqtt();
}

void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}
#else
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.print("Connected to Wi-Fi - ");
  Serial.print(" Signal strength:");
  Serial.println(WiFi.RSSI());
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}
#endif


