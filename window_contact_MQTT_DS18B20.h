bool bWindowClosed=true;
bool bWindowOldClosed=false;
uint8_t uiOverrideWindowSensor=1;

/** attention! the approach to use a pull down reed switch, 
 that is closed, when magnet is in range, requires to 
use a shortcut jumper, when there is no reed window switch

@todo config parameter for available reed switch?

 */
void GetWindowState(){
  Serial.print("WinState: Override: ");
  Serial.print(uiOverrideWindowSensor);
  Serial.print(", Port (0=WindowClosed): ");
  Serial.println(digitalRead(windowContact));
  bWindowClosed = ((uiOverrideWindowSensor != 0) || (digitalRead(windowContact) == 0));
  if (bWindowOldClosed != bWindowClosed) {
    uint16_t packetIdPub1 = mqttClient.publish((MQTT_PUB_TEMP_PREFIX + "WindowsSensor").c_str(), 1, true, String(bWindowClosed).c_str());
    bWindowOldClosed = bWindowClosed;
  }

}
