/*
LCD_h library for use with ea dips082-hn and  PCF 8574 
Initialises the connection and the lcd. Has simple commands to control the lcd
Created by Philipp Schuler the 29.09.2014
*/

#include "Arduino.h"
#include "lcd.h"


LCD::LCD(int adress){
	_adress = adress;
}

void LCD::set_pcf8574(unsigned char baite){
    Wire.beginTransmission(_adress);
    Wire.write(baite);
    Wire.endTransmission();
}

void LCD::send_nibble(unsigned char i2cbyte)
{
    set_pcf8574(i2cbyte|(LEDstatus<<LED_bit)); //Daten anlegen
    set_pcf8574(i2cbyte|(LEDstatus<<LED_bit)|(1<<E_bit)); //Enable_bit setzen
    set_pcf8574(i2cbyte|(LEDstatus<<LED_bit)); //Enable_bit löschen
}

void LCD::init(){
	send_nibble(0x30);
	delay(5);
	send_nibble(0x30);
	delay(1);
	send_nibble(0x30);
	delay(1);
	send_nibble(0x20);
	delay(1);
	command(0x28);
	delay(1);
	command(LCD_VIS_CUR_STAT);
	delay(1);
	command(LCD_CLEAR);
	delay(1);
	command(0x06);
	delay(1);
}

void LCD::command(unsigned char byte)
{
    send_nibble(byte&0xF0);   //Envoie le bit fort
    send_nibble(byte<<4); //Envoie le bit faible
}

void LCD::add_char(unsigned char byte)
{
    send_nibble((byte&0xF0)|(1<<RS_bit)); //Envoie le bit fort
    send_nibble((byte<<4)  |(1<<RS_bit)); //Envoie le bit faible
}

void LCD::move_cursor(int x, int y){ // se déplace au coord.
    command(0x80+(y*0x40)+x); 
}

void LCD::clear(){
    command(LCD_CLEAR);
}

void LCD::write(char *line1,  char *line2){ //ecrit chaque ligne (8 char max)
    clear();
    move_cursor(0,0);
    unsigned char c;
    while((c=*line1++)){
        add_char(c);
    }
    move_cursor(0,1);
    while((c=*line2++)){
        add_char(c);
    }
}

void LCD::write_2(char *line1){ //ecrit chaque ligne (8 char max)
    clear();
    move_cursor(0,1);
    unsigned char c;
    while((c=*line1++)){
        add_char(c);
    }
}

void LCD::separate(String text, int number, int line){
    char list[9];
    int total = 8-text.length();
    int len = String(number).length();
    
    for(int i=0; i<(total-len);i++){
        text +=" ";
    }
    text+=number;
    text.toCharArray(list,9);
    write_2(list);
}