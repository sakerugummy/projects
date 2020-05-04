void generarNuevoIdPRODUCCION(){
  if(produccion.idProduccion<0)   produccion.idProduccion=0;  
  produccion.idProduccion++;
  //myRTC.writeLongEeprom(eepromDirIdProduccion,produccion.idProduccion); delay(MAX_TIME_SAVE);
  //myFrame.setText(lblidPRODUCCION, (String)produccion.idProduccion);
  flagGuardar=true;
}
void actualizaNuevaPRODUCCION(){
    generarNuevoIdPRODUCCION(); 
    produccion.horaCREACION = myRTC.getUnixTime(myRTC.getTime());
    
    //myRTC.writeStringEeprom(eepromDirhoraCREACION,produccion.horaCREACION); delay(5*MAX_TIME_SAVE);     
    produccion.horaINICIO = 0;
    //myRTC.writeStringEeprom(eepromDirhoraINICIO,produccion.horaINICIO); delay(5*MAX_TIME_SAVE);    
    produccion.horaFINAL = 0;    
    //myRTC.writeStringEeprom(eepromDirhoraFINAL,produccion.horaFINAL); delay(5*MAX_TIME_SAVE); 
    
    resetearContador(false);
    contador.estadoACTUAL = PRD_NUEVA_PRODUCCION;
    //myRTC.writeByteEeprom(eepromDirestadoACTUAL,contador.estadoACTUAL);delay(MAX_TIME_SAVE);          
    
    transmitirNuevaPRODUCCION ();
    transmitirParametrosCONTEO();
    resetearTiemposPRODUCCION ();
    detenerContador();
    flagGuardar=true;
}

void iniciarProduccion(){
    detenerContador();  flagContador = false;    resetearContador(false);    contador.conteoTOTAL=0;
    produccion.Produciendo=true;     flagGuardar=true;
    
    // Almacenamos en la memoria EEPROM
    actualizarEstadoCONTADOR(PRD_INICIA_PRODUCCION);

    // Actualiza Hora de Inicio de Produccion    
    produccion.horaINICIO= myRTC.getUnixTime(myRTC.getTime());
    //myRTC.writeStringEeprom(eepromDirhoraINICIO,produccion.horaINICIO); delay(5*MAX_TIME_SAVE);    
    produccion.horaFINAL = 0;
    //myRTC.writeStringEeprom(eepromDirhoraFINAL,produccion.horaFINAL); delay(5*MAX_TIME_SAVE);    

    tiempos.horas_PRD_PRODUCIENDO     = 0;
    //myRTC.writeLongEeprom(eepromDirHoras_PRD_PRODUCIENDO,          tiempos.horas_PRD_PRODUCIENDO);          delay(MAX_TIME_SAVE);
    tiempos.horas_PRD_PRODUCCION_DETENIDA = 0;
    //myRTC.writeLongEeprom(eepromDirHoras_PRD_PRODUCCION_DETENIDA, tiempos.horas_PRD_PRODUCCION_DETENIDA);   delay(MAX_TIME_SAVE);       

    // Mostrar Datos de Inicio de PRODUCCION    
    transmitirIniciarPRODUCCION();
    // updateParametrosPRODUCCION(); //UTFT
    updateParametrosPRODUCCION_NEXTION();

    actualizarEstadoCONTADOR(PRD_PRODUCCION_DETENIDA);
    //transmitirTiemposPRODUCCION();
    // Reseteamos el temporizador del MUESTREO
    horaLAP=0;    
    // Iniciamos PRODUCCION
    iniciarContador();
    flagGuardar=true;
}

void finalizarProduccion(){
  detenerContador();
  actualizarConteoPARCIAL();
  produccion.horaFINAL = myRTC.getUnixTime(myRTC.getTime());
  produccion.Produciendo=false;
  produccion.Salvado =false;
  flagGuardar=true;
  actualizarEstadoCONTADOR(PRD_FINALIZO_PRODUCCION);  segundosTX=0;
 // updateParametrosPRODUCCION(); //UTFT
 //  mostrarPantallaContadores(); //UTFT
  updateParametrosPRODUCCION_NEXTION(); 
  transmitirFinalizarPRODUCCION();
  writeSDCardPROD();
  delay(1000);     
}

void resetearPRODUCCION(){
    // Detenemos el conteo TOTAL y TURNO
    detenerContador();
    // Generamos nuevo id_Produccion
    generarNuevoIdPRODUCCION();
    resetearDatosPRODUCCION();  resetearContador(false);     
    transmitirReseteaPRODUCCION();    
    tiempos.horas_PRD_NO_OPERABILIDAD  = 0;
    flagGuardar=true;
    // Modificar Pantalla Contadores
    //updateParametrosPRODUCCION(); //UTFT
    //mostrarPantallaContadores(); //UTFT
    updateParametrosPRODUCCION_NEXTION();    
    mostrarPageContadores();
    
    //myFrame.setText(txtNuevoPRODUCTO ,getStringPRODUCTO()); //UTFT
    //if (produccion.Lote.length()==0) myFrame.setText(txtNuevoLOTE     ,"SIN LOTE");//UTFT
    //else    myFrame.setText(txtNuevoLOTE     , produccion.Lote);    //UTFT
    //habilitarControlesProduccion();       //UTFT
}
