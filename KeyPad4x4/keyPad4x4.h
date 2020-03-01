/*
 * keypad4x4.h
 *
 * Created: 01.03.2020 
 *  Author: Serhii Senyk
 */ 


#ifndef KEYPAD4X4_H_
#define KEYPAD4X4_H_
#include <avr/io.h>
#include <util/delay.h>
#define DDR_KEYPAD  DDRK
#define PORT_KEYPAD PORTK
#define PIN_KEYPAD  PINK

#define KEY_1 0xEE
#define KEY_2 0xDE
#define KEY_3 0xBE
#define KEY_A 0x7E

#define KEY_4 0xED
#define KEY_5 0xDD
#define KEY_6 0xBD
#define KEY_B 0x7D

#define KEY_7 0xEB
#define KEY_8 0xDB
#define KEY_9 0xBB
#define KEY_C 0x7B

#define KEY_F 0xE7
#define KEY_0 0xD7
#define KEY_E 0xB7
#define KEY_D 0x77

void SetupKeyPad();
uint8_t isButtonPressed();
char readKeyFromPad4x4();

#endif /* KEYPAD4X4_H_ */