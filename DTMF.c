
//DTMF.c

 #include "iom128v.h"
 #include "TO_var.h"
 #include "TO.h"
 #include "TO_s.h"
 
  static char  Buf_addr=0;
  static char  Addr_request_TO=0;   
  static char  Code_com=0;  
  static char  Reply=0;     
  static char  Cnt_dtmf_tr=0;    
  static char  Cnt_dtmf_rc=0;    
  static char  Rc_TO=0;       
  static char  Flag_request_TO=0;
  static char  Flag_request_TA=0;
  static char  Flag_call_TA=0;
  static char  Cnt_rc_uart=0;
//------------------------------------------------
 void Initial_dtmf(void){
      char temp;
      temp=dtmf_rd();
      dtmf_wr(0xf0);
      dtmf_wr(0xf0);
      dtmf_wr(0xf8);
      dtmf_wr(0xf0);
      dtmf_wr(0xfd);    //wr CRA
      dtmf_wr(0xf0);    //wr CRB
      temp=dtmf_rd();
 }         
//------------------------------------------------
 void Tr_nibble_dtmf(char val){
    // while(!(UCSR1A & (1<< UDRE1))); UDR1=val;
                 val |=0xf0;
         	  dtmf_wr_data(val);
 }
//------------------------------------------------ 
  void Process_dtmf(void){
   char temp,temp1;
   
   asm("cli");
       temp1=temp=dtmf_rd();
       temp &=0x02;
	   if(temp){
           //Transmitt_dtmf
		   switch(Cnt_dtmf_tr){
		    case 1:
           		  Cnt_dtmf_tr=2;
				  if(Flag_request_TO)
                     Tr_nibble_dtmf(Addr_request_TO);
				  else{
                        temp=Addr_TO;
				        temp &=0x0f;   
                        Tr_nibble_dtmf(temp);				  
				  }	 
			break;
			case  2:  			
	                Cnt_dtmf_tr=0;
                if(!Flag_request_TO)             
                  Tr_nibble_dtmf(Reply);
				else{
				  temp=Addr_TO;
				  temp=bcd_hex(temp);   
                  Tr_nibble_dtmf(temp);
				  Flag_request_TO=0;
				}         
		    break; 
	        default:
			      Cnt_dtmf_tr=0;
			break;
		  }		   
      }      
//-----------------------------------------------		  
    else{ 
          //Receive_dtmf
	 temp1 &=0x04;
   
	 if(temp1){
        temp1=dtmf_rd_data();
        temp1 &=0x0f;
 while(!(UCSR1A & (1<< UDRE1))); UDR1=temp1;
              ++Cnt_dtmf_rc;
       switch(Cnt_dtmf_rc){
         case 1:
                 Start_T3();
              if(temp1 & 0x0c)
			     Rc_TO=1;
			  else
                 Buf_addr=temp1;
         break;
         case  2:
			   if(Rc_TO)
                  Buf_addr=hex_bcd(temp1);
			   else{
				 N_call=1;
				 Buf_addr=Buf_addr<<4;
                 Buf_addr |= temp1;
			   }		 
		 break; 
         case  3:
			     Code_com=temp1;
			     if(Rc_TO)
			        N_call=hex_bcd(temp1);
				 if(Buf_addr==Addr_TO){
				    if(Code_com==6)
                       cpc(); //control pack call					
				    else
					   Process_rc_byte();
				 }	
					Clr_T3(); 
     	 break;
         default:
		          Clr_T3(); 
		 break; 
 }
 }
 }
  asm("sei");
 }
//-------------------------------------------------
  void Com_exe(void){
    char temp;
	
       if(Rc_TO || (Code_com==0x04)){
  //handset: PG4=0-->off, PG4=1--> on
           if(Status_TA){
		    if(Flag_TA_TO)
	           Flag_TA_TO=0;		 
		       Tr_reply(3);
			   End_ring1();
			   return;
		   }  
		      temp=PING;
			 
           if(temp & 0x10)
			  Flag_ring=0;
		   else 
              Flag_ring=1;
	   }		  
		   	  	  
  } 			  
//-----------------------------------------------------
 void Aknowledge_dispatcher(void){
      char temp;
           Delay_50ms();
	       Cnt_dtmf_tr=1;
	       temp=Addr_TO;
	       if(temp <10)
              Tr_nibble_dtmf(0xf8);
	       else	 
		      Tr_nibble_dtmf(0xf9);
  }
//----------------------------------------------------------
  void Tr_reply(char value){
            Flag_ring=0;
            Reply=value;
            Aknowledge_dispatcher();
  }			
//----------------------------------------------------------
  void Requst_TO(char addr){
  	  if(!Cnt_dtmf_tr){
          Cnt_dtmf_tr=1;
		  Tr_nibble_dtmf(0xfc);
		  Flag_request_TO=1;
		  Addr_request_TO=addr;
 	  }	  
  }
//----------------------------------------------------------
  void Delay_50ms(void){
       char i;
	   unsigned int j;
  WDT_on();   
//       for(i=0;i<4;i++){
           for(j=0;j<0x0fff;j++)
              ;
//       }
  } 
 //---------------------------------------------- 
   void Start_T3(void){
                TCNT3H=0xb8;
			    TCNT3L=0x62;
		        TCCR3B=0x03;
  }		  
//--------------------------------------------------
  void Clr_T3(void){
       char temp;
	    
           ETIFR |=0x04;  //TOV3
           Cnt_dtmf_rc=0;
		   Buf_addr=0;
           Rc_TO=0;
           TCNT3H=0;
	       TCNT3L=0;
		   TCCR3B=0;
		   EIMSK=0xf7;
		 if( Flag_call_TA==1){
            Flag_call_TA=0;
		    Cnt_tr_uart=0;
			temp=PING;
            if(!(temp &=0x10))
                 Tr_reply(2);
		 }
   }     
//-----------------------------------------------
 void Request_T3(void){
      char temp;
	       temp=ETIFR;
           temp &=0x04;  //TOV3
		   if(!temp)
		      Clr_T3();
 } 
//------------------------------------------------
  char bcd_hex(char val){
       if(val >9)
		  val -=6;
		return val;  
  }      
  char hex_bcd(char val){
       if(val >9)
		  val +=6;
		return val;  
  } 
//------------------------------------------------
  void Process_rc_byte(void){
       char temp;
                 Com_exe();
				 if(Rc_TO)
				      ;
				 else if(!Cnt_dtmf_tr){	
					     temp=PING;
                      if(temp & 0x10)
		                   //hook on G4
		                  Reply=0x0d;
	                  else{
		                     if(Code_com==5)
		                        Reply=0x0f;
		                     else			
			                    Reply=0x09;
	                  }		  
			         Aknowledge_dispatcher();
                 }
	}
 //-----------------------------------------------
   void TA_tr(void){
	         ++Cnt_tr_uart;
		switch(Cnt_tr_uart){
		    case 2:   
	  		       UDR1=Qnt_TA;
			break;	   
 	        case 3:
		           UDR1=4;
				   Flag_call_TA=1;
			break;
			default:	   
	   	  	       Cnt_tr_uart=0;
			break;	   
		}	
   }				    
 //----------------------------------------------
 void TA_rc(void){
 //after request --> acknowledge(0x96,...,4)
 //command:d2,d3,d4,d6
     char temp,temp1;
	       temp=UDR1;
		   ++Cnt_rc_uart; 
      switch(Cnt_rc_uart){ 
	      case 1: 			
	     		if(temp !=0x96)
		           Cnt_rc_uart=0;
		  break;
		  case 2: 
		 		     ;
		  break;
		  case 3:		 
		 		 Cnt_rc_uart=0;
				 temp1=temp;
				 if(temp1 ==4){
				    TCCR3B=0;
				    ETIFR |=0x04; 
				 }
				 temp1 &=0xf0;
				 if(temp1 ==0xd0){
				    temp &=0x0f;
				    Process_reply_TA(temp);
				 }
		  break;
	  }	  	
 }	    
 //----------------------------------------------
 void Process_reply_TA(char reply){
      char temp;
	  if(Flag_TO_TA){
	     Flag_TO_TA=0;
		 Flag_call_TA=0;
		 LCD_clr8();
		 return;
	  }
      switch(reply){
	        case 2:
			      Status_TA=0;
				  if(Flag_TA_TO){
				     Flag_TA_TO=0;
					 LCD_clr8();
				     if(Flag_ring)
				        End_ring1();
				  }	 
  PORTB=PINB | 0x30;	
			break;
            case 3:
			         Status_TA=1;
	PORTB=PINB & 0xcf;						 
				   if(Flag_ring){
				      End_ring1();
				      Tr_reply(3);
					  break;
				   }  
				   if(Flag_call_TA){
				      Flag_call_TA=0;
					  Tr_reply(3);
					  break;  
				   }
	               if(Flag_request_TA)			
			          Flag_request_TA=0;
			       else{
				        temp=PING;
		             if((!(temp & 0x10)) && (!Flag_ring)){
				       Flag_TA_TO=1;
                       Flag_ring=1;
 			         }
				   }	 	
			break;	  
			case 4:	 
			       Requst_TO(1); //call dispetchar
				   Flag_request_TA=1;
			break;
			case 6: 	   
			       Status_TA=0;
				   Flag_call_TA=0; 
				   Tr_reply(2);
			break;	 
            default:
            break;
      }
   }
/*-----------------------------------------------*/
  void LCD_clr8(void){
       char i; 
	   
	   lcd_addr_set(0x0);
	   for(i=0;i<8;i++)   	  
          lcd_wr_data(0x20);
  }	   
/*-----------------------------------------------*/		