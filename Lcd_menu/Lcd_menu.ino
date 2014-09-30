/***************************************************
LCD MENU
Author: Philipp Schuler
Begun the: 29.09.2014
Comment: Domotic project. Info and command screen
****************************************************/

#include <Wire.h>
#include "lcd.h"
#include "timer.h"

/*******************
DEFINITION OF TABLES
********************/
#define MENU_ENTRY_NB 3
typedef void (*FctPtr) ();
FctPtr fct[MENU_ENTRY_NB]= 		{temp, light, air}; //menu entries pointer to the function
char* menuName[MENU_ENTRY_NB] =	{"Temp", "Light", "Air"}; // menu entries name 7 char max!

/******************
Global variables
*******************/

int selected_entry = 0;
boolean in_menu = false;

LCD lcd(0x20); //instancies the LCD and defines the lcd adress on the i2c bus

Timer t; //timer for update functions

int fct_timer =0;
int count =0;

/**************
Sub-Menu functions
***************/

void temp(){
	lcd.move_cursor(0,1);
}

void light(){
	lcd.write("1:","2:");
}

void air(){
    lcd.separate("H2:", count,1);
    lcd.move_cursor(0,1);
    count++;
}

/**************
Menu functions
***************/

void show_menu(){ //menu on first line, selected line (1 or 2)
    int second_entry = selected_entry + 1;
    if((second_entry+1)>MENU_ENTRY_NB)
        second_entry = 0;
	lcd.write(menuName[selected_entry], menuName[second_entry]);
	lcd.move_cursor(7,0);
	lcd.add_char('<');
}

int loop_menu(){
	selected_entry ++;
	if(selected_entry >= MENU_ENTRY_NB){
		selected_entry = 0;
	}
	show_menu();
    Serial.print("Selected: ");
    Serial.println(menuName[selected_entry]);
}

void select_menu(){
    if(in_menu){
        Serial.print("out: ");
        Serial.println(menuName[selected_entry]);
        t.stop(fct_timer);
        back_menu();
        in_menu = false;
    }
    else{
        fct[selected_entry]();
        fct_timer = t.every(5*1000, fct[selected_entry]);
        Serial.print("in: ");
        Serial.println(menuName[selected_entry]);
        in_menu = true;
    }
}

void back_menu(){
    show_menu();
}

/**********************
Arduino main functions
***********************/

void setup()
{
    /**********
    button pin
    ***********/
	pinMode(7, INPUT);
    pinMode(8, INPUT);

	Serial.begin(9600);
    delay(1000);
    Serial.println("Debug menu.");

    //uint8_t value = PCF_38.read8();
    Wire.begin();
    lcd.init();
    show_menu();
}

void loop() {
	delay(50);
    t.update();
    if(digitalRead(7)==LOW && not in_menu){
        loop_menu();
        while(digitalRead(7)==LOW){
            delay(50);
        }
    }
    else if(digitalRead(8)==LOW){
        select_menu();
        while(digitalRead(8)==LOW){
            delay(50);
        }
    }

}
