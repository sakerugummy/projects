/*=================================================================*/
// Verifica si la cadena recibida es un comando de modificacion.
void decodificarRecepcion(String strRecepcion){
    word index=-1,iniStr=0,finStr=0;
    strDecodificar="|";
    comando="", parametro="";

    if((strRecepcion.startsWith(ID) || strRecepcion.startsWith("RFID???")) && strRecepcion.length()>0){
        //___________________________________________________________________
        //boolean decodificar=false;
        while(index != strRecepcion.lastIndexOf("|")){            
            int index_ini,index_fin;
            iniStr=index+1;
            finStr=strRecepcion.indexOf("|",index+1);            
            parametroRecibido = strRecepcion.substring(iniStr,finStr);
            //Serial.print("Recibido:"); Serial.println(parametroRecibido); 
            index = finStr;                              
            index_ini = parametroRecibido.indexOf("=");   
            index_fin = parametroRecibido.lastIndexOf("=");   
            if ((index_ini != -1) && (index_ini == index_fin)){
                comando   = parametroRecibido.substring(0,index_ini);                    
                parametro = parametroRecibido.substring(index_fin+1);
                //Serial.print("Comando: ");  Serial.println(comando);
                //Serial.print("Parametro: ");  Serial.println(parametro);
                ejecutarComando();
                strDecodificar += respuesta;
                Serial.println(strDecodificar);                  
            }            
        }
    }
}
/*=================================================================*/
void ejecutarComando(){
    respuesta="";
    //_____________________ ID = RFIDXXX ___________________________
    if (comando == "ID"){
        if(parametro =="??")
            respuesta+=("ID~RFID0XX (00<XX<100)|");
        else{
            if (parametro =="?") 
                respuesta+=("ID~"+ID+"|");        
            else{ 
                if (parametro.startsWith("RFID") && parametro.length()==7 && isNumeric(parametro.substring(4,7))){
                    ID=parametro;
                    contador.id=parametro.substring(4,7).toInt();
                    flagGuardar=true;
                    respuesta+=("ID~"+ID+"|"); 
                }
                else
                    respuesta+="ID~ERROR|";                
            }
        }
    }    
    //______________________ INC = X (0<X<10) ___________________________
    if (comando == "INCR"){
        if(parametro == "??")
            respuesta+="INCR~X  (0<X<10)|";
        else{
            if (parametro == "?"){             
                respuesta+=("INCR~"+ (String)contador.Incremento +"|");
            }
            else{
                if(isNumeric(parametro)){
                    byte inc = parametro.toInt();
                    if (inc <=10 && inc >0){
                        contador.Incremento=inc;
                        flagGuardar=true;   
                        respuesta+=("INCR~"+ (String)contador.Incremento +"|");    
                    }
                    else respuesta+="INCR~ERROR";                   
                }
                else respuesta+="INCR~ERROR";  
            }
        }
    }
    //_____________________ TOTAL = X  (X>=0) ___________________________    
    if (comando == "TOTL"){
        if (parametro == "??")
            respuesta+=("TOTL~X (X>=0)|");        
        else{
            if (parametro =="?"){
                respuesta += ("TOTL~"+ (String)contador.conteoTOTAL +"|");
            }
            else{
                if(isNumeric(parametro) && (parametro.length()<9)){
                   contador.conteoTOTAL = parametro.toInt(); 
                   flagGuardar=true; 
                   contador.conteoANTERIOR=contador.conteoTOTAL;                
                   //Evento="S.INIT";           
                   respuesta += ("TOTL~"+ (String)contador.conteoTOTAL +"|");
               }
               else 
                  respuesta+="TOTL~ERROR";                                  
           }
        }               
    }
    if (comando == "TURN"){
        if (parametro == "??")
            respuesta+=("TURN~X (X>=0)|");        
        else{
            if (parametro =="?"){
                respuesta += ("TURN~"+ (String)contador.conteoTURNO +"|");
            }
            else{
                if(isNumeric(parametro) && (parametro.length()<9)){
                   contador.conteoTURNO = parametro.toInt(); 
                   flagGuardar=true;          
                   respuesta += ("TURN~"+ (String)contador.conteoTURNO +"|");
               }
               else 
                  respuesta+="TABS~ERROR";                                  
           }
        }               
    }
    //_____________________ ANTE=? ___________________________    
    if (comando == "ANTE"){
        if (parametro =="?"){
            respuesta += ("ANTE"+ (String) contador.conteoANTERIOR +"|");
        }
        else 
            respuesta+="ANTE~ERROR";
    }
    //_____________________ PARC=? ___________________________    
    if (comando == "PARC"){
        if (parametro =="?"){
            respuesta += ("PARC~"+ (String)contador.conteoPARCIAL +"|");
        }
        else 
            respuesta+="PARC~ERROR";
    }
    //_____________________ CONH=? ___________________________    
    if (comando == "CONH"){
        if (parametro =="?"){
            respuesta += ("CONH~"+ (String)contador.conteoHORA +"|");
        }
        else 
            respuesta+="CONH~ERROR";
    }
    //_____________________ VELC=? ___________________________    
    if (comando == "VELC"){
        if (parametro =="?"){
            respuesta += ("VELC~"+ (String)contador.Velocidad +"|");
        }
        else 
            respuesta+="VELC~ERROR";
    }
    //_____________________ COMP=? ___________________________ 
    if (comando == "COMP"){
        if (parametro == "??")
            respuesta+=("COMP~X [s] (3000<X<10000)|");        
        else{
            if (parametro =="?"){
                respuesta += ("COMP~"+ (String) contador.comparaLAP +"|");
            }
            else{
                if(isNumeric(parametro) && (parametro.length()<9)){
                   contador.comparaLAP = parametro.toInt();        
                   flagGuardar=true;
                   respuesta += ("COMP~"+ (String)contador.comparaLAP +"|");
                   //horaLAP=0;         
               }
               else 
                  respuesta+="COMP~ERROR";                                  
            }
        }               
    }
    //___________________ LAP=X [s] (60<X<3600) _________________________   
    if (comando == "LAPS"){
        if (parametro == "??")
            respuesta+=("LAPS~X [s] (60<X<3600)|");        
        else{
            if (parametro =="?"){
                respuesta += ("LAPS~"+ (String) contador.Muestreo +"|");
            }
            else{
               if(isNumeric(parametro) && (parametro.length()<9)){
                  contador.Muestreo = parametro.toInt();        
                  flagGuardar=true; 
                  respuesta += ("LAPS~"+ (String)contador.Muestreo +"|");
                  //horaLAP=0;         
               }
               else 
                  respuesta+="LAPS~ERROR";                                  
            }
        }               
    }
    if (comando == "FREC"){
        if(parametro == "??")
            respuesta+="FREC~X  (0<X<200)|";
        else{
            if (parametro == "?"){             
                respuesta+=("FREC~"+ (String)contador.Frecuencia +"|");
            }
            else{
                if(isNumeric(parametro)){
                    byte frec = parametro.toInt();
                    if (frec <=200 && frec >0){
                        contador.Frecuencia=frec;
                        flagGuardar=true;  
                        respuesta+=("INCR~"+ (String)contador.Frecuencia +"|");    
                    }
                    else respuesta+="INCR~ERROR";                   
                }
                else respuesta+="INCR~ERROR";  
            }
        }
    }


    //_____________________ ESTADO=? ___________________________    
    if (comando == "ESTD"){
        if (parametro =="?"){            
            respuesta += ("ESTD~"+ (String)contador.estadoACTUAL +"|");             
        }
        else {
          if(parametro.toInt()>=  MSG_CONTADOR_ENCENDIDO && parametro.toInt()<= MSG_CANCELAR_PRODUCCION){
              contador.estadoACTUAL = parametro.toInt();
              flagGuardar=true; 
          }
          else{
            respuesta+="ESTD~ERROR";
          }
        }
    }
    //_____________________ TEVT=? ___________________________    
    if (comando == "TEVT"){
        if (parametro =="?"){
            respuesta += ("CONH~"+ (String)contador.totalEVENTOS +"|");
        }
        else 
            respuesta+="TEVT~ERROR";
    }
    //_____________________ CTEN=? ___________________________    
    if (comando == "CTEN"){
        if (parametro =="?"){
            respuesta += ("CTEN~"+ (String)contador.Enabled +"|");
        }
        else 
            respuesta+="TEVT~ERROR";
    }
        
    //_____________________ ID PRODUCCION 4 BYTES ___________________________    
    if (comando == "IPRO"){
        if(isNumeric(parametro)){
           produccion.idProduccion = parametro.toInt();
           flagGuardar=true;
           myFrame.setText(lblidPRODUCCION, (String)produccion.idProduccion);        
       }           
    }
    //_____________________ PRODUCTO = X  (50 caracteres) ___________________________    
    if (comando == "PROD"){
        if (parametro == "??")
            respuesta+=("PROD~X (Nombre de 50 caracteres)|");        
        else{
            if (parametro == "?")
                respuesta += ("PROD~"+ getStringPRODUCTO() +"|");            
            else{
                if(parametro.length()<=MAX_LEN_PRODUCTO){
                   setStringPRODUCTO(parametro);
                   flagGuardar=true; 
                   respuesta += ("PROD~"+ getStringPRODUCTO() +"|");        
               }
               else 
                  respuesta+="PROD~ERROR";                                  
           }
        }               
    }    
    //_____________________ LOTE = X  (X>=0) ___________________________    
    if (comando == "LOTE"){
        if (parametro == "??")
            respuesta+=("LOTE~X (X Numero de 10 Digitos)|");        
        else{
            if (parametro =="?"){
                respuesta += ("LOTE~"+ (String) produccion.Lote +"|");
            }
            else{
                if(isNumeric(parametro)){
                  
                   produccion.Lote = parametro.toInt();
                   Serial.println(produccion.Lote);
                   flagGuardar=true;                       
                   respuesta += ("LOTE~"+ (String)produccion.Lote +"|");        
               }
               else 
                  respuesta+="LOTE~ERROR";                                  
           }
        }               
    }
    //_____________________ HNEW = DD/MM/YYYY HH:MM:SS ___________________________    
    if (comando == "HNEW"){
        if (parametro == "??")
            respuesta+=("NEW~DD/MM/YYYY HH:MM:SS|");        
        else{
            if (parametro =="?")
                respuesta += ("HNEW~"+ (String)produccion.horaCREACION+"|");            
            else{
                if(isNumeric(parametro) ){
                   produccion.horaCREACION = parametro.toInt();
                   flagGuardar=true;                      
                   respuesta += ("HNEW~"+ (String)produccion.horaCREACION+"|");    
               }
               else 
                  respuesta+="HNEW~ERROR";                                  
           }
        }               
    }
    //_____________________ HINI = DD/MM/YYYY HH:MM:SS ___________________________    
    if (comando == "HINI"){
        if (parametro == "??")
            respuesta+=("HINI~DD/MM/YYYY HH:MM:SS|");        
        else{
            if (parametro =="?")
                respuesta += ("HINI~"+ (String)produccion.horaINICIO+"|");            
            else{
                if(isNumeric(parametro) ){
                   produccion.horaINICIO = parametro.toInt();
                   flagGuardar=true;                       
                   respuesta += ("HINI~"+ (String)produccion.horaINICIO+"|");    
               }
               else 
                  respuesta+="HINI~ERROR";                                  
           }
        }               
    }
    //_____________________ HFIN = DD/MM/YYYY HH:MM:SS ___________________________    
    if (comando == "HFIN"){
        if (parametro == "??")
            respuesta+=("HFIN~DD/MM/YYYY HH:MM:SS|");        
        else{
            if (parametro =="?")
                respuesta += ("HFIN~"+ (String)produccion.horaFINAL+"|");            
            else{
                if(isNumeric(parametro) ){
                   produccion.horaFINAL = parametro.toInt();
                   flagGuardar=true;                       
                   respuesta += ("HFIN~"+ (String)produccion.horaFINAL+"|");    
               }
               else 
                  respuesta+="HFIN~ERROR";                                  
           }
        }               
    }
    if (comando == "PMCC"){
        if (parametro =="?"){
            respuesta += ("PMCC~"+ (String) produccion.MuestrasCC +"|");
        }
        else{
                if(parametro=="1"){
                        produccion.MuestrasCC=true;
                        flagGuardar=true;
                        respuesta += ("PMCC~"+ (String)produccion.MuestrasCC +"|");
      
                }
               else{
                  if(parametro=="0"){
                          produccion.MuestrasCC=false;
                          flagGuardar=true;
                          respuesta += ("PMCC~"+ (String)produccion.MuestrasCC +"|");
        
                  }
                  else
                    respuesta+="PMCC~ERROR";                                  
               }
        }
    }
    //_____________________ TEST=? ___________________________ 
    // TIEMPO en que la maquina estuvo DETENIDA POR NO OPERABILIDAD.      
    if (comando == "TEST"){
        byte pos = parametro.toInt();
        switch(pos){
            case PRD_NO_OPERABILIDAD: 
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_NO_OPERABILIDAD +"|") ;        
              break;
            case NOP_AJUSTE_MAQUINA:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_AJUSTE_MAQUINA +"|") ;
              break;        
            case NOP_LIMPIEZA_RADICAL:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_LIMPIEZA_RADICAL +"|") ;
              break;    
            case NOP_CAMBIO_FORMATO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_CAMBIO_FORMATO +"|") ;
              break;
            case NOP_CAMBIO_TURNO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_CAMBIO_TURNO +"|") ;
              break;
            case NOP_SIN_OPERADOR:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_SIN_OPERADOR +"|") ;
              break;
            case NOP_FALLA_MAQUINA:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_FALLA_MAQUINA +"|") ;
              break;       
            case NOP_MANTENIMIENTO_PREVENTIVO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO +"|") ;
              break;
            case NOP_MANTENIMIENTO_CORRECTIVO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO +"|") ;
              break;
            case NOP_ENSAYOS_DESARROLLO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_NOP_ENSAYOS_DESARROLLO +"|") ;
              break;
            case PRD_NUEVA_PRODUCCION:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_NUEVA_PRODUCCION +"|") ;
              break;
            case PRD_SACANDO_MUESTRAS:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_SACANDO_MUESTRAS +"|") ;
              break;
            case PRD_ESPERANDO_APROBACION:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_ESPERANDO_APROBACION +"|") ;
              break;
            case PRD_INICIA_PRODUCCION:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCIENDO +"|") ;
              break;
            case PRD_PRODUCIENDO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCIENDO +"|") ;       
              break;        
            case PRD_PRODUCCION_DETENIDA:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_PRODUCCION_DETENIDA +"|") ;
              break;
            case INT_AJUSTE_MAQUINA:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_AJUSTE_MAQUINA +"|") ;
              break;    
            case INT_LIMPIEZA_EQUIPO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_LIMPIEZA_EQUIPO +"|") ;
              break; 
            case INT_CAMBIO_INSUMOS:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_CAMBIO_INSUMOS +"|") ;
              break; 
            case INT_MATERIA_PRIMA:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_MATERIA_PRIMA +"|") ;
              break;
            case INT_CAMBIO_TURNO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_CAMBIO_TURNO +"|") ;
              break;
            case INT_FALLA_MAQUINA:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_FALLA_MAQUINA +"|") ;
              break;       
            case INT_MANTENIMIENTO_CORRECTIVO:
              respuesta += ("|TPRS~"+(String)tiempos.horas_INT_MANTENIMIENTO_CORRECTIVO +"|") ;
              break;        
            case PRD_FINALIZO_PRODUCCION:
              respuesta += ("|TPRS~"+(String)tiempos.horas_PRD_NO_OPERABILIDAD +"|") ;
              break;
        }
        respuesta += +"|";
    }
    /* COMANDO PARA DISPLAY NEXTION*/
    if (comando == "EVNT"){
        byte evento = parametro.toInt();
        decodificarParametrosNEXTION(evento);
    }
  
    if (comando == "IEST"){
        if(isNumeric(parametro)){
           recibidoEstado=parametro.toInt();                  
           //respuesta += ("IEST~"+ (String) recibidoEstado +"|");
           //myFrame.setText(lblidESTADO, (String)recibidoEstado);
           //myFrame.setText(lblidESTADO, "EVNT: "+(String)recibidoEstado);
       }           
    }
    
    if (comando == "PAUS"){
        byte evento = parametro.toInt();
        pausarContinuarNEXTION(evento);
    }
    if (comando == "PAGE"){
        byte page = parametro.toInt();
        if (page>=0 and page<5);  PAGE=page;
        if(PAGE==3) mostrarTiemposPROD_NEXTION();
    }
}
