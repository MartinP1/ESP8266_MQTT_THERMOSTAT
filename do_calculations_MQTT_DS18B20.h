void FanAutomat(float difftemp){
  // regulation of fan speed
  
  // first step check for off conditions
  if (( ventState == false) 
  || ((numberOfDevices > 1) && (temp[VORLAUF]< MIN_VORLAUF_FOR_FAN))
  || (difftemp>=temp_hyst)) {

    if (pwmActual!= PWM_OFF){
#if 1 // SERIAL_TRACE      
      // Serial.printf("pwmActual(%02X) to off\n",pwmActual);
      Serial.print("pwmActual ");
      Serial.print( pwmActual);
      Serial.println(" to off");
#endif      
      pwmActual = PWM_OFF; // no heating support possible/necessary 
    }                   //  if heating water temperature is low, 
                        // or vent is off
                        // or room temperature is above hysteresis
    return; // done
  }
  // at this point definitvely something different to fans off
  if (difftemp <= -temp_hyst) {
    if (pwmActual != PWM_FULL){
  /// temperature seriously too low -> turn fan to full speed
#if 1 // SERIAL_TRACE      
      // Serial.printf("pwmActual(%02X) to FULL - difftemp %f, th*2.0=%f\n",pwmActual, difftemp, (-temp_hyst * 2.0));
      Serial.print("pwmActual ");
      Serial.print( pwmActual);
      Serial.println(" to FULL");
#endif      
      pwmActual = PWM_FULL;
    }
    return; // done 
  }
  // at this point neither full speed nor off
  if ((difftemp >= 0.0)){
    // throttling to absolute minimum on room temp equal or above desired temp
    if (pwmActual!= PWM_THROTTLE) {
#if 1 // SERIAL_TRACE      
      // Serial.printf("pwmActual(%02X) to throttle\n",pwmActual);
      Serial.print("pwmActual ");
      Serial.print( pwmActual);
      Serial.println(" to THROTTLE");
#endif
      pwmActual = PWM_THROTTLE;
    }
    return; // done 
  }
  // at this point we are definitively in range between (-temp_hyst and 0) -> adaptive throttling
  // fan speed shall be PWM_FULL at difftemp =  -temp_hyst, and PWM_THROTTLE at difftemp=0
  
  // use a normalized control deviation to help to make it easier to use more sophisticated algorithms 
  // later on
  float normdev = difftemp / temp_hyst; // in range -1.0 (at difftemp=-temp_hyst).... 0.0 (difftemp=0.0)
  
  // some safety checks not necessary, but to be safe ....
  if (normdev > 0.0)
    pwmActual = PWM_THROTTLE;
  else if ( normdev <= -1.0)
    pwmActual = PWM_FULL;
  else {
    // simple proportional controller
    // avoid signed/unsigned issues by strange looking arithmetics
    pwmActual = PWM_THROTTLE + (uint8_t)((-normdev) * (float)(PWM_FULL - PWM_THROTTLE)); 
  }                                                                                 
  Serial.print(" pwmActual ");
  Serial.print(pwmActual);
  Serial.println(" (regulator range)");
  
}

void VentAutomat(float difftemp){
 
  if ((difftemp > temp_hyst)||(bWindowClosed==false)){
    // Serial.print ("temp high - vent off");
    // upper fan off and vent off hysteresis point
    ventState = false;
    return;  
  }
  if (difftemp < 0.0){
    // Serial.print ("temp low - vent on");
    ventState = true;
    return;  
  }  
}

/**
@brief use available input values to calculate new output values

numberOfDevices -> number of temp sensors

input values from global variables 
Temp Sensor 0 -> room temperature (temp[0])
Temp Sensor 1 -> heater inlet temp (temp[1] - optional sensor!)
Temp Sensor 2 -> heater return temp (temp[2] - optional sensor!)

desired Temperature (desired_temp, hyst_temp)
maximum fan speed (pwmSet)
Window contact state (future)

output values

uint8_t pwmActual PWM value 
bool ventState;
*/
void runTempControl()
{
  

  if (numberOfDevices < 1)
    return; // no measurements, cant do anything
  float difftemp = temp[RAUM] - desired_temp;
  VentAutomat(difftemp);
  FanAutomat(difftemp);
  Serial.print("Difftemp:");
  Serial.print(difftemp);
  Serial.print(" Hyst:");
  Serial.print(temp_hyst);
  Serial.print(" ventState:"); 
  Serial.print(ventState);
  Serial.print(" pwmActual:");
  Serial.print(String(pwmActual).c_str());
  // Serial.print(" Adresses: ");
  // for (int i=0; i<numberOfDevices; i++){
  //  printAddress(statDeviceAddress[i]);
  // }
  Serial.print(" Tmprtrs(");
  Serial.print(temp[RAUM]);
  if (numberOfDevices>RAUM){
    Serial.print(" "); 
    Serial.print(temp[VORLAUF]);
  }
  if (numberOfDevices>VORLAUF){
    Serial.print(" "); 
    Serial.print(temp[RUECKLAUF]);
  }
  Serial.print(" SollT ");
  Serial.print(desired_temp);
  Serial.print(" WiFi ");
  Serial.print(WiFi.isConnected());
  Serial.print(" MQTT ");
  Serial.print(mqttClient.connected());
  Serial.print(" WindowClosed ");
  Serial.print(bWindowClosed);
  Serial.println(" ok");
  setSpeed(pwmActual);
  setValve(ventState);
  uint16_t help=pwmActual&0xFF;
  if (mqttClient.connected()) {
    uint16_t packetIdPub1 = mqttClient.publish((MQTT_PUB_ACTOR_PREFIX + MQTT_PUB_FANACT_SUFFIX).c_str(), 1, true, String(help).c_str());                            
    delay(10);
    packetIdPub1 = mqttClient.publish((MQTT_PUB_ACTOR_PREFIX + MQTT_PUB_VALVE_SUFFIX).c_str(), 1, true, ventState ? "1" : "0");                            
    delay(10);
  }

}

bool wifiStatus()
{
  if (WiFi.isConnected()) {
    if (mqttClient.connected()) {
      uint16_t packetIdPub1;
      if (uiWifiDisconnects != uiWifiDisconnectsOld) {
        packetIdPub1 = mqttClient.publish((MQTT_PUB_TEMP_PREFIX + "WIFI/Disconnects").c_str(), 1, true, String(uiWifiDisconnects).c_str());
        uiWifiDisconnectsOld = uiWifiDisconnects;
        delay(10);
      }
      packetIdPub1 = mqttClient.publish((MQTT_PUB_TEMP_PREFIX + "WIFI/RSSI").c_str(), 1, true, String(WiFi.RSSI()).c_str());

      return true; // both up and running
    } else {
      connectToMqtt();
      return false; // mqtt not running
    }

  } else {
    connectToWifi();
    return false; // wifi (and mqtt) not running
  }
}