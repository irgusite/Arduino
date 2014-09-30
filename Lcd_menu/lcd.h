/*
LCD_h library for use with ea dips082-hn and  PCF 8574 
Initialises the connection and the lcd. Has simple commands to control the lcd
Created by Philipp Schuler the 29.09.2014
*/
#ifndef LCD_h
#define LCD_h

#include "Arduino.h"
#include "Wire.h"

#define PCF8574P_ADDRESS (0x20)

#define RS_bit      0 //Register Select Bit  ( high for data, low for command)
#define RW_bit    1 //H: Read    / L: Write
#define E_bit 2 //Enable Bit
#define LED_bit     3
#define LEDstatus 0
#define LCD_CLEAR 0x01
#define LCD_VIS_CUR_STAT 0x0E
#define LCD_VIS_NCUR_STAT 0x0C

class LCD{
public:
	LCD(int adress);
	void init(); //initialisation of the screen.
	
	void command(unsigned char byte); //sends a command byte to the screen
	void add_char(unsigned char byte); // adds a character to the current position

	void move_cursor(int x, int y); //obvious, i think
	void write(char *line1,  char *line2); //Writes a text on each line, 8 character maximum
	void write_2(char *line1); //Writes a text on each line, 8 character maximum
	void separate(String text, int number, int line);
	void clear();
private:
	int _adress;
	void set_pcf8574(unsigned char baite);
	void send_nibble(unsigned char i2cbyte);

};

#endif