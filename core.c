
//TO - operator"s telephone
//clock CPU 3579,545 Kz,5v ;280nsec
//beginning data: 11.08.2006

#define _core_

#include "iom128v.h"
#include "TO_var.h"
#include "TO.h"
#include "TO_s.h"

void main(void){

  char temp,i;

       TO_init();

  while(1)
  {
 WDT_on();	  
       if(Flag_key)
          Process_Key();

       if(Flag_1s)
          Process_int_1s();

       if(Flag_compB)
             Ring();

       if(ETIFR & 0x04)
           Clr_T3();

 //  handset off: PG4 --> 0 ;handset on: PG4 --> 1
 
         temp=PING;

      if(!(temp & 0x10)){
	      set_PGA(0x20,0,0xef);  //off Mk handset
		  Flag_TA_D=1;
		  set_PGA(0x20,0,0x7f);  //TA_D
	  }	  
      else{
	        temp=EEPROM_read(TA_D_eep); //TA_Mk
			Flag_TA_D=1; 
			if(temp==8)
			   set_PGA(0x20,0,0x7f);  //TA_D
			else   
               set_PGA(0x40,temp,0x7f);  //TA_D
			   
		   	temp=EEPROM_read(TA_Mk_eep); //TA_Mk
			if(temp==8)
			   set_PGA(0x20,0,0xef);  //TA_Mk
			else   
			   set_PGA(0x40,temp,0xef);
						   
			if(!(PING & 0x08)){   
		       Speakphone_off();
			   lcd_addr_set(0x40);
               lcd_wr_data(0x20);  
			   lcd_addr_set(0);
  			}   
            if(Flag_ring){
               Flag_ring=0;
               PORTG=PING | 0x02;
               Flag_15s=0;
               TCCR1B=0;
			   Flag_compB=0;
			   if(Flag_TA_TO){
				  Flag_TA_TO=0;
				  lcd_clr();
			   }	
               if(!Flag_rc_TO)
                   Tr_reply(3);
               else
                   Flag_rc_TO=0;
			}	   
       }
	
      }
   }
   
 //-------------------------------------------  
// while(!(UCSR1A & (1<< UDRE1))) ;  UDR1=N_Tract_Reply;



