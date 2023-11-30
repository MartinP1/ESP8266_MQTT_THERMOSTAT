
void setSpeed(uint8_t uiSpeed) {
  uiSpeed = 255 - uiSpeed; // circuitry is inverting, ensure 255 = max, 0 = off
#ifdef ARDUINO_D1_MINI32
  ledcWrite( 0, uiSpeed);
#else
  analogWrite(pwmGpio, uiSpeed);
#endif
}

void setValve (bool bState) {
  digitalWrite(valveGpio, bState);
}