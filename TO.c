  
// current data: 24.05.2007
//defend number TO

#include "iom128v.h"
#include "TO_var.h"
#include "TO.h"
#include "TO_s.h"
  
  static char  Cnt_1s=0;
  static char  Cnt_min=55;
  static char  Sign_cpc=0;
  static char  Time_set_addr=0;
  static char  Key=0;
  static char  Buf_config=0;
  static char  Flag_SP=0;
  static char  LCD_contrast=0;
  static char  Sign_SP_set=0;
  static char  Addr_reg_SP=0;
  static char  N_reg_SP=0;
  static char  Cnt_reg_SP;
  static char  Flag_input_SP=0;
  static char  SP_regl=0;
  static char  SP_regh=0;
  static char  Buf_digit=0;
  static char  Flag_RTC=0;
  static char  Flag_set_addr=0;
  static char  RTC_sec_buf=0;
  static char  RTC_min_buf=0;
  static char  RTC_hour_buf=0;
  static char  RTC_day_buf=0;
  static char  RTC_date_buf=0;
  static char  RTC_month_buf=0;
  static char  RTC_year_buf=0;

  static char buf_pw1=0;
  static char buf_pw2=0;
  static char buf_pw3=0;
  static char cnt_pw=0;
   
  #define  password1  1
  #define  password2  5
  #define  password3  9
  
  #define  pw_SP1     3
  #define  pw_SP2     5
  #define  pw_SP3     7
   
  // addr reg DS1305(RTC) on wr.
   #define  RTC_sec    0x80
   #define  RTC_min    0x81
   #define  RTC_hours  0x82
   #define  RTC_day    0x83
   #define  RTC_date   0x84
   #define  RTC_month  0x85
   #define  RTC_year   0x86
   #define  RTC_CR     0x8F
   #define  RTC_SR     0x90
   #define  RTC_Charger   0x91
     
// unsigned char Buf_bit_reg_SP[16];
 const unsigned char N_TO[]={5,0x4e,0x5f,0x54,0x4f};
 const unsigned char SP_Mk[]={6,0x53,0x50,0x5f,0xbc,0xba};
 const unsigned char SP_set[]={7,0x53,0x50,0x5f,0x79,0x63,0xbf};
 const unsigned char In_PGA[]={5,0x42,0x58,0x20,0x3e};
 const unsigned char Out_PGA[]={6,0x42,0xae,0x58,0x20,0x3e};
 const unsigned char TA_Mk[]={6,0x54,0x41,0x5f,0xbc,0xba};
 const unsigned char TA_D[]={5,0x54,0x41,0x20,0x8e};
 const unsigned char LCD_contrast_menu[]={7,0xa3,0x4B,0xa5,0x20,0x93,0x92};
 const unsigned char LCD_light[]={6,0xa3,0x4B,0xa5,0x20,0x2a};
 const unsigned char RTC[]={5,0xab,0x41,0x43,0xae};
 const unsigned char Call[]={6,0x42,0xae,0xa4,0x4f,0x42};
 const unsigned char Config[]={6,0x4f,0xa8,0xe1,0xa5,0xa5};
 const unsigned char N_TA[]={"\x5N_TA"};
 const unsigned char TA[]={"\x3TA"};
 const unsigned char Reg[]={4,0x50,0x45,0xA1};
 const unsigned char CPC[]={4,0x4B,0xA8,0x42};
//---------------------------------------------------------
 void wr_spi(char addr,char data){
   char i;

   //set CE
          PORTB=PINB | 0x01;
   for (i=0;i<5;i++) ;

        SPDR=addr;
   while(!(SPSR & (1<<SPIF)))
           ;
        SPDR=data;
   while(!(SPSR & (1<<SPIF)))
           ;
   //clr CE
          PORTB=PINB & 0xFE;
}
//---------------------------------------------------------
 void wr_spi_buf(char addr,char* buf){
   char i;

   //set CE
          PORTB=PINB | 0x01;
   for (i=0;i<5;i++);

          SPDR=addr;
   while(!(SPSR & (1<<SPIF)))
           ;
          SPDR=*buf;
   while(!(SPSR & (1<<SPIF)))
           ;
   //clr CE
          PORTB=PINB & 0xFE;
}
//---------------------------------------------------------
char rd_spi(char addr){
   char i;

     asm("cli");

   //set CE
          PORTB=PINB | 0x01;
   for (i=0;i<5;i++);

          SPDR=addr;
   while(!(SPSR & (1<<SPIF)))
           ;
          SPDR=0;

   while(!(SPSR & (1<<SPIF)))
           ;
   //clr CE
          PORTB=PINB & 0xFE;

     asm("sei");

    return SPDR;
}
//---------------------------------------------------------
void rd_RTC_hm(void){
  //  RTC_sec_buf=rd_spi(0x0);
    RTC_min_buf=rd_spi(0x01);
    RTC_hour_buf=rd_spi(0x02);
}
//---------------------------------------------------------
void rd_RTC_data(void){
    RTC_day_buf=rd_spi(0x03);
    RTC_date_buf=rd_spi(0x04);
    RTC_month_buf=rd_spi(0x05);
    RTC_year_buf=rd_spi(0x06);
}
/*---------------------------------------------------------*/
void wr_RTC_hm(void){
         wr_spi_buf(0x82,&(RTC_hour_buf));
         wr_spi_buf(0x81,&(RTC_min_buf));
         wr_spi_buf(0x80,0);
}
/*---------------------------------------------------------*/
void wr_RTC_data(void){
         wr_spi_buf(0x83,&(RTC_day_buf));
         wr_spi_buf(0x84,&(RTC_date_buf));
         wr_spi_buf(0x85,&(RTC_month_buf));
         wr_spi_buf(0x86,&(RTC_year_buf));
}
/*---------------------------------------------------------*/
 void RTC_LCD(void){
      char i;
      rd_RTC_hm();
      lcd_addr_set(0);
      for(i=0;i<9;i++)
          lcd_wr_data(0x20);
      lcd_2digit(0,RTC_hour_buf);
      lcd_wr_data(0x2d);
      lcd_2digit(3,RTC_min_buf );
 }
/*---------------------------------------------------------*/
 void  wr_reg_SP(char byte_h,char byte_l){
      unsigned int j;
                 PORTC =PINC & 0xbf;  // DRDY_SP=0;
                 ;
                 SPDR=byte_h;
            while(!(SPSR & (1<<SPIF)))
                  ;
                 SPDR=byte_l;
            while(!(SPSR & (1<<SPIF)))
                     ;
            PORTC =PINC | 0x40;  // DRDY_SP=0;
            ;
            SPDR=0;
            while(!(SPSR & (1<<SPIF)))
                 ;

     for(j=0;j<0x1c2;j++)  //125mks
              ;
 }
/*---------------------------------------------------------*/
 void  set_PGA(char byte_h,char byte_l,char mask){
       char temp;
                 if(Flag_TA_D)
                    PORTC =PINC & mask;
                 else
                    PORTF =PINF & mask;
                     ;
                 SPDR=byte_h;
            while(!(SPSR & (1<<SPIF)))
                  ;
                 SPDR=byte_l;
            while(!(SPSR & (1<<SPIF)))
                     ;
			temp=0xff - mask;
					 
                if(Flag_TA_D){
                   PORTC =PINC | temp;
				   Flag_TA_D=0;
				}   
                else
                   PORTF =PINF | temp;
 }
/*---------------------------------------------------------*/
void Process_Key(void){
     char i,j,temp;
                EIMSK=0;
                Key=keyboard(--Flag_key);
                Flag_key=0;
  
  if(Key ==0xff){
    EIMSK=0xf7;
    EIFR |=0xf0;
    return;
  }

   switch(Key){
      case  0x0a:// roll +
	            temp=PING;
	            if(!Flag_config && (temp & 0x10)){
	               i= EEPROM_read(0x0c);
				   if(i!=7){
					  ++i;
					  set_PGA(0x40,i,0x7f);  //TA_D
					  EEPROM_write(0x0c,i);
				   }	 
				}
				else if(Flag_config){	 
				     if(!Sign_SP_set && !LCD_contrast){
				   	    ++Flag_config;	
					    if(Flag_config==0x0c)
					       Flag_config=2;	   
				        Process_menu_config();
					 } 
					 if(LCD_contrast)
					    PORTD=PIND & 0xef;
						
					 if(Sign_SP_set==2){
				        if(Cnt_reg_SP & 0x01){
						   ++N_reg_SP;
						 if(N_reg_SP>5)
						    N_reg_SP=0;
						}   
						   ++Cnt_reg_SP; 
						if(Cnt_reg_SP>0x0b)
						   Cnt_reg_SP=0;
						   
						   LCD_reg_SP();
					 }	
	           	}
		    break;
      case  0x0b:// roll -
	            temp=PING;
	            if(!Flag_config && (temp & 0x10)){
	           	   i= EEPROM_read(0x0c);
				   if(i){
					  i--;
					  set_PGA(0x40,i,0x7f);  //TA_D
					  EEPROM_write(0x0c,i);
				   }	
				}
				else if(Flag_config){
				     if(!Sign_SP_set && !LCD_contrast){
				        if(Flag_config !=1)
		                --Flag_config;
					    if(Flag_config==1)
					      Flag_config=0x0b;
					      Process_menu_config();
				     }
					 if(Sign_SP_set==2){
				        if(!Cnt_reg_SP)
						   Cnt_reg_SP=0x0b;
					    else   
						   --Cnt_reg_SP;
						    
					    if(Cnt_reg_SP & 0x01){
						   if(!N_reg_SP)
		                      N_reg_SP=5;
						   else	  				   
						      --N_reg_SP;
						}
					   LCD_reg_SP();
					 }	
	           	}
			break;
      case  0x0c://
          break;
      case  0x0d:  //speakphone
            if(!Flag_SP){
                Flag_SP=1;
                // turn on MK & SP
				 Speakphone_on();
                 break;
            }
            else if(Flag_SP==1){
                 Flag_SP=2;
			 // turn off  MK
                 set_PGA(0x20,0,0xdf);
                 lcd_addr_set(0x40);
                 lcd_wr_data(0x20);
                 break;
            }
            else if(Flag_SP==2){
                 Flag_SP=0;
		 // turn off SP
                 PORTG=PING | 0x08;
                 lcd_addr_set(0x41);
                 lcd_wr_data(0x20);
                 break;
            }
            break;
 //---------------------------------------------------
      case  0x0e:// *
                      if(!Flag_config)				  
						 if (Buf_digit){
						   Buf_digit=0;
						   RTC_LCD();
						 } else {
                           Flag_config=1;
						 }
					  else{
					     lcd_clr();	 
		                 if(LCD_contrast){
						    LCD_contrast=0;
							PORTD=PIND | 0x10;
						   	break;
						 }
						 if(Sign_SP_set){
					      if(Flag_input_SP){
					         Flag_input_SP=0;
					         LCD_reg_SP();
						     break;
					      }	 
	                        Sign_SP_set=0;					 
						    Cnt_reg_SP=0;
							break;
						 }
						   Flag_config=0;	
						   RTC_LCD();
					  }
                break;
 //---------------------------------------------------
      case  0x0f:// # enter
                 if(Flag_config){
                    Process_enter();
                    break;
                 }
				 if(Buf_digit==0x88){	
                    Call_TA_88();
					break;			 
				 }
				 if(Buf_digit>9) Buf_digit -=6;
				    Requst_TO(Buf_digit);
                break;
 //---------------------------------------------------
      default:// 0..9
        if(Key<0x0a){

           if(Flag_config==1){
		      Test_password();
              break;
           }
		   if(Sign_SP_set==1){
		      Test_pw_SP();
              break;
		   }
		   if(Sign_SP_set==2){
		      Input_byte_SP();
		        break; 
		   }	
		   if(Flag_RTC){
              Input_RTC_hm();
              break;
           }
		   if(!Flag_config && !Buf_digit){
		      lcd_addr_set(0);
              for(i=0;i<9;i++)
                  lcd_wr_data(0x20);
			  lcd_addr_set(0);
		   }		  
		   if(!Buf_digit){
               Buf_digit=Key;
			   lcd_digit(Key);
		   }   
           else{
			   if(!Flag_config)
			      lcd_addr_set(1);
			     Buf_digit <<=4;
                 Buf_digit |= Key;
				 lcd_digit(Key);
		   }
        }
		  break;
      }
            EIFR |=0xf0;
            EIMSK=0xf7;           //0xf7;
}
/*---------------------------------------------------------*/
  void Test_password(void){
       char i;
              if(!cnt_pw){
                 buf_pw1=Key;
                 ++cnt_pw;
              }
              else if(cnt_pw==1){
                    buf_pw2=Key;
                    ++cnt_pw;
              }
              else if(cnt_pw==2){
                   buf_pw3=Key;
                   cnt_pw=0;
				   
                  if((buf_pw1==password1) && (buf_pw2==password2) &&
                     (buf_pw3==password3)){
					  lcd_clr();
                      LCD_line(Config);
					 
				  }	 
	              else
                     Flag_config=0;
              }
  }
/*----------------------------------------------------*/  
  void Test_pw_SP(void){
       char i;
              if(!cnt_pw){
                 buf_pw1=Key;
                 ++cnt_pw;
              }
              else if(cnt_pw==1){
                    buf_pw2=Key;
                    ++cnt_pw;
              }
              else if(cnt_pw==2){
                   buf_pw3=Key;
                   cnt_pw=0;
				   
                  if((buf_pw1==pw_SP1) && (buf_pw2==pw_SP2) &&
                     (buf_pw3==pw_SP3)){
					  Sign_SP_set=2;
					  Cnt_reg_SP=0;
					  N_reg_SP=0;
					  lcd_clr();
                      LCD_line(Reg);
					  lcd_digit(0);
					  lcd_symbol(0x4c); //l
				  }	 
	              else
                      Sign_SP_set=0;
              }
  }
 //---------------------------------------------------
 void EEPROM_write(char addr,char data){
      asm("cli");
   
      while(EECR & (1<< EEWE))
             ;
      EEAR=addr;
      EEDR=data;
      EECR |= (1<< EEMWE);
      EECR |= (1<< EEWE);

      while(EECR & (1<< EEWE))
            ;
      asm("sei");
 }
/*---------------------------------------------------------*/
 char EEPROM_read(char addr){
      while(EECR & (1<< EEWE))
            ;
      EEAR=addr;
      EECR |= (1<< EERE);
      return EEDR;
 }
/*---------------------------------------------------------*/
 void Input_RTC_hm(void){
      char i;
	                  
      switch(Flag_RTC){
            case  1:
                       RTC_hour_buf |= Key;
                       lcd_digit(Key);
                       Flag_RTC=2;
                  break;
            case  2:
                       RTC_hour_buf <<=4;
                       RTC_hour_buf |= Key;
                       lcd_digit(Key);
                       Flag_RTC=3;
                       lcd_wr_data(0x2D);
                  break;
            case  3:
                       RTC_min_buf |= Key;
                       lcd_digit(Key);
                       Flag_RTC=4;
                  break;
            case  4:
                       RTC_min_buf <<=4;
                       RTC_min_buf |= Key;
                       lcd_digit(Key);
                  break;
            default:
                       Flag_RTC=0;
                  break;
     }
 }
/*---------------------------------------------------------*/
   void Process_int_1s(void){
        char temp;
WDT_on();	
       Flag_1s=0;
       ++ Cnt_min;
       ++Time_set_addr;
//...............................................	   
	   if(Sign_cpc){
	      ++Sign_cpc;
		  if(Sign_cpc==2){
		  	 SP_ring_on();
		     TCCR1B=0x01;
          }		   
		  else if(Sign_cpc==4){	
		       Sign_cpc=0;
			   TCCR1B=0;
			   Flag_compB=0;	
			   PORTG=PING | 0x02;  
			   Speakphone_off(); 
	      }			   
	   }
//...............................................
	   
                   if(Flag_ring){
                      ++Cnt_1s;
					  TCCR1B=0;
                      if(!Flag_15s ||(Cnt_1s==3)){
		 // turn_on	speakphone, turn of	Mk_SP
		                 SP_ring_on();
		                 Cnt_1s=0;
                         TCCR1B=0x01;
                         Proc_15s();
                      }
                      if(Cnt_min==60)
                         Cnt_min=0;
                   }
                   else{
                        if(Cnt_min==60){
                             Cnt_min=0;
							 if(!Flag_config && !Buf_digit)
                                 RTC_LCD();
                          }
                        if(Time_set_addr > 30)
                           Flag_set_addr=0;
                   }
     }
/*---------------------------------------------------------*/
  void Proc_15s(void){
        char temp;
            if(!Flag_15s){
                Flag_15s=1;
	            LCD_line(Call);
				if(!Flag_TA_TO)
                   lcd_2digit(6,N_call);
				else 
				   LCD_line1(TA,6,3);
		    }
            else{
                 ++Flag_15s;
                 if(Flag_15s==7)
			        End_ring();
            }
   }
/*---------------------------------------------------------*/
  void End_ring(void){
       char temp;

               Flag_15s=0;
               Flag_ring=0;
               PORTG=PING | 0x02;
               TCCR1B=0;
			   Flag_compB=0;
               if(Flag_TA_TO){
			      Flag_TA_TO=0;
				  temp=PING;
                  if(!(temp &=0x10))
                       Tr_reply(2);
			   }	  
			   else{
			   	  if(Flag_rc_TO)
				     Flag_rc_TO=0;
				  Qnt_TA= EEPROM_read(Sign_TA_eep);
				  if(Qnt_TA && !Status_TA && !Cnt_tr_uart){
				          Speakphone_off();        
					      Cnt_tr_uart=1;
	                      UDR1=0x55;
						  Start_T3();
				  }
				  else{	 
                         temp=PING;
                         if(!(temp &=0x10))
                            Tr_reply(2);
                  }
			   }  
  }
/*-----------------------------------------------*/
 void End_ring1(void){
      Flag_15s=0;
      Flag_ring=0;
      PORTG=PING | 0x02;
      TCCR1B=0;
	  Flag_compB=0;
      if(Flag_rc_TO)
		 Flag_rc_TO=0;
	  if(Flag_TA_TO)
		 Flag_TA_TO=0;
}				  
/*-----------------------------------------------*/
  void Ring(void){

          PORTD=PIND | 0x08;
          PORTG=PING & 0xfd;   //alarm om
              Flag_compB=0;
              TCNT1H=0;
              TCNT1L=0;
            --Cnt_pulse;
			
            if(!Cnt_pulse){
               if(Sign_filling){
		          OCR1BH=0x03;         
                  OCR1BL=0x2c;         
                  Sign_filling=0;
                  Cnt_pulse=125;       
               }
               else{
                  OCR1BH=0x02;       
                  OCR1BL=0x53;       
                  Sign_filling=1;
                  Cnt_pulse=132;     
		       }
            }
  }
/*---------------------------------------------------------*/
   void Speakphone_on(void){
        char i;

              PORTG=PING & 0xf7;
              i=EEPROM_read(8);      //SP_Mk on
			  
			  if(i==8)
			     set_PGA(0x20,0,0xdf);  //TA_Mk
			  else   
			     set_PGA(0x40,i,0xdf);
				 
			  SP_regl=EEPROM_read(0x10); //SP_reg0
              SP_regh=EEPROM_read(0x11);
			  wr_reg_SP(SP_regh,SP_regl);
			  	 
                 lcd_addr_set(0x40);
                 lcd_wr_data(0xef);
                 lcd_wr_data(0x8e);
				 lcd_addr_set(0x0);
		       //alarm off
                 PORTG=PING | 0x02;
                 if(Flag_ring){
                    Flag_ring=0;
                    Flag_15s=0;
                    TCCR1B=0x0;
					Flag_compB=0;
                    if(!Flag_rc_TO)
                         Tr_reply(3);
                    else
                         Flag_rc_TO=0;
                 }
    }

   void Speakphone_off(void){
        PORTG=PING | 0x08;
        set_PGA(0x20,0,0xdf);  //off SP_Mk 
		SP_regl=EEPROM_read(0x10); //SP_reg0
        SP_regh=EEPROM_read(0x11);
		SP_regh=SP_regh |0x80;
		wr_reg_SP(SP_regh,SP_regl);
	    lcd_addr_set(0x41);
        lcd_wr_data(0x20);
		lcd_addr_set(0x0);
    }
	 void SP_ring_on(void){
	      Flag_SP=0;
          PORTG=PING & 0xf7;
		  set_PGA(0x20,0,0xdf);   //SP_Mk off 
          SP_regl=EEPROM_read(0x10); //SP_reg0
          SP_regh=EEPROM_read(0x11);
		  SP_regh |=0x0e;
		  wr_reg_SP(SP_regh,SP_regl);
	 }
/*---------------------------------------------------------*/
  void LCD_line(const unsigned char* buf){
           char i,j;
                  lcd_clr();
                  lcd_addr_set(0);
                  i=buf[0];
                  for(j=1;j<i;j++){
				      if(j==9)
					     lcd_addr_set(0x40);
                      lcd_wr_data(buf[j]);
				  }	  
					  if(i !=0x0b)
                         lcd_wr_data(0x20);
  }
/*---------------------------------------------------------*/
void LCD_line1(const unsigned char* buf,char addr,char qnt){
      char i,j,temp;
		          temp=qnt;
                  lcd_addr_set(addr);
                  for(i=0;i<temp;i++)
                      lcd_wr_data(0x20);
				  lcd_addr_set(addr);  
                  i=buf[0];
                  for(j=1;j<i;j++)
                      lcd_wr_data(buf[j]);
                      lcd_wr_data(0x20);
   }
/*--------------------------------------------------------*/   
 void Process_enter(void){
      char i;
	  
	  if(Sign_SP_set){
         if(Sign_SP_set==2)
	        Output_byte_SP();
	     else if(Sign_SP_set==3)
            Save_byte_SP();
		 return;	
	  }
	  	  
	 switch(Flag_config){
            case  2:   //set RTC
                     if(!Flag_RTC){
	               		Flag_RTC=1;	
						RTC_min_buf=0;
                        RTC_hour_buf=0;
						lcd_clr();
					 }		 
					 else{	 		
                          wr_RTC_hm();
                          wr_spi_buf(0x80,0);
						  RTC_LCD();
						  Flag_RTC=0;
					 }	  
				    break;
            case  3: //LCD_contrast
			        if(LCD_contrast){
					   LCD_contrast=0;
					   PORTD=PIND | 0x10;
					}   
					else   
			           LCD_contrast=1;
				    break;
            case  4: //lcd_light
                    i=PING & 0x01 ;
                    if(!i)
                       PORTG=PING | 0x01;
					else
                       PORTG=PING & 0xfe;
				  break;
            case  5:
			        Process_PGA(0xfe,9);  
                  break;
            case  6:
			        Process_PGA(0xfd,0x0a); 
			      break;
            case  7:
                    Process_PGA(0xdf,8);  
                  break;
            case  8:
                    Process_PGA(0xef,0x0b);  
                  break;
            case  9:
			        Flag_TA_D=1;
                    Process_PGA(0x7f,0x0c);   
                  break;
            case  0x0a:
			           if(!Buf_config){
                          Buf_config=1;
						  lcd_clr();
					   } 	  
					   else{	
				            EEPROM_write(7,Buf_digit);
							Addr_TO=Buf_digit;
				            Buf_config=0;
					   }
                  break;
            case  0x0b:
			           if(!Buf_config){
                          Buf_config=1;
						  lcd_clr();
					   } 	  
					   else{
					        if(Buf_digit==2)
							   Buf_digit=3;	
				            EEPROM_write(Sign_TA_eep,Buf_digit);
						    Buf_config=0;
					   }
                  break;
			case  0x0c:
                        if(!Sign_SP_set){
                           Sign_SP_set=1;
						   lcd_clr();
						}   
				  break;	  
							  
	        default:
                  break;
     }
 }       
/*---------------------------------------------------------*/
 void Process_menu_config(void){
      char i;
                lcd_clr();

     switch(Flag_config){
            case  2:
		              LCD_line(RTC);
                  break;			
            case  3:
                      LCD_line(LCD_contrast_menu);
                  break;
            case  4:
                      LCD_line(LCD_light);
                  break;
            case  5:
                      LCD_line(In_PGA);
					  i=EEPROM_read(In_PGA_eep);
					  lcd_2digit(6,i);
			      break;
            case  6:
                      LCD_line(Out_PGA);
					  i=EEPROM_read(Out_PGA_eep);
					  lcd_2digit(6,i);
				  break;
            case  7:
                      LCD_line(SP_Mk);
					  i=EEPROM_read(SP_Mk_eep);
					  lcd_2digit(6,i);
				  break;
            case  8:
                      LCD_line(TA_Mk);
					  i=EEPROM_read(TA_Mk_eep);
					  lcd_2digit(6,i);
			      break;
            case  9:
                      LCD_line(TA_D);
					  i=EEPROM_read(TA_D_eep);
					  lcd_2digit(6,i);
				  break;
            case  0x0a:
                      LCD_line(N_TO);
					  i=EEPROM_read(Addr_TO_eep);
					  lcd_2digit(6,i);
				  break;
            case  0x0b:
			          LCD_line(TA);
					  i=EEPROM_read(Sign_TA_eep);
					  if(i==3) i=2;
                      lcd_2digit(6,i);    
                  break;	  
			case  0x0c:
                      LCD_line(SP_set);
				  break;
		    default:
                   break;
      }
 }
/*---------------------------------------------------------*/
  void Process_PGA(char mask,char addr){  
            if(!Buf_config){
               Buf_config=1;
			   lcd_clr();
			}  	  
			else{	
			      Buf_digit &=0x0f;
			
			     if(Buf_digit==8)
				    set_PGA(0x20,0,mask); 
				 else
				   set_PGA(0x40,Buf_digit,mask);
				   
				   EEPROM_write(addr,Buf_digit);
				 	  
			  Buf_config=0;
			}
  }
/*---------------------------------------------------------*/
   void Output_byte_SP(void){
        char  temp;
		
	            lcd_clr();
				temp=Addr_reg_SP=0x10 +(N_reg_SP*2);
	SP_regl=EEPROM_read(temp);
	SP_regh=EEPROM_read(temp+1);
				if(Cnt_reg_SP & 0x01)
				   ++Addr_reg_SP;
				temp=EEPROM_read(Addr_reg_SP);
                LCD_byte_bin(temp);
        		lcd_addr_set(0);
   }
/*---------------------------------------------------------*/
   void LCD_byte_bin(char byte){
        char i,mask;
        lcd_addr_set(0);
		mask=0x80;
		for(i=0;i<8;i++){
		    if(byte & mask) 
               lcd_digit(1);  
            else
               lcd_digit(0);
			   mask >>=1;   
		}	    
   }
/*--------------------------------------------------------*/   
   void Save_byte_SP(void){
        char temp;
		
        Flag_input_SP=0;
  	 	wr_reg_SP(SP_regh,SP_regl);
		if(Cnt_reg_SP & 0x01)
	        temp=SP_regh;
	    else
	        temp=SP_regl;
						
	EEPROM_write(Addr_reg_SP,temp);
		    Sign_SP_set=2;
	}
/*---------------------------------------------------------*/
  void Input_byte_SP(void){
       static char cnt=0;
	   static char buf=0;
	          char temp;
	   if(!cnt)	
	      lcd_clr();
	   Flag_input_SP=1;
	     
	   ++cnt;
	   lcd_digit(Key);
	   temp=Key;
   
        switch(cnt){
            case  1:
			      if(temp)
				     buf=buf | 0x80;	
		          break;			
            case  2:
			      if(temp)
				     buf=buf | 0x40;	
		          break;	
            case  3:
			      if(temp)
				     buf=buf | 0x20;	
		          break;	
            case  4:
			      if(temp)
				     buf=buf | 0x10;	
		          break;	
	        case  5:
			      if(temp)
				     buf=buf | 0x08;	
		          break;	
	        case  6:
			      if(temp)
				     buf=buf | 0x04;	
		          break;				  
		    case  7:
			      if(temp)
				     buf=buf | 0x02;	
		          break;	
			case  8:
			      if(temp)
				     buf=buf | 0x01;	
		          break;
			default:
				  ;	
				  break;  
	  }
	  if(cnt==8){
	  
	     if(Cnt_reg_SP & 0x01)
	        SP_regh=buf;
	     else
	        SP_regl=buf;
		    Sign_SP_set=3;
			cnt=0;
			buf=0;
	  }		
  }		 
 /*---------------------------------------------------------*/
  void LCD_reg_SP(void){
            lcd_clr();
            LCD_line(Reg);
			lcd_digit(N_reg_SP);
			if(Cnt_reg_SP & 0x01)
			   lcd_symbol(0x48); //h
			else
			   lcd_symbol(0x4c); //l
  }			   
 /*----------------------------------------------------*/							   
 void WDT_on(void){
      char temp;
     temp=PINB & 0x80;
     if(temp)
        PORTB=PINB & 0x7f;
     else
        PORTB=PINB | 0x80;
 }
 /*---------------------------------------------------------*/
   void Call_TA_88(void){
         Buf_digit=0;
      Qnt_TA= EEPROM_read(Sign_TA_eep);
	  if(Qnt_TA && !Status_TA && !Cnt_tr_uart){
		 Cnt_tr_uart=1;
	     UDR1=0x55;
		 Start_T3();
		 Flag_TO_TA=1;
		 LCD_line(Call);
		 LCD_line1(TA,6,3);
	  }
   }	  
 /*---------------------------------------------------------*/
 void cpc(void){
      lcd_addr_set(2);
      LCD_line(CPC);
	  Sign_cpc=1;
 }
 /*----------------------------------------------*/