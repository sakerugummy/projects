#define uGMT -4 * 3600L
#define SEC_1970_TO_2000 946684800
static const uint8_t daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Time currentTime;
String output="";
const int pinSQW  = 7;  // INT7

void initRTC(){
  myRTC.begin();
  myRTC.setOutput(OUTPUT_SQW);
  myRTC.setSQWRate(SQW_RATE_1);
  updateDateTime();
  attachInterrupt(digitalPinToInterrupt(pinSQW), updateDateTime, RISING );
}

void updateDateTime(){
  flagMostrar=true;
  currentTime   = myRTC.getTime();
  velocidad.POS = currentTime.hour;
  epochCurrent  = getUnixTime();
  Serial1.println(epochCurrent);
}


uint32_t getUnixTime(){
  uint16_t  dc;
  dc = currentTime.day;
  for (uint8_t i = 0; i<(currentTime.mon-1); i++)
    dc += daysInMonth[i];
  if ((currentTime.mon > 2) && (((currentTime.year-2000) % 4) == 0))
    ++dc;
  dc = dc + (365 * (currentTime.year-2000)) + (((currentTime.year-2000) + 3) / 4) - 1;
  return ((((((dc * 24L) + currentTime.hour) * 60) + currentTime.min) * 60) + currentTime.sec) + SEC_1970_TO_2000 - uGMT;
}

void setUnixTime(uint32_t unixTime){
  uint8_t seconds, minutes, hours; 
  uint8_t dayOfWeek, days, month;
  uint16_t year;

  unixTime +=uGMT;
  
  seconds = (unixTime % 60);
  minutes = (unixTime % 3600L)/60;
  hours   = (unixTime % 86400L)/3600;
  unixTime /= 86400L;

  //Unix time starts in 1970 on a Thursday
  year = 1970;
  dayOfWeek = 4;

  while(1)  {
    bool leapYear   = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    uint16_t daysInYear = leapYear ? 366 : 365;
    if (unixTime >= daysInYear){
      dayOfWeek += leapYear ? 2 : 1;
      unixTime  -= daysInYear;
      if (dayOfWeek >= 7)
        dayOfWeek -= 7;
      ++year;
    }
    else {
      dayOfWeek =(unixTime+dayOfWeek) %7;
      //static const uint8_t daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
      for(month = 0; month < 12; ++month){
        uint8_t dim = daysInMonth[month];
        // add a day to feburary if this is a leap year
        if (month == 1 && leapYear)
          ++dim;
        if (unixTime >= dim)
          unixTime -= dim;
        else
          break;
      }
      days=unixTime+1;
      month++;
      dayOfWeek=dayOfWeek==0? 7 : dayOfWeek;
      break;
    }
  }
  myRTC.setTime(hours,minutes,seconds);
  myRTC.setDate(days,month,year);
  myRTC.setDOW(dayOfWeek);
}
