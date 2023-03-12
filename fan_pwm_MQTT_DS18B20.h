
void setSpeed(uint8_t uiSpeed) {
  uiSpeed = 255 - uiSpeed; // circuitry is inverting, ensure 255 = max, 0 = off
  analogWrite(pwmGpio, uiSpeed);
}

void setValve (bool bState) {
  digitalWrite(valveGpio, bState);
}