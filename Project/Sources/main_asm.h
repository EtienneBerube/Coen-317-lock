#ifndef _MAIN_ASM_H
#define _MAIN_ASM_H

#ifdef __cplusplus
    extern "C" { /* our assembly functions have C calling convention */
#endif

void rgb_light(char);
  /* interface to my assembly main function */
  
void delay1ms(void);

#ifdef __cplusplus
    }
#endif

#endif /* _MAIN_ASM_H */
