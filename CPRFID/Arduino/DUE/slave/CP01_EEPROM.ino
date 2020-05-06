#include <DueFlashStorage.h>

#define FLASH_DIR_DEFAULT     0x0000
#define FLASH_DIR_CONTADOR    0x0004
#define FLASH_DIR_PRODUCCION  0x0030

//#define FLASH_DIR_TIEMPOS     0x00A0
DueFlashStorage FLASH;
void initRTC(){
  myRTC.begin();
}
void initFLASH(){
  uint32_t codeRunningForTheFirstTime=FLASH.get(FLASH_DIR_DEFAULT, codeRunningForTheFirstTime);
  Serial.println(codeRunningForTheFirstTime);
  if(codeRunningForTheFirstTime!= EPOCH_COMPILE)
    defaultFLASH();
  loadFLASH();
}

void defaultFLASH(){
  Serial.println("Valores por Defecto!");
  contador.id=0;
  contador.Incremento=1;
  contador.Muestreo=3600;
  contador.comparaLAP=5000;
  contador.Frecuencia=10;
  contador.estadoACTUAL=0;
  //Resetea los datos de PRODUCCION.
  produccion.idProduccion=0;
  produccion.MuestrasCC=true;
  resetearDatosPRODUCCION();
  saveFLASH();    
  FLASH.put(FLASH_DIR_DEFAULT,  EPOCH_COMPILE); 
}
void loadFLASH(){
  //----------------------------------------------------------------------------------------------
  // CONTADOR
  FLASH.get(FLASH_DIR_CONTADOR, contador);
  setRFID();
  if(contador.Frecuencia == 0)  contador.Frecuencia = 200;
  //----------------------------------------------------------------------------------------------
  // PRODUCCION
  FLASH.get(FLASH_DIR_PRODUCCION, produccion);
  //----------------------------------------------------------------------------------------------
  // TIEMPOS
  //FLASH.get(FLASH_DIR_TIEMPOS, tiempos);
}

void saveFLASH(){
  FLASH.put(FLASH_DIR_CONTADOR, contador);
  FLASH.put(FLASH_DIR_PRODUCCION, produccion);
  //FLASH.put(FLASH_DIR_TIEMPOS, tiempos);  
}

/*=================================================================*/
/*______________________ ALMACENAMIENTO EN LA EEPROM ______________*/
/*=================================================================*/
// String para mostrar la hora actual y modificarla.
String tiempoACTUAL="", horaANTERIOR="", horaACTUAL="";
unsigned long horaCAPTURA=0; byte id_captura=0;
byte eepromI2C;   // byte auxiliar para el almacenamiento en EEPROM I2C
const byte eepromDirDefault = 2;  // Direccion en EEPROM para la configuracion por Defecto 1 Bytes (Byte)
//  CONTADOR
const byte eepromDirId = 0;               // Direccion en la EEPROM para la variable   id del contador  2 Bytes (Word)
const byte eepromDirIncremento  = 3;      // Direccion en la EEPROM para la variable   Incremento       1 Byte  (Byte)
const byte eepromDirConteoTOTAL = 4;      // Direccion en la EEPROM para la variable   ConteoTOTAL      4 Bytes (Long)
const byte eepromDirConteoANTERIOR = 8;   // Direccion en la EEPROM para la variable   ConteoANTERIOR   4 Bytes (Long)
const byte eepromDirConteoTURNO = 12;     // Direccion en la EEPROM para la variable   ConteoTURNO      4 Bytes (Long)
const byte eepromDirComparaLAP = 16;      // Direccion en la EEPROM para la variable   compararLAP      4 Bytes (Long)
const byte eepromDirMuestreo = 20;        // Direccion en la EEPROM para la variable   Muestreo         4 Bytes (Long)
const byte eepromDirFrecuencia  = 24;     // Direccion en la EEPROM para la variable   Frecuencia       1 Byte  (Byte)
const byte eepromDirestadoACTUAL = 25;    // Direccion en la EEPROM para la variable   estadoACTUAL     1 Byte  (Boolean)
const byte eepromDirTotalEVENTOS = 26;    // Direccion en la EEPROM para la variable   totalEVENTOS     4 Bytes (Long)
const byte eepromDirEnabled = 30;         // Direccion en la EEPROM para la variable   Enabled          1 Byte  (Boolean)

// PRODUCCION
const byte eepromDirIdProduccion = 32;  // Direccion en la EEPROM para la variable  idProduccion        4 Bytes (Long)
const byte eepromDirProducto =  36;     // Direccion en la EEPROM para la variable  Producto           45 Bytes (String)
const byte eepromDirLote = 81;          // Direccion en la EEPROM para la variable  Lote               15 Bytes (String)
const byte eepromDirhoraCREACION = 96;  // Direccion en la EEPROM para la variable  horaCREACION       20 Bytes (String)
const byte eepromDirhoraINICIO = 116;   // Direccion en la EEPROM para la variable  horaINICIO         20 Bytes (String)
const byte eepromDirhoraFINAL = 136;    // Direccion en la EEPROM para la variable  horaFIN            20 Bytes (String)
const byte eepromDirSalvado = 156;      // Direccion en la EEPROM para la variable  Produciendo         1 Byte  (Boolean)
const byte eepromDirProduciendo = 157;  // Direccion en la EEPROM para la variable  Produciendo         1 Byte  (Boolean)
const byte eepromDirMuestrasCC = 158;   // Direccion en la EEPROM para la variable  habilitaMuestrasCC  1 Byte  (Boolean)

//  TIEMPOS
const byte eepromDirHoras_PRD_NO_OPERABILIDAD  = 160;
const byte eepromDirHoras_NOP_AJUSTE_MAQUINA   = 164;
const byte eepromDirHoras_NOP_LIMPIEZA_RADICAL = 168;      
const byte eepromDirHoras_NOP_CAMBIO_FORMATO   = 172;
const byte eepromDirHoras_NOP_CAMBIO_TURNO     = 176;
const byte eepromDirHoras_NOP_SIN_OPERADOR     = 180;
const byte eepromDirHoras_NOP_FALLA_MAQUINA    = 184;
const byte eepromDirHoras_NOP_MANTENIMIENTO_PREVENTIVO = 188;
const byte eepromDirHoras_NOP_MANTENIMIENTO_CORRECTIVO = 192;
const byte eepromDirHoras_NOP_ENSAYOS_DESARROLLO = 196;
        
const byte eepromDirHoras_PRD_NUEVA_PRODUCCION = 200;
const byte eepromDirHoras_PRD_SACANDO_MUESTRAS = 204;
const byte eepromDirHoras_PRD_ESPERANDO_APROBACION = 208 ;      
const byte eepromDirHoras_PRD_PRODUCIENDO = 212;
const byte eepromDirHoras_PRD_PRODUCCION_DETENIDA =216;

const byte eepromDirHoras_INT_AJUSTE_MAQUINA = 220;
const byte eepromDirHoras_INT_LIMPIEZA_EQUIPO= 224;
const byte eepromDirHoras_INT_CAMBIO_INSUMOS = 228;
const byte eepromDirHoras_INT_MATERIA_PRIMA  = 232;
const byte eepromDirHoras_INT_CAMBIO_TURNO   = 236;
const byte eepromDirHoras_INT_FALLA_MAQUINA  = 240;
const byte eepromDirHoras_INT_MANTENIMIENTO_CORRECTIVO = 244;

/*==================================================================*/
/*_______ Este segmento es para inicializar la memoria EEPROM ______*/
/*==================================================================*/ 

/*==================================================================*/
/*___ Este segmento es para la lectura de parametros de la EEPROM __*/
/*==================================================================*/ 
void leerParametros(){

}
/*_______________________________________________________________*/
/*___________________________ EEPROM ____________________________*/
/*---- Este segmento de codigo solo se programa una sola vez --- */   
void cargarParametrosporDEFECTO(){

}

void resetearDatosPRODUCCION(){     
    // CONTADOR
    contador.conteoTOTAL = 0;
    contador.conteoANTERIOR = 0;
    contador.conteoPARCIAL  = 0;
    contador.conteoTURNO    = 0;
    contador.estadoACTUAL = PRD_NO_OPERABILIDAD;
    contador.totalEVENTOS = 0;
    contador.Enabled = false;
    // PRODUCCION
    //generarNuevoIdPRODUCCION();
    setStringPRODUCTO("SIN PRODUCTO");
    produccion.Lote = 0;
    produccion.horaCREACION = 0;
    produccion.horaINICIO = 0;
    produccion.horaFINAL = 0;
    produccion.Produciendo = false;
    produccion.Salvado = false;
    // TIEMPOS
    resetearTiemposNOOPERABILIDAD();
    resetearTiemposPRODUCCION(); 
    saveFLASH();   
 }

void resetearTiemposNOOPERABILIDAD(){
    tiempos.horas_PRD_NO_OPERABILIDAD  = 0;
    tiempos.horas_NOP_AJUSTE_MAQUINA   = 0;
    tiempos.horas_NOP_LIMPIEZA_RADICAL = 0;
    tiempos.horas_NOP_CAMBIO_FORMATO   = 0;
    tiempos.horas_NOP_CAMBIO_TURNO     = 0;
    tiempos.horas_NOP_SIN_OPERADOR     = 0;
    tiempos.horas_NOP_FALLA_MAQUINA    = 0;
    tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO = 0;
    tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO = 0;
    tiempos.horas_NOP_ENSAYOS_DESARROLLO = 0;
    //FLASH.put(FLASH_DIR_TIEMPOS, tiempos);
}

void resetearTiemposPRODUCCION(){
    resetSegundosTX();

    tiempos.horas_PRD_NUEVA_PRODUCCION = 0;
    tiempos.horas_PRD_SACANDO_MUESTRAS = 0;
    tiempos.horas_PRD_ESPERANDO_APROBACION = 0;
    tiempos.horas_PRD_PRODUCIENDO     = 0;
    tiempos.horas_PRD_PRODUCCION_DETENIDA = 0;
      
    tiempos.horas_INT_AJUSTE_MAQUINA  = 0;
    tiempos.horas_INT_LIMPIEZA_EQUIPO = 0;
    tiempos.horas_INT_CAMBIO_INSUMOS  = 0;
    tiempos.horas_INT_MATERIA_PRIMA   = 0;
    tiempos.horas_INT_CAMBIO_TURNO    = 0;
    tiempos.horas_INT_FALLA_MAQUINA   = 0;
    tiempos.horas_INT_MANTENIMIENTO_CORRECTIVO = 0;
    //FLASH.put(FLASH_DIR_TIEMPOS, tiempos);
}
//_________________________________________________________________
// Al cargar el ID  de la EEPROM lo convertimos en Cadena de texto.
void setRFID(){  
    if (contador.id<=0 && contador.id>=1000){
      contador.id=0;
    }
    String RFID="RFID000";
    if (contador.id<100){ if(contador.id<10)RFID="RFID00";
                          else RFID="RFID0";  }
    else   RFID="RFID"; 
    ID=RFID+contador.id;
}

void setStringPRODUCTO(String Cadena){
  byte lenCadena=Cadena.length();
  // La cadena debe de estar el rango de 0-23 chars.
  if ((lenCadena > 0) && (lenCadena <=MAX_LEN_PRODUCTO)){
    // Almacenamiento en la EEPROM
    for(int i=0;i<lenCadena;i++){
      char inChar = Cadena.charAt(i);
      Serial.println(inChar);
      produccion.Producto[i]=(byte)inChar;      
    }
    produccion.Producto[lenCadena]=0;
    Serial.println(lenCadena);        
  }
  else{ 
    // Si no cumple retorna null.
    produccion.Producto[0]=0;
  }
}

String getStringPRODUCTO(){
  String Producto="";
  byte inChar;
  byte i=0;
  while(i<MAX_LEN_PRODUCTO){
    inChar=produccion.Producto[i];      
    i++;                
    if (inChar == 0) i = MAX_LEN_PRODUCTO;
    else Producto += (char) inChar;
  }
  return Producto;
}

void getFullTimeSTR(){
  
  tiempoACTUAL ="";
  tiempoACTUAL += ((String)myRTC.getDOWStr(FORMAT_SHORT) +", ");
  tiempoACTUAL += ((String)myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/') +" ");
  //METODO CAPTURA POR HORA.
  //tiempoACTUAL +=  myRTC.getTimeStr(FORMAT_LONG);  
  Time t;  t = myRTC.getTime();
  // Actualiza la hora de la captura
  id_captura = t.hour; 
  horaCAPTURA = t.min * 60 + t.sec * 1;
  // GENERAR HORA hh:mm:ss  
  String strCAPTURA= "xxxxxxxx";
  if (t.hour<10)    strCAPTURA[0]=48;
  else    strCAPTURA[0]=char((t.hour / 10)+48);
  strCAPTURA[1]=char((t.hour % 10)+48);  strCAPTURA[2]=58;
  
  if (t.min<10)    strCAPTURA[3]=48;
  else    strCAPTURA[3]=char((t.min / 10)+48);
  strCAPTURA[4]=char((t.min % 10)+48);
  strCAPTURA[5]=58;

  if (t.sec<10)    strCAPTURA[6]=48;
  else    strCAPTURA[6]=char((t.sec / 10)+48);
  strCAPTURA[7]=char((t.sec % 10)+48);
  strCAPTURA=strCAPTURA.substring(0,8);

  tiempoACTUAL += strCAPTURA;
  
}

void almacenarParametrosCONTADOR(){
   // Almacena los datos en la EEPROM cada segundo
   FLASH.put(FLASH_DIR_CONTADOR,contador);
   
   // Almacena los tiempos acumulados en la EEPROM
   /*
   switch(contador.estadoACTUAL){
      case PRD_NO_OPERABILIDAD:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_NO_OPERABILIDAD, tiempos.horas_PRD_NO_OPERABILIDAD);
        break;
      case NOP_AJUSTE_MAQUINA:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_AJUSTE_MAQUINA,  tiempos.horas_NOP_AJUSTE_MAQUINA);
        break;
      case NOP_LIMPIEZA_RADICAL:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_LIMPIEZA_RADICAL,tiempos.horas_NOP_LIMPIEZA_RADICAL);
        break;
      case NOP_CAMBIO_FORMATO:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_CAMBIO_FORMATO,  tiempos.horas_NOP_CAMBIO_FORMATO);
        break;
      case NOP_CAMBIO_TURNO:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_CAMBIO_TURNO,    tiempos.horas_NOP_CAMBIO_TURNO);
        break;        
      case NOP_SIN_OPERADOR:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_SIN_OPERADOR,    tiempos.horas_NOP_SIN_OPERADOR);
        break;
      case NOP_FALLA_MAQUINA:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_FALLA_MAQUINA,   tiempos.horas_NOP_FALLA_MAQUINA);  
        break;
      case NOP_MANTENIMIENTO_PREVENTIVO:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_MANTENIMIENTO_PREVENTIVO, tiempos.horas_NOP_MANTENIMIENTO_PREVENTIVO);
        break;
      case NOP_MANTENIMIENTO_CORRECTIVO:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_MANTENIMIENTO_CORRECTIVO, tiempos.horas_NOP_MANTENIMIENTO_CORRECTIVO); 
        break;
      case NOP_ENSAYOS_DESARROLLO:
        myRTC.writeLongEeprom(eepromDirHoras_NOP_ENSAYOS_DESARROLLO,       tiempos.horas_NOP_ENSAYOS_DESARROLLO);
        break;        
      case PRD_NUEVA_PRODUCCION:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_NUEVA_PRODUCCION,        tiempos.horas_PRD_NUEVA_PRODUCCION);
        break;
      case PRD_SACANDO_MUESTRAS:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_SACANDO_MUESTRAS,        tiempos.horas_PRD_SACANDO_MUESTRAS);
        break;
      case PRD_ESPERANDO_APROBACION:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_ESPERANDO_APROBACION,    tiempos.horas_PRD_ESPERANDO_APROBACION);
        break;
      case PRD_INICIA_PRODUCCION:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_PRODUCIENDO,             tiempos.horas_PRD_PRODUCIENDO);
        break;
      case PRD_PRODUCIENDO:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_PRODUCIENDO,             tiempos.horas_PRD_PRODUCIENDO); 
        break;        
      case PRD_PRODUCCION_DETENIDA:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_PRODUCCION_DETENIDA,     tiempos.horas_PRD_PRODUCCION_DETENIDA);
        break;        
      case INT_AJUSTE_MAQUINA:
        myRTC.writeLongEeprom(eepromDirHoras_INT_AJUSTE_MAQUINA,  tiempos.horas_INT_AJUSTE_MAQUINA);
        break;  
      case INT_LIMPIEZA_EQUIPO:
        myRTC.writeLongEeprom(eepromDirHoras_INT_LIMPIEZA_EQUIPO, tiempos.horas_INT_LIMPIEZA_EQUIPO);
        break;
      case INT_CAMBIO_INSUMOS:
        myRTC.writeLongEeprom(eepromDirHoras_INT_CAMBIO_INSUMOS,  tiempos.horas_INT_CAMBIO_INSUMOS);
        break;
      case INT_MATERIA_PRIMA:
        myRTC.writeLongEeprom(eepromDirHoras_INT_MATERIA_PRIMA,   tiempos.horas_INT_MATERIA_PRIMA);
        break;
      case INT_CAMBIO_TURNO:
        myRTC.writeLongEeprom(eepromDirHoras_INT_CAMBIO_TURNO,    tiempos.horas_INT_CAMBIO_TURNO);
        break;
      case INT_FALLA_MAQUINA:
        myRTC.writeLongEeprom(eepromDirHoras_INT_FALLA_MAQUINA,   tiempos.horas_INT_FALLA_MAQUINA);; 
        break;
      case INT_MANTENIMIENTO_CORRECTIVO:
        myRTC.writeLongEeprom(eepromDirHoras_INT_MANTENIMIENTO_CORRECTIVO, tiempos.horas_INT_MANTENIMIENTO_CORRECTIVO); 
        break;
      case PRD_FINALIZO_PRODUCCION:
        myRTC.writeLongEeprom(eepromDirHoras_PRD_NO_OPERABILIDAD, tiempos.horas_PRD_NO_OPERABILIDAD);
        break;
    } 
    delay(MAX_TIME_SAVE);
    */
}
