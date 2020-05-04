/*================================================================*/
/*---------------------------    TIMER    ------------------------*/
/*================================================================*/
/*----------------------------------------------------------------*/
void initTemporizador(){
  #if defined (_VARIANT_ARDUINO_DUE_X_)
    Timer1.attachInterrupt(isrTimer1).setFrequency(1);
    Timer1.start();
  #else
    Timer1.initialize(100000);  
    Timer1.attachInterrupt(isrTimer1); 
  #endif
}
/*----------------------------------------------------------------*/
// Temporizador de 1000 ms.
void isrTimer1(void){   
  flagVerificar=true;
  // Verificamos si el HC06 sigue activo
  if(enableDebug){
    _debug_time++;
    if(_debug_time>=MAX_TIME_DEBUG){
      enableDebug = false;
      _debug_time = 0;
      Serial.println("DEBUG-OFF");
    }
  }
}



