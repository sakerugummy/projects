//==================================================================
//======================   NEXTION DISPLAY   =======================
//==================================================================
// Retardo en ms para la conexion con puerto
const byte usdelayRateNEXTION= 600;
// Velocidad de transmicion de la comuncacion serial  
const byte bufferSizeNEXTION = 200;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputStringNEXTION = "", rxStringNEXTION="";
// Recepcion finalizada del puerto(true)  
boolean stringCompleteNEXTION = false;

void initNEXTION(){
    Nextion.begin(NextionBaud); 
    updateParametrosPRODUCCION_NEXTION();
    //------> INICIALIZAMOS LA PANTALLAS    
    mostrarCargando(0);
    mostrarPageCargando();  
}
void mostrarPageCargando(){
  Nextion.print("page pnlCargando");    eofNextion();  
}
void mostrarCargando(byte val){
  Nextion.print("pnlCargando.pbarParam.val="); Nextion.print(val); eofNextion();
  Nextion.print("pnlCargando.txtval.txt="); Nextion.print((char) 34);    Nextion.print(val); Nextion.print("%");  Nextion.print((char) 34);  eofNextion(); 
  
}

void mostrarPageContadores(){
    Nextion.print("page pnlContador");    eofNextion();  
}
void mostrarPageProduccion(){
    Nextion.print("page pnlProduccion");    eofNextion();  
}
void eofNextion(){
    Nextion.write(0xFF);    Nextion.write(0xFF);    Nextion.write(0xFF);
}

// Verificamos si se ha recepcionado una cadena de texto en COM-1.
void verificarNEXTION(){ 
    boolean flagEventNEXTION=false;      
    while (Nextion.available() && !flagEventNEXTION) {
        char inCharNEXTION = (char) Nextion.read();
        if(inCharNEXTION == 0x1A) {Serial.print((int) inCharNEXTION); Serial.print("|");}
        if (inCharNEXTION!=0x00 && inCharNEXTION!=0x0D && inCharNEXTION!=0x0A && inCharNEXTION!=0xFF && inCharNEXTION!=0x1A)
            inputStringNEXTION += inCharNEXTION;            
        else{
            //if (inCharNEXTION==0x00 || inCharNEXTION==0xFF) inputStringNEXTION = "";
            if (inCharNEXTION==0x0D){ 
                rxStringNEXTION=inputStringNEXTION;  
                inputStringNEXTION=""; 
                stringCompleteNEXTION = true;
                flagEventNEXTION = true;
            }
        }      
        //Retardo necesario para la recepcion de datos.
        delayMicroseconds(usdelayRateNEXTION);
        // Si no hay mas datos salir de la rutina.         
    }  
 
    if (stringCompleteNEXTION) {
        Serial.println(rxStringNEXTION);        
        stringCompleteNEXTION = false;          
        if (rxStringNEXTION.indexOf("RFID")>=0){            
            attachCmdNEXTION(rxStringNEXTION);                
        }     
    }
}

void attachCmdNEXTION(String cmdNextion){
  //Serial.print  (cmdNextion);
  //Serial.println(cmdNextion.length());
  decodificarRecepcion(cmdNextion);
  if(strDecodificar.length()>4){
      Serial.write(6);
      Serial.print(ID); 
      Serial.print(strDecodificar);
      Serial.write(3);Serial.println("");
  }
}

void updateParametrosPRODUCCION_NEXTION(){
    parametrosContadorNEXTION();
    parametrosProduccionNEXTION();
    parametrosTiemposNEXTION();
}

void mostrarEstadoSTR(){
    // lblSTATUS
    Nextion.print("pnlContador.strEstado.txt=");    Nextion.print((char) 34);    Nextion.print(getEstatusSTR());  Nextion.print((char) 34);  eofNextion();     
}

void mostrarHoraNEXTION(){
    // tiempoACTUAL
    Nextion.print("pnlContador.lblHORA.txt=");  Nextion.print((char) 34);
    Nextion.print(tiempoACTUAL);    Nextion.print((char) 34);
    eofNextion();
    /*if(NIVEL == MENU_CONFIGURACIONES && produccion.Produciendo){
        myFrame.setText(txtHora,  myRTC.getTimeStr(FORMAT_LONG));        
    }*/
    Nextion.print("pnlTiempos.hPROD.txt=");   Nextion.print((char) 34);  Nextion.print(secToHour(tiempos.horas_PRD_PRODUCIENDO));   Nextion.print((char) 34);   eofNextion();
    Nextion.print("pnlTiempos.hDET.txt=");    Nextion.print((char) 34);  Nextion.print(secToHour(tiempos.horas_PRD_PRODUCCION_DETENIDA));   Nextion.print((char) 34);   eofNextion();
}

void actualizarTotalizadorNEXTION(){
  actualizarContadorNEXTION();
  actualizarParcialNEXTION();
  actualizarRpmNEXTION();
  actualizarEstadoPROD_NEXTION();
  mostrarTiemposPROD_NEXTION();
}

void actualizarContadorNEXTION(){
    // conteoTOTAL
    Nextion.print("pnlContador.cntTOTAL.txt=");Nextion.print((char) 34);
    Nextion.print(getDigitos(contador.conteoTOTAL,8));    Nextion.print((char) 34); eofNextion();
    // conteoTURNO
    Nextion.print("pnlContador.cntTURNO.txt=");Nextion.print((char) 34);
    Nextion.print(contador.conteoTURNO);    Nextion.print((char) 34);    eofNextion();
}
void actualizarParcialNEXTION(){
    // conteoPARCIAL
    Nextion.print("pnlContador.cntPARCIAL.txt=");Nextion.print((char) 34);
    Nextion.print(contador.conteoPARCIAL);    Nextion.print((char) 34); eofNextion();
}

void actualizarRpmNEXTION(){
  // Velocidad
  Nextion.print("pnlContador.Velocidad.txt=");  Nextion.print((char) 34);
  Nextion.print(contador.Velocidad);    Nextion.print((char) 34);   eofNextion();  
  // RPM  
  Nextion.print("pnlContador.lblRPM.txt=");  Nextion.print((char) 34);
  Nextion.print(RPM);   Nextion.print((char) 34);   eofNextion();  
}

void actualizarEstadoCONT_NEXTION(){
  Nextion.print("pnlContador.estadoCONT.val="); Nextion.print(contador.estadoACTUAL); eofNextion();
}
void actualizarEstadoPROD_NEXTION(){
  Nextion.print("pnlContador.estadoCONT.val="); Nextion.print(contador.estadoACTUAL); eofNextion();
  Nextion.print("pnlProduccion.estadoPROD.val="); Nextion.print(contador.estadoACTUAL); eofNextion();
}

void mostrarTiemposPROD_NEXTION(){
        // NOP_AJUSTE_MAQUINA
        Nextion.print("pnlTiempos.tm2.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_AJUSTE_MAQUINA));    Nextion.print((char) 34);    eofNextion();         
        // NOP_LIMPIEZA_RADICAL
        Nextion.print("pnlTiempos.tm3.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_LIMPIEZA_RADICAL));  Nextion.print((char) 34);    eofNextion();  
        // NOP_CAMBIO_FORMATO
        Nextion.print("pnlTiempos.tm4.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_CAMBIO_FORMATO));    Nextion.print((char) 34);    eofNextion();
        // NOP_CAMBIO_TURNO
        Nextion.print("pnlTiempos.tm5.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_CAMBIO_TURNO));      Nextion.print((char) 34);    eofNextion();
        // NOP_MANTENIMIENTO_PREVENTIVO
        Nextion.print("pnlTiempos.tm8.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO));    Nextion.print((char) 34);    eofNextion();
        // NOP_MANTENIMIENTO_CORRECTIVO
        Nextion.print("pnlTiempos.tm9.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO));    Nextion.print((char) 34);    eofNextion();
        // PRD_SACANDO_MUESTRAS
        Nextion.print("pnlTiempos.tm16.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_SACANDO_MUESTRAS));        Nextion.print((char) 34);    eofNextion();
        // PRD_ESPERANDO_APROBACION
        Nextion.print("pnlTiempos.tm17.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_ESPERANDO_APROBACION));    Nextion.print((char) 34);    eofNextion();        
        // PRD_PRODUCIENDO        
        Nextion.print("pnlTiempos.tm19.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_PRODUCIENDO));    Nextion.print((char) 34);    eofNextion();  
        // PRD_PRODUCCION_DETENIDA
        Nextion.print("pnlTiempos.tm20.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_PRODUCCION_DETENIDA));    Nextion.print((char) 34);    eofNextion();                
        // INT_AJUSTE_MAQUINA
        Nextion.print("pnlTiempos.tm21.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_AJUSTE_MAQUINA));  Nextion.print((char) 34);    eofNextion();        
        // INT_LIMPIEZA_EQUIPO
        Nextion.print("pnlTiempos.tm22.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_LIMPIEZA_EQUIPO)); Nextion.print((char) 34);    eofNextion(); 
        // INT_CAMBIO_INSUMOS
        Nextion.print("pnlTiempos.tm23.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_CAMBIO_INSUMOS));  Nextion.print((char) 34);    eofNextion();
        // INT_MATERIA_PRIMA
        Nextion.print("pnlTiempos.tm24.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_MATERIA_PRIMA));   Nextion.print((char) 34);    eofNextion();        
        // INT_CAMBIO_TURNO
        Nextion.print("pnlTiempos.tm25.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_CAMBIO_TURNO));    Nextion.print((char) 34);    eofNextion();
        //INT_FALLA_MAQUINA
        Nextion.print("pnlTiempos.tm26.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_FALLA_MAQUINA));   Nextion.print((char) 34);    eofNextion();
}

void parametrosTiemposNEXTION(){
    Nextion.print("pnlTiempos.hPROD.txt=");    Nextion.print((char) 34);
    Nextion.print(secToHour(tiempos.horas_PRD_PRODUCIENDO));   Nextion.print((char) 34); eofNextion(); 

    Nextion.print("pnlTiempos.hDET.txt=");     Nextion.print((char) 34);
    Nextion.print(secToHour(tiempos.horas_PRD_PRODUCCION_DETENIDA));   Nextion.print((char) 34); eofNextion(); 
}

void parametrosProduccionNEXTION(){
    //------> ID PRODUCCION
    Nextion.print("pnlProduccion.idProd.txt="); Nextion.print((char) 34); 
    Nextion.print(produccion.idProduccion); Nextion.print((char) 34);  eofNextion(); 
    //------> PRODUCTO
    Nextion.print("pnlProduccion.Producto.txt=");     Nextion.print((char) 34);
    if(getStringPRODUCTO().length()==0)               Nextion.print("SIN PRODUCTO");
    else  Nextion.print(getStringPRODUCTO());         Nextion.print((char) 34); eofNextion();       
    //------> LOTE
    Nextion.print("pnlProduccion.Lote.txt=");         Nextion.print((char) 34);
    if(produccion.Lote==0)                            Nextion.print("SIN LOTE");
    else Nextion.print((String)produccion.Lote);              Nextion.print((char) 34); eofNextion();
    //------> HORA CREACION
    Nextion.print("pnlProduccion.horaNUEVO.txt="); Nextion.print((char) 34);
    if(produccion.horaCREACION==0)           Nextion.print("--:--:-- --/--/----");
    else    Nextion.print((String)produccion.horaCREACION);   Nextion.print((char) 34); eofNextion();     
    //------> HORA INICIO
    Nextion.print("pnlProduccion.horaINICIO.txt=");   Nextion.print((char) 34);
    if(produccion.horaINICIO==0)             Nextion.print("--:--:-- --/--/----");
    else    Nextion.print((String)produccion.horaINICIO);     Nextion.print((char) 34); eofNextion();
    //------> HORA FINAL
    Nextion.print("pnlProduccion.horaFINAL.txt=");    Nextion.print((char) 34);
    if(produccion.horaFINAL==0)              Nextion.print("--:--:-- --/--/----");
    else    Nextion.print((String)produccion.horaFINAL);      Nextion.print((char) 34); eofNextion(); 
    //------> FLAG PRODUCCION
    Nextion.print("pnlProduccion.Salvado.val=");      Nextion.print(produccion.Salvado);  eofNextion();
    //------> Produciendo
    Nextion.print("pnlProduccion.Produciendo.val=");  Nextion.print(produccion.Produciendo); eofNextion(); 
    //------> MuestrasCC
    Nextion.print("pnlProduccion.MuestrasCC.val=");   Nextion.print(produccion.MuestrasCC);   eofNextion();   
    
}


void parametrosContadorNEXTION(){
    //INICIALIZAMOS
    eofNextion();
    //ID
    Nextion.print("pnlContador.ID.txt="); Nextion.print((char) 34);    
    Nextion.print(ID);    Nextion.print((char) 34);  eofNextion(); 
    //Incremento
    Nextion.print("pnlContador.Incremento.txt="); Nextion.print((char) 34);    
    Nextion.print(contador.Incremento);    Nextion.print((char) 34);  eofNextion();
    // conteoTOTAL
    Nextion.print("pnlContador.cntTOTAL.txt=");Nextion.print((char) 34);
    Nextion.print(getDigitos(contador.conteoTOTAL,8));    Nextion.print((char) 34); eofNextion();
    // conteoTURNO
    Nextion.print("pnlContador.cntTURNO.txt=");Nextion.print((char) 34);
    Nextion.print(contador.conteoTURNO);    Nextion.print((char) 34);    eofNextion();
    // conteoPARCIAL
    Nextion.print("pnlContador.cntPARCIAL.txt=");Nextion.print((char) 34);
    Nextion.print(contador.conteoPARCIAL);    Nextion.print((char) 34); eofNextion();
    // Velocidad
    Nextion.print("pnlContador.Velocidad.txt=");Nextion.print((char) 34);
    Nextion.print(contador.Velocidad);    Nextion.print((char) 34);   eofNextion();    
    // Frecuencia
    Nextion.print("pnlContador.Frecuencia.txt=");Nextion.print((char) 34);
    Nextion.print(contador.Frecuencia);    Nextion.print((char) 34);    eofNextion();
    // comparaLAP
    Nextion.print("pnlContador.comparaLAP.txt=");Nextion.print((char) 34);
    Nextion.print(contador.comparaLAP);    Nextion.print((char) 34);  eofNextion();  
    // Muestreo
    Nextion.print("pnlContador.Muestreo.txt=");Nextion.print((char) 34);
    Nextion.print(contador.Muestreo);    Nextion.print((char) 34);    eofNextion();
    // estadoACTUAL
    Nextion.print("pnlContador.estadoCONT.val="); Nextion.print(contador.estadoACTUAL); eofNextion();
    Nextion.print("pnlProduccion.estadoPROD.val="); Nextion.print(contador.estadoACTUAL); eofNextion();
    // totalEVENTOS
    Nextion.print("pnlContador.totalEVENTOS.val="); Nextion.print(contador.totalEVENTOS); eofNextion();
    // Enabled
    Nextion.print("pnlContador.Enabled.val="); Nextion.print(contador.Enabled); eofNextion();        
    mostrarEstadoSTR();
}


void updateTiemposPRODUCCION_NEXTION(){
// TIEMPOS
    switch(contador.estadoACTUAL){
      case PRD_NO_OPERABILIDAD:        
        break;
      case NOP_AJUSTE_MAQUINA:
        Nextion.print("pnlTiempos.tm2.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_AJUSTE_MAQUINA));    Nextion.print((char) 34);    eofNextion();         
        break;        
      case NOP_LIMPIEZA_RADICAL:
        Nextion.print("pnlTiempos.tm3.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_LIMPIEZA_RADICAL));    Nextion.print((char) 34);    eofNextion();  
        break;    
      case NOP_CAMBIO_FORMATO:
        Nextion.print("pnlTiempos.tm4.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_CAMBIO_FORMATO));    Nextion.print((char) 34);    eofNextion();
        break;
      case NOP_CAMBIO_TURNO:
        Nextion.print("pnlTiempos.tm5.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_CAMBIO_TURNO));    Nextion.print((char) 34);    eofNextion();
        break;
      case NOP_SIN_OPERADOR:
        break;
      case NOP_FALLA_MAQUINA:
        break;       
      case NOP_MANTENIMIENTO_PREVENTIVO:
        Nextion.print("pnlTiempos.tm8.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO));    Nextion.print((char) 34);    eofNextion();
        break;
      case NOP_MANTENIMIENTO_CORRECTIVO:
        Nextion.print("pnlTiempos.tm9.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO));    Nextion.print((char) 34);    eofNextion();
        break;
      case NOP_ENSAYOS_DESARROLLO:
        break;
      case PRD_NUEVA_PRODUCCION:
        break;
      case PRD_SACANDO_MUESTRAS:
        Nextion.print("pnlTiempos.tm16.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_SACANDO_MUESTRAS));    Nextion.print((char) 34);    eofNextion();
        break;
      case PRD_ESPERANDO_APROBACION:
        Nextion.print("pnlTiempos.tm17.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_ESPERANDO_APROBACION));    Nextion.print((char) 34);    eofNextion();
        break;
      case PRD_INICIA_PRODUCCION:
        Nextion.print("pnlTiempos.tm20.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_PRODUCCION_DETENIDA));    Nextion.print((char) 34);    eofNextion();
        break;
      case PRD_PRODUCIENDO:
        if (contador.Enabled) {
          Nextion.print("pnlTiempos.tm19.txt="); Nextion.print((char) 34);
          Nextion.print(secToHour(tiempos.horas_PRD_PRODUCIENDO));    Nextion.print((char) 34);    eofNextion();
        }
        else{
          Nextion.print("pnlTiempos.tm21.txt="); Nextion.print((char) 34);
          Nextion.print(secToHour(tiempos.horas_INT_AJUSTE_MAQUINA));    Nextion.print((char) 34);    eofNextion();
        }
        break;        
      case PRD_PRODUCCION_DETENIDA:
        Nextion.print("pnlTiempos.tm20.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_PRD_PRODUCCION_DETENIDA));    Nextion.print((char) 34);    eofNextion();        
        break;
      case INT_AJUSTE_MAQUINA:
        Nextion.print("pnlTiempos.tm21.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_AJUSTE_MAQUINA));    Nextion.print((char) 34);    eofNextion();
        break;    
      case INT_LIMPIEZA_EQUIPO:
      Nextion.print("pnlTiempos.tm22.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_LIMPIEZA_EQUIPO));    Nextion.print((char) 34);    eofNextion();
        break; 
      case INT_CAMBIO_INSUMOS:
        Nextion.print("pnlTiempos.tm23.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_CAMBIO_INSUMOS));    Nextion.print((char) 34);    eofNextion();
        break; 
      case INT_MATERIA_PRIMA:
        Nextion.print("pnlTiempos.tm24.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_MATERIA_PRIMA));    Nextion.print((char) 34);    eofNextion();
        break;
      case INT_CAMBIO_TURNO:
        Nextion.print("pnlTiempos.tm25.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_CAMBIO_TURNO));    Nextion.print((char) 34);    eofNextion();
        break;
      case INT_FALLA_MAQUINA:
        Nextion.print("pnlTiempos.tm26.txt="); Nextion.print((char) 34);
        Nextion.print(secToHour(tiempos.horas_INT_FALLA_MAQUINA));    Nextion.print((char) 34);    eofNextion();
        break;       
      case INT_MANTENIMIENTO_CORRECTIVO:
        break;        
      case PRD_FINALIZO_PRODUCCION:
        break;        
    }
}
