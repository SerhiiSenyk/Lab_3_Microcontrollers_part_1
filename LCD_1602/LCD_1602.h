/*
 * LCD_1602.h
 *
 * Created: 01.03.2020
 *  Author: Serhii Senyk
 */ 


#ifndef LCD_1602_H_
#define LCD_1602_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define RS 0
#define RW 1
#define E  2
#define DATA_DDR DDRF
#define DATA_PORT PORTF
#define DATA_PIN PINF
#define COMMAND_DDR DDRB
#define COMMAND_PORT PORTB
#define SIZE_DATA 8
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