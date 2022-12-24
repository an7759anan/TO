
;Preserved reg: R28,R29 /R10...R15/R20...R23
;Volatile reg: R0...R9,R24...R31,SREG

             ddra   =0x1a
             porta  =0x1b
             pina   =0x19
             pinf   =0x0
             ddrf   =0x61
             portf  =0x62
			 
	.define  A  r24

 _dtmf_wr::
               ldi   A,0xff  ;dtmf_out
			   out   ddra,A              
               out   porta,r16
			   rcall dtmf_cs_wr
			   ret 
			   
 _dtmf_wr_data::
               ldi   A,0xff  ;dtmf_out
			   out   ddra,A 
			   ori   r16,0xf0
			   andi  r16,0xef
			   out   porta,r16
			   rcall dtmf_cs_wr
			   ret 
			   
_dtmf_rd::
               ldi    A,0xf0  ;dtmf_in
			   out    ddra,A  
	           ldi    A,0xff
			   out    porta,A
			   rcall  dtmf_cs_rd
		       ret		   
			   		
 _dtmf_rd_data::
               ldi   A,0xf0  ;dtmf_in
			   out   ddra,A  
	           ldi   A,0xef
			   out   porta,A
			   rcall dtmf_cs_rd
		       ret						
				   
 dtmf_cs_wr:			      
               in   A,pinf   ;cs: F3
			   andi A,0xf7
			   sts  portf,A
			   nop
			   in   A,pinf   ;wr: F2
			   andi A,0xfb
			   sts  portf,A
		       nop
			   in   A,pinf   
			   ori  A,0x04
			   sts  portf,A
		       nop
			   in   A,pinf   
			   ori  A,0x08
			   sts  portf,A
		       nop
               ret
			          
 dtmf_cs_rd:             
		       in    A,pinf   ;cs: F3
			   andi  A,0xf7
			   sts   portf,A 
			   cbi   porta,5   ;rd A5
			   nop
			   nop
			   nop
               in    r16,pina
               andi  r16,0x0f
			   sbi   porta,5
               in    A,pinf   
			   ori   A,0x08
			   sts   portf,A	  
               ret			  
			  
			