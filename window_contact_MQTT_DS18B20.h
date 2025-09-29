bool bWindowOpen=false;

void GetWindowState(){
  bWindowOpen = ( DigitalRead(windowContact) == 0);
}
