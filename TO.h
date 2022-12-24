
#ifndef __TO_h_
#define __TO_h_

void TO_init(void);
void wr_spi(char addr,char data);
void wr_spi_buf(char addr,char* buf);
char rd_spi(char addr);

void rd_RTC_hm(void);
void rd_RTC_data(void);
void wr_RTC_hm(void);
void wr_RTC_data(void);
void Input_RTC_hm(void);
void Input_RTC_data(void);
void RTC_LCD(void);
void RTC_LCD_data(void);
void RTC_LCD_data1(void);
void LCD_line(const unsigned char* buf);
void LCD_line1(const unsigned char* buf,char addr,char qnt);
void LCD_contrast_on(void);
void LCD_byte_bin(char byte);
void LCD_reg_SP(void);
void LCD_clr8(void);
void Process_menu_config(void);
void Process_enter(void);
void wr_reg_SP(char byte_h,char byte_l);
void Output_byte_SP(void);
void Input_byte_SP(void);
void Save_byte_SP(void);
void Process_PGA(char mask,char addr);  
void set_PGA(char byte_h,char byte_l,char mask);

void Process_Key(void);
void Test_password(void);
void Test_pw_SP(void);
void EEPROM_write(char addr,char data);
char EEPROM_read(char addr);
void Process_int_1s(void);
void cpc(void);
void Ring(void);
void Speakphone_on(void);
void Speakphone_off(void);
void SP_ring_on(void);
void Transform_vol_speakphone(void);
void Set_control_bit(void);
void Proc_15s(void);
void End_ring(void);
void End_ring1(void);
void uart1_rx(void);
void uart1_tx_end(void);
void TA_rc(void);
void TA_tr(void);
void Call_TA_88(void);
void Process_reply_TA(char reply);
void DTMF_wr(char val);
void DTMF_wr_data(char val);
char DTMF_rd(void);
char DTMF_rd_data(void);
void Initial_dtmf(void);
void Tr_nibble_dtmf(char val);
void Process_dtmf(void);
void Process_rc_byte(void);
void Start_T3(void);
void Clr_T3(void);
void Request_T3(void);
void Com_exe(void);
void Aknowledge_dispatcher(void);
void Tr_reply(char value);
void Requst_TO(char addr);
void Delay_50ms(void);
char hex_bcd(char val);
char bcd_hex(char val);
void WDT_on(void);

#endif
