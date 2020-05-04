// Realiza la transmicion si existe algun dato en la Lista de Transmision.
void realizarComunicacion1(){
    if(strDecodificar.length()!=0){
        Serial1.write(6);
        Serial1.print(ID); 
        Serial1.print(strDecodificar);
        Serial1.write(3);Serial1.println("");
    }
}
