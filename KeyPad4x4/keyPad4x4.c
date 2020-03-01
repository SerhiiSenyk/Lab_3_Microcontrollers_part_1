/*
 * keyPad4x4.c
 *
 * Created: 01.03.2020
 *  Author: Serhii Senyk
 */ 
#include "keypad4x4.h"

uint8_t wasPressed = 0;
uint8_t isPressed = 0;
uint8_t logicPressed = 0;
char pressedButton;

void SetupKeyPad()
{
	DDR_KEYPAD = 0x0F;
	PORT_KEYPAD = 0xF0;
}

char readKeyFromPad4x4() {
	logicPressed = 0;
	return pressedButton;
}


uint8_t isButtonPressed()
{
	if(!wasPressed)
	{
		if(PIN_KEYPAD != 0xF0)
		{
			_delay_ms(50);
			wasPressed = 1;
			isPressed = 1;
			PORT_KEYPAD = 0xFE;//A row
			asm("nop");
			if(PORT_KEYPAD == PIN_KEYPAD)
			{
				PORT_KEYPAD = 0xFD;//B row
				asm("nop");
				if(PORT_KEYPAD == PIN_KEYPAD){
					PORT_KEYPAD = 0xFB;//C row
					asm("nop");
					if(PORT_KEYPAD == PIN_KEYPAD){
						PORT_KEYPAD = 0xF7;//D row
						asm("nop");
						if(PORT_KEYPAD == PIN_KEYPAD){
							isPressed = 0;
						}
					}
				}	
			}
			if(isPressed)
			{
				switch(PIN_KEYPAD){
					case KEY_1: pressedButton = '1'; break;
					case KEY_2: pressedButton = '2'; break;
					case KEY_3: pressedButton = '3'; break;
					case KEY_4: pressedButton = '4'; break;
					case KEY_5: pressedButton = '5'; break;
					case KEY_6: pressedButton = '6'; break;
					case KEY_7: pressedButton = '7'; break;
					case KEY_8: pressedButton = '8'; break;
					case KEY_9: pressedButton = '9'; break;
					case KEY_0: pressedButton = '0'; break;
					case KEY_A: pressedButton = 'A'; break;
					case KEY_B: pressedButton = 'B'; break;
					case KEY_C: pressedButton = 'C'; break;
					case KEY_D: pressedButton = 'D'; break;
					case KEY_E: pressedButton = '#'; break;
					case KEY_F: pressedButton = '*'; break;
					default:break;	
				}
				logicPressed = 1;
			}
			PORT_KEYPAD = 0xF0;
		}	
	}
	else if(PIN_KEYPAD == 0xF0){
		_delay_ms(200);
		wasPressed = 0;
	}
	return logicPressed;
}