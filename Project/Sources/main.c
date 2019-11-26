#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "main_asm.h" /* interface to the assembly module */
#include <math.h> /* for mathematical computations */   //TODO: REMOVE Since not using anymore

#define CYCLES 15
unsigned int paov_cnt;

/* Note of how to select cycles
 * Clock = 24MHz / pre-scaler => 24*10^6/128 = 187.5KHz
 * Period = 5.33uS
 * We want a delay of 15 seconds.
 * Thus we need 2,812,500 cycles
 */
#pragma CODE_SEG NON_BANKED
interrupt void oc5ISR(void){
  //TSCR1 = 0x90;
  //PAFLG = 0x20;
  TFLG2 = 0x80;  
  
 
  paov_cnt = paov_cnt + 1;
  //if(paov_cnt == 43){
  //  delayFlag =0;
  //}
  
}

#pragma CODE_SEG DEFAULT /* change code section to DEFAULT (for Small Memory Model, this is $C000) */

typedef void (*near tIsrFunc)(void); // keyword in HCS12 so that the following is in nonbanked (a PPAGE value is not added) memory
 const tIsrFunc _vect[] @0xFFDE = { // 0xFFCC is the address to store the PORTH interrupt vector
    /* Interrupt table */
    oc5ISR // OC5 Interrupt
 };
 
 

 

void setupBradsSpecialFancySuperDeluxSaucyDelayWhosFunctionNameWillNeedToBeChangedSoon(){
  paov_cnt = 0;
  TCNT = 0;
  TSCR2 = 0x87;  //prescale factor == 128 & enable TOI(time overflow interrupt bit)
  TSCR1 = 0x80;  //start timer
  TFLG1 = 0xFF;  //
  TIOS = 0x20;
  asm("cli");
  
  while(paov_cnt != 43);
  TSCR1 = 0x00; //Stop timer
  TFLG2 = 0x80;
   
}

int checkCode(int result[4], int correctPasscode){
  int numberEntered = 0;
  int i, j;
  int exponent;
  
  for(i = 0;i < 4;i++){
    exponent = 1;
    
      for(j = 3-i ; j> 0;j--){
        exponent *= 10;
      }
    numberEntered += result[i] * exponent; 
   }

  if (numberEntered == correctPasscode) {
    // rgb_light(0x01);
    return 1;
  } else {
    //Light up LED RED
    // rgb_light(0x00);
    //TODO add flashing RGBled
    return -1;
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
  
  PTP = 0x0F;
  
  for(i = 0; i < pointer; i++){
    if(values[i] != -1){
     PTP = ~(1 << i);
     PORTB = segment_map[values[i]];
     delay1ms(); 
    }
  }
}

int decodeValue(unsigned char value){
 int counter;
    int values[9] = {0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    for (counter = 0; counter < 9; counter ++) {
        if (value == values[counter]) {
            return counter;
        }
    }
    // did not get a good value
    counter = - 1;
    return counter;
  
}

void showNo(){
 char no[] = {0x54,0x5C};
 int i, j;
 
 for(j = 0; j < 500; j++){
  
    for(i = 0; i < 2; i++){
        PTP = ~(1 << i);
        PORTB = no[i];
        delay1ms(); 
    }
  }
}

void showYes(){
 char yes[] = {0x6E,0x79,0x6D};
 int i, j;
 
 for(j = 0; j < 1000; j++){
  
    for(i = 0; i < 3; i++){
        PTP = ~(1 << i);
        PORTB = yes[i];
        delay1ms(); 
    }
  }
}

void main(void) {
  
  int badCounter = 0;
  int values[4] = { -1,-1,-1,-1};
  int pointer = 0;
  int input[4];
  int temp[4] = {1,2,3,4};
  int counter = 0;
  int delayIndex = 0;
  delayFlag = 0;
  
  DDRA = 0xF0; // Port A (Keypad) 0..3 input and 4..7 outputs
  
  DDRB = 0xFF; //Port B (7-Segment) output
  DDRP = 0x0F; //Port P(0..3) (7-segment selection) output
  DDRA = 0x00; //port a as input
  DDRT = 0xFF; //output port enable
  
  PORTB = 0xFF;
  
  
                        
    EnableInterrupts;
	
	
	
	//lightUpLED(temp, 1234);

  for(;;) {
       if(delayFlag == 0){
            
       showValues(values, pointer);
    
          if((PORTA & 1) == 0x01){
            //Button pushed               ;
            unsigned char input = PTH;
            int value = decodeValue(input);

            if(value != -1){
                values[pointer++]=value;
                
                for(delayIndex = 0; delayIndex < 200; delayIndex++)
                  delay1ms();
            }
          }
            
          showValues(values, pointer);
              
          if(pointer == 4){
              int result = checkCode(values, 1234);
              
              showValues(values, pointer);
              
              
              if(result == 1){
                  showYes();
                  badCounter = 0;
              }else{
                  showNo();
                  badCounter++;
                  if(badCounter == 3){
                        //LEDs all on and stalls
                        PORTB = 0xAA;
                        PTP = 0x0F;
                        setupBradsSpecialFancySuperDeluxSaucyDelayWhosFunctionNameWillNeedToBeChangedSoon();
                        badCounter = 0;
                  }else{
                        
                  }
              }

              pointer = 0;
              values[0] = -1;
              values[1] = -1;
              values[2] = -1;
              values[3] = -1;
              
          }
            
      } 
  }
 
     //check numpad
//    if(counter == 0){
//      int value = checkNumpad();
      
//        if(value != -1){
//          input[pointer++] = value;
          //disable numpad for aproximately 20ms (debouncing)
///          counter = 8000;
//        }
//    }else{
//      counter--; 
}
    
    
