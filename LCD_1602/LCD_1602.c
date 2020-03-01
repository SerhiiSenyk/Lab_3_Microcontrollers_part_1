/*
 * LCD_1602.c
 *
 * Created: 01.03.2020
 *  Author: Serhii Senyk
 */ 
#include "LCD_1602.h"
void LCD_Settup()
{
	_delay_ms(40);
	DDR_DATA = 0xFF;
	DDR_COMMAND = (1 << RS)|(1 << RW)|(1 << E);
	PORT_COMMAND &= ~((1 << RS)|(1 << RW)|(1 << E));
	PORT_DATA = 0x38;
	for(uint8_t i = 0;i < 2; ++i){
		HIGH(PORT_COMMAND, E);
		_delay_us(DELAY_COM_US);
		LOW(PORT_COMMAND, E);
		_delay_us(DELAY);
	}
	PORT_DATA = 0xC;
	HIGH(PORT_COMMAND, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_COMMAND, E);
	_delay_us(DELAY);
	PORT_DATA = 0x4;
	HIGH(PORT_COMMAND, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_COMMAND, E);
	_delay_us(DELAY);
	PORT_DATA = 1;
	HIGH(PORT_COMMAND, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_COMMAND, E);
}

void LCD_PollBusyFlag()
{
	DDR_DATA = 0;
	PORT_DATA = 1;
	LOW(PORT_COMMAND, RS);
	HIGH(PORT_COMMAND, RW);
	do{
		LOW(PORT_COMMAND, E);
		_delay_us(DELAY_COM_US);
		HIGH(PORT_COMMAND, E);
	}while(PIN_DATA & (1 << 7));
	LOW(PORT_COMMAND, E);
}

void LCD_WriteCommand(uint8_t command)
{
	LCD_PollBusyFlag();
	PORT_COMMAND &= ~((1 << RS)|(1 << RW)|(1 << E));
	DDR_DATA = 0xFF;
	PORT_DATA = command;
	HIGH(PORT_COMMAND, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_COMMAND, E);
}

void LCD_writeSymbol(uint8_t symbol)
{
	LCD_PollBusyFlag();
	HIGH(PORT_COMMAND, RS);
	LOW(PORT_COMMAND, RW);
	DDR_DATA = 0xFF;
	PORT_DATA = symbol;
	HIGH(PORT_COMMAND, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_COMMAND, E);
}

void LCD_GotoYX(uint8_t Y, uint8_t X)
{
	if(Y == 1) LCD_WriteCommand(((1<<7)|(X - 1)));
	else LCD_WriteCommand(((1<<7)|(0x40 + X - 1)));
}

void LCD_WriteStr(volatile char *str)
{
	for(int i = 0;i < 255; ++i){
		if(str[i] == '\0') return;
		LCD_writeSymbol(str[i]);
	}
}
