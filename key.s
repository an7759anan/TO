
;Key.S
;Preserved reg: R28,R29 /R10...R15/R20...R23
;Volatile reg: R0...R9,R24...R31,SREG
;10ms -->0x8bff

;uart_test:
;          sbis ucsr0a,0x05      ;udr0e
;          rjmp  uart_test
;          out udr0,cnt_cdsh

  udr0  = 0x0c
  ucsr0a= 0x0b
  udr0e = 0x05

              eimsk =0x59
              eifr  =0x58
              portE =0x03
              pinE  =0x01

             .define   A    R24
             .define   ZL   R30
             .define   ZH   R31

             .define   n_col       R25
             .define   n_row       R26
             .define   byte_scan   R9

_keyboard::
    ;r16 - n_row
    ;r16 - code return

               mov      n_row,r16
               rcall    _bounce_contact
               rcall    _scan_key

 release_key:
                in     A,pinE
                andi   A,0xf0
                cpi    A,0xf0
                brne   release_key
 keyboard_end:
                ldi    A,0xf0
                out    portE,A
                ret
 ;---------------------------------------------------------
 _scan_key::
                ldi    n_col,0
                ldi    A,0xfe
                mov    byte_scan,A
 scan_line:
                rcall  _set_byte_scan
                sec
                rol    byte_scan

                in     A,pinE
                andi   A,0xf0
                cpi    A,0xf0
                brne   scan_key_end

                inc    n_col
                cpi    n_col,4
                breq   scan_key_end0
                rjmp   scan_line
 scan_key_end0:
                ldi    r16,0xff
                ret
 scan_key_end:
                rcall  _transform_code
                ret
 ;---------------------------------------------------------
_set_byte_scan::
                sbrs  byte_scan,0
                rjmp  set_bit0
                sbi   portE,0
                rjmp  set_bit1
   set_bit0:
                cbi   portE,0
   set_bit1:
                sbrs  byte_scan,1
                rjmp  set_bit1_1
                sbi   portE,1
                rjmp  set_bit2
   set_bit1_1:  cbi   portE,1
   set_bit2:
                sbrs  byte_scan,2
                rjmp  set_bit2_1
                sbi   portE,2
                rjmp  set_bit3
   set_bit2_1:  cbi   portE,2
   set_bit3:
                sbrs  byte_scan,3
                rjmp  set_bit3_1
                sbi   portE,3
                rjmp  set_bit_end
   set_bit3_1:  cbi   portE,3
   set_bit_end:
                ret
 ;---------------------------------------------------------
 ;10ms -->0x8bff
 _bounce_contact::
                ldi   ZL,0xff
                ldi   ZH,0x8b
   bounce_cyc:
                sbiw  ZL,1
                brne  bounce_cyc
                ret
 ;---------------------------------------------------------
 _transform_code::
                  ldi   ZH,>(_code_key)
                  ldi   ZL,<(_code_key)
                  ldi   A,4
                  mul   A,n_row
                  add   r0,n_col
                  clr   A
                  adc   r1,A
                  add   ZL,r0
                  adc   ZH,r1
                  lpm   r16,Z
                  ret
 ;----------------------------------------------------------
_code_key::
;     e0   e1   e2   e3
 .db 0x01,0x02,0x03,0x0a  ;e4
 .db 0x04,0x05,0x06,0x0b  ;e5
 .db 0x07,0x08,0x09,0x0c  ;e6
 .db 0x0e,0x0 ,0x0f,0x0d  ;e7
 ;    *        #
 ;----------------------------------------------------------

