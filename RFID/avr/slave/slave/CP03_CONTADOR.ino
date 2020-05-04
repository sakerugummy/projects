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
const int sensorPin1  = 11;
const int sensorPin2  = 12;
// Muestra si se encuentra contando
const int sensorLed   = 13;
// Mide las revoluciones por minuto de la maquina
unsigned long _RPM=0,  RPM=0;

boolean flagContador=false;     // FLAG que muestra si el contador se encuentra detenido.
                                // flagContador =  true   ->  CONTANDO
                                // flagContador =  false  ->  DETENIDO
                                
unsigned long horaLAP;          // Para mostrar la velocidad por HORAS(ConteoPARCIAL)
unsigned long timerLAP=0;       // Compara con el valor de Muestreo en seg.
byte pos_estatus=0;             // Pos DETENIDO/PRODUCIENDO/PAUSE

//=================================================================
// 1.- Obtiene los digitos de un numero con ceros a la izquierda
//=================================================================
void initCONTADOR(){
  // Configura el pin 2 como una entrada para el sensor  
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  // Configura el pin13 para visualizar el pulso del contador
  pinMode(sensorLed, OUTPUT); 
  // Habilitamos PULLUP
  digitalWrite(sensorPin1,HIGH);
  digitalWrite(sensorPin2,HIGH);
  // Reseteamos el Muestreo cuando el contador se enciende.
  if(contador.estadoACTUAL == PRD_NO_OPERABILIDAD || contador.estadoACTUAL == PRD_FINALIZO_PRODUCCION){         
        resetearDatosPRODUCCION();  resetearContador(false);
        
        //updateParametrosPRODUCCION();
        updateParametrosPRODUCCION_NEXTION();
        
        transmitirReseteaPRODUCCION();
  }
  else{
        if(contador.estadoACTUAL >= PRD_INICIA_PRODUCCION){ 
            actualizarConteoPARCIAL();
            transmitirConteoPARCIAL();
            if (contador.estadoACTUAL == PRD_PRODUCIENDO){
                actualizarEstadoCONTADOR(PRD_PRODUCCION_DETENIDA);
                flagContador=false;
            }
        } 
  }
  //mostrarPageContadores();
}
//=================================================================
// 2.- Resetea conteoTURNO y transmite el evento con TRUE
//=================================================================
void resetearContador(boolean confirmarTX){
  //----> Actualizamos conteoPARCIAL
  actualizarConteoPARCIAL();    //transmitirConteoPARCIAL();
  //----> TRANSMITIMOS
  if(confirmarTX) transmitirReseteaCONTADOR();
  //----> RESETEAMOS
  contador.conteoTURNO = 0;
  contador.conteoANTERIOR = 0;
  contador.conteoPARCIAL  = 0;
  contador.Velocidad=0;  RPM=0;
  //----> MOSTRAR EN PANTALLA
  //actualizarTotalizadorUTFT();
  actualizarTotalizadorNEXTION();
  //----> Temporizador de Muestreo
  horaLAP=0;
  transmitirEstadoACTUAL();
  flagGuardar=true;
}
//=================================================================
// 2.- Incrementa el conteoTOTAL y conteoTURNO
//=================================================================
void incrementaConteoTOTAL(){
    //Incrementa el conteo TOTAL
    contador.conteoTOTAL+=contador.Incremento;
    contador.conteoTURNO+=contador.Incremento;
    //if( !(contador.estadoACTUAL < PRD_INICIA_PRODUCCION ) && (contador.Enabled) )
    //Incrementa RPM
    _RPM+=contador.Incremento;
    
    if(contador.estadoACTUAL >= PRD_INICIA_PRODUCCION && contador.estadoACTUAL <= PRD_PRODUCCION_DETENIDA){
          if (flagContador){       
              timerLAP=get_millis();  
              clear_millis();     
              flagContador=true;      
          }
          else {
              pos_estatus = contador.estadoACTUAL = PRD_PRODUCIENDO;  
              //transmitirContinuaCONTEO();               
              
              flagContador=true;
              _RPM=0; RPM=0;
              timerLAP=0; clear_millis(); 
          }
    }
    enableTX=true;    
}
//=================================================================
// 3.- Verifica si el contador esta DETENIDO.
//=================================================================
void verificarContadorDETENIDO(){
     // ESTA RUTINA VERIFICA SI EL CONTADOR SE ENCUENTRA DETENIDO
    // Verificamos si el contador se encuentra contando o detenido.
    // Si esta contando y el tiempo de conteo es mayor al triple el tiempo anterior
    if (contador.estadoACTUAL>=PRD_INICIA_PRODUCCION && contador.estadoACTUAL <= PRD_PRODUCCION_DETENIDA){    
        if (  (flagContador==true) && ( get_millis() > contador.comparaLAP) )  {
            // Detener Contador
            flagContador=false;    timerLAP=0;      clear_millis();   
            pos_estatus = contador.estadoACTUAL = PRD_PRODUCCION_DETENIDA;
            ////////////////////////////////////
            //contador.Velocidad = 0;  
//================================================
//            transmitirContadorDETENIDO();
//            // UTFT
//            myFrame.setText(lblESTATUS     ,getEstatusSTR());
//            // NEXTION
//            mostrarEstadoSTR();
//            // Detener las transmiciones con el equipo.
//            enableTX=false;    
//================================================
        }  
    }
}
//=================================================================
// 3.- Actualiza el teimpo de Muestreo y Transmte PARCIAL
//=================================================================
void actualizarHoraLAP(){  
  if (horaLAP >= contador.Muestreo){ 
      // Esta Produciendo = ?.   
      if (contador.estadoACTUAL >= PRD_INICIA_PRODUCCION){
          actualizarConteoPARCIAL();
          transmitirConteoPARCIAL();           
      }
      else{
          contador.conteoPARCIAL=0;
          contador.conteoANTERIOR=0;                                                        
      } 
      horaLAP=0;  
  } 
}
//=================================================================
// 3.- Actualiza en conteo PARCIAL
//=================================================================
void actualizarConteoPARCIAL(){
       // Obtenemos el conteoPARCIAL
      long conteoTOTAL=contador.conteoTOTAL;
      // Almacenamos el valor del conteoPARCIAL en la EEPROM
      if (contador.conteoANTERIOR>contador.conteoTOTAL)    contador.conteoANTERIOR=0;
      contador.conteoPARCIAL=conteoTOTAL-contador.conteoANTERIOR;
      // Almacenamos el valor del conteoANTERIOR en la EEPROM
      contador.conteoANTERIOR=conteoTOTAL;
      flagGuardar=true;
      // Mostramos el nuevo valor de conteoPARCIAL
      myFrame.setText(lblCONTEOPARCIAL   ,(String)contador.conteoPARCIAL);
      actualizarParcialNEXTION();          
      horaLAP=0;
}
//=================================================================
// 4.- Actualiza el estado del Contador.
//=================================================================
void actualizarEstadoCONTADOR(byte STATUS){
    contador.estadoACTUAL=STATUS; 
    flagGuardar=true;    
    //myFrame.setText(lblESTATUS     ,getEstatusSTR());
}
//=================================================================
// 5.- Muestra el estadoACTUAL del Contador.
//=================================================================
void mostrarEstadoCONTADOR(){
    switch(pos_estatus){
      case PRD_PRODUCIENDO:
          // UART
          transmitirContinuaCONTEO();  
          // UTFT
          myFrame.setText(lblESTATUS,getEstatusSTR());
          // NEXTION
          actualizarEstadoCONT_NEXTION();
          mostrarEstadoSTR();
          //
          pos_estatus=0;
      break;
      case PRD_PRODUCCION_DETENIDA:
          // UART
          transmitirContadorDETENIDO();
          // UTFT
          myFrame.setText(lblESTATUS     ,getEstatusSTR());
          // NEXTION
          actualizarEstadoCONT_NEXTION();
          mostrarEstadoSTR();
          // Detener las transmiciones con el equipo.
          enableTX=false;
          //    
          pos_estatus=0;
      break;
    }
}

//=================================================================
// 5.- Obtiene los digitos de un numero con ceros a la izquierda
//=================================================================
String getDigitos(unsigned long numero, byte fillZero){
  //String _numero ="";  _numero=_numero+numero;
  String Digitos="";
  Digitos.reserve(fillZero+1);
  for(byte i=0;i<fillZero;i++){
    Digitos+="0";
  }
  String _numero = (String) numero;
  _numero = Digitos.substring(0,Digitos.length()-_numero.length()) + _numero;
  return _numero;
}
//=================================================================
// 6.- Obtiene el nombre del estado en que se encuentra el Contador
//=================================================================
String getEstatusSTR(){
  switch(contador.estadoACTUAL){
    case PRD_NO_OPERABILIDAD:           return "SIN PRODUC";    break;
    case NOP_AJUSTE_MAQUINA:            return "AJUS.OPER.";    break;
    case NOP_LIMPIEZA_RADICAL:          return "LIMPZ.RAD.";    break;     
    case NOP_CAMBIO_FORMATO:            return "CAMB.FORM.";    break;
    case NOP_CAMBIO_TURNO:              return "CAMB.TURNO";    break;
    case NOP_SIN_OPERADOR:              return "FALLA OPER";    break;
    case NOP_FALLA_MAQUINA:             return "FALLA MAQ.";    break;
    case NOP_MANTENIMIENTO_PREVENTIVO:  return "MANT.PREV.";    break;
    case NOP_MANTENIMIENTO_CORRECTIVO:  return "MANT.CORR.";    break;
    case NOP_ENSAYOS_DESARROLLO:        return "ENSAY.DES.";    break;

    case PRD_NUEVA_PRODUCCION:          return "P.GUARDADO";    break;
    case PRD_SACANDO_MUESTRAS:          return "S.MUESTRAS";    break;
    case PRD_ESPERANDO_APROBACION:      return "ESPERA CC.";    break;
    case PRD_INICIA_PRODUCCION:         return "PRODUCIEND";    break;
    case PRD_PRODUCIENDO:               return "PRODUCIEND";    break;
    case PRD_PRODUCCION_DETENIDA:       return "P.DETENIDO";    break;

    case INT_AJUSTE_MAQUINA:            return "AJUS.OPER.";    break;
    case INT_LIMPIEZA_EQUIPO:           return "LIMP. MAQ.";    break;
    case INT_CAMBIO_INSUMOS:            return "CAMB.INSM.";    break; 
    case INT_MATERIA_PRIMA:             return "MATERIA P.";    break;
    case INT_CAMBIO_TURNO:              return "CAMB.TURNO";    break;    
    case INT_FALLA_MAQUINA:             return "FALLA MAQ.";    break;
    case INT_MANTENIMIENTO_CORRECTIVO:  return "MANT.CORR.";    break;
    
    case PRD_FINALIZO_PRODUCCION:       return "FINALIZ P.";    break;
    default:                            return "SIN PRODUC";    break;
   }
}
