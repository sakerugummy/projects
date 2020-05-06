// Realiza la transmicion si existe algun dato en la Lista de Transmision.
void realizarTransmision(){
    Serial1.print(getRFID());
    Serial1.print("|UNIX~");Serial1.print(epochCurrent,16);
    Serial1.print(strTransmitir);
    Serial1.print("|ESTD~");Serial1.print(estado.ACTUAL);
    //Serial1.print("|UNIX~");Serial1.print(epochCurrent,16);
    Serial1.println("|");
    strTransmitir="";
}
// Transmite el estado Actual del CONTADOR
void transmitirEstadoACTUAL(){
    transmitirCantidades();
    transmitirTiempos();
    realizarTransmision();      
}
// Transmite los totales de produccion
void transmitirCantidades(){
    strTransmitir = "";
    strTransmitir += ("|TOTL~"+(String)conteo.TOTAL) ;
    strTransmitir += ("|TURN~"+(String)conteo.TURNO) ;
    strTransmitir += ("|ANTE~"+(String)conteo.ANTERIOR) ;
    strTransmitir += ("|VELC~"+(String)(velocidad.RPM0 + velocidad.RPM1));
}
// Transmitir tiempos
void transmitirTiempos(){
  strTransmitir += ("|TEST~"+(String) estado.ACTUAL);

  switch(estado.ACTUAL){
    case PRD_NO_OPERABILIDAD: 
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_NO_OPERABILIDAD) ;        
      break;
    case NOP_AJUSTE_MAQUINA:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_AJUSTE_MAQUINA) ;
      break;        
    case NOP_LIMPIEZA_RADICAL:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_LIMPIEZA_RADICAL) ;
      break;    
    case NOP_CAMBIO_FORMATO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_CAMBIO_FORMATO) ;
      break;
    case NOP_CAMBIO_TURNO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_CAMBIO_TURNO) ;
      break;
    case NOP_SIN_OPERADOR:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_SIN_OPERADOR) ;
      break;
    case NOP_FALLA_MAQUINA:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_FALLA_MAQUINA) ;
      break;       
    case NOP_MANTENIMIENTO_PREVENTIVO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO) ;
      break;
    case NOP_MANTENIMIENTO_CORRECTIVO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO) ;
      break;
    case NOP_ENSAYOS_DESARROLLO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_NOP_ENSAYOS_DESARROLLO) ;
      break;
    case PRD_NUEVA_PRODUCCION:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_NUEVA_PRODUCCION) ;
      break;
    case PRD_SACANDO_MUESTRAS:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_SACANDO_MUESTRAS) ;
      break;
    case PRD_ESPERANDO_APROBACION:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_ESPERANDO_APROBACION) ;
      break;
    case PRD_INICIA_PRODUCCION:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCIENDO) ;
      break;
    case PRD_PRODUCIENDO:
    if (estado.ACTIVO)   strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCIENDO) ;
      else          strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCCION_DETENIDA) ;        
      break;        
    case PRD_PRODUCCION_DETENIDA:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCCION_DETENIDA) ;
      break;
    case INT_AJUSTE_MAQUINA:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_AJUSTE_MAQUINA) ;
      break;    
    case INT_LIMPIEZA_EQUIPO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_LIMPIEZA_EQUIPO) ;
      break; 
    case INT_CAMBIO_INSUMOS:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_CAMBIO_INSUMOS) ;
      break; 
    case INT_MATERIA_PRIMA:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_MATERIA_PRIMA) ;
      break;
    case INT_CAMBIO_TURNO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_CAMBIO_TURNO) ;
      break;
    case INT_FALLA_MAQUINA:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_FALLA_MAQUINA) ;
      break;       
    case INT_MANTENIMIENTO_CORRECTIVO:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_INT_MANTENIMIENTO_CORRECTIVO) ;
      break;        
    case PRD_FINALIZO_PRODUCCION:
      strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_NO_OPERABILIDAD) ;
      break;        
  }
}

void transmitirContinuaCONTEO(){
    transmitirCantidades();
    strTransmitir += ("|EVNT="+(String)PRD_PRODUCIENDO);
    realizarTransmision();
}

void transmitirContadorDETENIDO(){  
    transmitirCantidades();
    strTransmitir += ("|EVNT="+(String)PRD_PRODUCCION_DETENIDA) ;
    realizarTransmision();
} 

void transmitirReseteaCONTADOR(){
    transmitirCantidades();
    strTransmitir += ("|EVNT="+(String)MSG_RESETEA_CONTADOR);
    realizarTransmision();
}

void transmitirContadorENCENDIDO(){
    strTransmitir = "";
    strTransmitir += ("|EVNT="+(String)MSG_CONTADOR_ENCENDIDO) ;
    realizarTransmision(); 
}

void transmitirVelocidadHORA(){
    strTransmitir = "";
    strTransmitir += ("|ICAP~"+(String)velocidad.POS);
    strTransmitir += ("|TOTL~"+(String)conteo.TOTAL);
    strTransmitir += ("|ANTE~"+(String)conteo.ANTERIOR);
    strTransmitir += ("|PROH~"+(String)velocidad.HORA);
    strTransmitir += ("|EVNT="+(String)MSG_CAPTURA_CONTEOHORA);
    strTransmitir += ("|");
    realizarTransmision();
}

/*----------------------------------------------------------------*/
boolean isNumeric(String cadena){
    byte i,c=0;
    for(i=0;i<cadena.length();i++)
        if(isDigit(cadena.charAt(i)))  c++;
    if (c==cadena.length() && c!=0)    return true;
    else   return false;  
} 
