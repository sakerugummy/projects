
// Velocidad de transmicion de la comuncacion serial
const long baudRate1 = 115200;
// Retardo en ms para la conexion con puerto
const int usdelayRate1= 1000;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputString1 = "", rxString1="";
// Recepcion finalizada del puerto(true)  
boolean stringComplete1 = false; 
// Recepcion de modificacion de parametros por puerto serie.
String parametroRecibido = "";
String strDecodificar = "";
String comando = "", parametro = "", respuesta = "";
// Contador de segundos para relizar una TX 
String strTransmitir="";

void initSERIAL1(){      
  Serial.begin(baudRate1);
  Serial1.begin(baudRate1);
}
/*=================================================================*/
// Verificamos si se ha recepcionado una cadena de texto en COM-0.
void verificarRX1(){
    // Hay datos en el buffer del puerto serie.
    boolean flagEvent1=false;
    while (Serial1.available() && !flagEvent1) {
        char inChar = (char)Serial1.read();  
        //Serial.write(inChar);
        if (inChar!=13 && inChar!=10 && inChar!=6)
            inputString1 += inChar;            
        else{
            if (inChar== 6)
                inputString1 = "";
            if (inChar==13){
                Serial1.flush();
                rxString1 = inputString1;  
                inputString1 = "";
                stringComplete1 = true;
                flagEvent1=true; 
            }
        }
        //Retardo necesario para la recepcion de datos.
        //delayMicroseconds(usdelayRate1);
    }
    if (stringComplete1) {
        stringComplete1 = false;
        if (rxString1.length()>4){
          Serial.println(rxString1);
          decodificarRecepcion(rxString1);
          rxString1="";
          if (strDecodificar.length()>4){
              strTransmitir=strDecodificar;        
              realizarTransmision();
          }
       }
    }
}
