#define EEPROM_DIR_DEFAULT    0x0000
#define EEPROM_DIR_CONTEO     0x0010
#define EEPROM_DIR_ESTADO     0x0020
#define EEPROM_DIR_CONTADOR   0x0030
#define EEPROM_DIR_PRODUCCION 0x0050
#define EEPROM_DIR_TIEMPOS    0x0100

void initEEPROM(){
  byte config_default=0;
  EEPROM.get(EEPROM_DIR_DEFAULT, config_default);
  if (config_default != 123)    parametrosPorDEFECTO();
  leerParametros();
}

void parametrosPorDEFECTO(){
  guardarParametros();
  EEPROM.put(EEPROM_DIR_DEFAULT,  123);     
}

void leerParametros(){
  noInterrupts();
  Serial.println("LEYENDO...");
  EEPROM.get(EEPROM_DIR_CONTEO,     conteo);
  EEPROM.get(EEPROM_DIR_ESTADO,     estado);
  EEPROM.get(EEPROM_DIR_CONTADOR,   contador);
  EEPROM.get(EEPROM_DIR_PRODUCCION, produccion);
  EEPROM.get(EEPROM_DIR_TIEMPOS,    tiempos);
  interrupts();
}

void guardarParametros(){
  noInterrupts();
  Serial.println("GUARDANDO...");
  EEPROM.put(EEPROM_DIR_CONTEO,     conteo);
  EEPROM.put(EEPROM_DIR_ESTADO,     estado);
  EEPROM.put(EEPROM_DIR_CONTADOR,   contador);
  EEPROM.put(EEPROM_DIR_PRODUCCION, produccion);
  EEPROM.put(EEPROM_DIR_TIEMPOS,    tiempos);
  interrupts();
}
