;**************************************************************
;* This stationery serves as the framework for a              *
;* user application. For a more comprehensive program that    *
;* demonstrates the more advanced functionality of this       *
;* processor, please see the demonstration applications       *
;* located in the examples subdirectory of the                *
;* Freescale CodeWarrior for the HC12 Program directory       *
;**************************************************************

; export symbols
            XDEF asm_main
            ; we use export 'Entry' as symbol. This allows us to
            ; reference 'Entry' either in the linker .prm file
            ; or from C/C++ later on
            XDEF delay1ms

; Include derivative-specific definitions 
		INCLUDE 'derivative.inc'  		

		    
; variable/data section
MY_EXTENDED_RAM: SECTION
; Insert here your data definition. For demonstration, temp_byte is used.
temp_byte:  DS.B   1

; code section
MyCode:     SECTION
; this assembly routine is called by the C/C++ application
asm_main:
            MOVB   #1,temp_byte   ; just some demonstration code
            NOP                   ; Insert here your own code
   
    ; CODE FOR THE RGB LED        
            MOVB $00, DDRM        ; Set the port to output
            MOVB $00, PTM         ; Allow for the RBG LED
            CMPA #$00             ; check if A has 0
            beq loadRed           ; Proceed to making the RGB LED red if a != 0
            movb $40, PTP         ; else make the RGB LED green
            
            RTS                   ; return to caller

loadRed:    MOVb $10, PTP            ; macro to load Red in the LED   
            RTS   
                  
delay1ms:
         LDX #8000
         
   loop: nop
         nop
         nop
         dbne x,loop         
