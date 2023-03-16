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

  if (difftemp > temp_hyst){
    Serial.print ("temp high ");
    pwmActual = PWM_OFF; // fan off
    ventState = false;
  }
  else if ((difftemp < (-temp_hyst)) && (difftemp > (-2*temp_hyst)))  { // temperature is not very low
    Serial.print ("temp slightly low ");
    // hysteresis to avoid fast toggling between full and throttle
    if (pwmActual != PWM_FULL){
       pwmActual = throttleFanspeed; // no full fan power in case of sinking temp caused fan on
                                     // on rising temperature preserve full fan speed 'til inside 2*hysteresis
    }
    ventState = true;
  }
  else if (difftemp <= (-2* temp_hyst)) { // temperature is low
    Serial.print ("temp significant low ");
    ventState = true;
    if ((numberOfDevices<2) || (temp[1]>30.0)) // possible to measure inlet temperature?
     pwmActual = PWM_FULL;
    else
       pwmActual = throttleFanspeed;
  }
  else {
    Serial.print ("inside 2*hysteresis throttle fan ");
    if (ventState) // do not change vent state
    {
      pwmActual = throttleFanspeed;
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