//=================================================================
// 0.- Configuraciones para la SD CARD
//=================================================================
Sd2Card card;
SdVolume volume;
SdFile directory_root;

const int chipSelect = 53;         //ChipSelect del SD (SPI)

//=================================================================
// 1.- Inicializamos la SD CARD
//=================================================================
void initSDCard(){
  Serial.print("Initializing SD Card...");
  actualizarHoraSDCard();
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    existeSDCard=false;    
  }
  else{
    Serial.println("SD Card OK.");
    existeSDCard=true; 
  }
}
//=================================================================
// 2.- Actualiza la hora y fecha actual en la SD CARD
//=================================================================
void actualizarHoraSDCard(){
  SdFile::dateTimeCallback(dateTime);
  /*String _time = myRTC.getTimeStr(FORMAT_LONG);
  String _date = myRTC.getDateStr(FORMAT_LONG,FORMAT_BIGENDIAN,'/');
  FileName = _date; 
  FileName.replace("/","");
  horaACTUAL =_time;*/
}
//=================================================================
// 3.- Agrega una hora y fecha en la creacion de archivos
//=================================================================
void dateTime(uint16_t* date, uint16_t* time) {
  Time now; 
  now = myRTC.getTime();
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year, now.mon, now.date);
  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour, now.min, now.sec);
}
//=================================================================
// 4.- Verifica si se ha inicializado la SD CARD
//=================================================================
boolean verifySDCard(){
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    return false;    
  }
  else {
    return true;
  }
}
//=================================================================
// 4.- Genera una nombre para el archivo
//=================================================================
String getFileName(){
  String _FileName = String (produccion.idProduccion,HEX);
  byte _len = _FileName.length();
  for(byte i=0;i< 8 -_len ;i++){
    _FileName = "0"+_FileName;
  }
  return _FileName +".log";
}
//=================================================================
// 5.- Lee el archivo de nombre _FileName
//=================================================================
void readFileSDCard(String _FileName){
  //Verifica si existe SDCard  
  if(verifySDCard()){   
      File dataFile = SD.open(_FileName);
      if (dataFile) {
        while (dataFile.available()) {
          Serial.write(dataFile.read());          
        }
        dataFile.close();
        Serial.println("~EOF~");
      } 
  }
}
//=================================================================
// 5.- Escribe un texto en el archivo actual
//=================================================================
void writeFileSDCard(String _inputText){
  actualizarHoraSDCard();
  if(existeSDCard){
    horaACTUAL    =  (String)myRTC.getTimeStr(FORMAT_LONG) +" "+ (String)myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/');
    String dataString = ID  + "|HREC~"+horaACTUAL +_inputText;
    //Serial.println(getFileName());
    //Serial.print("SD:");Serial.println(dataString);
    File dataFile = SD.open(getFileName(), FILE_WRITE);    
    // Si el archivo está disponible, se escribir en él:
    if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
    }
    // Si el archivo no está abierto, aparecerá un error:
    else {
       //Serial.print("error opening"); Serial.println(getFileName());
    }
  }  
}
void writeSDCardEVNT(String _inputText){
  actualizarHoraSDCard();
  if(existeSDCard){
    horaACTUAL    =  (String)myRTC.getTimeStr(FORMAT_LONG) +" "+ myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/');
    String dataString = ID  + "|HREC~"+horaACTUAL +_inputText;
    //Serial.println(getFileName());
    //Serial.print("SD:");Serial.println(dataString);

      String DirName="/EVNT/";
      //Serial.print("STR: ");Serial. println(DirName);
      //Crea NameFolder si no EXISTE.
      char DirCreate[11];     DirName.toCharArray(DirCreate,DirName.length()+1);
      //Serial.print  ("CHR: ");Serial.println(DirCreate);
      if( !SD.exists(DirCreate) ){        //Serial.println("File directory not exists");       
        if( SD.mkdir(DirCreate) ){          //Serial.println("File directory created: ");
        }else{          //Serial.println("File directory not created");
        }
      }else{        //Serial1.println("File directory exists");
      }
      String FileLocation = DirName + getFileName(); 
    
    File dataFile = SD.open(FileLocation, FILE_WRITE);    
    // Si el archivo está disponible, se escribir en él:
    if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
    }
    // Si el archivo no está abierto, aparecerá un error:
    else {       //Serial.print("error opening"); Serial.println(getFileName());
    }
  }  
}
void writeSDCardPROD(){
  actualizarHoraSDCard();
  if(existeSDCard){
    //horaACTUAL    =  myRTC.getTimeStr(FORMAT_LONG) +" "+ myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/');
    //String dataString = ID  + "|HREC~"+horaACTUAL +_inputText;
    //Serial.println(getFileName());
    //Serial.print("SD:");Serial.println(dataString);

      String DirName="/PROD/";
      //Serial.print("STR: ");Serial. println(DirName);
      //Crea NameFolder si no EXISTE.
      char DirCreate[11];     DirName.toCharArray(DirCreate,DirName.length()+1);
      //Serial.print  ("CHR: ");Serial.println(DirCreate);
      if( !SD.exists(DirCreate) ){        //Serial.println("File directory not exists");       
        if( SD.mkdir(DirCreate) ){          //Serial.println("File directory created: ");
        }else{          //Serial.println("File directory not created");
        }
      }else{        //Serial1.println("File directory exists");
      }
      String FileLocation = DirName + getFileName(); 
    
    File dataFile = SD.open(FileLocation, FILE_WRITE);    
    // Si el archivo está disponible, se escribir en él:
    if (dataFile) {
        //-----------------------------------------------------------------------------
        // PRODUCCION
        dataFile.print("ID PRODUCTO     : ");  dataFile.println(produccion.idProduccion);
        dataFile.print("NOMBRE PRODUCTO : ");  dataFile.println(getStringPRODUCTO());
        dataFile.print("NUMERO LOTE     : ");  dataFile.println(produccion.Lote);
        dataFile.print("HORA CREACION   : ");  dataFile.println(produccion.horaCREACION);
        dataFile.print("HORA INICIO     : ");  dataFile.println(produccion.horaINICIO);
        dataFile.print("HORA FINAL      : ");  dataFile.println(produccion.horaFINAL);
        dataFile.println();
        //-----------------------------------------------------------------------------
        // CONTADOR
        dataFile.print("ID CONTADOR     : ");  dataFile.println(ID);
        dataFile.print("INCREMENTO      : ");  dataFile.println(contador.Incremento);
        dataFile.print("TOTAL PRODUCIDO : ");  dataFile.println(contador.conteoTOTAL);
        dataFile.print("TOTAL EVENTOS   : ");  dataFile.println(contador.totalEVENTOS);
        dataFile.println();
        //-----------------------------------------------------------------------------
        dataFile.print("HORAS PRODUCIDAS: ");  dataFile.println(tiempos.horas_PRD_PRODUCIENDO);
        dataFile.print("HORAS DETENIDAS : ");  dataFile.println(tiempos.horas_PRD_PRODUCCION_DETENIDA);
        dataFile.close();
    }
    // Si el archivo no está abierto, aparecerá un error:
    else {       //Serial.print("error opening"); Serial.println(getFileName());
    }
  }  
}
void writeSDCardVELC(){
  actualizarHoraSDCard();
  if(existeSDCard){
    horaACTUAL    =  myRTC.getTimeStr(FORMAT_LONG);
    String FileName = myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/');
    FileName.replace("/","");
    //String dataString = ID  + "|HREC~"+horaACTUAL +_inputText;
    //Serial.println(getFileName());
    //Serial.print("SD:");Serial.println(dataString);
    String DirName="/VELC/";
      //Serial.print("STR: ");Serial. println(DirName);
      //Crea NameFolder si no EXISTE.
      char DirCreate[11];     DirName.toCharArray(DirCreate,DirName.length()+1);
      //Serial.print  ("CHR: ");Serial.println(DirCreate);
      if( !SD.exists(DirCreate) ){        //Serial.println("File directory not exists");       
        if( SD.mkdir(DirCreate) ){          //Serial.println("File directory created: ");
        }else{          //Serial.println("File directory not created");
        }
      }else{        //Serial1.println("File directory exists");
      }
      String FileLocation = DirName + FileName+".csv";
    File dataFile = SD.open(FileLocation, FILE_WRITE);    
    // Si el archivo está disponible, se escribir en él:
    if (dataFile) {
        dataFile.print(horaACTUAL); dataFile.print(",");
        dataFile.print(id_captura);dataFile.print(",");
        dataFile.print(contador.conteoHORA);dataFile.print(",");
        dataFile.println(produccion.idProduccion);
        dataFile.close();
    }
    // Si el archivo no está abierto, aparecerá un error:
    else {
       //Serial.print("error opening"); Serial.println(getFileName());
    }
  }  
}
 
