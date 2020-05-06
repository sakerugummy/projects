//FUNCIONA 20181029
/*=================================================================*/
/*___________________  IMPORTAMOS LIBRERIAS  ______________________*/
/*=================================================================*/
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>   // Ethernet Shield WIZ550
//#include <TimerOne.h> //Timer para AVR
#include <DueTimer.h>   //Timer para ARM
#include <tinyRTC.h>    // Reloj de Tiempo Real DS1307 + 24C32
#include <UTFT.h>       //21/07/2016

/*=================================================================*/
/*____________________  DEFINIMOS CONSTANTES ______________________*/
/*=================================================================*/
#define SS_NET 53 // MEGA
#define SS_SD   4 // SD
#define SS_UNO 10 // UNO, DUE
#define MAX_TIME_DEBUG 300   // Segundos que demora la perdida de conexion para el HC06
#define PIN_LED_RX 13
#define PIN_RESET_NET 8
#define MAX_CON_ERROR 5
/*=================================================================*/
/*_____________________ RECURSOS DE HARDWARE EXTERNOS _____________*/
/*=================================================================*/
// Introducir una dirección MAC y la dirección IP para el controlador de abajo.
//byte mac[] = {0x48, 0x55, 0x42, 0x45, 0x52, 0xA1};
//byte mac[] = {0x48, 0x55, 0x42, 0x45, 0x52, 0x08}; // BETALACTAMICOS
//byte mac[] = {0x48, 0x55, 0x42, 0x45, 0x52, 0x16}; // SOLIDOS NO ESTERILES P1
byte mac[] = {0x48, 0x55, 0x42, 0x45, 0x52, 0x24}; // INYECTABLES
//byte mac[] = {0x48, 0x55, 0x42, 0x45, 0x52, 0x42}; // LIQUIDOS NO ESTERILES Y SEMISOLIDOS


//Rango:   de 192.168.70.2 a 192.168.70.30
//IPAddress ip      (192,168, 70,  7);
//IPAddress ip      (192,168, 70,  8); // BETALACTAMICOS
//IPAddress ip      (192,168, 70, 16); // SOLIDOS NO ESTERILES P1
IPAddress ip      (192,168, 70, 24); // INYECTABLES
//IPAddress ip      (192,168, 70, 42);  // LIQUIDOS NO ESTERILES Y SEMISOLIDOS
//IPAddress ip      (192,168, 70, 22);
/*
IPAddress dnServer(192,168, 70,  1);
IPAddress subnet  (255,255,255,192);
IPAddress gateway (192,168, 70,  1);
IPAddress serverRFID  (172, 16, 10,203);
*/
//byte mac[] = {0x48, 0x55, 0x42, 0x45, 0x52, 0x05}; // HOME
//IPAddress ip      (192,168, 70, 5);//IPAddress ip      (192,168,  1, 16);
IPAddress dnServer(192,168, 70,  1);
IPAddress subnet  (255,255,255, 0);
IPAddress gateway (192,168, 70,  1);
IPAddress serverRFID  (192,168, 70, 254);


/*
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
IPAddress ip      (192,168,  1, 81);
IPAddress dnServer(192,168,  1,  1);
IPAddress subnet  (255,255,255,  0);
IPAddress gateway (192,168,  1,  1);
IPAddress serverRFID (192,168,  1, 42); */ 
EthernetClient client;

extern uint8_t LittleFont[];
extern uint8_t TinyFont[];

UTFT myGLCD(ITDB32WD,38,39,40,41); 
byte pos_y,dim_y,lim_y;

//tinyRTC myRTC(SDA,SCL);
tinyRTC myRTC(20,21);

/*=================================================================*/
/*___________________ DECLARACION DE VARIABLES ____________________*/
/*=================================================================*/
String horaACTUAL;
/*=================================================================*/
/*______________________ PARA LA MEMORIA SD _______________________*/
/*=================================================================*/
// Configuramos SDCARD
Sd2Card card;
SdVolume volume;
SdFile directory_root;
/*_________________________________________________________________*/
boolean existeSDCard = false;
const int chipSelect = 4;         //ChipSelect del SD (SPI)
String FileName ="20141231.csv";  //Nombre del Archivo por defecto
/*=================================================================*/
/*___________________ PARA LA COMUNICACION SERIAL _________________*/
/*=================================================================*/
// Velocidad de transmision de la comuncacion serial
const long baudRate = 115200;
// Retardo en ms para la conexion con puerto
const byte usdelayRate= 200;
// Tamaño del Buffer de Recepcion del puerto Serie
const byte bufferSize = 200;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputString = "", rxString="";
// Recepcion finalizada del puerto(true)  
boolean stringComplete = false; 
//___________________________________________________________________
// Velocidad de transmicion de la comuncacion serial
const long baudRate1 = 115200;
// Retardo en ms para la conexion con puerto
const byte usdelayRate1= 200;
// Tamaño del Buffer de Recepcion del puerto Serie 
const byte bufferSize1 = 200;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputString1 = "", rxString1="";
// Recepcion finalizada del puerto(true)  
boolean stringComplete1 = false;
//___________________________________________________________________
// Velocidad de transmicion de la comuncacion serial
const long baudRate2 = 9600;
// Retardo en ms para la conexion con puerto
const byte usdelayRate2= 200;
// Tamaño del Buffer de Recepcion del puerto Serie 
const byte bufferSize2 = 200;
// inputString es el buffer RX del puerto  
// rxString es el último String recibido de RX
String inputString2 = "", rxString2="";
// Recepcion finalizada del puerto(true)  
boolean stringComplete2 = false;
/*__________________________________________________________________*/
// Habilita modo DEBUG  en la comunicacion Serial1
boolean enableDebug = false;
unsigned int _debug_time=0;
/*__________________________________________________________________*/
// Recepcion de modificacion de parametros por puerto serie.
String parametroRecibido="";
String strDecodificar=""; String strTransmitir ="";
String comando="", parametro="", respuesta="";
/*=================================================================*/
/*_________________ PARA LA COMUNICACION ETHERNET _________________*/
/*=================================================================*/
boolean     status_ethernet = true;
String      inputRequest = "";
String      strRequest = ""; 
boolean     requestComplete = false;
const byte  usdelayRequest= 200;

/*__________________________________________________________________*/
boolean flagVerificar=false;
byte toggle=0;
/*=================================================================*/
/*___________________ CONFIGURACIONES INICIALES ___________________*/
/*=================================================================*/
void setup() {
  pinMode(PIN_LED_RX, OUTPUT);
  pinMode(PIN_RESET_NET,OUTPUT);   delay(10);
  resetNET();
  delay(100);
  initUTFT();
  //Habilitamos el puerto de comunicacion Serie
  initUART(); 
  //Cargamos datos de la EEPROM 
  initAlmacenamiento();
  //Configuramos el reloj de tiempo Real
  initRTC();
  //Configuramos el bus SPI SLAVE
  initSPI();
  //Configuraciones del SD
  initSDCARD();
  //Comfiguramos como cliente HTTP webService.
  initClient();
  // Inicializamos Temporizador
  initTemporizador();

}
/*=================================================================*/
/*______________________ PROGRAMA PRINCIPAL _______________________*/
/*=================================================================*/
void loop() {   
    if (flagVerificar){     
       flagVerificar=false;        
       actualizarHoraACTUAL();
       toggle=(toggle+1)%2;
       digitalWrite(13, toggle);   
    }
    verificarUART ();
    verificarUART1();
    if (enableDebug) verificarUART2();
    verificarNET();   
}

void initSPI(){
  pinMode(SS_UNO, OUTPUT);
  pinMode(SS_SD,  OUTPUT);
  pinMode(SS_NET, OUTPUT);
  digitalWrite(SS_NET, HIGH);
  digitalWrite(SS_SD,  HIGH);
  digitalWrite(SS_UNO, HIGH);
}
/* Habilita el MicroSD */
void enableSD(){
  // Para comunicar con sd desabilitamos w5100 SPI (pin 10 HIGH)
  // Para comunicar con sd habilitamos SD SPI (pin 4 LOW)
  digitalWrite(SS_NET, HIGH);
  digitalWrite(SS_SD,  LOW);
  digitalWrite(SS_UNO, HIGH);
}
/* Habilita el protocolo Ethernet */
void enableNET(){
  // Para comunicar con sd habilitamos w5100 SPI (pin 10 LOW)
  // Para comunicar con sd deshabilitamos SD SPI (pin  4 HIGH)
  digitalWrite(SS_NET, LOW);
  digitalWrite(SS_SD,  HIGH);
  digitalWrite(SS_UNO, LOW);
}

void resetNET(){
  digitalWrite(PIN_RESET_NET,LOW); delay(10);
  digitalWrite(PIN_RESET_NET,HIGH);delay(10);
  digitalWrite(PIN_RESET_NET,LOW); delay(10);
  digitalWrite(PIN_RESET_NET,HIGH);delay(10);
}
