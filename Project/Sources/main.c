#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "main_asm.h" /* interface to the assembly module */


int checkNumpad(){
  //Define values for keypad
  uint8_t keycodes[4][4];
  keycodes[0]={1,2,3,10}
  keycodes[1]={4,5,6,11}
  keycodes[2]={7,8,9,12}
  keycodes[3]={-1,0,-1,13}
  
  int toReturn = -1;

  int counter = 0;
  int test = 0x08
   
   
   for(uint8_t i = 0 ; i < 4; i++){
     test = test << 1 
     
     PORTA = test;
     int result = PORTA & 0x0F; // take result
     if(result != 0 && result <= 4){
       toReturn = keycodes[(test >> 4) & 0x0F][result-1]; //Get right spot in array
     }
   }
   return toReturn;
    
}

void showValues(int[4] values, int pointer){
//0-F on 7-segment display
  int segment_map[]  = {0x3F, 0x06,0x5B, 0x4F, 0x66,0x6D,0x7D,0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  
  for(int i = 0; i < pointer; i++){
    if(values[i] != -1){
     PTP = ~(i+1);
     PORTB = segment_map[values[i]];
     delay1ms() 
    }
  }
}

void main(void) {
  
    
  int input[4];
  int pointer = 0;
  
  int counter = 0;
  
  DDRA = 0xF0; // Port A (Keypad) 0..3 input and 4..7 outputs
  
  DDRB = 0xFF; //Port B (7-Segment) output
  DDRP = 0x0F; //Port P(0..3) (7-segment selection) output
  
  
 
	EnableInterrupts;

  for(;;) {
    
    //check numpad
    if(counter == 0){
      int value = checkNumpad();
      
        if(value != -1){
          input[pointer++] = value
          //disbale numpad for aproximately 20ms (debouncing)
          counter = 8000;
        }
    }else{
      counter--; 
    }
    
    
    //show numbers on seven segment
    showValues(input, pointer);
    
    //Check if matches
    
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
