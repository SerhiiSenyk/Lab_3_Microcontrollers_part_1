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
	DATA_DDR = 0xFF;
	COMMAND_DDR = (1 << RS)|(1 << RW)|(1 << E);
	COMMAND_PORT &= ~((1 << RS)|(1 << RW)|(1 << E));
	DATA_PORT = 0x38;
	for(uint8_t i = 0;i < 2; ++i){
		HIGH(COMMAND_PORT, E);
		_delay_us(DELAY_COM_US);
		LOW(COMMAND_PORT, E);
		_delay_us(DELAY);
	}
	DATA_PORT = 0xC;
	HIGH(COMMAND_PORT, E);
	_delay_us(DELAY_COM_US);
	LOW(COMMAND_PORT, E);
	_delay_us(DELAY);
	DATA_PORT = 0x4;
	HIGH(COMMAND_PORT, E);
	_delay_us(DELAY_COM_US);
	LOW(COMMAND_PORT, E);
	_delay_us(DELAY);
	DATA_PORT = 1;
	HIGH(COMMAND_PORT, E);
	_delay_us(DELAY_COM_US);
	LOW(COMMAND_PORT, E);
}

void LCD_PollBusyFlag()
{
	DATA_DDR = 0;
	DATA_PORT = 1;
	LOW(COMMAND_PORT, RS);
	HIGH(COMMAND_PORT, RW);
	do{
		LOW(COMMAND_PORT, E);
		_delay_us(DELAY_COM_US);
		HIGH(COMMAND_PORT, E);
	}while(DATA_PIN & (1 << 7));
	LOW(COMMAND_PORT, E);
}

void LCD_WriteCommand(uint8_t command)
{
	LCD_PollBusyFlag();
	COMMAND_PORT &= ~((1 << RS)|(1 << RW)|(1 << E));
	DATA_DDR = 0xFF;
	DATA_PORT = command;
	HIGH(COMMAND_PORT, E);
	_delay_us(DELAY_COM_US);
	LOW(COMMAND_PORT, E);
}

void LCD_writeSymbol(uint8_t symbol)
{
	LCD_PollBusyFlag();
	HIGH(COMMAND_PORT, RS);
	LOW(COMMAND_PORT, RW);
	DATA_DDR = 0xFF;
	DATA_PORT = symbol;
	HIGH(COMMAND_PORT, E);
	_delay_us(DELAY_COM_US);
	LOW(COMMAND_PORT, E);
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
