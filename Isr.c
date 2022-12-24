
 /* Interrupt Handling*/

#include"iom128v.h"
#include"TO_var.h"
#include "TO.h"
#include "TO_s.h"

  #pragma interrupt_handler  isr_int0:2 isr_int1:3
  #pragma interrupt_handler  isr_int4:6 isr_int5:7 isr_int6:8
  #pragma interrupt_handler  isr_int7:9
  #pragma interrupt_handler  isr_t1_compB:14
 // #pragma interrupt_handler  isr_t2:11
  #pragma interrupt_handler  uart1_rx:31
 // #pragma interrupt_handler  uart1_tx:32
  #pragma interrupt_handler  uart1_tx_end:33

      //--------- DTMF -----------------
         void isr_int0(void){
              Process_dtmf();
         }
      //-------- mark 1sec -------------
         void isr_int1(void){
		      Flag_1s=1;
         }
      // -----row keyboard -------------
         void isr_int4(void){
              Flag_key=1;
         }
      //---- row keyboard --------------
         void isr_int5(void){
              Flag_key=2;
         }
      //----- row keyboard -------------
         void isr_int6(void){
              Flag_key=3;
         }
      //------ row keyboard ------------
         void isr_int7(void){
              Flag_key=4;
         }

       //----- output wave 1kHz --------
          void isr_t1_compB(void){
		       Flag_compB=1;
          }
	   //-------------------------------	  
          void uart1_rx(void){
		       TA_rc();
		  }
	   //-------------------------------	  
          void uart1_tx_end(void){
		       TA_tr();
		  }
	   //-------------------------------	