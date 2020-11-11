/*
 * Kiersten Page
 * Embedded HW2
 * Due: February 21, 2020
 * 
 * 4.1 Practice compile-time debugging by fixing all of the syntax errors in the 
 * following listing. Do yourself a favor and work through all the bugs using the 
 * Arduino IDE compile output information.Turn in the source code listing of your 
 * debugged program. Clearly state all of the syntax errors that you found.
 */

void MyDelay (unsigned long mSecondsApx );
void setup(){
  unsigned char *portDDRB;
  portDDRB = (unsigned char *) 0x24;
  *portDDRB |= 0x20;

}
void loop (){
  unsigned char *portB;
  portB = (unsigned char *) 0x25;
  *portB |= 0x20; //replaced O's with 0's
  MyDelay(1000); //replaced {} with () and add a semicolon
  *portB &= 0xDF; //replaced O with 0 and : with ;
  MyDelay(1000); //replaced [] with () and , with ;
}
void MyDelay (unsigned long mSecondsApx ){
  volatile unsigned long i;
  unsigned long endTime = 1000 * mSecondsApx ;
  for (i = 0; i < endTime ; i++);
}
