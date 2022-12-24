
;LCD.S
;Preserved reg: R28,R29 /R10...R15/R20...R23
;Volatile reg: R0...R9,R24...R31,SREG

;PA3...PA0 bus data
;PD6 -  wr/rd_LCD
;PD7 -  A0_LCD
;PF6 -  E_LCD
             
		      ddrA  = 0x1A
              portA = 0x1B
			  
			  ddrB  = 0x17
              portB = 0x18
			  
			  pinA  = 0x19
              portD = 0x12
              pinD  = 0x10
              portF = 0x62
              pinf  = 0x0

  udr1  = 0x9c
  ucsr1a= 0x9b

             .define   A    R24
             .define   B    R25

             .define   XL   R26
             .define   XH   R27
             .define   ZL   R30
             .define   ZH   R31
;----------------------------------------------------------
  .macro   lcd_port_wr
              in      B,ddrA
              ori     B,0x0f
              out     ddrA,B
  .endmacro
  .macro   lcd_port_rd
              in      B,ddrA
              andi    B,0xf0
              out     ddrA,B
  .endmacro
  .macro   lcd_enable_0
              in      B,pinf    ;E=0
              andi    B,0xbf
              sts     portF,B
			  nop
			  nop
			  nop
  .endmacro

  .macro   lcd_enable_1
              in      B,pinf    ;E=1
              ori     B,0x40
              sts     portF,B
              nop
              nop
			  nop
  .endmacro

;----------------------------------------------------------
 _lcd_data_set::
;data - r16
                mov     B,r16
                swap    B
                ori     B,0xf0
                out     portA,B

              lcd_enable_1
              lcd_enable_0

                ori     r16,0xf0
                out     portA,r16

              lcd_enable_1
              lcd_enable_0
              ret
 ;---------------------------------------------------------
 _lcd_wr_com::
              lcd_port_wr
              lcd_enable_0

              cbi     portD,7   ;A0
              cbi     portD,6   ;wr
              rcall   _lcd_data_set
              rcall   _lcd_rd_busy
              ret
;----------------------------------------------------------
 _lcd_wr_data::
;data - r16
              lcd_port_wr
              lcd_enable_0

              sbi     portD,7   ;A0
              cbi     portD,6   ;wr
              rcall   _lcd_data_set
              rcall   _lcd_rd_busy
              ret
;----------------------------------------------------------
 _lcd_addr_set::
                 ori    r16,0x80
                 rcall  _lcd_wr_com
                 ret
;----------------------------------------------------------
 _lcd_rd_busy::
            lcd_port_rd
            lcd_enable_0

                cbi     portD,7   ;A0
                sbi     portD,6   ;rd
				nop
                nop
   rd_busy:
            lcd_enable_1
                in      A,pinA
            lcd_enable_0

            lcd_enable_1
                in      r0,pinA
            lcd_enable_0
                sbrc   A,3
                rjmp   rd_busy
            lcd_port_wr
		
              ret
;---------------------------------------------------------
 _lcd_init::
                push    r16
           lcd_port_wr
           lcd_enable_0
                cbi     portD,7   ;A0
                cbi     portD,6   ;wr
				
                ldi     A,0x03
                out     portA,A
           lcd_enable_1
           lcd_enable_0
                rcall    _lcd_delay
 ;....................................
             ;   ldi     A,0x03
             ;   out     portA,A
           lcd_enable_1
           lcd_enable_0
                rcall    _lcd_delay
 ;.................................... 
             ;   ldi     A,0x03
             ;   out     portA,A
           lcd_enable_1
           lcd_enable_0
                rcall    _lcd_delay
 ;.................................... 
                ldi     A,0x02
			    out     portA,A
		   lcd_enable_1
           lcd_enable_0
		        rcall    _lcd_delay
 ;.....................................
                ldi      r16,0x28
				rcall   _lcd_wr_com
                ldi      r16,0x08
                rcall   _lcd_wr_com
                ldi      r16,0x01
                rcall   _lcd_wr_com
                ldi      r16,0x06
                rcall   _lcd_wr_com
				ldi      r16,0x0c
				rcall   _lcd_wr_com
                pop      r16
                ret
;---------------------------------------------------------
_lcd_delay::
               ldi    A,0xff     ;280ns*178= 50mks
   lcd_dly:                      ;280ns*256= 70mks
               dec    A
               brne   lcd_dly
               ret
;---------------------------------------------------------
_lcd_clr::
               ldi      r16,1
               rcall   _lcd_wr_com
               ret
;---------------------------------------------------------
_cursor_on::
               ldi      r16,0x0e
               rcall   _lcd_wr_com
               ret
;---------------------------------------------------------
_cursor_off::
               ldi      r16,0x0c
               rcall   _lcd_wr_com
               ret
;---------------------------------------------------------
 _lcd_2digit::
  ;r16 - addr, r18 - digit
               rcall    _lcd_addr_set
               mov       r16,r18
               swap      r16
               andi      r16,0x0f
               ori       r16,0x30
               rcall     _lcd_wr_data
               andi      r18,0x0f
               ori       r18,0x30
               mov       r16,r18
               rcall     _lcd_wr_data
               ret
;---------------------------------------------------------
 _lcd_digit_A::
  ;r16 - addr, r18 - digit
               rcall    _lcd_addr_set
               mov       r16,r18
               andi      r16,0x0f
               ori       r16,0x30
               rcall     _lcd_wr_data
               ret
;---------------------------------------------------------
 _lcd_digit::
  ;r16 - digit
               andi      r16,0x0f
               ori       r16,0x30
               rcall     _lcd_wr_data
               ret
;---------------------------------------------------------
 _lcd_symbol::
  ;r16 - symbol
               rcall     _lcd_wr_data
               ret 
;-------------------------------------------------------
  