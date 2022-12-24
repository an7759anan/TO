
#ifndef __TO_s_h_
#define __TO_s_h_
 char keyboard(char n_row);
 void scan_key(void);
 void set_byte_scan(void);
 void bounce_contact(void);
 void transform_code(void);

 void lcd_init(void);
 void lcd_clr(void);
 void cursor_on(void);
 void cursor_off(void);
 void lcd_wr_com(char data);
 void lcd_wr_data(char data);
 void lcd_addr_set(char data);
 void lcd_2digit(char addr,char data);
 void lcd_digit_A(char addr,char data);
 void lcd_digit(char data);
 void lcd_symbol(char data);
 void lcd_delay(void);

 void set_addr(char addr);
 void shift_byte_D5(void);
 void prog_gain(char buf1,char buf2);
 void dtmf_wr(char val); 
 void dtmf_wr_data(char val);
 char dtmf_rd(void);
 char dtmf_rd_data(void);         
 #endif
