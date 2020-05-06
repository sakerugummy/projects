#define SEGUNDOS_GUARDAR   60
#define SEGUNDOS_MUESTREO  120
//=================================================================
// 0.- Variables para el proceso de Temporizacion
//=================================================================
unsigned long cont_millis=0;    // Tiempo transcurrido en milisegundos desde el conteo anterior
unsigned long _old_millis=0;    //Para obtener el Tiempo acumulado en cada LAP
unsigned long _new_millis=0;
boolean flagMostrar=false;      // FLAG para realizar la muestra del proceso de conteo se activa cada segundo.

void isrTimer1(){
}

void cargarParametros(){
  while(contador.id==0){    
    verificarRX1();
    if(flagMostrar){  flagMostrar = false;      transmitirContadorENCENDIDO();    }
  }
  //mostrarParametrosContador();
  // Reseteamos el Muestreo cuando el contador se enciende.
  if(estado.ACTUAL == PRD_NO_OPERABILIDAD || estado.ACTUAL == PRD_FINALIZO_PRODUCCION){         
    if(estado.ACTUAL == PRD_FINALIZO_PRODUCCION)  resetearPRODUCCION();
    //else  resetearDatosPRODUCCION();  
    resetearContador(false);    
  }
  else{
    if(estado.ACTUAL >= PRD_INICIA_PRODUCCION){ 
      if (estado.ACTUAL == PRD_PRODUCIENDO){
          actualizarEstadoCONTADOR(PRD_PRODUCCION_DETENIDA);
          flagContador=false;
      }
    } 
  }
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

void actualizarContador(){
   /*_____________________________________________________________*/
   /* Ha transcurrido un Segundo? Para mostrar la hora Actual.    */
   if (flagMostrar){
      flagMostrar=false;              // Esperamos a que transcurra otro segundo.
      actualizarTiemposPRODUCCION();
      if(estado.ACTUAL >= PRD_INICIA_PRODUCCION &&  estado.ACTUAL < PRD_FINALIZO_PRODUCCION){
          verificarCAPTURA();
          verificarContadorDETENIDO();    // Verificamos si el contador se encuentra Detenido.
          verificarEstadoCONTADOR();
      }
      transmitirEstadoACTUAL();
   }
}

void verificarCAPTURA(){
  if(epochCurrent %  60  == 0){
    EEPROM.put(EEPROM_DIR_CONTEO,     conteo);
    EEPROM.put(EEPROM_DIR_ESTADO,     estado);
    EEPROM.put(EEPROM_DIR_TIEMPOS,    tiempos);    
  }
  if(epochCurrent % SEGUNDOS_MUESTREO == 0){
     actualizarVelocidadHORA();
  }
}

void actualizarVelocidadHORA(){
    unsigned long TOTAL = conteo.TOTAL;
    if(TOTAL>conteo.ANTERIOR) velocidad.HORA = TOTAL - conteo.ANTERIOR;
    else  velocidad.HORA=0;
    transmitirVelocidadHORA();
    conteo.ANTERIOR = TOTAL;
    EEPROM.put(EEPROM_DIR_CONTEO,     conteo);   
}

void actualizarTiemposPRODUCCION(){
    // TIEMPOS
    switch(estado.ACTUAL){
      case PRD_NO_OPERABILIDAD:
        tiempos.horas_PRD_NO_OPERABILIDAD++;
        break;
      case NOP_AJUSTE_MAQUINA:
        tiempos.horas_NOP_AJUSTE_MAQUINA++;
        break;
      case NOP_LIMPIEZA_RADICAL:
        tiempos.horas_NOP_LIMPIEZA_RADICAL++;
        break;
      case NOP_CAMBIO_FORMATO:
        tiempos.horas_NOP_CAMBIO_FORMATO++;
        break;
      case NOP_CAMBIO_TURNO:
        tiempos.horas_NOP_CAMBIO_TURNO++;
        break;
      case NOP_SIN_OPERADOR:
        tiempos.horas_NOP_SIN_OPERADOR++;
        break;
      case NOP_FALLA_MAQUINA:
        tiempos.horas_NOP_FALLA_MAQUINA++;
        break;
      case NOP_MANTENIMIENTO_PREVENTIVO:
        tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO++;
        break;
      case NOP_MANTENIMIENTO_CORRECTIVO:
        tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO++;
        break;
      case NOP_ENSAYOS_DESARROLLO:
        tiempos.horas_NOP_ENSAYOS_DESARROLLO++;
        break;
      case PRD_NUEVA_PRODUCCION:
        tiempos.horas_PRD_NUEVA_PRODUCCION++;
        break;
      case PRD_SACANDO_MUESTRAS:
        tiempos.horas_PRD_SACANDO_MUESTRAS++;
        break;
      case PRD_ESPERANDO_APROBACION:
        tiempos.horas_PRD_ESPERANDO_APROBACION++;
        break;
      case PRD_INICIA_PRODUCCION:
        tiempos.horas_PRD_PRODUCCION_DETENIDA++;
        break;
      case PRD_PRODUCIENDO:
        if (estado.ACTIVO)   tiempos.horas_PRD_PRODUCIENDO++;
        else    tiempos.horas_INT_AJUSTE_MAQUINA++;
        break;
      case PRD_PRODUCCION_DETENIDA:
        tiempos.horas_PRD_PRODUCCION_DETENIDA++;
        break;        
      case INT_AJUSTE_MAQUINA:
        tiempos.horas_INT_AJUSTE_MAQUINA++;
        break;
      case INT_LIMPIEZA_EQUIPO:
        tiempos.horas_INT_LIMPIEZA_EQUIPO++;
        break;
      case INT_CAMBIO_INSUMOS:
        tiempos.horas_INT_CAMBIO_INSUMOS++;
        break;
      case INT_MATERIA_PRIMA:
        tiempos.horas_INT_MATERIA_PRIMA++;
        break;
      case INT_CAMBIO_TURNO:
        tiempos.horas_INT_CAMBIO_TURNO++;
        break;
      case INT_FALLA_MAQUINA:
        tiempos.horas_INT_FALLA_MAQUINA++;
        break;
      case INT_MANTENIMIENTO_CORRECTIVO:
        tiempos.horas_INT_MANTENIMIENTO_CORRECTIVO++;
        break;
      case PRD_FINALIZO_PRODUCCION:
        tiempos.horas_PRD_NO_OPERABILIDAD++;
        break;
    }
}

void resetearTiemposPRODUCCION(){
  // NO OPERABILIDAD - SIN PRODUCCION
  tiempos.horas_PRD_NO_OPERABILIDAD   = 0;
  tiempos.horas_NOP_AJUSTE_MAQUINA    = 0;
  tiempos.horas_NOP_LIMPIEZA_RADICAL  = 0;      
  tiempos.horas_NOP_CAMBIO_FORMATO    = 0;
  tiempos.horas_NOP_CAMBIO_TURNO      = 0;
  tiempos.horas_NOP_SIN_OPERADOR      = 0;
  tiempos.horas_NOP_FALLA_MAQUINA     = 0;
  tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO = 0;
  tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO = 0;
  tiempos.horas_NOP_ENSAYOS_DESARROLLO = 0;
  // DURANTE LA PRODUCCION        
  tiempos.horas_PRD_NUEVA_PRODUCCION   = 0;
  tiempos.horas_PRD_SACANDO_MUESTRAS   = 0;
  tiempos.horas_PRD_ESPERANDO_APROBACION = 0;      
  tiempos.horas_PRD_PRODUCIENDO = 0;
  tiempos.horas_PRD_PRODUCCION_DETENIDA = 0;
  // INTERRUMPIR PRODUCCION
  tiempos.horas_INT_AJUSTE_MAQUINA    = 0;
  tiempos.horas_INT_LIMPIEZA_EQUIPO   = 0;
  tiempos.horas_INT_CAMBIO_INSUMOS    = 0;
  tiempos.horas_INT_MATERIA_PRIMA     = 0;
  tiempos.horas_INT_CAMBIO_TURNO      = 0;
  tiempos.horas_INT_FALLA_MAQUINA     = 0;
  tiempos.horas_INT_MANTENIMIENTO_CORRECTIVO = 0;
}
