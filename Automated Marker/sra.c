/* I N C L U D E S */
#include<avr/io.h>
#include<stdlib.h>
#include<compat/deprecated.h>
#include<util/delay.h>
#include<avr/eeprom.h>
#include<io.h>
#define threshold 50
/* E N D S */

unsigned char min[sensor_num],max[sensor_num],threshold1[sensor_num];
unsigned char sensorbyte=0;
#define threshold 100

//PORT INITIALIZE
void port_init(void)
{
 PORTA = 0xFF;
 DDRA  = 0x00;
 PORTB = 0xFF;  
 DDRB  = 0x00;
 PORTC = 0x00; //m103 output only
 DDRC  = 0xFF;
 PORTD = 0xFF;
 DDRD  = 0x00;
}

//PWM1 INITIALIZE
void pwm1_init(void)
{

 PWM1A_DIR=1;
 PWM1B_DIR=1;
 TCCR1B = 0x00; //stop
 TCNT1H = 0x00; //setup
 TCNT1L = 0x00;
 OCR1AH = 0x00;
 OCR1AL = 0x00;
 OCR1BH = 0x00;
 OCR1BL = 0x00;
 ICR1H  = 0x01;
 ICR1L  = 0x8F;
 TCCR1A = 0xA2;
 TCCR1B = 0x19; //start Timer
}

//ADC INITIALIZE
void adc_init(void)
{
 ADC_DIR=0X00;
 ADCSRA=0X00;
 ADMUX=0X60;//0x40 for 10 bits
 ADCSRA=0X87;
 ACSR=0X80;
}

//ADC START
unsigned char adc_start(unsigned char channel)
{
 unsigned char i;
 
     ADCH=0x00;

	 i=channel&0x07;
	 ADMUX=i|0x60;                //i|0x40 for 10 bits
	 ADCSRA|=1<<ADSC;
	   
		 while(ADCSRA & (1<<ADSC));       // wait for conv. to complete
		    unsigned char temp=ADCH;      //unsigned int temp=ADC;   for 10 bits
   
 return temp;
}

//DELAY FUNCTIONS
void delay_sec(int x)
{
 unsigned char i,j;
 for(i=0;i<x;i++)
  for(j=0;j<4;j++)
   _delay_ms(250);
}

void delay_millisec(int n)
{
	_delay_ms(n);
}

void delay_microsec(int n)
{
	_delay_us(n);
}

//CHECK THE SENSOR VALUES
void check_sensors(void)
{
sensorbyte=0;

unsigned char i,temp[sensor_num];

	 for(i=0;i<sensor_num;i++)
	 {
	 
	  temp[i]=adc_start(i);
	  if(temp[i]<threshold)
	  sensorbyte|=(1<<i);
	  
	  /*
	  if(temp[i]<threshold1[i])         // use for calibration
	  sensorbyte|=(1<<i);
	 */
	 } 
}
 
 //CALIBRATE FOR WHITE SURFACE
 void calibrate_white(void)
{
	unsigned char j,i,temp[sensor_num];

	for(j=0;j<sensor_num;j++) 
	 {
		  max[j]=adc_start(j);
	}

		
}

//CALIBRATE FOR BLACK SURFACE
void calibrate_black(void)
{

	unsigned char j,i,temp[sensor_num];
	 
	 for(j=0;j<sensor_num;j++) 
	 {
		  min[j]=adc_start(j);
 	}

		
}

//SET THRESHOLD VALUE
void set_threshold(void)
{

	unsigned char i,eeprom_addr=0x0000;
	
	
	 for(i=0;i<sensor_num;i++)
	 {
	 
		 threshold1[i]=((max[i]+min[i])>>1);
		  
	 }
	 
	 for(int i=0;i<sensor_num;i++)
	{
		eeprom_write_byte(eeprom_addr,threshold1[i]);
		eeprom_addr++;
	}
	 
}

//LED FLICKER FUNCTION
void flick (void)
{
unsigned int i=0;

	for(i=0;i<5;i++)
	{
		LED=0xff;
		delay_millisec(100);
		LED=0x00;
		delay_millisec(100);
	}

}

//SET PWM1A
void set_pwm1a(int a)
{
OCR1A=a;
}

//SET PWM1B
void set_pwm1b(int b)
{
OCR1B=b;
}

//LCD FUNCTIONS

//LCD DEFINITIONS

#define LCD_DATA_PORT 	PORT(LCD_DATA)
#define LCD_E_PORT 		PORT(LCD_E)
#define LCD_RS_PORT 		PORT(LCD_RS)
#define LCD_RW_PORT 		PORT(LCD_RW)

#define LCD_DATA_DDR 	DDR(LCD_DATA)
#define LCD_E_DDR 		DDR(LCD_E)
#define LCD_RS_DDR 		DDR(LCD_RS)
#define LCD_RW_DDR 		DDR(LCD_RW)

#define LCD_DATA_PIN		PIN(LCD_DATA)

#define SET_E() (LCD_E_PORT|=(1<<LCD_E_POS))
#define SET_RS() (LCD_RS_PORT|=(1<<LCD_RS_POS))
#define SET_RW() (LCD_RW_PORT|=(1<<LCD_RW_POS))

#define CLEAR_E() (LCD_E_PORT&=(~(1<<LCD_E_POS)))
#define CLEAR_RS() (LCD_RS_PORT&=(~(1<<LCD_RS_POS)))
#define CLEAR_RW() (LCD_RW_PORT&=(~(1<<LCD_RW_POS)))

#define blink 	    0B00000001
#define underline 0B00000010

#define lcd_cmd(c) (lcd_byte(c,0))
#define lcd_data(d) (lcd_byte(d,1))

#define lcd_clear()	lcd_cmd (0b00000001)
#define lcd_home()	lcd_cmd (0b00000010);

#define _CONCAT(a,b) a##b
 #define PORT(x) _CONCAT(PORT,x)
 #define PIN(x) _CONCAT(PIN,x)
 #define DDR(x) _CONCAT(DDR,x)


//LCD FUNCTIONS
void lcd_byte(uint8_t c,uint8_t isdata)
{
uint8_t hn,ln;			//Nibbles
uint8_t temp;
hn=c>>4;
ln=(c & 0x0F);
if(isdata==0)
	CLEAR_RS();
else
	SET_RS();
_delay_us(0.500);		//tAS
SET_E();
temp=(LCD_DATA_PORT & 0XF0)|(hn);
LCD_DATA_PORT=temp;
_delay_us(1);			//the
CLEAR_E();
_delay_us(1);
SET_E();
temp=(LCD_DATA_PORT & 0XF0)|(ln);
LCD_DATA_PORT=temp;
_delay_us(1);			//tEH
CLEAR_E();
_delay_us(1);			//tEL
lcd_busy_loop();
}

void lcd_busy_loop(void)
{
	uint8_t busy,status=0x00,temp;
	LCD_DATA_DDR&=0xF0;
	SET_RW();		//Read mode
	CLEAR_RS();		//Read status
	_delay_us(0.5);		//tAS
	do
	{

		SET_E();
		_delay_us(0.5);
		status=LCD_DATA_PIN;
		status=status<<4;
		_delay_us(0.5);
		CLEAR_E();
		_delay_us(1);	//tEL
		SET_E();
		_delay_us(0.5);
		temp=LCD_DATA_PIN;
		temp&=0x0F;
		status=status|temp;
		busy=status & 0b10000000;
		_delay_us(0.5);
		CLEAR_E();
		_delay_us(1);	//tEL
	}while(busy);
CLEAR_RW();		//write mode
	//Change Port to output
	LCD_DATA_DDR|=0x0F;

}

void lcd_init(uint8_t style)
{
	
_delay_ms(30);
	
	//Set IO Ports
	LCD_DATA_DDR|=(0x0F);
	LCD_E_DDR|=(1<<LCD_E_POS);
	LCD_RS_DDR|=(1<<LCD_RS_POS);
	LCD_RW_DDR|=(1<<LCD_RW_POS);

	LCD_DATA_PORT&=0XF0;
	CLEAR_E();
	CLEAR_RW();
	CLEAR_RS();
	_delay_us(0.3);	//tAS
	SET_E();
	LCD_DATA_PORT|=(0b00000010);
	_delay_us(1);
	CLEAR_E();
	_delay_us(1);
	lcd_busy_loop();                                    //[B] Forgot this delay
	lcd_cmd (0b00001100|style);	//Display On
	lcd_cmd (0b00101000);			//function set 4-bit,2 line 5x7 dot format
}
void lcd_write_string(const char *msg)
{
while(*msg!='\0')
 {
	lcd_data (*msg);
	msg++;
 }
}

void lcd_write_int(int val,unsigned int field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
	str[i]=val%10;
	val=val/10;
	i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) lcd_data ('-');
	for(i=j;i<5;i++)
	{
	lcd_data (48+str[i]);
	}
}
void lcd_goto_xy(uint8_t x,uint8_t y)
{
 if(x<40)
 {
  if(y) x|=0b01000000;
  x|=0b10000000;
  lcd_cmd (x);
  }
}
void lcd_write_string_xy(int x,int y,char *msg)
 {
 lcd_goto_xy(x,y);
 lcd_write_string(msg);
}

void lcd_write_int_xy(int x,int y,int val,int fl) {
 lcd_goto_xy(x,y);
 lcd_write_int(val,fl);
}

//USART COMMANDS
void usart_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = (URSEL) | 0x06;
 UBRRL = 0x67; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x98;
}
void usart_transmit_char( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) )
;
/* Put data into buffer, sends the data */
UDR = data;
}

void usart_transmit_string(char *msg )
{
while(*msg!='\0')
 {
	usart_transmit_char(*msg);
	msg++;
 }
}
void usart_transmit_newline(void)
{
usart_transmit_char(10);
usart_transmit_char(13);
}
unsigned char usart_receive_char(void)
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) )
;
/* Get and return received data from buffer */
return UDR;
}

//BOT MOTIONS
void bot_motion_init(void)
{
DDRC=0xff;
}
void bot_left_forward(void)
{
 MOTOR1A=1;
 MOTOR1B=0;
 MOTOR2A=0;
 MOTOR2B=0;
}
void bot_left_backward(void)
{
 MOTOR1A=0;
 MOTOR1B=1;
 MOTOR2A=0;
 MOTOR2B=0;
}
void bot_right_forward(void)
{
 MOTOR1A=0;
 MOTOR1B=0;
 MOTOR2A=1;
 MOTOR2B=0; 
}

void bot_left(void)
{
 bot_right_forward();
}

void bot_right(void)
{
 bot_left_forward();
}

void bot_right_backward(void)
{
 MOTOR1A=0;
 MOTOR1B=0;
 MOTOR2A=0;
 MOTOR2B=1; 
}
void bot_forward(void)
{
MOTOR1A=1;
 MOTOR1B=0;
 MOTOR2A=1;
 MOTOR2B=0;
}
void bot_backward(void)
{
 MOTOR1A=0;
 MOTOR1B=1;
 MOTOR2A=0;
 MOTOR2B=1;
}
void bot_spot_left(void)
{
 MOTOR1A=0;
 MOTOR1B=1;
 MOTOR2A=1;
 MOTOR2B=0;
}

void bot_spot_right(void)
{
MOTOR1A=1;
 MOTOR1B=0;
 MOTOR2A=0;
 MOTOR2B=1;
}
void bot_stop(void)
{
 MOTOR1A=0;
 MOTOR1B=0;
 MOTOR2A=0;
 MOTOR2B=0;
}
void bot_brake(void)
{
MOTOR1A=1;
 MOTOR1B=1;
 MOTOR2A=1;
 MOTOR2B=1;
}

void switch_init(void)
{
PORTD|=0x0F;
DDRD&=0xF0;
}

int pressed_switch0(void)
{
if(bit_is_clear(PIND,0))
return 1;
else
return 0;
}

int pressed_switch1(void)
{
if(bit_is_clear(PIND,1))
return 1;
else
return 0;
}

int pressed_switch2(void)
{
if(bit_is_clear(PIND,2))
return 1;
else
return 0;
}

int pressed_switch3(void)
{
if(bit_is_clear(PIND,3))
return 1;
else
return 0;
}

/*EEPROM FUNCTIONS*/
//Variabes

void retrieve_threshold(void)
{
	unsigned char eeprom_addr=0x0000;
	for(int i=0;i<sensor_num;i++)
	{
		threshold1[i]=eeprom_read_byte(eeprom_addr);
		eeprom_addr++;
	}
}

