// Velocidad de transmicion de la comuncacion serial
const long baudRate = 115200;
// Retardo en ms para la conexion con puerto
const byte usdelayRate= 200;
// Velocidad de transmicion de la comuncacion serial  
const byte bufferSize = 200;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputString = "", rxString="";
// Recepcion finalizada del puerto(true)  
boolean stringComplete = false;
// Recepcion de modificacion de parametros por puerto serie.
String parametroRecibido="";
String strDecodificar=""; String strComunicar ="";
String comando="", parametro="", respuesta="";


//Para la transmicion y re-transmicion de los eventos.
//List <String> lstTransmitir;
//List <byte>   lstVerificaciones;

//Cadena de TRANSMISION
String strTransmitir;
String strRetransmitir="";
byte vecesTX=0;
byte verificarEstado=0;
byte recibidoEstado=0;
// Contador de segundos para relizar una TX 
unsigned long segundosTX = 0;
// Este flag habilita(true) o deshabilita(false) la TX.
boolean enableTX = false;
/*=================================================================*/
// Configuramos puerto 0 para comunicacion con el XBEE S2B PRO

void initSERIAL(){      
    Serial.begin(baudRate);
    inputString.reserve(bufferSize);  rxString.reserve(bufferSize); 
    //transmitirContadorENCENDIDO();    
 
    // parametros para las rutinas de decodificacion temporal 
    strDecodificar.reserve(100);
    strComunicar.reserve(100);
    comando.reserve(10);
    parametro.reserve(60);
    respuesta.reserve(bufferSize);
    // Habilita o deshabilita la comunicacion Serial
    enableTX=false;     
    //lstTransmitir.setPrinter (Serial);
}

/*=================================================================*/
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
/*=================================================================*/
// Verificamos si se ha recepcionado una cadena de texto en COM-0.
void verificarRX0(){
    if (stringComplete) {
        stringComplete = false;
        if (rxString.length()>4){         
          decodificarRecepcion(rxString);
          //Serial.println(strDecodificar);
          if (strDecodificar.length()>4){        
              //lstTransmitir.push(strDecodificar); flagPrioridadTX=true;      
              strTransmitir=strDecodificar;
              realizarTransmision();
          }
       }
    }
}
