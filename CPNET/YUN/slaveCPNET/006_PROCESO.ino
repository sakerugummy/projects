void nuevaProduccion(){
  produccion.ID = epochCurrent;
  conteo.TOTAL=0;   conteo.TURNO=0; conteo.ANTERIOR=0;
  guardarParametros();
}
void iniciarProduccion(){
  detenerContador();  flagContador = false; 
  conteo.TOTAL=0;   conteo.TURNO=0; conteo.ANTERIOR=0;
  actualizarEstadoCONTADOR(PRD_INICIA_PRODUCCION);
  produccion.INICIO = epochCurrent;
  guardarParametros();
  // Iniciamos PRODUCCION
  iniciarContador();
}

void finalizarProduccion(){
  produccion.FINAL = epochCurrent;
  detenerContador();  
  actualizarEstadoCONTADOR(PRD_FINALIZO_PRODUCCION);
  actualizarVelocidadHORA();
  guardarParametros();  
  
}

void resetearPRODUCCION(){
    // Detenemos el conteo TOTAL y TURNO
    conteo.TOTAL=0;   conteo.TURNO=0; conteo.ANTERIOR=0; velocidad.HORA=0;
    estado.ACTUAL=PRD_NO_OPERABILIDAD;  estado.ACTIVO =false; 
    detenerContador();
    guardarParametros();
    resetearTiemposPRODUCCION();
}
