#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "main_asm.h" /* interface to the assembly module */



void main(void) {
  
 // initialize
 
	EnableInterrupts;

 asm_main(); /* call the assembly function */


  for(;;) {
    
    //check numpad
    
    //show numebrs on seven segment
    
    //if 4 numbers entered 
    //check to see if match
    //if works
    //LED -> green
    //else
    //Buzzer + LED -> RED
    //if 3 attemps -> ALL LEDS RED
    
    
  } /* loop forever */
  /* please make sure that you never leave main */
}
