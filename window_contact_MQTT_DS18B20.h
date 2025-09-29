bool bWindowClosed=true;
uint8_t uiOverrideWindowSensor=1;

/** attention! the approach to use a pull down reed switch, 
 that is closed, when magnet is in range, requires to 
use a shortcut jumper, when there is no reed window switch

@todo config parameter for available reed switch?

 */
void GetWindowState(){
  bWindowClosed = ((uiOverrideWindowSensor != 0) || (digitalRead(windowContact) == 0)|( digitalRead(windowContact) == 0));
}
