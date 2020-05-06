/*================================================================*/
/*-------------------------     ETHERNET     ---------------------*/
/*================================================================*/
/*----------------------------------------------------------------*/
byte error_count=0;

void initClient(){
 
    Serial.println("\nInicializando ETHERNET...");    
    enableNET();
    //Ethernet.begin(mac);            //Solo Para DHCP
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);    
   
    Serial.print("El Cliente es: ") ;    Serial.println(Ethernet.localIP()); 
    Serial.print("El Servidor es: ");    Serial.println(serverRFID); Serial.println(); 
    
    printLineUTFT("NUMERO MAC  : "+DisplayMACAddress(),VGA_YELLOW);  
    printLineUTFT("DIRECCION IP: "+DisplayIPAddress(Ethernet.localIP()),VGA_YELLOW); 
    printLineUTFT("IP SERVIDOR : "+DisplayIPAddress(serverRFID),VGA_YELLOW); 
        
    inputRequest.reserve(100);
    strRequest.reserve(100);
    
    // Obtiene la hora Actual del Sistema
    rxString="RFID???|TNOW~???|";
    httpRequest(rxString);
    rxString="";  
}

String DisplayMACAddress(){
  return String(mac[0]) + "." + 
         String(mac[1]) + "." + 
         String(mac[2]) + "." + 
         String(mac[3]) + "." +
         String(mac[4]) + "." +
         String(mac[5]);
}
String DisplayIPAddress(IPAddress address)
{
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}
/*----------------------------------------------------------------*/
void verificarNET(){
  enableNET();
  requestComplete=false;
  while (client.available() && !requestComplete) {
    char inChar = client.read();  //Serial.write(inChar);
    if (inChar!=13 && inChar!=10 && inChar!=6)
        inputRequest += inChar;            
    else{
      if (inChar==6) inputRequest = "";
      //if (inChar==10) inputRequest = "";
      if (inChar==13){ 
        strRequest=inputRequest;  inputRequest=""; 
        requestComplete = true;
        if (strRequest.startsWith("RFID")){
          
          if(strRequest.length()!=0){
            // Transmite la respuesta del Servidor
            Serial1.println(strRequest);
            // Muestra el resultado si se encuentra en modo DEBUG.
            Serial.print("TX: "); Serial.println(strRequest);
            printLineUTFT(strRequest,VGA_LIME);
            if(enableDebug){
                Serial2.print("TX: ");  
                Serial2.println(strRequest);
            }
          }
          decodificarRecepcion(strRequest);
        }
      }
    }
    //Retardo necesario para la recepcion de datos.
    delayMicroseconds(usdelayRequest);
  }
}
/*----------------------------------------------------------------*/
// Realiza la conexion http al servidor
void httpRequest(String rxRequest) {  
  if (rxRequest.startsWith("RFID")){
     enableNET();
     client.stop();
     if (client.connect(serverRFID, 8080)) {
        rxRequest.trim();
        status_ethernet = true;
        client.print("GET /RFID/enlaceRFID.php?");
        client.print("result=");
        client.print(rxRequest);
        //client.println(" HTTP/1.0");
        client.println();
        Serial.print("NET:");   Serial.println(rxRequest);
        printLineUTFT(rxRequest,VGA_AQUA);
        if(enableDebug) {
          Serial2.print("NET:");    Serial2.println(rxRequest);
        }
    } 
    else {
      error_count++;
      if (error_count>=MAX_CON_ERROR){
        error_count = 0;
        resetNET();
        delay(1000);
        initClient();
      }
      
      Serial.print("NET:(ERROR) ");     Serial.println(rxRequest);
      printLineUTFT(rxRequest,VGA_RED);
      if(enableDebug) {
        Serial2.print("NET:(ERROR) ");    Serial2.println(rxRequest);
      }
      status_ethernet = false;
      writeErrorSDCard(rxRequest);
    }
    if (!client.connected()) {
      //initClient(); delay(1000);
    }
  }
}
