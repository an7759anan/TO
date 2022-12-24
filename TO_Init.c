
        /* Initialization hardware */

#include "iom128v.h"
#include "TO_var.h"
#include "TO.h"
#include "TO_s.h"

/*
  pass to boot: asm("cli");
              asm("jmp 0x1e200");   
*/

   const unsigned char Header[]={0x54,0x4f,0x20,0x76,0x31,0x5f,0x33,0x20};
   static char  SP_regl=0;
   static char  SP_regh=0;

  //___________ Protocol change__________________

  //max qnt. operator=15

 // **** dispatcher  ---> operator :
   // 1_nibble: d3=0, d2,d1,d0 ---> h_addr
  // 2_nibble: l_addr
 // 3_nibble: code_command ---> 4

// **** aknowledge operator ---> to dispatcher:
   // 1_nibble: d3=1, d2,d1,d0 ---> h_addr
  // 2_nibble: l_addr
 // 3_nibble: code_command ---> 4

// **** request from operator to dispatcher
   // 1_nibble: d3=1, d2=1,  d1,d0 =0;
  // 2_nibble: addr dispatcher
 // 3_nibble:  addr operator

// **** request from operator1 to operator2
   // 1_nibble: d3=1, d2=1,  d1,d0 =0;
  // 2_nibble: addr operator2
 // 3_nibble:  addr operator1

/* coefficient=8 for PGA turn on PGA
   into shut_down regim 
*/ 
  
//________________________________________________

     void TO_init(void){

        char i;
        int  j;

 //---- initial set control reg CPU -----------------------
            SREG=0;
            XDIV=OSCCAL=0;
            WDTCR=0;
            RAMPZ=0;
            EECR=0;
            EICRA=EICRB=EIMSK=0;
            MCUCR=MCUCSR=0;
            XMCRA=XMCRB=0;
            TCCR0=0;
            TCCR2=0;
            TCCR1A=TCCR1B=TCCR1C=0;
            TCCR3A=TCCR3B=TCCR3C=0;
            TIMSK=ETIMSK=0;
            UCSR0A=UCSR0B=UCSR0C=0;
            UCSR1A=UCSR1B=UCSR1C=0;
            ACSR=ADCSRA=0;
            TWCR=SPMCSR=0;
            SPCR=0;
            SPSR=0;
 //---------------------------------------------------
            UCSR1B = 0x00; //disable while setting baud rate
            UCSR1A = 0x0;
            UCSR1C = 0x06;
            UBRR1L = 0x16; //set baud rate lo (22)
            UBRR1H = 0x00; //set baud rate hi
         //   UCSR1B = 0x08;
			UCSR1B = 0xD8;            					
 //-----initial set port CPU ------------------------------
 // PA3..PA0 - bus data
 // PA4 - RS0_dtmf
 // PA5 - rd_dtmf
 // free: PA6,PA7
            DDRA=0xff;
            PORTA=0xff;
 //--------------------------------------------------------
 //CS_RC-PB0,SCK-PB1,MOSI-PB2,MISO-PB3
 //PB7-wdt PB6-ring;
 //free:PB4,PB5
            DDRB=0xf7;
            PORTB=0xcf;
 //--------------------------------------------------------
 //DRDY_SP-PC6, RST_SP-PC2
 //PC3 - alarm RT
 //PC7- cs amplifier  handset TA_D
 //free:PC0,PC1,PC4,PC5
            DDRC=0xff;
            PORTC=0xff;
 //--------------------------------------------------------
 // scan_line keyboard PE3...PE0
 //PE4...PE7 - isr keyboard
            DDRE=0x0f;
            PORTE=0xf0;      // scan_line is active
 //--------------------------------------------------------
 //PD0 - int0,PD1-int1
 //PD2 -  RX udr1
 //PD3-   TX udr1
 //PD4 -  control bright LCD
 //PD6 -  wr/rd_LCD
 //PD7 -  A0_LCD
 //PD5 -  free
            DDRD=0xf8;
            PORTD=0x90;        //8;
 //--------------------------------------------------------
  //PF3 - cs_dtmf PF2 - wr_dtmf,
  //PF6 - E_LCD
  //PF0 - cs amplifier  input
  //PF1 - cs amplifier output
  //PF4 - cs microphone MT
  //PF5 - cs microphone SP (speakphone)
  //PF7 - free
            DDRF=0xff;
            PORTF=0xff;   
 //--------------------------------------------------------
 //PG0 -  LCD_light PG0=1 - active
 //PG1 -  alarm
 //PG2 -  dtmf on/off  1-on
 //PG3 -  sp   on/off  0-on
 //PG4 -  hook on/off

            DDRG=0x0b;
            PORTG=0x1a;
//-----------------------------------------------------------
//WDT_on_off();	 		
//---- CPU--> master for SPI --------------------------------
            SPCR=0x5d;
            wr_spi(RTC_CR,0x0);
            wr_spi(RTC_Charger,0xA5);
            wr_spi(RTC_CR,0x04);
/*---------------------------------------------------------*/
            EIMSK=0x0;         //0xf7;
            EICRA=0x08;        //int1 - falling edge, rest low level
            EICRB=0xAA;
/*---------------------------------------------------------*/
            Flag_key=0;
            Flag_1s=0;
            Flag_15s=0;
            Flag_compB=0;
		    Flag_TA_D=0;  
			Flag_ring=0;
		    Flag_TA_TO=0;
		    Flag_TO_TA=0;
			Flag_rc_TO=0;
            Cnt_tr_uart=0;
			Status_TA=0;
 /*--- formation ring: output wave 1kHz on pin PB5 -----*/
            Sign_filling=0;
            OCR1BH=0x03; 
            OCR1BL=0x2c; 
            Cnt_pulse=125;
            TCNT1H=0;
            TCNT1L=0;
            TCCR1A=0x10;
           // TCCR1C=0x40;
            DDRB=0xf7;
            TIMSK=0x08; //T1,T2
            TCCR1B=0;
 /*---------------------------------------------------------*/
            PORTC = 0xfb;   //RST_SP=0
        for(i=0;i<20;i++);
            PORTC = 0xff;   //RST_SP=1
			
  // delay  180ms
   for(i=0;i<10;i++){
      for(j=0;j<0xffff;j++)
          WDT_on();	
   }
/*------   initial  speakphone   -----------------------------*/
//         wr_reg_SP(0x64,0);
		 SP_regl=EEPROM_read(0x10);
         SP_regh=EEPROM_read(0x11);
		 wr_reg_SP(SP_regh,SP_regl);
//         wr_reg_SP(0x2a,2);
		 SP_regl=EEPROM_read(0x12);
         SP_regh=EEPROM_read(0x13);
		 wr_reg_SP(SP_regh,SP_regl);
//         wr_reg_SP(0,4);
		 SP_regl=EEPROM_read(0x14);
         SP_regh=EEPROM_read(0x15);
		 wr_reg_SP(SP_regh,SP_regl);
//         wr_reg_SP(0,6);
		 SP_regl=EEPROM_read(0x16);
         SP_regh=EEPROM_read(0x17);
		 wr_reg_SP(SP_regh,SP_regl);
//         wr_reg_SP(0,0x78);
		 SP_regl=EEPROM_read(0x18);
         SP_regh=EEPROM_read(0x19);
		 wr_reg_SP(SP_regh,SP_regl);
//         wr_reg_SP(0,0x0a);
		 SP_regl=EEPROM_read(0x1A);
         SP_regh=EEPROM_read(0x1B);
		 wr_reg_SP(SP_regh,SP_regl);
		 
/*----------------------------------------------
         set_PGA(0x40,0,0xfe); //in_PGA
		 EEPROM_write(9,0);
		 
	     set_PGA(0x40,5,0xfd); //out_PGA
		 EEPROM_write(0x0a,5); 
		  
		 set_PGA(0x40,7,0xef); //TA_Mk
		 EEPROM_write(0x0b,7);
		   
		 set_PGA(0x40,7,0xdf); //SP_Mk
		 EEPROM_write(8,7);
		 
		 Flag_TA_D=1;  
         set_PGA(0x40,2,0x7f);  //TA_D
		 EEPROM_write(0x0c,2);
 ----------------------------------------------*/
         i= EEPROM_read(9);		 
         set_PGA(0x40,i,0xfe); //in_PGA
		 i= EEPROM_read(0x0a);	
		 set_PGA(0x40,i,0xfd); //out_PGA
		 i= EEPROM_read(0x0b);	 
		 set_PGA(0x40,i,0xef); //TA_Mk
		 i= EEPROM_read(8);	
		 set_PGA(0x40,i,0xdf); //SP_Mk
		 Flag_TA_D=1; 
		 i= EEPROM_read(0x0c);	 
         set_PGA(0x40,i,0x7f);  //TA_D
/*---------------------------------------------------------*/		  
		 Speakphone_off();
/*--------------------------------------------------------*/
           lcd_init();
		   lcd_clr();
/*---------------------------------------------------------*/
            for(i=0;i<9;i++)
                lcd_wr_data(Header[i]);
 /*---------------------------------------------------------*/
            i=EEPROM_read(7);
            lcd_2digit(0x40,i);
			Addr_TO=i;
/*---------------------------------------------------------*/
             Initial_dtmf();
             i=dtmf_rd();
 	         EIFR=0xff;
             i=UDR1;
 // while(!(UCSR1A & (1<< UDRE1))); UDR1=0x77;
 PORTB=PINB | 0x30;
 WDT_on();	
 PORTD=PIND | 0x10;
             EIMSK=0xf7;
	         asm("sei");
  }
/*---------------------------------------------------------*/
//while(!(UCSR1A & (1<< UDRE1))); UDR1=Buf_digit;	
//   asm("cli");
