 

#define MAX_DS18B20_DEVICES 3
void printAddress(DeviceAddress deviceAddress);
DeviceAddress statDeviceAddress[MAX_DS18B20_DEVICES]={ {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
                                                      {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
                                                      {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} }; 
bool is_old_valid[MAX_DS18B20_DEVICES] = { true, true, true};
bool is_valid[MAX_DS18B20_DEVICES] = { false, false, false};
int numberOfDevices;
// to provide functionality with only one sensor, make "raum" the first one!
// later we will sort the sensors in ascending sequence
const char* TempsensRole[MAX_DS18B20_DEVICES] {"raum",  "vorlauf", "ruecklauf"};
float temp[MAX_DS18B20_DEVICES];
#define RAUM 0
#define VORLAUF 1
#define RUECKLAUF 2

#define RT_AVG_PERIOD 6


/// @brief publish sensor state, invoked on initialization and sensor state change
/// @param iSensId the ID of Sensor
void publishSensorState(int iSensId) {
  if (iSensId>MAX_DS18B20_DEVICES) return; // range error!
  String Topic(MQTT_PUB_TEMP_PREFIX);
  Topic += TempsensRole[iSensId];
  Topic += String(MQTT_PUB_TEMP_SENAV_SUFFIX);
  if (mqttClient.connected()) {
    uint16_t packetIdPub1 = mqttClient.publish(Topic.c_str(), 1, true, is_valid[iSensId] ? "1" : "0");
    delay(10);                            
  }
  Serial.print("Published Sensor state: "); 
  Serial.print(Topic.c_str());
  Serial.print(", State=");
  Serial.println( is_valid[iSensId] );

}



/**
@todo, this function is a baustelle ;-)
*/
void avgRoomtemp() {
  static bool bMeasRoundCompleted=false;
  static float RoomtempArray[RT_AVG_PERIOD] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  static uint8_t dt_index = 0;
  RoomtempArray[dt_index++] = temp[0];
  if (dt_index==RT_AVG_PERIOD)
  { 
    bMeasRoundCompleted=true;
    dt_index=0;
  }
  if (bMeasRoundCompleted) {
    uint8_t lidx=dt_index;
    float avgtemp = 0.0;
    // avg filter over some measurements....
    do{
      avgtemp += RoomtempArray[lidx];
      lidx++;
      if (lidx==RT_AVG_PERIOD)
        lidx=0;
    }while(lidx!=dt_index);

    // Serial.print ("RoomTemp before avg: ");
    // Serial.print (temp[0]);
    temp[0] = avgtemp / RT_AVG_PERIOD;
    // Serial.print (" RoomTemp after avg: ");
    // Serial.println (temp[0]);
    
  }

}


void getTemperatures() {
  sensors.requestTemperatures(); 
    // Temperature in Celsius degrees
  Serial.print("DS18B20s:");
  for (int i=0; i<MAX_DS18B20_DEVICES; i++) {
    is_valid[i]=false;
    if (statDeviceAddress[i][6]!=0xFF) {
      float fTemp = sensors.getTempC(statDeviceAddress[i]);
      if (fTemp > DEVICE_DISCONNECTED_C) {
        temp[i] = fTemp;
        is_valid[i]=true;
      }
    }
    Serial.print(" ");
    printAddress(statDeviceAddress[i]);
    if (is_valid[i])
      Serial.print("(good");
    else
      Serial.print("(bad");
      
    if (is_valid[i] != is_old_valid[i]) {
      Serial.print("*) ");
      publishSensorState(i);
      is_old_valid[i] = is_valid[i];
#if 1        
      MQTTLogPrintf("Thermosensor[%d] %s (%02X%02X%02X%02X%02X%02X)", 
        i, 
        is_valid[i] ? "regained" : "lost",
        // statDeviceAddress[i][7], // omit crc
        statDeviceAddress[i][6], // begin serial number
        statDeviceAddress[i][5],
        statDeviceAddress[i][4],
        statDeviceAddress[i][3],
        statDeviceAddress[i][2],
        statDeviceAddress[i][1]);
        // statDeviceAddress[i][0]);  // omit family code 0x28
#endif     
    }
    else
      Serial.print(") ");
    // Temperature in Fahrenheit degrees
    //temp = sensors.getTempFByIndex(0);
    if (i==0)
    {
      avgRoomtemp(); // no respect to valid not valid....
    }
    // Publish an MQTT message on topic esp/ds18b20/temperature
    String Topic(MQTT_PUB_TEMP_PREFIX);
    Topic += TempsensRole[i];
    Topic += String(MQTT_PUB_TEMP_SUFFIX);
    if (is_valid[i] && (mqttClient.connected())) {
      uint16_t packetIdPub1 = mqttClient.publish(Topic.c_str(), 1, true, String(temp[i]).c_str());
      delay(10);                            
    }
    // Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", Topic.c_str(), packetIdPub1);
    // Serial.printf("Msg: %.2f \n", temp[i]);
  }
  Serial.println(" ");
  // DumpFreeRAM();
  DumpSysInfo();
}

// function to print a device address
// 0x28 family code is printed first!
// https://cdn.sparkfun.com/datasheets/Sensors/Temp/DS18B20.pdf
// Last byte of adress is transferred first ...

#if 0
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
#else
// print only bytes of interest (omit family code and crc) and in correct order
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 6; i > 0; i--) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
  Serial.print(" ");
}
#if 0
void printAddress2Buffer(DeviceAddress deviceAddress, char* buffer) {
  for (uint8_t i = 6; i > 0; i--) {
    if (deviceAddress[i] < 16) strResult->;
      Serial.print(deviceAddress[i], HEX);
  }
  Serial.print(" ");
}
#endif
#endif


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
#if SERIAL_TRACE  
  Serial.print (left);
  Serial.print (" <-> ");
  Serial.print (right);
  Serial.println ("swapped");    
#endif
}

bool isDevAdrGreater(int left, int right){
  // omit family and checksum bytepublishDesSpeed
  if (left>=numberOfDevices)
    return false;
  if (right>=numberOfDevices)
    return false;
  for (int i=6; i>0; i--){
    if (statDeviceAddress[left][i]<statDeviceAddress[right][i]){
      return false;
    } else if (statDeviceAddress[left][i]>statDeviceAddress[right][i]){
      return true;
    }
  }
  return false; /// @todo manage duplicates (equality, when reaching this point)
} 

void initTemperatureSensors(){
  Serial.print("Locating devices...");
  numberOfDevices = sensors.getDeviceCount();
  // numberOfDevices = sensors.getDS18Count();
  // numberOfDevices = MAX_DS18B20_DEVICES;
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  if(numberOfDevices > MAX_DS18B20_DEVICES){
    Serial.println("too many temp devices, limit to 3");
    numberOfDevices = MAX_DS18B20_DEVICES;
  }
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

   

  Serial.println(" swap devices ...."); 
  if ((numberOfDevices>1) && isDevAdrGreater(0, 1)){
    swapDevAdr(0, 1);
  }
  if ((numberOfDevices>2) && isDevAdrGreater(1, 2)) {
    swapDevAdr(1, 2);
    if (isDevAdrGreater(0, 1)){
      swapDevAdr(0, 1);
    }
      
  }
  for(int i=0;i<numberOfDevices; i++){
      printAddress(statDeviceAddress[i]);
      Serial.println();
  }

}