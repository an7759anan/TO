
#ifndef __TO_var_h_
#define __TO_var_h_

 #ifndef _core_
  extern
 #endif
         char 
		      Addr_TO, 
		      Flag_rc_TO,
		      N_call,
			  Qnt_TA,
			  Sign_filling,
              Cnt_pulse,
              Flag_ring,
		      Flag_config,
			  Flag_key,
			  Flag_TA_D,  
              Flag_compB,
			  Cnt_tr_uart,
              Flag_TA_TO,
			  Flag_TO_TA,
			  Status_TA,
			  Flag_1s,
              Flag_15s;

			  
  // addr reg DS1305(RTC) on wr.
    #define  RTC_CR        0x8F
    #define  RTC_Charger   0x91
    #define  Addr_TO_eep   0x07
	#define  SP_Mk_eep     0x08
	#define  In_PGA_eep    0x09
    #define  Out_PGA_eep   0x0a
    #define  TA_Mk_eep     0x0b
    #define  TA_D_eep      0x0c
	#define  reg_SP_eep    0x10  //...0x1c
    #define  Sign_TA_eep   0x20   
     
   #endif
