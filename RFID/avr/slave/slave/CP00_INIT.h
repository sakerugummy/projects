  /*=================================================================*/
/*==================== DEFINICION DE CONSTANTES ===================*/
/*=================================================================*/
 // Comunicacion Serial para Display Nextion  
 #define Xbee Serial
 #define Nextion Serial3
 #define NextionBaud 9600
 // Nuevos COLORES para UTFT
 #define VGA_HEAVENLY 0x0F3E
 #define VGA_ORANGE   0xF3E6
 #define VGA_AMBAR    0xFEA0
 // Valor maximo en segundos para realizar una Transmision
 #define MAX_TIME_TX      10
 #define MAX_TIME_SAVE     5
 #define MAX_TIME_DELAY   10
 #define MAX_TIMES_CONFIRM 1
 #define TIME_RESET_PROD 120
 
 #define MAX_LEN_PRODUCTO 48
/*=================================================================*/
/*=============== RECURSOS EXTERNOS DE OTRAS LIBRERIAS ============*/
/*=================================================================*/
// FUENTES PARA EL LCD
extern uint8_t BigFont[];
//extern uint8_t SevenSegNumFont[];
extern uint8_t SevenSegNumFont48[];
extern uint8_t SevenSegNumFont64[];
extern uint8_t Inconsola[];

// IMAGENES Y LOGOS
//extern prog_uint16_t LogoCofarN[];
extern prog_uint16_t RESET[];
extern prog_uint16_t ON[];
extern prog_uint16_t GO[];
extern prog_uint16_t OFF[];
extern prog_uint16_t SETTINGS[]; 
boolean existeSDCard = false;
/*=================================================================*/
/*_____________________ RECURSOS DE HARDWARE EXTERNOS _____________*/
/*=================================================================*/
//----> Configuramos los pines del Display ITDB02-3.2WD
//UTFT myGLCD(Model,RS,WR,CS,RST,[ALE]);  
//UTFT       myGLCD(ITDB50,38,39,40,41);      // Para el Display ITDB02-5.0   TFT LCD Module IM120419008
UTFT        myGLCD(ITDB32WD,38,39,40,41);     // Para el Display ITDB02-3.2WD TFT LCD Module IM120419006 
//UTouch   myTouch(TCLK,TCS,TDIN,TDOUT,TIRQ)
//UTouch      myTouch(6,5,4,3,2);             // Para ITDB02 Arduino MEGA Shield IM120417024
UTouch     myTouch(44,45,46,48,49);         // Solo Cables.
//Configuramos los Frames
UFrame myFrame(&myGLCD, &myTouch);
// Configuramos los pines del Reloj de Tiempo REAL 
DS3231     myRTC(A4,A5);  // Para ITEAD Arduino Sensor Shield IM121211001
//tinyRTC   myRTC(20,21);  // Para ITEAD Arduino Mega Proto Screw Shield IM130715001
/*=================================================================*/
/*___________________ DECLARACION DE VARIABLES ____________________*/
/*=================================================================*/

//-----> Estados del CONTADOR  
enum estadoContador{
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

//_________________________________
//-----> Estados del MENU 
enum frameContador{
  MENU_PRINCIPAL,
  MENU_PRODUCCION,
  MENU_CONFIGURACIONES,
  MENU_TIEMPOS,
  MENU_INTERRUPCIONES,
  MENU_ESTADOS
};
byte PAGE=0;
String ID="RFID000";
//_________________________________
//-----> Estados del MENU 
struct contador{
//-----> Datos del contador
    int id=0;                       // ID del contador RFID001
    byte Incremento=1;              // Incremento del contador (+1)
    unsigned long conteoTOTAL = 0;    // Cantidad acumulada en el proceso de produccion    
    unsigned long conteoTURNO = 0;    // Cantidad acumulada por turno de operador
    unsigned long conteoANTERIOR = 0; // Cantidad acumulada en el conteo del periodo anterior de MUESTREO.
    unsigned long conteoPARCIAL = 0;  // Cantidad PARCIAL por cada periodo de MUESTREO
    unsigned long conteoHORA = 0;     // Cantidad acumulada en una HORA
    unsigned long Velocidad = 0;  // Velocidad Revoluciones por MUESTREO
    unsigned long comparaLAP = 3000;     // Tiempo en milisegundos para determinar si se encuentra DETENIDO    
    unsigned long Muestreo = 3600;       // Tiempo en segundos para tomar el muestreo del conteoPARCIAL
    byte Frecuencia = 50;              // Frecuencia para medir el conteo    
    byte estadoACTUAL = 0;            // Estado actual del contador
    unsigned long totalEVENTOS = 0;   // Total Eventos ocurridos.
    boolean Enabled = true;              // Conteo habilitado(true) [Utilizado en isrTimer1]
} contador;

//_________________________________
//-----> Datos de Produccion
struct produccion{
    unsigned long idProduccion; // Identitificador de produccion 
    byte Producto[MAX_LEN_PRODUCTO];  // Nombre del producto
    unsigned long  Lote;        // Numero de lote    
    unsigned long  horaCREACION;       // Hora que se asigno la produccion
    unsigned long  horaINICIO;         // Hora que inicio produccion
    unsigned long  horaFINAL;          // Hora que finalizo produccione
    boolean Salvado;            // Determina si la NUEVA PRODUCCION fue Guardada
    boolean Produciendo;        // Determina si esta EN MODO NO_OPERABILIDAD o MODO_PRODUCCION 
    boolean MuestrasCC;         // Permite consultar al operador si desea sacar muestras para CC
} produccion;


//_________________________________
//-----> Datos de Temporizacion
struct tiempos{
      // NO OPERABILIDAD - SINN PRODUCCION
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

/*=================================================================*/
/*___________________ COMPONENTENTES PARA LA PANTALLA _____________*/
/*=================================================================*/
// componentes tipo PANELS 
int pnlContador,  
    pnlProduccion,  
    pnlInterrupir,  
    pnlTiempos, 
    pnlConfigurar, 
    pnlAsignar;

// Componentes para Frame CONTADORES
int lblTituloPRODUCCION,lblTituloPRODUCTO,lblTituloLOTE,lblTituloESTATUS;
int lblPRODUCTO,lblLOTE,lblESTATUS;
int lblTituloH_INICIO,lblH_INICIO;
int lblTituloHDET,lblTituloHPROD,lblHDET,lblHPROD;
int lblTituloCONTEOTOTAL,lblTituloVELOCIDAD,lblTituloCONTEOPARCIAL,lblTituloRPM,lblTituloLAP;
int lblCONTEOTOTAL, lblVELOCIDAD,lblCONTEOPARCIAL,lblRPM, lblhoraACTUAL; 
int lblidPRODUCCION,lblconteoTURNO;
int btnRESET,btnPLAYSTOP,btnCONFIG;

// Componentes para el Frame PRODUCCION
int lblMenuPRODUCCION;
int lblNuevoPRODUCTO, lblNuevoLOTE;
int lblProcesarDATOS,lblForzarDETENIDO;
int txtNuevoPRODUCTO,txtNuevoLOTE;
int btnNuevaPRODUCCION,btnIniciarPRODUCCION,btnInterrumpirPRODUCCION;
int btnVolverPRODUCCION, btnResetearPRODUCCION;
int btnIzquierdaPRODUCCION, btnDerechaPRODUCCION;

// Componentes para el PANEL TIEMPOS
int lblMenuTIEMPOS;
int lblTitulohorasNOOPERABILIDAD,   lblhorasNOOPERABILIDAD;
int lblTitulohorasAJUSTEMAQUINA,   lblhorasAJUSTEMAQUINA;
int lblTitulohorasCAMBIOINSUMOS,    lblhorasCAMBIOINSUMOS;
int lblTitulohorasCAMBIOTURNO,      lblhorasCAMBIOTURNO;
int lblTitulohorasCAMBIOMATERIAPRIMA,      lblhorasCAMBIOMATERIAPRIMA;
int lblTitulohorasMANTENIMIENTOCORRECTIVO, lblhorasMANTENIMIENTOCORRECTIVO;
int lblTitulohorasMANTENIMIENTOPREVENTIVO, lblhorasMANTENIMIENTOPREVENTIVO;
int lblTitulohorasLIMPIEZAEQUIPO,   lblhorasLIMPIEZARADICAL;
int lblTitulohorasMUESTRASCC  ,     lblhorasMUESTRASCC;
int lblTitulohorasAPROBACIONCC,     lblhorasAPROBACIONCC;
int lblTitulohorasPRODUCIDAS, lblhorasPRODUCIDAS;
int lblTitulohorasDETENIDAS,  lblhorasDETENIDAS;
int btnIzquierdaTIEMPOS, btnDerechaTIEMPOS, btnVolverTIEMPOS;

//Componentes  para el Frame CONFIGURACIONES
int lblMenuCONFIGURACIONES;
int lblID, lblHoraFecha, lblIncremento , lblComparaLAP,lblFrecuencia, lblMuestreo;
int txtID, txtHora, txtFecha, txtIncremento, txtComparaLAP, txtFrecuencia, txtMuestreo;
int btnIzquierdaCONFIGURACIONES, btnDerechaCONFIGURACIONES;
int btnHabilitaMUESTRASCC;
int btnGuardarCONFIGURACIONES;
int btnVolverCONFIGURACIONES;

//Componentes para el Frame INTERRUPCIONES
int lblMenuINTERRUPCIONES;

// Eventos que ocurren al INTERRUMPIR PRODUCCION
int btnAjusteMAQUINA;
int btnCambioMATERIAPRIMA;
int btnCambioINSUMOS;
int btnCambioTURNO;
int btnRealizarLIMPIEZA;
int btnFallaMAQUINA;
int btnMantenimientoCORRECTIVO;
int btnCancelarPRODUCCION;
int btnVolverINTERRUPCIONES;

//Componentes para el Frame ESTADOS
int lblMenuESTADOS;
// EVENTOS QUE OCURREN ANTES DE INICIAR PRODUCCION
int btnAjustedeMAQUINA;
int btnLimpiezaRADICAL;
int btnCambiodeFORMATO;
int btnCambiodeTURNO;
int btnMantPREVENTIVO;
int btnMantCORRECTIVO;
int btnEnsayoDESARROLLO;
int btnFalladeMAQUINA;
int btnSinOPERADOR;
int btnVolverESTADOS;

// Manejo de los Niveles de los FRAMEs
int NIVEL = MENU_PRINCIPAL; // Muestra Pantalla de los Contadores.
 
