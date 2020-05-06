//2018-08-20 08:30:00
//2018-10-27 11:49:00
#define EPOCH_COMPILE 1534753800L
#include <EEPROM.h>
#include <URTC.h>

#define FORMAT_SHORT 1
#define FORMAT_LONG  2
#define FORMAT_LITTLEENDIAN 1
#define FORMAT_BIGENDIAN    2

URTC myRTC(A4, A5);
unsigned long epochCurrent;
boolean flagConfig=false;

//-----> Datos del contador
struct conteo{
  unsigned long TOTAL = 0;      //(4) Cantidad acumulada en el proceso de produccion.
  unsigned long TURNO = 0;      //(4) Cantidad acumulada por turno de operador.
  unsigned long ANTERIOR  = 0;  //(4) Cantidad acumulada en la captura anterior.
}conteo;

struct estado{
  byte ACTUAL = 0;            //(2) Estado actual del contador.
  boolean ACTIVO =false;      //(1) Conteo habilitado(true) [Utilizado en isrTimer1].
}estado;

struct velocidad{
  byte POS=0;
  unsigned long HORA = 0;   //(4) Cantidad acumulada en una hora.
  unsigned long RPM0 = 0;   //(4) Revoluciones por minuto de INT0.
  unsigned long RPM1 = 0;   //(4) Revoluciones por minuto de INT1.
  unsigned long LAP0 = 0;   //(4) Duracion en (ms) del conteo en INT0.
  unsigned long LAP1 = 0;   //(4) Duracion en (ms) del conteo en INT1.
}velocidad;

struct contador{
  int id=0;                         //(1) ID del contador RFID001
  byte Incremento = 1;              //(2) Incremento del contador (+1)
  unsigned long comparaLAP = 5000;  //(4) Tiempo en milisegundos para determinar si se encuentra DETENIDO      
} contador;

//_________________________________
//-----> Datos de Produccion
struct produccion{
    unsigned long LOTE = 0;     // Numero de lote    
    unsigned long ID = 0;       // Hora que se asigno la produccion
    unsigned long INICIO = 0;   // Hora que inicio produccion
    unsigned long FINAL = 0;    // Hora que finalizo produccione
} produccion;

//_________________________________
//-----> DATOS DE TEMPORIZACION
struct tiempos{
      // NO OPERABILIDAD - SIN PRODUCCION
      unsigned long horas_PRD_NO_OPERABILIDAD;
      unsigned long horas_NOP_AJUSTE_MAQUINA;
      unsigned long horas_NOP_LIMPIEZA_RADICAL;      
      unsigned long horas_NOP_CAMBIO_FORMATO;
      unsigned long horas_NOP_CAMBIO_TURNO;
      unsigned long horas_NOP_SIN_OPERADOR;
      unsigned long horas_NOP_FALLA_MAQUINA;
      unsigned long horas_NOP_MANTENIMIENTO_PREVENTIVO;
      unsigned long horas_NOP_MANTENIMIENTO_CORRECTIVO;
      unsigned long horas_NOP_ENSAYOS_DESARROLLO;
      // DURANTE LA PRODUCCION        
      unsigned long horas_PRD_NUEVA_PRODUCCION;
      unsigned long horas_PRD_SACANDO_MUESTRAS;
      unsigned long horas_PRD_ESPERANDO_APROBACION;      
      unsigned long horas_PRD_PRODUCIENDO;
      unsigned long horas_PRD_PRODUCCION_DETENIDA;
      // INTERRUMPIR PRODUCCION
      unsigned long horas_INT_AJUSTE_MAQUINA;
      unsigned long horas_INT_LIMPIEZA_EQUIPO;
      unsigned long horas_INT_CAMBIO_INSUMOS;
      unsigned long horas_INT_MATERIA_PRIMA;
      unsigned long horas_INT_CAMBIO_TURNO;
      unsigned long horas_INT_FALLA_MAQUINA;
      unsigned long horas_INT_MANTENIMIENTO_CORRECTIVO;
}tiempos;

void setup() {
  initSERIAL1();
  initEEPROM();
  initRTC();
  initCONTADOR();
}

void loop() {
  verificarRX1();  
  actualizarContador();
}
//_________________________________________________________________
// Al cargar el ID  de la EEPROM lo convertimos en Cadena de texto.
String getRFID(){
  if (contador.id<=0 && contador.id>=1000){
    contador.id=0;
  }
  String RFID="RFID000";
  if (contador.id<100){ if(contador.id<10)RFID="RFID00";
                        else RFID="RFID0";  }
  else   RFID="RFID"; 
  return (RFID+contador.id);
}

//-----> Estados del CONTADOR  
enum eventos{
  //0
  MSG_CONTADOR_ENCENDIDO,
  //1
  PRD_NO_OPERABILIDAD,
  //2-14
  NOP_AJUSTE_MAQUINA,
  NOP_LIMPIEZA_RADICAL,      
  NOP_CAMBIO_FORMATO,
  NOP_CAMBIO_TURNO,
  NOP_SIN_OPERADOR,
  NOP_FALLA_MAQUINA,
  NOP_MANTENIMIENTO_PREVENTIVO,
  NOP_MANTENIMIENTO_CORRECTIVO,
  NOP_ENSAYOS_DESARROLLO,
  //     
  NOP_TAREA_NO_CONCLUIDA,
  NOP_FALTAN_RECURSOS,
  NOP_FALLA_CONTADOR,
  NOP_FINALIZO_TAREA,
  //15-20              
  PRD_NUEVA_PRODUCCION,
  PRD_SACANDO_MUESTRAS,
  PRD_ESPERANDO_APROBACION,
  PRD_INICIA_PRODUCCION,
  PRD_PRODUCIENDO,
  PRD_PRODUCCION_DETENIDA,
  //21-27
  INT_AJUSTE_MAQUINA,
  INT_LIMPIEZA_EQUIPO,
  INT_CAMBIO_INSUMOS,            // (OXIGENO, NITROGENO)
  INT_MATERIA_PRIMA,
  INT_CAMBIO_TURNO,
  INT_FALLA_MAQUINA,
  INT_MANTENIMIENTO_CORRECTIVO,
  
  INT_FALTAN_RECURSOS,
  INT_FALLA_CONTADOR,
  //30
  PRD_FINALIZO_PRODUCCION,
  //31-36
  MSG_CONTINUA_PRODUCCION,      
  MSG_MUESTREO_PARCIAL,
  MSG_RESETEA_CONTADOR,
  MSG_RESETEA_PRODUCCION,
  MSG_CANCELAR_PRODUCCION,
  MSG_RETRANSMITE_PRODUCCION, 
  //37-38        
  MSG_HABILITAR_CONTADOR ,
  MSG_DESHABILITAR_CONTADOR,
  //39
  MSG_CAPTURA_CONTEOHORA
};
