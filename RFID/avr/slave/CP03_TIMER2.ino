byte segundos=0;  
boolean flagMostrar=false;      // FLAG para realizar la muestra del proceso de conteo se activa cada segundo.
boolean flagTemporizador;       // FLAG para resetear el Temporizador.

void initTIMER2(){
    Timer2.attachInterrupt(isrTimer2).setFrequency(1);
    Timer2.start();
}

void isrTimer2(){
    flagMostrar=true;
    segundos++;  horaLAP++;     // Acumula los segundos transcurridos para comparar con el muestreo 
}

void actualizarContador(){
   /*_____________________________________________________________*/
   /* Ha transcurrido un Segundo? Para mostrar la hora Actual.    */
   if (flagMostrar){
        flagMostrar=false;              // Esperamos a que transcurra otro segundo.
        actualizarHoraACTUAL();         // Actualiza horaACTUAL
        verificarCAPTURA();
        actualizarHoraLAP();            // Verificamos si ha transcurrido el tiempo del muestreo
        actualizarTiemposPRODUCCION();  // Actualizamos Tiempos de PRODUCCION        
        verificarTransmision();         // Realizamos Transmisiones Periodicas del estado del CONTADOR    
        //actualizarHoraACTUAL();         // Actualiza horaACTUAL
        //verificarCAPTURA();
        //mostrarHoraACTUAL();            // Mostramos la hora en la parte inferior de la pantalla UTFT 
        mostrarHoraNEXTION();           // Mostramos la hora en la parte inferior de la pantalla   
        almacenarParametrosCONTADOR();  // Guardamos Datos en la memoria EEPROM        
        verificarContadorDETENIDO();    // Verificamos si el contador se encuentra Detenido.
        mostrarEstadoCONTADOR();
   }
   actualizarContadorNEXTION();
   //myFrame.setText(lblCONTEOTOTAL,getDigitos8(contador.conteoTOTAL)); 
   //myFrame.setText(lblconteoTURNO,(String) contador.conteoTURNO);
}

void actualizarHoraACTUAL(){
  getFullTimeSTR();
}

void verificarCAPTURA(){
  if(horaCAPTURA % 3600 ==0){
    contador.conteoHORA = contador.conteoTOTAL;    
    transmitirConteoHORA();
    writeSDCardVELC();
  }
}

void actualizarTiemposPRODUCCION(){
    // VELOCIDADES   
    if (segundos==60){    // Ha transcurrido un Minuto? Para actualizar RPM. 
        segundos = 0;     // Esperamos a que transcurra otro minuto.         
        RPM=_RPM; contador.Velocidad = _RPM * 60; _RPM=0;      //Actualiza Velocidad        
        //mostrarRPM();     // Mostrarmos las revoluciones por minuto UTFT.
        actualizarRpmNEXTION();
    }
    // TIEMPOS
    switch(contador.estadoACTUAL){
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
        if (contador.Enabled)   tiempos.horas_PRD_PRODUCIENDO++;
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
    // PAGE
    if (PAGE == MENU_TIEMPOS){
        updateTiemposPRODUCCION_NEXTION(); 
    }
    // UTFT    
    /*if (NIVEL == MENU_TIEMPOS){
        setTextTiemposPRODUCCION();
    }*/  
}

void resetSegundosTX(){
  segundosTX=0;
}

String secToHour(long time){
  String Output="";
  int _horas=0,_minutos=0,_segundos=0;
  long dif=time;
  
  _horas = dif/3600;
  dif=dif%3600;
  _minutos = dif/60;
  dif=dif%60;
  _segundos = dif;
  
  if (_horas < 10)    Output+="0";
  Output+=_horas;       Output+=":";
  if (_minutos < 10)  Output+="0";
  Output+=_minutos;     Output+=":";
  if (_segundos < 10) Output+="0";
  Output+=_segundos;
  
  return Output;
}
