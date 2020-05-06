#define EPOCH_COMPILE 1588521600UL
/*=================================================================*/
/*______________________ MANEJO DE LIBRERIAS ______________________*/
/*=================================================================*/
#include <UTFT.h>
#include <UTouch.h>
#include <UFrame.h>
#include <DueTimer.h>
#include <DS3231.h> 
//#include <List.h>
#include <SPI.h>
#include <SD.h>
#include "CP00_INIT.h"
/*=================================================================*/
/*___________________ CONFIGURACIONES INICIALES ___________________*/
/*=================================================================*/
unsigned long ant,pos;
boolean flagGuardar=false;
void setup() {
  // put your setup code here, to run once:  
  initRTC();
  initSERIAL();
  initFLASH();
  
  initNEXTION();
  mostrarCargando(15);
  //initSDCard();
  mostrarCargando(45);
  initTIMER1();
  mostrarCargando(75);
  initTIMER2();
  mostrarCargando(95);
  initCONTADOR();
  mostrarCargando(100);delay(100);
  mostrarPageContadores();
}

/*=================================================================*/
/*______________________ PROGRAMA PRINCIPAL _______________________*/
/*=================================================================*/
void loop() {  
  delay(MAX_TIME_DELAY);
  verificarRX0();
  verificarNEXTION();
  actualizarContador();
}
