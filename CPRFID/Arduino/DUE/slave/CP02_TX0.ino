

// Verifica si el buffer de transmision se encuentra lleno.
void verificarTransmision(){
    // Transmitir TIEMPOS DE PRODUCCION   
    //if (segundosTX % (MAX_TIME_TX/2) == 0){
        // 3 segundos
        if(recibidoEstado == verificarEstado || vecesTX >= MAX_TIMES_CONFIRM){
           strRetransmitir="";           
        }
    //}
    // 30 minutos
    if(segundosTX % 1800 == 905 && produccion.Produciendo){
        retransmitirDatosPRODUCCION();
    }
    if(segundosTX % MAX_TIME_TX == 0){     //00,10,20,30,40,50,00
     
        // Si la transmision se completo => Verificamos si hay mas transmisiones

        // Si aun no recibe la confirma la recepcion.
        if(strRetransmitir.length() != 0)  {
          realizarComunicacion();
          // Se incrementa el numero de envios.
          vecesTX++;  
          // Se adelanta un segundo al transmitir.
          delay(250);
        }
        else{
            if (strTransmitir.length()!=0)
                realizarTransmision();   
            // 1 minuto     
            if(segundosTX % 60 == 0 && enableTX){      
                transmitirEstadoACTUAL(); 
            }     
            else{
              // 6 segundos
              if(segundosTX % MAX_TIME_TX == 0 ){ 
                  if(contador.estadoACTUAL == PRD_NO_OPERABILIDAD || contador.estadoACTUAL == PRD_FINALIZO_PRODUCCION ){
                      // 60 segundos
                      if(segundosTX % 60 == 0) {
                         transmitirTiemposPRODUCCION();
                        if(segundosTX % TIME_RESET_PROD == 0 && contador.estadoACTUAL == PRD_FINALIZO_PRODUCCION){
                            // Transmite todos los registros.
                            
                            //readFileSDCard("/EVNT/"+getFileName());
                            //delay(2000);
                            resetearPRODUCCION();                              
                            //mostrarPageContadores();
                        }
                     }
                  }
                  else{
                     // 6 segundos
                     if(contador.estadoACTUAL>=NOP_AJUSTE_MAQUINA && contador.estadoACTUAL<PRD_FINALIZO_PRODUCCION && enableTX){
                         transmitirTiemposPRODUCCION();  
                     }
                     else{
                         //6 segundos
                         if(segundosTX % (2*MAX_TIME_TX) == 0)           
                           transmitirTiemposPRODUCCION();
                     }
                 }
              }          
           }
       }
    }
    segundosTX++;  
}
// Realiza la transmicion si existe algun dato en la Lista de Transmision.
void realizarTransmision(){
    Serial.write(6);
    Serial.print(ID); 
    //Serial.print(strTransmitir);Serial.print("ESTD~");Serial.print(contador.estadoACTUAL);Serial.print("|");Serial.print(lstTransmitir.count()); Serial.print("-");Serial.print(segundosTX % 100);    
    Serial.write(3);Serial.println("");
    strTransmitir="";
}
// Realiza la transmicion si existe algun dato en la Lista de Transmision.
void realizarComunicacion(){
    Serial.write(6);
    Serial.print(ID); 
    //Serial.print(lstTransmitir.pop());Serial.print(lstTransmitir.count()); Serial.print("-");Serial.print(segundosTX);
    //Serial.print(strRetransmitir);Serial.print(lstTransmitir.count()); Serial.print("-");Serial.print(segundosTX%100);
    Serial.write(3);Serial.println("");   
}

// Transmite el estado Actual del CONTADOR
void transmitirEstadoACTUAL(){
    strTransmitir = "";
    strTransmitir += ("|IPRO~"+(String) produccion.idProduccion);
    strTransmitir += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    //strTransmitir += ("|TURN~"+(String)contador.conteoTURNO) ;
    strTransmitir += ("|VELC~"+(String)contador.Velocidad) ;
    strTransmitir += ("|");         
}
void transmitirNuevoEVENTO(){
    contador.totalEVENTOS++;   
    strComunicar = "";
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL);
    strComunicar += ("|TURN~"+(String)contador.conteoTURNO) ;
    strComunicar += ("|VELC~"+(String)contador.Velocidad) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)contador.estadoACTUAL);   
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());     
}
// transmite los Tiempos de PRODUCCION
void transmitirTiemposPRODUCCION(){
    strTransmitir = "";
    strTransmitir += ("|IPRO~"+(String) produccion.idProduccion);
    strTransmitir += ("|TOTL~"+(String) contador.conteoTOTAL) ;
    //strComunicar += ("|TURN~"+(String)contador.conteoTURNO) ;
    strTransmitir += ("|TEST~"+(String) contador.estadoACTUAL);

    switch(contador.estadoACTUAL){
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
      if (contador.Enabled)   strTransmitir += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCIENDO) ;
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
    strTransmitir += ("|");          
}
// Rutina de transmision del conteoPARCIAL del contador (cada MUESTREO)
void transmitirConteoPARCIAL(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    //strComunicar += ("|TURN~"+(String)contador.conteoTURNO) ;
    strComunicar += ("|PARC~"+(String)contador.conteoPARCIAL) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)MSG_MUESTREO_PARCIAL);     
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());
    transmitirEstadoACTUAL();
}
// Rutina de transmision del conteoHORA del contador (cada HORA HH:00:00)
void transmitirConteoHORA(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|ICAP~"+(String)id_captura) ;
    strComunicar += ("|TCAP~"+(String)contador.conteoHORA) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)MSG_CAPTURA_CONTEOHORA);     
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());
    transmitirEstadoACTUAL();
}
// Rutina de transmision del almacenamiento de la nueva Produccion.
void transmitirNuevaPRODUCCION(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|HNEW~"+ produccion.horaCREACION);
    strComunicar += ("|IPRO~"+(String) produccion.idProduccion);
    strComunicar += ("|PROD~"+ getStringPRODUCTO());
    strComunicar += ("|LOTE~"+ produccion.Lote);    
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS);
    strComunicar += ("|EVNT~"+(String)PRD_NUEVA_PRODUCCION);
    strComunicar += ("|");
    strComunicar.replace((char)127,(char)209); strComunicar.replace((char)128,(char)241); 
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());         
}

// Rutina de transmision del Inicio de Produccion.
void transmitirIniciarPRODUCCION(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|IPRO~"+(String) produccion.idProduccion);
    //strComunicar += ("|PROD~"+getStringPRODUCTO()) ;
    //strComunicar += ("|LOTE~"+produccion.Lote) ;
    strComunicar += ("|HINI~"+produccion.horaINICIO) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS);
    strComunicar += ("|EVNT~"+(String)PRD_INICIA_PRODUCCION) ;    
    strComunicar += ("|"); 
    //strComunicar.replace((char)127,(char)209); strComunicar.replace((char)128,(char)241);     
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());
}

// Rutina de trnasmision cuando Finaliza Produccion.
void  transmitirFinalizarPRODUCCION(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|IPRO~"+(String) produccion.idProduccion);
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    strComunicar += ("|TURN~"+(String)contador.conteoTURNO) ;
    strComunicar += ("|PARC~"+(String)contador.conteoPARCIAL) ;
    strComunicar += ("|HFIN~"+ produccion.horaFINAL) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS);
    strComunicar += ("|EVNT~"+(String)PRD_FINALIZO_PRODUCCION) ;
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());   
}

void retransmitirDatosPRODUCCION(){
    strTransmitir  = "";
    strTransmitir += ("|IPRO~"+(String) produccion.idProduccion);
    strTransmitir += ("|PROD~"+ getStringPRODUCTO()) ;
    strTransmitir += ("|LOTE~"+ produccion.Lote) ;
    strTransmitir += ("|HNEW~"+ produccion.horaCREACION);
    strTransmitir += ("|HINI~"+ produccion.horaINICIO) ;
    strTransmitir += ("|EVNT~"+(String) MSG_RETRANSMITE_PRODUCCION) ;
    strTransmitir += ("|");   
    realizarTransmision();
}
//*****************************************************************************
//******************      EVENTOS DE PRODUCCION      **************************
//*****************************************************************************
void transmitirContadorENCENDIDO(){
    contador.totalEVENTOS++; 
    strComunicar = "";
    strComunicar += "|TNOW~???";
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)MSG_CONTADOR_ENCENDIDO) ;        
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar); 
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());   
}
void transmitirParametrosCONTEO(){
    strTransmitir = "";
    strTransmitir += ("|INCR~"+(String)contador.Incremento) ;
    strTransmitir += ("|LAPS~"+(String)contador.Muestreo) ;
    strTransmitir += ("|COMP~"+(String)contador.comparaLAP ) ;
    strTransmitir += ("|");     
}
void transmitirContinuaCONTEO(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)PRD_PRODUCIENDO) ;    
    strComunicar += ("|ESTD~"+(String)contador.estadoACTUAL) ;    
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar); 
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());       
}
void transmitirContinuaPRODUCCION(){
    contador.totalEVENTOS++;
    strComunicar = "";    
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)MSG_CONTINUA_PRODUCCION) ;    
    strComunicar += ("|ESTD~"+(String)contador.estadoACTUAL) ;    
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());   
} 
void transmitirContadorDETENIDO(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    strComunicar += ("|VELC~"+(String)contador.Velocidad) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS); 
    strComunicar += ("|EVNT~"+(String)PRD_PRODUCCION_DETENIDA) ;    
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());   
} 
void transmitirReseteaCONTADOR(){
    contador.totalEVENTOS++;
    strComunicar = "";    
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    strComunicar += ("|TURN~"+(String)contador.conteoTURNO) ;
    strComunicar += ("|PARC~"+(String)contador.conteoPARCIAL) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS);
    strComunicar += ("|EVNT~"+(String)MSG_RESETEA_CONTADOR) ;
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());    
} 
void transmitirReseteaPRODUCCION(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|IPRO~"+(String) produccion.idProduccion);
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    //strComunicar += ("|TURN~"+(String)contador.conteoTURNO) ;
    strComunicar += ("|PARC~"+(String)contador.conteoPARCIAL) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS);
    strComunicar += ("|EVNT~"+(String)MSG_RESETEA_PRODUCCION) ;     
    strComunicar += ("|");    
    Serial.println(strComunicar);
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());    
} 
void transmitirCancelarPRODUCCION(){
    contador.totalEVENTOS++;
    strComunicar = "";
    strComunicar += ("|TOTL~"+(String)contador.conteoTOTAL) ;
    strComunicar += ("|PARC~"+(String)contador.conteoPARCIAL) ;
    strComunicar += ("|TEVT~"+(String)contador.totalEVENTOS);
    strComunicar += ("|EVNT~"+(String)MSG_CANCELAR_PRODUCCION) ;
    strComunicar += ("|");
    Serial.println(strComunicar); 
    //writeSDCardEVNT(strComunicar);
    //myFrame.setText(lblBufferTX       ,(String) lstTransmitir.count());   
}
/*----------------------------------------------------------------*/
boolean isNumeric(String cadena){
    byte i,c=0;
    for(i=0;i<cadena.length();i++)
        if(isDigit(cadena.charAt(i)))  c++;    
    if (c==cadena.length() && c!=0)    return true;
    else   return false;  
} 
void mostrarParametrosContador(){
  //-----------------------------------------------------------------------------
  // CONTADOR
  Serial.println("=====================");
  Serial.print("ID: ");                 Serial.println(ID);
  Serial.print("INCREMENTO: ");         Serial.println(contador.Incremento);
  Serial.print("MUESTREO: ");           Serial.println(contador.Muestreo);
  Serial.print("CONTEO TOTAL: ");       Serial.println(contador.conteoTOTAL);
  Serial.print("CONTEO ANTERIOR: ");    Serial.println(contador.conteoANTERIOR);
  Serial.print("CONTEO TURNO: " );      Serial.println(contador.conteoTURNO);
  Serial.print("TIEMPO COMP(ms): ");    Serial.println(contador.comparaLAP);
  Serial.print("TIEMPO MUESTREO(s): "); Serial.println(contador.Muestreo);
  Serial.print("FRECUENCIA CONTEO: " ); Serial.println(contador.Frecuencia);
  Serial.print("ESTADO ACTUAL: " );     Serial.println(contador.estadoACTUAL);
  Serial.print("TOTAL EVENTOS: " );     Serial.println(contador.totalEVENTOS);
  Serial.print("CONTEO HABILITADO: ");  Serial.println(contador.Enabled);
  Serial.println("=====================");
  //-----------------------------------------------------------------------------
  // PRODUCCION
  Serial.print("ID PRODUCTO: ");      Serial.println(produccion.idProduccion);
  Serial.print("NOMBRE PRODUCTO: ");  Serial.println(getStringPRODUCTO());
  Serial.print("NUMERO LOTE: ");      Serial.println(produccion.Lote);
  Serial.print("HORA CREACION: ");    Serial.println(produccion.horaCREACION);
  Serial.print("HORA INICIO: ");      Serial.println(produccion.horaINICIO);
  Serial.print("HORA FINAL: ");       Serial.println(produccion.horaFINAL);
  Serial.print("PRODUCIENDO: ");      Serial.println(produccion.Produciendo);
  Serial.print("GUARDADO: ");         Serial.println(produccion.Salvado);
  Serial.print("SACANDO MUESTRAS: "); Serial.println(produccion.MuestrasCC);
  Serial.println("=====================");
}
