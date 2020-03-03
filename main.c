/*
 * Lab_3_Atmega2560.c
 *
 * Created: 27.02.2020 7:06:56
 * Author : Serhii Senyk
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "LCD_1602.h"
#include "keyPad4x4.h"



PROGMEM const  char hundred[100][3] = {
	{"00"},{"01"},{"02"},{"03"},{"04"},{"05"},{"06"},{"07"},{"08"},{"09"},
	{"10"},{"11"},{"12"},{"13"},{"14"},{"15"},{"16"},{"17"},{"18"},{"19"},
	{"20"},{"21"},{"22"},{"23"},{"24"},{"25"},{"26"},{"27"},{"28"},{"29"},
	{"30"},{"31"},{"32"},{"33"},{"34"},{"35"},{"36"},{"37"},{"38"},{"39"},
	{"40"},{"41"},{"42"},{"43"},{"44"},{"45"},{"46"},{"47"},{"48"},{"49"},
	{"50"},{"51"},{"52"},{"53"},{"54"},{"55"},{"56"},{"57"},{"58"},{"59"},
	{"60"},{"61"},{"62"},{"63"},{"64"},{"65"},{"66"},{"67"},{"68"},{"69"},
	{"70"},{"71"},{"72"},{"73"},{"74"},{"75"},{"76"},{"77"},{"78"},{"79"},
	{"80"},{"81"},{"82"},{"83"},{"84"},{"85"},{"86"},{"87"},{"88"},{"89"},
	{"90"},{"91"},{"92"},{"93"},{"94"},{"95"},{"96"},{"97"},{"98"},{"99"},
		 
};
		
volatile struct Time
{
	int8_t hour, minute, second;
	uint8_t isRunning;
	uint8_t count_stop_beep;
}tmpTimer;

void showOnLCD(const struct Time *timer,const uint8_t enable, const uint8_t y, const uint8_t x);
void startTimer(struct Time *timer, const uint8_t y, const uint8_t x);
void modeSettingTimer(struct Time *timer, const uint8_t countTimer);
static void beep(const int time_in_ms);
struct Time timer_first = {0, 10, 0, 1};
struct Time timer_second = {0, 5, 0, 1};


ISR(TIMER1_COMPA_vect)
{
	if(timer_first.isRunning){
		if(--timer_first.second == -1){
			timer_first.second = 59;
			if(--timer_first.minute == -1){
				timer_first.minute = 59;
				if(--timer_first.hour == -1){
					timer_first.isRunning = 0;
					timer_first.count_stop_beep = 30;
					timer_first.hour = 0;
					timer_first.minute = 0;
					timer_first.second = 0;
				}
			}
		}
	}
	if(timer_second.isRunning){
		if(--timer_second.second == -1){
			timer_second.second = 59;
			if(--timer_second.minute == -1){
				timer_second.minute = 59;
				if(--timer_second.hour == -1){
					timer_second.isRunning = 0;
					timer_second.count_stop_beep = 30;
					timer_second.hour = 0;
					timer_second.minute = 0;
					timer_second.second = 0;
				}
			}
		}
	}
	if(timer_first.count_stop_beep > 0){
		beep(BEEP_SHORT_MS);
		--timer_first.count_stop_beep;
	}
	
	if(timer_second.count_stop_beep > 0){
		beep(BEEP_SHORT_MS);
		--timer_second.count_stop_beep;
	}
	
	showOnLCD(&timer_first, 0, 1,1);
	showOnLCD(&timer_second, 0, 2,1);
	
}


void Settup()
{
	LCD_Settup();
	SetupKeyPad();
	OCR1A = 0xF423;//62499  T/(1/F_CPU)/prescalar - 1	
	TCCR1A = 0x00;//normal mode
	TCCR1B = (1 << WGM12) | (1 << CS12); //CTC mode , clkI/O/256(prescalar)
	TIMSK1 |= (1 << OCIE1A);//interrupts	
}

int main(void)
{
    Settup();
	sei();
    while(1) 
    {
		if(isButtonPressed())
		{
			beep(BEEP_SHORT_MS);
			switch(readKeyFromPad4x4())
			{
				case 'A': modeSettingTimer(&timer_first, 1); break;
				case 'B': modeSettingTimer(&timer_second, 2); break;
				case 'C': startTimer(&timer_first, 1, 1); break;
				case 'D': startTimer(&timer_second, 2, 1); break;
				default:break;
			}
		}	
		
    }
	return 0;
}

void showOnLCD(const struct Time *timer,const uint8_t enable, const uint8_t y, const uint8_t x)
{
	if(timer->isRunning || enable){
		LCD_GotoYX(y, x);
		LCD_WriteStrPROGMEM(hundred[timer->hour]);
		LCD_writeSymbol(':');
		LCD_WriteStrPROGMEM(hundred[timer->minute]);
		if(!enable){
			timer->second % 2 ? LCD_writeSymbol(' '):LCD_writeSymbol(':');
		}
		else
			LCD_writeSymbol(':');
		LCD_WriteStrPROGMEM(hundred[timer->second]);
	}
}


void modeSettingTimer(struct Time *timer, const uint8_t countTimer)
{
	if(timer->isRunning){
		timer->isRunning = 0;
	}
	tmpTimer.hour = timer->hour;
	tmpTimer.minute = timer->minute;
	tmpTimer.second = timer->second;
	timer->hour = 0;
	timer->minute = 0;
	timer->second = 0;
	timer_first.count_stop_beep = 0;
	timer_second.count_stop_beep = 0;
	char key = 0;
	uint8_t sizeDigit = 1;
	uint8_t timeValue = 0;
	uint8_t count = 0;
	uint8_t x_cursor = 7;
	showOnLCD(timer, 1, countTimer, 1);
	LCD_GotoYX(countTimer, 10);
	LCD_WriteStr("seconds");
	while(count < 3)
	{
		if(isButtonPressed())
		{
			beep(BEEP_SHORT_MS);
			key = readKeyFromPad4x4();
			if(isdigit(key))
			{
				if(sizeDigit > 2){
					timeValue = 0;
					sizeDigit = 1;
				}
				timeValue = 10*timeValue + (key - '0');	
				LCD_GotoYX(countTimer, x_cursor);
				LCD_WriteStrPROGMEM(hundred[timeValue]);
				if(timeValue != 0)
					++sizeDigit;
			}
			else if(key == '#')
			{
				if(timeValue > 59 && count < 2){
					HIGH(PORT_BUZZER, BUZZER);
					_delay_ms(500);
					LOW(PORT_BUZZER, BUZZER);
					timeValue = 0;
					sizeDigit = 1;
					LCD_GotoYX(countTimer, x_cursor);
					LCD_WriteStrPROGMEM(hundred[timeValue]);
				}
				else{
					_delay_ms(BEEP_SHORT_MS);
					beep(100);
					if(count == 0){
						timer->second = timeValue;
						LCD_clearRow();
						LCD_GotoYX(countTimer, 10);
						LCD_WriteStr("minutes");
					}
					else if(count == 1){
						timer->minute = timeValue;
						LCD_GotoYX(countTimer, 10);
						LCD_clearRow();
						LCD_GotoYX(countTimer, 10);
						LCD_WriteStr("hours");
					}
					else {
						timer->hour = timeValue;
						LCD_GotoYX(countTimer, 10);
						LCD_clearRow();
					}
					++count;
					x_cursor -= 3;
					timeValue = 0;
					sizeDigit = 1;
				}	
			}
			else if(key == '*')
			{
				HIGH(PORT_BUZZER, BUZZER);
				_delay_ms(500);
				LOW(PORT_BUZZER, BUZZER);
				timer->hour = tmpTimer.hour;
				timer->minute = tmpTimer.minute;
				timer->second = tmpTimer.second;
				timer->isRunning = 0;
				LCD_GotoYX(countTimer, 10);
				LCD_clearRow();
				showOnLCD(timer, 1, countTimer, 1);
				return;
			}
		}
	}
	return;	
}

void startTimer(struct Time *timer, const uint8_t y, const uint8_t x)
{
	_delay_ms(100);
	HIGH(PORT_BUZZER, BUZZER);
	_delay_ms(150);
	LOW(PORT_BUZZER, BUZZER);
	if(!timer->isRunning && (timer->hour != 0 || timer->minute != 0 || timer->second != 0 )){
		timer->isRunning = 1;
	}
	else{
		timer->isRunning = 0;
		timer->hour = 0;
		timer->minute = 0;
		timer->second = 0;
		timer->count_stop_beep = 0;
		showOnLCD(timer, 1, y, x);
	}
}

static void beep(const int time_in_ms)
{
	HIGH(PORT_BUZZER, BUZZER);
	_delay_ms(time_in_ms);
	LOW(PORT_BUZZER, BUZZER);
}
	