/*
 * Kiersten Page
 * Embedded HW2
 * Due: February 21, 2020
 * 
 * 4.2 Practice run-time debugging by fixing the following listing, so that 
 * the LED will blink like it was meant to. Note: you should be able to do 
 * this with two minor changes.
 */

void NewDelay (unsigned long mSecondsApx ); //change char to long
void setup (){
  unsigned char *portDDRB ;
  portDDRB = (unsigned char *) 0x24;
  *portDDRB |= 0x20;
}
void loop (){
  unsigned char *portB ;
  portB = (unsigned char *) 0x25;
  *portB |= 0x20;
  NewDelay(100);
  *portB &= 0xDF; 
  NewDelay(100);
 }
 
void NewDelay (unsigned long mSecondsApx ){ //change char to long
  volatile unsigned long i; //change char to long
  unsigned long endTime = 1000 * mSecondsApx ;
  for (i = 0; i < endTime ; i++);
}
