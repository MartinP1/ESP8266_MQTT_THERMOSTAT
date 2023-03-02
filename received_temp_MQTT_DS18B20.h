#define MAX_DS18B20_DEVICES 3

DeviceAddress statDeviceAddress[3]; 
int numberOfDevices;
// to provide functionality with only one sensor, make "raum" the first one!
// later we will sort the sensors in ascending sequence
const char* TempsensRole[3] {"raum",  "vorlauf", "ruecklauf"};



void getTemperatures() {
   sensors.requestTemperatures(); 
    // Temperature in Celsius degrees
    for (int i=0; i<numberOfDevices; i++)
    {
      float temp = sensors.getTempC(statDeviceAddress[i]);
      // Temperature in Fahrenheit degrees
      //temp = sensors.getTempFByIndex(0);
    
      // Publish an MQTT message on topic esp/ds18b20/temperature
      String Topic(MQTT_PUB_TEMP_PREFIX);
      Topic += TempsensRole[i];
      Topic += String(MQTT_PUB_TEMP_SUFFIX);
      uint16_t packetIdPub1 = mqttClient.publish(Topic.c_str(), 1, true, String(temp).c_str());                            
      Serial.printf("Pubng on topic %s at QoS 1, packetId: %i ", Topic.c_str(), packetIdPub1);
      Serial.printf("Msg: %.2f \n", temp);
    }
}

// function to print a device address
// 0x28 family code is printed first!
// https://cdn.sparkfun.com/datasheets/Sensors/Temp/DS18B20.pdf
// Last byte of adress is transferred first ...
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}

void swapDevAdr(int left, int right)
{
  if (left>=numberOfDevices)
    return;
  if (right>=numberOfDevices)
    return;
  for (int i=0; i<8; i++){
    int8_t iSwap = statDeviceAddress[left][i];
    statDeviceAddress[left][i] = statDeviceAddress[right][i];
    statDeviceAddress[right][i] = iSwap;
  }
}

bool isDevAdrGreater(int left, int right){
  // omit family and checksum byte
  if (left>=numberOfDevices)
    return false;
  if (right>=numberOfDevices)
    return false;
  for (int i=1; i<7; i++){
    if (statDeviceAddress[left][i]>statDeviceAddress[right][i]){
      return true;
    } else if (statDeviceAddress[left][i]<statDeviceAddress[right][i]){
      return false;
    }
  }
  return false; /// @todo manage duplicates (equality, when reaching this point)
} 





void initTemperatureSensors(){
  Serial.print("Locating devices...");
  numberOfDevices = sensors.getDeviceCount();
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  if(numberOfDevices > MAX_DS18B20_DEVICES){
    Serial.println("too many temp devices, limit to 3");
    numberOfDevices = MAX_DS18B20_DEVICES;
  }

  if ((numberOfDevices>1) && isDevAdrGreater(0, 1)){
    swapDevAdr(0, 1);
  }
  if ((numberOfDevices>2) && isDevAdrGreater(1, 2)) {
    swapDevAdr(1, 2);
    if (isDevAdrGreater(0, 1)){
      swapDevAdr(0, 1);
    }
      
  }



  
// Found device 0 with address: 28B8D281E3B53CF0

  // 
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(sensors.getAddress(statDeviceAddress[i], i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(statDeviceAddress[i]);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }

}