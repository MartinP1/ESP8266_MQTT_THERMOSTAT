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

  if (difftemp > temp_hyst)
  {
    pwmActual = PWM_OFF; // fan off
    ventState = false;
  }
  else if ((difftemp < (-temp_hyst)) && (difftemp > (-2*temp_hyst)))
  {
    pwmActual = pwmSet;
    ventState = true;
  }




}