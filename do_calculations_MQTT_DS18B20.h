void FanAutomat(float difftemp){
    // regulation of fan speed
  
  if (( ventState == false) 
  || ((numberOfDevices > 1) && (temp[1]< 30.0))) {

    if (pwmActual!= PWM_OFF){
#if 1 // SERIAL_TRACE      
      // Serial.printf("pwmActual(%02X) to off\n",pwmActual);
      Serial.print("pwmActual ");
      Serial.print( pwmActual);
      Serial.println(" to off");
#endif      
      pwmActual = PWM_OFF; // no heating support possible 
    }                   //  if heating water temperature is low, or vent is off
    return;
  }
  if (difftemp < (-temp_hyst * 2.0)){
    if (pwmActual != PWM_FULL){
#if 1 // SERIAL_TRACE      
      // Serial.printf("pwmActual(%02X) to FULL - difftemp %f, th*2.0=%f\n",pwmActual, difftemp, (-temp_hyst * 2.0));
      Serial.print("pwmActual ");
      Serial.print( pwmActual);
      Serial.println(" to FULL");
#endif      
      pwmActual = PWM_FULL;
    }
    return;
  }
  if (((pwmActual == PWM_FULL) && (difftemp > 0.0)) 
  || ((pwmActual != PWM_FULL) && (difftemp > -temp_hyst))){

    if (pwmActual!= throttleFanspeed) {
#if 1 // SERIAL_TRACE      
      // Serial.printf("pwmActual(%02X) to throttle\n",pwmActual);
      Serial.print("pwmActual ");
      Serial.print( pwmActual);
      Serial.println(" to THROTTLE");
#endif
      pwmActual = throttleFanspeed;
    }
    return; 
  }
    // no need to take care about difftemp > temp_hyst, 
    // since vent_state == false covers that
}


void VentAutomat(float difftemp){
  if (difftemp > temp_hyst){
    // Serial.print ("temp high - vent off");
    // upper fan off and vent off hysteresis point
    ventState = false;
    return;  
  }
  if (difftemp < -temp_hyst){
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
  float difftemp = temp[0] - desired_temp;
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
  Serial.print(numberOfDevices);
  Serial.print("):");
  Serial.print(temp[0]);
  if (numberOfDevices>1){
    Serial.print(" "); 
    Serial.print(temp[1]);
  }
  if (numberOfDevices>2){
    Serial.print(" "); 
    Serial.print(temp[2]);
  }
  Serial.print(" SollT ");
  Serial.print(desired_temp);
  Serial.print(" WiFi ");
  Serial.print(WiFi.isConnected());
  Serial.print(" MQTT ");
  Serial.print(mqttClient.connected());

  Serial.println(" ok");
  setSpeed(pwmActual);
  setValve(ventState);
  uint16_t help=pwmActual&0xFF;
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_ACTOR_PREFIX MQTT_PUB_FANACT_SUFFIX, 1, true, String(help).c_str());                            
  delay(10);
  packetIdPub1 = mqttClient.publish(MQTT_PUB_ACTOR_PREFIX MQTT_PUB_VALVE_SUFFIX, 1, true, ventState ? "1" : "0");                            
  delay(10);

}

void wifiStatus()
{
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP_PREFIX "WIFI/RSSI", 1, true, String(WiFi.RSSI()).c_str());
  delay(10);
}