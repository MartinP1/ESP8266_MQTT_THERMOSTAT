bool bWindowOpen=false;

void GetWindowState(){
  bWindowOpen = ( digitalRead(windowContact) == 0);
}
