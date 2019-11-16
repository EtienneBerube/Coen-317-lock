#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "main_asm.h" /* interface to the assembly module */
#include <math.h> /* for mathematical computations */   //TODO: REMOVE Since not using anymore

#define CYCLES 2812500
/* Note of how to select cycles
 * Clock = 24MHz / pre-scaler => 24*10^6/128 = 187.5KHz
 * Period = 5.33uS
 * We want a delay of 15 seconds.
 * Thus we need 2,812,500 cycles
 */
#pragma CODE_SEG NON_BANKED
interrupt void oc5ISR(void){
   TC5 = TC5 + CYCLES;
}
#pragma CODE_SEG DEFAULT /* change code section to DEFAULT (for Small Memory Model, this is $C000) */
 // Interrupt Vector Table   
 typedef void (*near tIsrFunc)(void); // keyword in HCS12 so that the following is in nonbanked (a PPAGE value is not added) memory
// const tIsrFunc _vect[] @0xFFE4 = { // 0xFFCC is the address to store the PORTH interrupt vector
    /* Interrupt table */
//    oc5ISR // OC5 Interrupt
// };
void setupBradsSpecialFancySuperDeluxSaucyDelayWhosFunctionNameWillNeedToBeChangedSoon(void){
    //Make the timer using the output compare method demonstrated in the slides
    TIOS = 0x20; //Enable input capture
    TCTL1 = 0x0C; //Select OC5 action: pull high
    TSCR2 = 0x03;
    TSCR2 = 0x06; //Set pre-scaler to 64. TODO: Verify what a good value for this would be -> clock cycle = E-cycle/prescaler where E-cycle = 24MHz
    TSCR1 = 0x90; //Enable the timer;
    TFLG1 = 0xFF; //clear all CnF flags
    TC5 = TCNT + 10; //TODO: verify cycle count here to change delay? maybe can just use this instead of ISR? If so, use same math as for cycles define but add math to define it in seconds
    while (TFLG1&0x20 != 0x20); //Wait for succesful comparison
    TCTL1 = 0x04;
    //TC5 = TC5 + HCYCLES;
    TIE = 0x20; //Enable OC5 interrupt
    asm("cli"); //Global Enable


    //other method code (pulse accumulator) k
    // TIOS = 0x01; //Set channel 0 to output compare;
    // TCTL1 = 0x0C; //TODO: Verify what this does
    // TSCR2 = 0x03; //Set pre-scaler to 8. TODO: Verify what a good value for this would be -> clock cycle = E-cycle/prescaler where E-cycle = 24MHz
    // TSCR1 = 0x90; //Enable the timer;


}

void lightUpLED(int result[4], int correctPasscode){
  int numberEntered = 0;
  int i, j;
  
  for(i = 0;i < 4;i++){
    int exponent = 1;
    
      for(j = 3-i ; j> 0;j--){
        exponent *= 10;
      }
    numberEntered += result[i] * exponent; 
   }

  if (numberEntered == correctPasscode) {
    rgb_light(0x01);
  } else {
    //Light up LED RED
    rgb_light(0x00);
  }

}
int checkNumpad(){
  //Define values for keypad
  short keycodes[4][4] = {{1,2,3,10},{4,5,6,11},{7,8,9,12},{-1,0,-1,13}};
  
  int toReturn = -1;

  int counter = 0;
  int test = 0x08;
  
  unsigned int i;
  int result;
   
  for(i=0; i < 4; i++){
     test = test << 1; 
     
     PORTA = test;
     result = PORTA & 0x0F; // take result
     if(result != 0 && result <= 4){
       toReturn = keycodes[(test >> 4) & 0x0F][result-1]; //Get right spot in array
     }
   }
   return toReturn;
    
}

void showValues(int values[4], int pointer){
//0-F on 7-segment display
  int segment_map[16]  = {0x3F, 0x06,0x5B, 0x4F, 0x66,0x6D,0x7D,0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  
  unsigned int i;
  
  for(i = 0; i < pointer; i++){
    if(values[i] != -1){
     PTP = ~(i+1);
     PORTB = segment_map[values[i]];
     delay1ms(); 
    }
  }
}

void main(void) {
  
    
  int input[4];
  int pointer = 0;
  int temp[4] = {1,2,3,4};
  int counter = 0;
  
  DDRA = 0xF0; // Port A (Keypad) 0..3 input and 4..7 outputs
  
  DDRB = 0xFF; //Port B (7-Segment) output
  DDRP = 0x0F; //Port P(0..3) (7-segment selection) output
  
  PORTB = 0xFF;
  
  
                        
	EnableInterrupts;
	
	
	
	lightUpLED(temp, 1234);

  for(;;) {
    
    //check numpad
    if(counter == 0){
      int value = checkNumpad();
      
        if(value != -1){
          input[pointer++] = value;
          //disable numpad for aproximately 20ms (debouncing)
          counter = 8000;
        }
    }else{
      counter--; 
    }
    
    
      
    //show numbers on seven segment
    showValues(input, pointer);
    if(pointer == 4){
    
    //Check if matches
      lightUpLED(input, 6969);
      pointer = 0;
      input[0] = -1;
      input[1] = -1;
      input[2] = -1;
      input[3] = -1;
    }
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

/* Function which checks whether the number inputted in the numpad is correct
   IF the number is correct, it loads 01 into the register A
   If the number is incorrect, it loads 00, 
   The asm function will then light up the RGB LED with the correct colour
*/