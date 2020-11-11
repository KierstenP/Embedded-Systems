/*
 * Kiersten Page
 * Embedded HW2
 * Due: February 21, 2020
 * 
 * 4.4 Practice run-time debugging by fixing the following listing, so that 
 * the LED will blink like it
 * was meant to.
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
  NewDelay (1000); //changed to 1000 to slow down blinking
  *portB &= 0xDF;
  NewDelay (1000); //changed to 1000 to slow down blinking
 }
void NewDelay (unsigned long mSecondsApx ){
  volatile unsigned long i;
  unsigned long j; //change char to long
  unsigned long k;
  unsigned long endTime = 100 * mSecondsApx ;
  for (i = 0; i < endTime ; i++){
    j = 10;
  }
  do{
    j = j - 1;
    k = i / j;
    } while (k > 0);
    }
