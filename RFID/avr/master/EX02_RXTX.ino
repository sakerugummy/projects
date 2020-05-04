/*================================================================*/
/*-------------------------------- UART --------------------------*/
/*================================================================*/
/*----------------------------------------------------------------*/
// Inicializamos el puerto Serie 
void initUART(){
    pinMode(PIN_LED_RX,OUTPUT);
    digitalWrite(PIN_LED_RX,LOW);
    Serial.begin(baudRate);
    inputString.reserve(bufferSize);  
    rxString.reserve(bufferSize); 
    Serial.println("\nInicializando COM...");  
    Serial.print("Baudrate: "); Serial.println(baudRate);
    
    // Configuramos puerto Serial1 para comunicacion con el XBEE S2B PRO
    Serial1.begin(baudRate1);
    inputString1.reserve(bufferSize1);  
    rxString1.reserve(bufferSize1);
    
    // Configuramos puerto Serial2 para comunicacion con el HC06    
    Serial2.begin(baudRate2);
    inputString2.reserve(bufferSize2);  
    rxString2.reserve(bufferSize1);   
    //configurarHC06();
    // Parametros para las rutinas de decodificacion temporal 
    strDecodificar.reserve(100);
    strTransmitir.reserve(100);
    comando.reserve(10);
    parametro.reserve(60);
    respuesta.reserve(bufferSize);
}

void configurarHC06(){
  delay(5000);
  Serial2.print("AT"); delay(1000);
  //Cambio de nombre donde se envia AT+NAME y seguido el nombre que deseemos
  Serial2.print("AT+NAMERFID_BET"); delay(1000);
  //Serial2.print("AT+NAMERFID_LNE");  delay(1000);
  //Serial2.print("AT+NAMERFID_SNE");  delay(1000);
  //Serial2.print("AT+NAMERFID_ACC");  delay(1000);
  //Serial2.print("AT+NAMERFID_LE");  delay(1000);
  Serial2.print("AT+PIN1217");  delay(1000);
}
/*----------------------------------------------------------------*/
// Rutina de Interrupcion por buffer lleno en recepcion.
 void serialEvent() {  
    // Hay datos en el buffer del puerto serie.
    boolean flagEvent=false;
    while (Serial.available() && !flagEvent) {
        char inChar = (char)Serial.read();          
        if (inChar!=13 && inChar!=10 && inChar!=6)
            inputString += inChar;            
        else{
            if (inChar== 6)
                inputString = "";
            if (inChar==13){
                rxString = inputString;  
                inputString = "";
                stringComplete = true;
                flagEvent=true; 
            }
        }
        //Retardo necesario para la recepcion de datos.
        delayMicroseconds(usdelayRate);
    }
}

/*----------------------------------------------------------------*/
// Verificamos la recepcion de datos en el puerto Serial1.
void verificarUART(){
    if (stringComplete) {
        stringComplete = false;
        if(rxString.length()>0){
          Serial.print("RX: ");   Serial.println(rxString);
          printLineUTFT(rxString,VGA_WHITE);
          if(enableDebug){
              Serial2.print("RX: ");    Serial2.println(rxString);
          }
          // Decodificamos y almacenamos eventos en la SD
          decodificarRecepcion(rxString);
          writeFileSDCard(rxString);
          // Transmitimos el evento al WebService
          rxString.replace(" ", "%20");
          if(rxString.length()>5)
            httpRequest(rxString);
          
        }
    }
}
/*----------------------------------------------------------------*/
// Verificamos la recepcionde datos en el puerto Serial1.
void verificarUART1(){
    boolean flagEvent1=false;      
    while (Serial1.available() && !flagEvent1) {
        char inChar1 = (char)Serial1.read();
        _debug_time = 0;          
        if (inChar1!=13 && inChar1!=10 && inChar1!=6)
            inputString1 += inChar1;            
        else{
            if (inChar1==6) inputString1 = "";
            if (inChar1==13){ 
              rxString1=inputString1;  
              inputString1=""; 
              stringComplete1 = true;
              flagEvent1 = true;
            }
        }      
        //Retardo necesario para la recepcion de datos.
        delayMicroseconds(usdelayRate1);
        // Si no hay mas datos salir de la rutina.         
    }
    if (stringComplete1) {
        digitalWrite(PIN_LED_RX,HIGH);
        stringComplete1 = false;
        if(rxString1.length()>5){
          Serial.print("RX: ");   Serial.println(rxString1);
          printLineUTFT(rxString1,VGA_WHITE);
          if(enableDebug){
              Serial2.print("RX : ");    Serial2.println(rxString1);
          }
          // Decodificamos y almacenamos eventos en la SD
          decodificarRecepcion(rxString1);
          writeFileSDCard(rxString1);
          // Transmitimos el evento al WebService
          rxString1.replace(" ", "%20");
          httpRequest(rxString1);
        }
        digitalWrite(PIN_LED_RX,LOW);
    }
}
void verificarUART2(){
    boolean flagEvent2=false;      
    while (Serial2.available() && !flagEvent2) {
        char inChar2 = (char)Serial2.read();
        _debug_time = 0;          
        if (inChar2!=13 && inChar2!=10 && inChar2!=6)
            inputString2 += inChar2;            
        else{
            if (inChar2==6) inputString2 = "";
            if (inChar2==13){ 
              rxString2=inputString2;  
              inputString2=""; 
              stringComplete2 = true;
              flagEvent2 = true;
            }
        }      
        //Retardo necesario para la recepcion de datos.
        delayMicroseconds(usdelayRate2);
        // Si no hay mas datos salir de la rutina.         
    }  
    if (stringComplete2) {        
        stringComplete2 = false;   
        if(rxString2.length()>0){
          decodificarRecepcion(rxString2);
        }
    }
}
/*----------------------------------------------------------------*/
// Verifica si la cadena recibida es un comando de modificacion.
void decodificarRecepcion(String strRecepcion){
    word index=-1,iniStr=0,finStr=0;
    strDecodificar="";
    comando="", parametro="";
    if(strRecepcion.startsWith("RFID???")){
        //Serial1.println(strRecepcion);
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
            }            
        }        
        if(strDecodificar.length()!=0){          
          Serial.print("CMD: RFID???|");    Serial.println(strDecodificar);
          printLineUTFT(strDecodificar,VGA_TEAL);
          if(enableDebug){
            Serial2.print("CMD: RFID???|");   Serial2.println(strDecodificar);
          }
        }
    }
    
}
/*----------------------------------------------------------------*/
//Ejecuta el comando con un parametro recibido.
void ejecutarComando(){
    respuesta="";
    //_____________________ TIME = HH:MM:SS ___________________________
    if (comando == "TIME"){
        if(parametro=="??")
            respuesta+=("TIME~HH:MM:SS|");  
        else {
            if (parametro=="?")
                    respuesta+=("TIME~"+myRTC.getTimeStr(FORMAT_LONG)+"|");        
            else{
                if (myRTC.setTimeStr(parametro)) 
                    respuesta+=("TIME~"+myRTC.getTimeStr(FORMAT_LONG)+"|");            
                else                              
                    respuesta+="TIME~ERROR";   
            }
        }
    }
    //_____________________ DATE = DD/MM/YYYY ___________________________
    if (comando == "DATE"){
        if(parametro=="??")  
              respuesta+=("DATE~DD/MM/YYYY|");         
        else{
            if (parametro=="?")
                    respuesta+=("DATE~"+myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/')+"|");        
            else{
                if (myRTC.setDateStr(parametro)) 
                    respuesta+=("DATE~"+myRTC.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/')+"|");            
                else                              
                    respuesta+="DATE~ERROR"; 
            }
        }
    }
    //_____________________ FILE=DIRECTORY/FILE.EXT ___________________________
    if (comando == "FILE"){
        if(parametro=="??"){
            showSDCardINFO();
            showVolumeSizeINFO();
        }
        else{
            if (parametro=="?"){
                //listFilesINFO();
                showFilesINFO();
            }
            else {
                readFileSDCard(parametro);
            }
       }
    }
    //________________________ DEBUG = ON or OFF _______________________________
    if (comando == "DEBUG"){
        if (parametro=="ON"){
            enableDebug =true;
            _debug_time=0;
            Serial.println("DEBUG-ON");
            Serial2.println("DEBUG-ON");
        }
        else
            enableDebug = false;
    }
    if (comando == "RESET"){
        if (parametro=="ON"){
          //rstc_start_software_reset(RSTC);
        }
    }
}
