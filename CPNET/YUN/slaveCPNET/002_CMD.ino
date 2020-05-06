String resultado="";
boolean flagGuardar = false;
/*=================================================================*/
// Verifica si la cadena recibida es un comando de modificacion.
void decodificarRecepcion(String strRecepcion){
    word index=-1,iniStr=0,finStr=0;
    strDecodificar="|";
    comando="", parametro="";
    if((strRecepcion.startsWith(getRFID()) || strRecepcion.startsWith("RFID???")) && strRecepcion.length()>0){
        //___________________________________________________________________
        //boolean decodificar=false;
        flagGuardar = false;
        while(index != strRecepcion.lastIndexOf("|")){            
            int index_ini,index_fin;
            iniStr=index+1;
            finStr=strRecepcion.indexOf("|",index+1);            
            parametroRecibido = strRecepcion.substring(iniStr,finStr);
            index = finStr;                              
            index_ini = parametroRecibido.indexOf("=");   
            index_fin = parametroRecibido.lastIndexOf("=");   
            if ((index_ini != -1) && (index_ini == index_fin)){
                comando   = parametroRecibido.substring(0,index_ini);                    
                parametro = parametroRecibido.substring(index_fin+1);
                ejecutarComando();
                strDecodificar += respuesta;                    
            }
        }
        if (flagGuardar)
          guardarParametros();
    }
}

void ejecutarComando(){
  respuesta="";
  //_____________________ ID = RFIDXXX ___________________________
  if (comando == "ID"){
    if (parametro=="?")
      resultado=getRFID();
    else{
      if (parametro.startsWith("RFID") && parametro.length()==7 && isNumeric(parametro.substring(4,7))){
        contador.id=parametro.substring(4,7).toInt();
        flagGuardar = true;
        resultado=getRFID();        
      }
      else
        resultado="ERROR";              
    }
    respuesta+=(comando+":"+resultado+"|");
  }
  //______________________ INC = X (0<X<10) ___________________________
  if (comando == "INCR"){    
    if (parametro == "?")             
      resultado = (String) contador.Incremento;
    else{
        if(isNumeric(parametro)){
            byte inc = (byte) parametro.toInt();
            if (inc <=10 && inc >0){
                contador.Incremento=inc;
                flagGuardar = true;
                resultado = (String) contador.Incremento;
            }
            else  resultado="ERROR";
        }
        else resultado="ERROR";
    }
    respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ TOTAL = X  (X>=0) ___________________________    
  if (comando == "TOTL"){
    if (parametro == "?")
        resultado = conteo.TOTAL;
    else{
      if(isNumeric(parametro) && (parametro.length()<9)){
        conteo.TOTAL = parametro.toInt();
        flagGuardar = true;
        resultado = (String) conteo.TOTAL;
      }
      else
        resultado = "ERROR";
    }
    respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ TURN=? ___________________________
  if (comando == "TURN"){
    if (parametro =="?")
        resultado = (String) conteo.TURNO;
    else{
      if(isNumeric(parametro) && (parametro.length()<9)){
        conteo.TURNO = parametro.toInt();
        flagGuardar = true;
        resultado = (String) conteo.TURNO;
      }
      else
        resultado = "ERROR";
    }
    respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ PROH=? ___________________________    
  if (comando == "PROH"){
    if (parametro =="?")
      resultado = (String)velocidad.HORA;
     else
        resultado = "ERROR";
    respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ COMP=? ___________________________ 
  if (comando == "COMP"){
    if (parametro =="?")
      resultado = (String) contador.comparaLAP;
    else{
      if(isNumeric(parametro) && (parametro.length()<9)){
        contador.comparaLAP = parametro.toInt();
        flagGuardar = true;
        resultado = (String) contador.comparaLAP;       
      }
      else 
        resultado = "ERROR";                                  
    }
    respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ ESTADO=? ___________________________    
  if (comando == "ESTD"){
      if (parametro =="?")
          resultado = (String)estado.ACTUAL;
      else {
        if(parametro.toInt()>=  MSG_CONTADOR_ENCENDIDO && parametro.toInt()<= MSG_CANCELAR_PRODUCCION){
          estado.ACTUAL = parametro.toInt();
          resultado = (String)estado.ACTUAL;
        }
        else 
          resultado = "ERROR";
      }
      respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ CTEN=? ___________________________    
  if (comando == "CTEN"){
      if (parametro =="?")
        resultado = (String)estado.ACTIVO;
      else{          
        if(parametro=="0"){
          detenerContador();
          flagGuardar = true;
          resultado = (String)estado.ACTIVO;
        }
        else{
          if(parametro=="1"){
            iniciarContador();
            resultado = (String)estado.ACTIVO;
          }
          else
            resultado = "ERROR";
        }
      }
      respuesta+=(comando+":"+resultado+"|");
  }
 //_____________________ LOTE = X  (X>=0) ___________________________    
  if (comando == "LOTE"){
    if (parametro =="?")
      resultado = String(produccion.LOTE, HEX);
    else{
      if(parametro.length()<10 && isNumeric(parametro)){
        produccion.LOTE = parametro.toInt();
        resultado = String(produccion.LOTE, HEX);
      }
      else 
        respuesta += (comando+":"+resultado+"|");                                  
    }
  }
  //_____________________ UNIX = (timestamp) ___________________________
  if (comando == "UNIX"){
    if (parametro=="?")
        resultado=(String)getUnixTime();
    else
        resultado="ERROR";
    respuesta+=(comando+":"+resultado+"|");
  }
  //_____________________ UNIX = (timestamp) ___________________________
  if (comando == "TNOW"){
    long valParametro=parametro.toInt();
    if (valParametro>EPOCH_COMPILE){
        setUnixTime(valParametro);
    }
  }
  //_____________________ EVENTOS DEL CONTADOR ___________________________
  if (comando == "EVNT"){
    byte evento = parametro.toInt();
    switch(evento){
      case MSG_CONTADOR_ENCENDIDO:
        break;
      case PRD_NO_OPERABILIDAD:
      case NOP_AJUSTE_MAQUINA:
      case NOP_LIMPIEZA_RADICAL:
      case NOP_CAMBIO_FORMATO:
      case NOP_CAMBIO_TURNO:
      case NOP_SIN_OPERADOR:
      case NOP_FALLA_MAQUINA:
      case NOP_MANTENIMIENTO_PREVENTIVO:
      case NOP_MANTENIMIENTO_CORRECTIVO:
      case NOP_ENSAYOS_DESARROLLO:
        detenerContador();
        estado.ACTUAL=evento;
        EEPROM.put(EEPROM_DIR_ESTADO,     estado);
        break;       
      case PRD_NUEVA_PRODUCCION:
          nuevaProduccion();
        break;
      case PRD_SACANDO_MUESTRAS:
        conteo.TOTAL=0; iniciarContador();
        estado.ACTUAL = PRD_SACANDO_MUESTRAS;
        EEPROM.put(EEPROM_DIR_ESTADO,     estado);         
        break;
      case PRD_ESPERANDO_APROBACION:   
        detenerContador();     
        estado.ACTUAL = PRD_ESPERANDO_APROBACION;
        EEPROM.put(EEPROM_DIR_ESTADO,     estado);
        break;
      case PRD_INICIA_PRODUCCION:
        iniciarProduccion();
        break;
      case PRD_PRODUCCION_DETENIDA:
        estado.ACTUAL=PRD_PRODUCCION_DETENIDA;
        EEPROM.put(EEPROM_DIR_CONTEO,     conteo);
        EEPROM.put(EEPROM_DIR_ESTADO,     estado);
        break;
      case INT_AJUSTE_MAQUINA:
      case INT_LIMPIEZA_EQUIPO:
      case INT_CAMBIO_INSUMOS:
      case INT_MATERIA_PRIMA:
      case INT_CAMBIO_TURNO:
      case INT_FALLA_MAQUINA:     
      case INT_MANTENIMIENTO_CORRECTIVO:
        detenerContador();  
        actualizarEstadoCONTADOR(evento);
        EEPROM.put(EEPROM_DIR_CONTEO,     conteo);
        EEPROM.put(EEPROM_DIR_ESTADO,     estado);
        break;  
      case PRD_FINALIZO_PRODUCCION:
        finalizarProduccion();
        break;
      case MSG_RESETEA_CONTADOR:
        resetearContador(true);
        break;
      case MSG_RESETEA_PRODUCCION:
        resetearPRODUCCION();
        break;
      case MSG_CONTINUA_PRODUCCION:
        estado.ACTUAL=PRD_PRODUCCION_DETENIDA;
        EEPROM.put(EEPROM_DIR_CONTEO,     conteo);
        EEPROM.put(EEPROM_DIR_ESTADO,     estado);
        //flagContador=false;
        break;
    }        
  }
}
