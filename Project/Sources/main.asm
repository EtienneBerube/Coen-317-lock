;**************************************************************
;* This stationery serves as the framework for a              *
;* user application. For a more comprehensive program that    *
;* demonstrates the more advanced functionality of this       *
;* processor, please see the demonstration applications       *
;* located in the examples subdirectory of the                *
;* Freescale CodeWarrior for the HC12 Program directory       *
;**************************************************************

; export symbols
            XDEF rgb_light
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

rgb_light:   
    ; CODE FOR THE RGB LED
            pshx        
            MOVB $FF, DDRM        ; Set the port to output
            MOVB $FF, DDRP        ; Set to output
            MOVB $00, PTM         ; Allow for the RBG LED  
            CMPB #$00             ; check if B has 0
            beq loadRed           ; Proceed to making the RGB LED red if a != 0
            movb #%00010000, PTP         ; else make the RGB LED green
            
            pulx
            RTC                   ; return to caller

loadRed:    MOVb #%01000000, PTP            ; macro to load Red in the LED   
            pulx
            RTC   
                  
delay1ms:
         LDX #8000
         
   loop: nop
         nop
         nop
         dbne x,loop