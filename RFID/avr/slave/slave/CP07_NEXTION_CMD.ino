
void decodificarParametrosNEXTION(byte estado){
    switch(estado){
        case PRD_NO_OPERABILIDAD:        
          break;
        case NOP_AJUSTE_MAQUINA:
          actualizarEstadoCONTADOR(NOP_AJUSTE_MAQUINA);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;        
        case NOP_LIMPIEZA_RADICAL:
          actualizarEstadoCONTADOR(NOP_LIMPIEZA_RADICAL);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;    
        case NOP_CAMBIO_FORMATO:
          actualizarEstadoCONTADOR(NOP_CAMBIO_FORMATO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case NOP_CAMBIO_TURNO:
        actualizarEstadoCONTADOR(NOP_CAMBIO_TURNO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case NOP_SIN_OPERADOR:
          actualizarEstadoCONTADOR(NOP_SIN_OPERADOR);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case NOP_FALLA_MAQUINA:
          actualizarEstadoCONTADOR(NOP_FALLA_MAQUINA);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;       
        case NOP_MANTENIMIENTO_PREVENTIVO:  
          actualizarEstadoCONTADOR(NOP_MANTENIMIENTO_PREVENTIVO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();            
          break;
        case NOP_MANTENIMIENTO_CORRECTIVO:
          actualizarEstadoCONTADOR(NOP_MANTENIMIENTO_CORRECTIVO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case NOP_ENSAYOS_DESARROLLO:
          actualizarEstadoCONTADOR(NOP_ENSAYOS_DESARROLLO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case PRD_NUEVA_PRODUCCION:
          actualizaNuevaPRODUCCION();
          updateParametrosPRODUCCION_NEXTION();
          mostrarPageProduccion();
          Nextion.print("msgOK.flag.val=15");  eofNextion(); 
          Nextion.print("page msgOK");  eofNextion();              
          break;
        case PRD_SACANDO_MUESTRAS:
          contador.conteoTOTAL=0; iniciarContador();
          contador.estadoACTUAL = PRD_SACANDO_MUESTRAS;
          flagGuardar=true;           
          parametrosContadorNEXTION();          
          Nextion.print("msgOK.flag.val=16");  eofNextion(); 
          Nextion.print("page msgOK");  eofNextion(); 
          break;
        case PRD_ESPERANDO_APROBACION:   
          detenerContador();     
          contador.estadoACTUAL = PRD_ESPERANDO_APROBACION;
          flagGuardar=true; 
          parametrosContadorNEXTION();
          Nextion.print("msgOK.flag.val=17");  eofNextion(); 
          Nextion.print("page msgOK");  eofNextion();
          break;
        case PRD_INICIA_PRODUCCION:
          iniciarProduccion();
          parametrosContadorNEXTION();
          Nextion.print("msgOK.flag.val=18");  eofNextion(); 
          Nextion.print("page msgOK");  eofNextion();  
          break;
        case PRD_PRODUCIENDO:
          break;        
        case PRD_PRODUCCION_DETENIDA:
          break;
        case INT_AJUSTE_MAQUINA:
          detenerContador();
          actualizarEstadoCONTADOR(INT_AJUSTE_MAQUINA);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;    
        case INT_LIMPIEZA_EQUIPO:
          detenerContador();
          actualizarEstadoCONTADOR(INT_LIMPIEZA_EQUIPO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break; 
        case INT_CAMBIO_INSUMOS:
          detenerContador();
          actualizarEstadoCONTADOR(INT_CAMBIO_INSUMOS);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break; 
        case INT_MATERIA_PRIMA:
          detenerContador();   
          actualizarEstadoCONTADOR(INT_MATERIA_PRIMA);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case INT_CAMBIO_TURNO:
          detenerContador();
          actualizarEstadoCONTADOR(INT_CAMBIO_TURNO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;
        case INT_FALLA_MAQUINA:
          detenerContador();  
          actualizarEstadoCONTADOR(INT_FALLA_MAQUINA);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;       
        case INT_MANTENIMIENTO_CORRECTIVO:
          detenerContador();  
          actualizarEstadoCONTADOR(INT_MANTENIMIENTO_CORRECTIVO);  transmitirNuevoEVENTO();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          break;        
        case PRD_FINALIZO_PRODUCCION:          
          finalizarProduccion();
          updateParametrosPRODUCCION_NEXTION();    
          mostrarPageContadores();     
          break;
        case MSG_CONTINUA_PRODUCCION:
          flagContador=false; 
          actualizarEstadoCONTADOR(PRD_PRODUCCION_DETENIDA);  
          transmitirContinuaPRODUCCION();
          //delay(100);
          iniciarContador();
          delay(100);
          updateParametrosPRODUCCION_NEXTION();
          mostrarPageContadores();
          break;
        case MSG_RESETEA_CONTADOR:
          resetearContador(true);
          break;
        case MSG_RESETEA_PRODUCCION:
            //detenerContador(); 
            //resetearDatosPRODUCCION();  
            //resetearContador(false);   
            //transmitirReseteaPRODUCCION();
            //updateParametrosPRODUCCION_NEXTION();
            //mostrarPageContadores();  
            resetearPRODUCCION();
         break;
       case MSG_HABILITAR_CONTADOR:
          actualizarEstadoCONTADOR(PRD_PRODUCCION_DETENIDA);
          //habilitarControlesProduccion();
          iniciarContador();
          updateParametrosPRODUCCION_NEXTION();
          mostrarPageContadores();
        break;
       case MSG_DESHABILITAR_CONTADOR:
          actualizarEstadoCONTADOR(INT_AJUSTE_MAQUINA);    transmitirNuevoEVENTO();
          //habilitarControlesProduccion();
          detenerContador();
          parametrosContadorNEXTION();
          mostrarPageContadores();
          
        break;
    }
}

void pausarContinuarNEXTION(byte pos){
    switch(pos){
        case NOP_AJUSTE_MAQUINA:
              if(produccion.Salvado) actualizarEstadoCONTADOR(PRD_NUEVA_PRODUCCION);  
              else  actualizarEstadoCONTADOR(PRD_NO_OPERABILIDAD);  
              transmitirContinuaPRODUCCION();
              updateParametrosPRODUCCION_NEXTION();
              mostrarPageContadores();
        break;
        case NOP_FINALIZO_TAREA:
              actualizarEstadoCONTADOR(PRD_NO_OPERABILIDAD);  
              transmitirContinuaPRODUCCION();
              updateParametrosPRODUCCION_NEXTION();
              mostrarPageContadores();
        break;
        case INT_AJUSTE_MAQUINA:
            if(produccion.Produciendo){
                actualizarEstadoCONTADOR(PRD_PRODUCCION_DETENIDA);
                transmitirContinuaPRODUCCION();
                iniciarContador(); 
              }
              else{
                /*
                  detenerContador();
                  if(myFrame.showDialog(pnlContador,"DESEA SACAR MUESTRAS PARA CONTROL DE CALIDAD?","SACAR MUESTRAS",QUESTION,BIG)){
                    resetearContador(false); actualizarEstadoCONTADOR(PRD_SACANDO_MUESTRAS); transmitirNuevoEVENTO();
                    myFrame.showDialog(pnlContador,"AHORA PUEDE SACAR MUESTRAS PARA CONTROL DE CALIDAD","SACAR MUESTRAS",CORRECT,BIG);
                    iniciarContador();                
                  }
                  else{
                      myFrame.showDialog(pnlContador,"AHORA PUEDE INICIAR PRODUCCION.","INICIA PRODUCCION",CORRECT,BIG);
                      iniciarProduccion();
                  } */
              }
              parametrosContadorNEXTION();
              mostrarPageContadores();
        break;
    }
}
