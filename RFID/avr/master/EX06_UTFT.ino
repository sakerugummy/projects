void initUTFT(){
    myGLCD.InitLCD();
    myGLCD.setFont(LittleFont);
    myGLCD.clrScr();    
    pos_y=0; dim_y=myGLCD.getFontYsize(); 
    lim_y = myGLCD.getDisplayYSize()/myGLCD.getFontYsize();
}

void printLineUTFT(String inputText,word colorText){
    if(pos_y>=lim_y) {
      pos_y=0; myGLCD.clrScr();
    }
    myGLCD.setColor(colorText);
    myGLCD.print(inputText, 0, pos_y * dim_y );
    pos_y++;
  
}


