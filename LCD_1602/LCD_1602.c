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
	DDR_CONTROL |= (1 << RS)|(1 << RW)|(1 << E);
	PORT_CONTROL &= ~((1 << RS)|(1 << RW)|(1 << E));
	PORT_DATA = 0x38;
	for(uint8_t i = 0;i < 2; ++i){
		HIGH(PORT_CONTROL, E);
		_delay_us(DELAY_COM_US);
		LOW(PORT_CONTROL, E);
		_delay_us(DELAY);
	}
	PORT_DATA = 0xC;
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
	_delay_us(DELAY);
	PORT_DATA = 0x4;
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
	_delay_us(DELAY);
	PORT_DATA = 1;
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
}

void LCD_PollBusyFlag()
{
	DDR_DATA = 0;
	PORT_DATA = 1;
	LOW(PORT_CONTROL, RS);
	HIGH(PORT_CONTROL, RW);
	do{
		LOW(PORT_CONTROL, E);
		_delay_us(DELAY_COM_US);
		HIGH(PORT_CONTROL, E);
	}while(PIN_DATA & (1 << 7));
	LOW(PORT_CONTROL, E);
}

void LCD_WriteCommand(uint8_t command)
{
	LCD_PollBusyFlag();
	PORT_CONTROL &= ~((1 << RS)|(1 << RW)|(1 << E));
	DDR_DATA = 0xFF;
	PORT_DATA = command;
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
}

void LCD_clear()
{
	LCD_PollBusyFlag();
	PORT_CONTROL &= ~((1 << RS)|(1 << RW)|(1 << E));
	DDR_DATA = 0xFF;
	PORT_DATA = 1;
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
}

void LCD_EnabledCursor()
{
	LCD_PollBusyFlag();
	PORT_CONTROL &= ~((1 << RS)|(1 << RW)|(1 << E));
	DDR_DATA = 0xFF;
	PORT_DATA = (1 << 3)|(1 << 2);
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
}

void LCD_DisabledCursor()
{
	LCD_PollBusyFlag();
	PORT_CONTROL &= ~((1 << RS)|(1 << RW)|(1 << E));
	DDR_DATA = 0xFF;
	PORT_DATA = (1 << 3);
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);	
}

void LCD_writeSymbol(uint8_t symbol)
{
	LCD_PollBusyFlag();
	HIGH(PORT_CONTROL, RS);
	LOW(PORT_CONTROL, RW);
	DDR_DATA = 0xFF;
	PORT_DATA = symbol;
	HIGH(PORT_CONTROL, E);
	_delay_us(DELAY_COM_US);
	LOW(PORT_CONTROL, E);
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

void LCD_WriteStrPROGMEM(volatile char *str)
{
	for(int i = 0;i < 255; ++i){
		if(pgm_read_byte(&(str[i])) == '\0') return;
		LCD_writeSymbol(pgm_read_byte(&(str[i])));
	}
}

void LCD_clearRow()
{
	for(int i = 0;i < 8; ++i){
		LCD_writeSymbol(' ');
	}
}
