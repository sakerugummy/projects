/*----------------------------------------------------------------*/
void initRTC(){
  // Habilita el modo RUN al RTC
  myRTC.halt(false);
  myRTC.writeProtect(false);
  Serial.println("\nInicializando RTC...");
  actualizarHoraACTUAL();
  delay(20);
  Serial.print("La hora actual es: "); Serial.println(horaACTUAL);
}
/*----------------------------------------------------------------*/
// Actualiza la hora y fecha actual en un String
void actualizarHoraACTUAL(){
  SdFile::dateTimeCallback(dateTime);
  String _time = myRTC.getTimeStr(FORMAT_LONG);
  String _date = myRTC.getDateStr(FORMAT_LONG,FORMAT_BIGENDIAN,'/');
  FileName = _date; 
  FileName.replace("/","");
  horaACTUAL =_time;
}
//------------------------------------------------------------------------------
// Agrega una fecha a la creacion de Archivos
void dateTime(uint16_t* date, uint16_t* time) {
  Time now; 
  now = myRTC.getTime();
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year, now.mon, now.date);
  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour, now.min, now.sec);  
}
