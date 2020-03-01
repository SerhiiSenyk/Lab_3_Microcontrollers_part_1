/*
 * LCD_1602.h
 *
 * Created: 01.03.2020
 *  Author: Serhii Senyk
 */ 

/*
	Atmega 2560:
	data ports: PORTF (PF0 - PF7)
	command ports: PORTB (PB0 - PB3)
		RS - PB0
		RW - PB1
		E  - PB2

*/
#ifndef LCD_1602_H_
#define LCD_1602_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define RS 0
#define RW 1
#define E  2
#define DDR_DATA DDRF
#define PORT_DATA PORTF
#define PIN_DATA PINF
#define DDR_COMMAND DDRB
#define PORT_COMMAND PORTB
#define HIGH(port, pin) ((port) |= (1 << (pin)))
#define LOW(port, pin) ((port) &= ~(1 << (pin)))
#define DELAY_COM_US 0.20
#define DELAY 40

void LCD_Settup();
void LCD_PollBusyFlag();
void LCD_WriteCommand(uint8_t command);
void LCD_writeSymbol(uint8_t symbol);
void LCD_GotoYX(uint8_t Y, uint8_t X);
void LCD_WriteStr(volatile char *str);

#endif /* LCD_1602_H_ */