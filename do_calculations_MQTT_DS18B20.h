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
  static float fan_k0, fan_delta;
  if (new_hyst || new_throttle)
  {
    new_hyst = false;
    new_throttle = false;
    fan_k0 = ((float)PWM_FULL + (2.0*(float)throttleFanspeed))/3.0;
    fan_delta = ((float)throttleFanspeed - (float)PWM_FULL)/(3.0 * temp_hyst); // negative slope!
  }


  if (numberOfDevices < 1)
    return; // no measurements, cant do anything
  float difftemp = temp[0] - desired_temp;
  
  if (difftemp > temp_hyst){
    Serial.print ("temp high ");
    // upper fan off and vent off hysteresis point
    pwmActual = PWM_OFF; // fan off
    ventState = false;  
  }
  else {
    // ventile, quite simple
    if (difftemp < (-temp_hyst)){
      Serial.print ("temp low ");
      ventState = true;  
    }
    // regulation of fan speed
    if ((numberOfDevices > 1) && (temp[1]< 30.0))
    {
      pwmActual = PWM_OFF; // no sense if water temperature is low
    }
    else if (difftemp < (-temp_hyst * 2.0)){
      pwmActual = PWM_FULL;
    }
    else{
      float pat = fan_k0 + (fan_delta * difftemp);
      Serial.print("pwm_over (float): ");
      Serial.print(pat);
      pwmActual = (int8_t)(pat);
    }
       
  }
  Serial.print("Difftemp: ");
  Serial.print(difftemp);
  Serial.print(" Hyst:");
  Serial.print(temp_hyst);
  Serial.print(" ventState: ");
  Serial.print(ventState);
  Serial.print(" pwmActual: ");
  Serial.print(String(pwmActual).c_str());
  Serial.print(" Adresses: ");
  for (int i=0; i<numberOfDevices; i++){
    printAddress(statDeviceAddress[i]);
  }
  Serial.print("\nRaumtemperatur: "); 
  Serial.print(temp[0]);
  Serial.print(" Solltemperatur:");
  Serial.print(desired_temp);
  Serial.println(" ok");
  setSpeed(pwmActual);
  setValve(ventState);
  uint16_t help=pwmActual&0xFF;
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_ACTOR_PREFIX MQTT_PUB_FANACT_SUFFIX, 1, true, String(help).c_str());                            
  packetIdPub1 = mqttClient.publish(MQTT_PUB_ACTOR_PREFIX MQTT_PUB_VALVE_SUFFIX, 1, true, ventState ? "1" : "0");                            


}