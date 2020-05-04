//=================================================================
// 0.- Variables para el proceso de Temporizacion
//=================================================================
unsigned long cont_millis=0;    // Tiempo transcurrido en milisegundos desde el conteo anterior
unsigned long _old_millis=0;    //Para obtener el Tiempo acumulado en cada LAP
unsigned long _new_millis=0;


//=================================================================
// 1.- Inicializa el Timer1
//=================================================================
void initTIMER1(){
    Timer1.attachInterrupt(isrTimer1).setFrequency(contador.Frecuencia);
    //Timer1.attachInterrupt(isrTimer1).setFrequency(1);
    Timer1.stop();  
    delay(2);  
    habilitarTemporizador();  
    transmitirEstadoACTUAL();    //flagPrioridadTX =true;
}

void habilitarTemporizador(){
    if(contador.Enabled)    iniciarContador(); 
    else    detenerContador();     
}

void isrTimer1(){
    //Realizamos la lectura del sensor
    sensorRead1 = digitalRead(sensorPin1);
    sensorRead2 = digitalRead(sensorPin2);
    // Si el pulsador esta oprimido, flanco de subida
    if (sensorRead1 ==  HIGH && sensorState1 == false){
        sensorState1=true;
        digitalWrite(sensorLed, HIGH); 
    }
    else {  digitalWrite(sensorLed, LOW);  }
    // Si el pulsador no esta oprimido, flanco de bajada
    if (sensorRead1==LOW && sensorState1 ==true){
        sensorState1=false;
        incrementaConteoTOTAL();  
    } 
    // Si el pulsador esta oprimido, flanco de subida
    if (sensorRead2 ==  HIGH && sensorState2 == false){
        sensorState2=true;
    }
    // Si el pulsador no esta oprimido, flanco de bajada
    if (sensorRead2 == LOW && sensorState2 ==true){
      sensorState2=false;
      incrementaConteoTOTAL();  
    }  
}

//=================================================================
// 4.- DESHABILITA el proceso de Conteo
//=================================================================
// Detenido forzado del contador. 
void detenerContador(){
                      contador.Velocidad=0;
                      enableTX=false;
                      Timer1.stop();
                      contador.Enabled = false;
                      flagGuardar=true;          
                      // Para visualizar
                      //myFrame.setIcon(btnPLAYSTOP,OFF);
                      //setIconPLAYSTOP();
}
//=================================================================
// 4.- HABILITA el proceso de conteo
//=================================================================
// Activacion del contador al detenido forzado
void iniciarContador(){
                      RPM=0;_RPM=0;
                      Timer1.start();
                      contador.Enabled = true;
                      flagGuardar=true;
                      // Para visualizar.
                      //setIconPLAYSTOP();
}

unsigned long get_millis(){
    _new_millis=millis();
    if (_old_millis > _new_millis) _old_millis = _new_millis;
    return _new_millis - _old_millis;
}

void clear_millis(){
    //cont_millis=new_millis;
    _new_millis = millis();
    _old_millis = _new_millis;  
}
