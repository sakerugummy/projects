// Velocidad de transmicion de la comuncacion serial
const long baudRate1 = 115200;
// Retardo en ms para la conexion con puerto
const byte usdelayRate1= 200;
// Velocidad de transmicion de la comuncacion serial  
const byte bufferSize1 = 200;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputString1 = "", rxString1="";
// Recepcion finalizada del puerto(true)  
boolean stringComplete1 = false; 

void initSERIAL1(){
  Serial1.begin(baudRate1);
    inputString1.reserve(bufferSize1);  rxString1.reserve(bufferSize1); 
}
/*----------------------------------------------------------------*/
// Verificamos si se ha recepcionado una cadena de texto en COM-1.
void verificarRX1(){
    boolean flagEvent1=false;      
    while (Serial1.available() && !flagEvent1) {
        char inChar1 = (char)Serial1.read();        
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
        stringComplete1 = false;   
        Serial1.println("BTH: ");Serial1.println(rxString1);
        if (rxString1.length()>4){
            decodificarRecepcion(rxString1);       
            if (strDecodificar.length() > 4){ 
                realizarComunicacion1();
                strTransmitir=strDecodificar;
                realizarTransmision();
            }   
        }     
    }
}
