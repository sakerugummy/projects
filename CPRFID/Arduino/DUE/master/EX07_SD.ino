/*================================================================*/
/*-------------------------     SD CARD     ----------------------*/
/*================================================================*/
void initSDCARD(){
  Serial.println("\nInicializando SD card...");
  //if(enableDebug) Serial2.println("\nInicializando SD card...");
  enableSD();
  if (!SD.begin(chipSelect)) {
    Serial.println("SDcard ERROR!");
    //if(enableDebug) Serial2.println("SDcard ERROR!");
    existeSDCard=false;
    return;
  }
  Serial.println("SDCard OK.");
  //if(enableDebug) Serial2.println("SDCard OK.");
  existeSDCard = true;
  showSDCardINFO();
  //showVolumeSizeINFO();
  //listFilesINFO();
  //enableNET();
}
/*----------------------------------------------------------------*/
boolean verifySDCard(){
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("ERROR: No se puede inicializar el SD. Verifique que:");
    Serial.println("* SDCard esta insertada?");
    Serial.println("* La conexion es correcta?");
    Serial.println("* talves modificar el pin chipSelect pin del modulo EthernetShield?");
    if(enableDebug){
      Serial2.println("ERROR: No se puede inicializar el SD. Verifique que:");
      Serial2.println("* SDCard esta insertada?");
      Serial2.println("* La conexion es correcta?");
      Serial2.println("* talves modificar el pin chipSelect pin del modulo EthernetShield?");
    }
    return false;
  }
  else {
    return true;
  }
}
/*----------------------------------------------------------------*/
void showSDCardINFO(){
  enableSD();
  if(verifySDCard()){
    Serial.print("Tipo de SD Card: ");
    if(enableDebug) Serial2.print("Tipo de SD Card: ");
      switch(card.type()) {
        case SD_CARD_TYPE_SD1:
          Serial.println("SD1");
          if(enableDebug) Serial2.println("SD1");
          break;
        case SD_CARD_TYPE_SD2:
          Serial.println("SD2");
          if(enableDebug) Serial2.println("SD2");
          break;
        case SD_CARD_TYPE_SDHC:
          Serial.println("SDHC");
          if(enableDebug) Serial2.println("SDHC");
          break;
        default:
          Serial.println("Unknown");
          if(enableDebug) Serial2.println("Unknown");
          break;
    }   
  }
}
/*----------------------------------------------------------------*/
void showVolumeSizeINFO(){
  enableSD();
  if(verifySDCard()){
      // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
      if (!volume.init(card)) {
        Serial.println("No se reconoce la particion FAT16/FAT32.\nAsegurece de que el SD Card tenga formato");
        if(enableDebug) Serial2.println("No se reconoce la particion FAT16/FAT32.\nAsegurece de que el SD Card tenga formato");
        return;
      }
      // print the type and size of the first FAT-type volume
      uint32_t volumesize;
      Serial.print("Espacio de SD FAT");      Serial.println(volume.fatType(), DEC);
      if(enableDebug){  Serial2.print("Espacio de SD FAT");      Serial2.println(volume.fatType(), DEC); }
      
      volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
      volumesize *= volume.clusterCount();       // we'll have a lot of clusters
      volumesize *= 512;                         // SD card blocks are always 512 bytes
      Serial.print("Tamaño del SD en (bytes): ");
      Serial.println(volumesize);
      Serial.print("Tamaño del SD en (Kbytes): ");
      if(enableDebug){ 
        Serial2.print("Tamaño del SD en (bytes): ");
        Serial2.println(volumesize);
        Serial2.print("Tamaño del SD en (Kbytes): ");
      }
      volumesize /= 1024;
      Serial.println(volumesize);
      Serial.print("Tamaño del SD en (Mbytes): ");
      if(enableDebug){ 
        Serial2.println(volumesize);
        Serial2.print("Tamaño del SD en (Mbytes): ");
      }
      volumesize /= 1024;
      Serial.println(volumesize);
      if(enableDebug) Serial2.println(volumesize);    
  }
   //enableNET();
}
/*----------------------------------------------------------------*/
void showFilesINFO(){
  enableSD();
  if(verifySDCard()){
    // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
    if (!volume.init(card)) {
      Serial.println("\nNo se reconoce la particion FAT16/FAT32.\nAsegurece de que el SD Card tenga formato");
      if(enableDebug) Serial2.println("\nNo se reconoce la particion FAT16/FAT32.\nAsegurece de que el SD Card tenga formato");
      return;
    }
    Serial.println("\nArchivos encontrados en la SD Card\n(FILE NAME, DATE CREATE , SIZE in bytes): ");
    if(enableDebug) Serial2.println("\nArchivos encontrados en la SD Card\n(FILE NAME, DATE CREATE , SIZE in bytes): ");
    directory_root.openRoot(volume);
    // list all files in the card with date and size
    //directory_root.ls(LS_R | LS_DATE | LS_SIZE, 0,  0);
    directory_root.ls(LS_R | LS_DATE | LS_SIZE, 0);
    if(enableDebug) directory_root.ls(LS_R | LS_DATE | LS_SIZE);
    Serial.println("~EOF~"); 
    if(enableDebug)Serial2.println("~EOF~");    
  }
}
/*----------------------------------------------------------------*/
void listFilesINFO(){
  enableSD();
  Serial.println("\nArchivos encontrados en la SD Card\n(FILE NAME \tSIZE in bytes): ");
  if(enableDebug) Serial2.println("\nArchivos encontrados en la SD Card\n(FILE NAME \tSIZE in bytes): ");
  if(verifySDCard()){
    File root = SD.open("/");
    printDirectory(root, 0);
  }
}
/*----------------------------------------------------------------*/
void printDirectory(File dir, int numTabs) {
   while(true) {     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
       if(enableDebug) Serial2.print('\t');
     }
     Serial.print(entry.name());
     if(enableDebug) Serial2.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       if(enableDebug) Serial2.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t"); Serial.println(entry.size(), DEC);
       if(enableDebug){Serial2.print("\t\t"); Serial2.println(entry.size(), DEC);}
     }
     entry.close();
   }
}
/*----------------------------------------------------------------*/
void readFileSDCard(String _FileName){
  enableSD();
  if(verifySDCard()){   
      File dataFile = SD.open(_FileName);
      // if the file is available, write to it:
      Serial.print("\nLeyendo archivo: "); Serial.println(_FileName);
      if(enableDebug){Serial2.print("\nLeyendo archivo: "); Serial2.println(_FileName);}
      if (dataFile) {
        while (dataFile.available()) {
          Serial.write(dataFile.read());
          if(enableDebug) Serial2.write(dataFile.read());
        }
        dataFile.close();
        Serial.println("~EOF~");
        if(enableDebug) Serial2.println("~EOF~");
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.print("ERROR: No se puede abrir ");Serial.println(FileName);
        if(enableDebug){Serial2.print("ERROR: No se puede abrir ");Serial2.println(FileName);}
      }   
  }
  //enableNET();
}
/*----------------------------------------------------------------*/
void writeFileSDCard(String _inputText){
  enableSD();
  if(existeSDCard){ 
    String saveData = horaACTUAL + "|"+_inputText;
    saveData.replace("|",";");
    // Verificamos que dato es un evento.
    if(_inputText.startsWith("RFID") && (_inputText.indexOf("EVNT") !=-1)){
      //Serial.print("RX: "); Serial.println(_inputText);
      String DirName="/"+_inputText.substring(0,7)+"/";
      //Serial.print("STR: ");Serial. println(DirName);
      //Crea NameFolder si no EXISTE.
      char DirCreate[11];     DirName.toCharArray(DirCreate,DirName.length()+1);
      //Serial.print  ("CHR: ");Serial.println(DirCreate);
      if( !SD.exists(DirCreate) ){
        Serial.println("File directory not exists");
        if(enableDebug) Serial2.println("File directory not exists");
        
        if( SD.mkdir(DirCreate) ){
          Serial.println("File directory created: ");
          if(enableDebug) Serial2.println("File directory created: ");
        }
        else{
          Serial.println("File directory not created");
          if(enableDebug) Serial2.println("File directory not created");
        }
      }
      else{
        //Serial1.println("File directory exists");
      }
      String FileLocation = DirName + FileName + ".csv";  
      //String FileLocation = FileName;
      File myFile = SD.open(FileLocation, FILE_WRITE);
      if(myFile){
        myFile.println(saveData);
        myFile.close();
      }
      delay(10);
    }
  }
}
/*----------------------------------------------------------------*/
void writeErrorSDCard(String _inputText){
  enableSD();
  if(existeSDCard){
    String saveData = horaACTUAL + "|"+_inputText; 
    String FileLocation = FileName + ".log";  
    File myFile = SD.open(FileLocation, FILE_WRITE);
    if(myFile){
      myFile.println(saveData);
      myFile.close();
      Serial.println("Log File Update...");
      if(enableDebug) Serial2.println("Log File Update...");
    }
    delay(10);
  }
}

