//=================================================================
// 0.- Variable para el estado de conteo
//=================================================================
// Estado anterior del sensor para el flanco de subida o bajada
boolean sensorState1 = false;
boolean sensorState2 = false;
// Estado de Lectura del Sensor;
int sensorRead1 =0;
int sensorRead2 =0;
// Direccion del puerto de entrada del contador y el led
const int sensorPin1  = 2;  // INT0
const int sensorPin2  = 3;  // INT1
// Muestra si se encuentra contando
const int sensorLed   = 13;
// Mide las revoluciones por minuto de la maquina
unsigned long _RPM0=0,  RPM0=0;
unsigned long _RPM1=0,  RPM1=0;
boolean flagContador=false;     // FLAG que muestra si el contador se encuentra detenido.
                                // flagContador =  true   ->  CONTANDO
                                // flagContador =  false  ->  DETENIDO
                                
unsigned long timerLAP=0;       // Compara con el valor de Muestreo en seg.
byte pos_estatus=0;             // Pos DETENIDO/PRODUCIENDO/PAUSE

unsigned long previusLAP0=0, currentLAP0=0;
unsigned long previusLAP1=0, currentLAP1=0;
//=================================================================
// 1.- Inicializa el contador
//=================================================================
void initCONTADOR(){
  attachInterrupt(digitalPinToInterrupt(sensorPin1), isrINT0, RISING );
  attachInterrupt(digitalPinToInterrupt(sensorPin2), isrINT1, RISING );
  previusLAP0=previusLAP1 = millis();
  // Configura el pin13 para visualizar el pulso del contador
  pinMode(sensorLed, OUTPUT); 
  // Habilitamos PULLUP 
  pinMode(sensorPin1, INPUT_PULLUP);
  pinMode(sensorPin2, INPUT_PULLUP);
  cargarParametros(); 
}

void mostrarParametrosContador(){
  //-----------------------------------------------------------------------------
  // CONTADOR
  Serial1.println(getRFID());
  Serial1.println(contador.Incremento);
  Serial1.println(conteo.TOTAL);
  Serial1.println(conteo.TURNO);
  Serial1.println(contador.comparaLAP);
  Serial1.println(estado.ACTUAL);
  Serial1.println(estado.ACTIVO);
}

void isrINT0(){
  currentLAP0 = millis();
  if(!flagContador) previusLAP0 = millis();
  velocidad.LAP0 = currentLAP0-previusLAP0;  
  previusLAP0 = currentLAP0;
  velocidad.LAP0 = redondear(velocidad.LAP0);
  if(estado.ACTIVO){
    if (velocidad.LAP0 >0 && velocidad.LAP0<=60000L){
      velocidad.RPM0= 60000L/velocidad.LAP0;
    }
    else
      velocidad.RPM0= 0;
    incrementaTOTAL();
    _RPM0+=contador.Incremento;
  }
}
void isrINT1(){
  currentLAP1 = millis();
  if(!flagContador) previusLAP1 = millis();
  velocidad.LAP1 = currentLAP1-previusLAP1;
  previusLAP1 = currentLAP1;
  velocidad.LAP1 = redondear(velocidad.LAP1);
  if(estado.ACTIVO){
    if (velocidad.LAP1 >0 && velocidad.LAP1<=60000L){
      velocidad.RPM1= 60000L/velocidad.LAP1;
    }
    else
      velocidad.RPM1= 0;      
    incrementaTOTAL();
        //Incrementa RPM
    _RPM1+=contador.Incremento;
  }
    
}

unsigned long redondear(unsigned long value){
  switch(value % 10){
    case 0: case 1: case 2:
      value = (value/10)*10 + 0;
      break;
    case 3: case 4: case 5: case 6: 
      value = (value/10)*10 + 5;
      break;
   case 7: case 8: case 9:
      value = (value/10)*10 + 10;
      break;
  }
  return value;
}

//=================================================================
// 2.- Resetea TURNO y transmite el evento con TRUE
//=================================================================
void resetearContador(boolean confirmarTX){
    //----> RESETEAMOS
    conteo.TURNO = 0;
    velocidad.RPM0=0;  RPM0=0;
    velocidad.RPM1=0;  RPM1=0;
    transmitirEstadoACTUAL();
}
//=================================================================
// 2.- Incrementa el TOTAL y TURNO
//=================================================================
void incrementaTOTAL(){
    //Incrementa el conteo TOTAL
    conteo.TOTAL+=contador.Incremento;
    conteo.TURNO+=contador.Incremento;
    
    if(estado.ACTUAL >= PRD_INICIA_PRODUCCION && estado.ACTUAL <= PRD_PRODUCCION_DETENIDA){
        if(estado.ACTUAL == PRD_PRODUCCION_DETENIDA) {
          flagContador = false;
          estado.ACTUAL = PRD_PRODUCIENDO;
        }
        if (flagContador){
            timerLAP=get_millis();
            clear_millis();
            flagContador=true;
        }
        else {
            pos_estatus = PRD_PRODUCIENDO;                           
            flagContador=true;
            _RPM0=0; RPM0=0;
            _RPM1=0; RPM1=0;
            timerLAP=0; clear_millis(); 
        }
    }  
}

//=================================================================
// 3.- Verifica si el contador esta DETENIDO.
//=================================================================
void verificarContadorDETENIDO(){
     // ESTA RUTINA VERIFICA SI EL CONTADOR SE ENCUENTRA DETENIDO
    // Verificamos si el contador se encuentra contando o detenido.
    if (  (flagContador==true) && ( get_millis() > contador.comparaLAP) )  {
        // Detener Contador
        flagContador=false;     timerLAP=0;    clear_millis(); 
        previusLAP0=0;  currentLAP0=0;  previusLAP1=0;  currentLAP1=0; velocidad.RPM0=0; velocidad.RPM1=0;
        pos_estatus = estado.ACTUAL = PRD_PRODUCCION_DETENIDA;
    }      
}

//=================================================================
// 4.- DESHABILITA el proceso de Conteo
//=================================================================
// Detenido forzado del conteo 
void detenerContador(){
    flagContador=false;
    velocidad.RPM0=0;
    velocidad.RPM1=0;    
    estado.ACTIVO = false;
}
//=================================================================
// 4.- HABILITA el proceso de conteo
//=================================================================
// Activacion del contador al detenido forzado
void iniciarContador(){
    flagContador=false;
    RPM0=0;_RPM0=0;
    RPM1=0;_RPM1=0;
    previusLAP0=0, currentLAP0=0;
    previusLAP1=0, currentLAP1=0;
    estado.ACTIVO = true;    
}
//=================================================================
// 5.- Muestra el estadoACTUAL del conteo
//=================================================================
void verificarEstadoCONTADOR(){
    switch(pos_estatus){
      case PRD_PRODUCIENDO:
          //Serial.println("PRODUCIENDO");
          EEPROM.put(EEPROM_DIR_ESTADO,     estado);
          transmitirContinuaCONTEO();
          pos_estatus=0;
      break;
      case PRD_PRODUCCION_DETENIDA:
          //Serial.println("DETENIDO");
          EEPROM.put(EEPROM_DIR_CONTEO,     conteo);
          EEPROM.put(EEPROM_DIR_ESTADO,     estado);
          // UART
          transmitirContadorDETENIDO();
          pos_estatus=0;
      break;
    }
}
//=================================================================
// 4.- Actualiza el estado del conteo
//=================================================================
void actualizarEstadoCONTADOR(byte STATUS){
    estado.ACTUAL=STATUS;
}
