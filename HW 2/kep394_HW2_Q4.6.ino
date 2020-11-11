/*
 * Kiersten Page
 * Embedded HW2
 * Due: February 21, 2020
 * 
 * 4.6 Practice run-time debugging by fixing the following listing, so that the 
 * LED will blink like it was meant to.
 */
void NewDelay (unsigned long mSecondsApx );
void setup (){
  unsigned char * portDDRB ;
  portDDRB = (unsigned char *) 0x24;
  *portDDRB |= 0x20;
}
void loop (){
  unsigned char * portB ;
  portB = (unsigned char *) 0x25;
  *portB |= 0x20;
  NewDelay(1000); //changed to 1000 to slow down blinking
  *portB &= 0xDF;
  NewDelay(1000); //changed to 1000 to slow down blinking
}
void NewDelay (unsigned long mSecondsApx ){
  volatile unsigned long i;
  unsigned char j = 0; 
  unsigned long endTime = 100 * mSecondsApx ; 
  i = 0;
  while (j == 0){ //changed one equal sign to two equal signs (comparison operator)
    i++;
    if (i == endTime ){ //changed one equal sign to two equal signs (comparison operator)
      j = 1;
    }
  }
}
