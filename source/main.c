/*	Author: lab
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  Compile command: avr-gcc -mmcu=atmega1284 -I/usr/csshare/pkgs/simavr/ -Iheader/ -D_SIMULATE_ -Wall -g -Og -Wl,--undefined=_mmcu,--section-start=.mmcu=910000 -o build/objects/main.elf source/main.c
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#define A0 PINA & 0x01

void ON_PB0() { PORTB = PORTB | 0x01; }
void ON_PB1() { PORTB = PORTB | 0x02; }

void OFF_PB0() { PORTB = PORTB & 0xFE; }
void OFF_PB1() { PORTB = PORTB & 0xFD; }

enum states {start, press_1, rel_1, press_2, rel_2};
enum states state = start;

void tick();

int main(void) {
    DDRB = 0xFF, PORTB = 0x00; //outputs
    DDRA = 0x00, PORTA = 0xFF; //inputs

    PORTB = 0x00;
    state = start;
    while (1) {
        tick();
    }
    return 1;
}

void tick() {
    switch (state) {
        case start:
            ON_PB0();
            if (A0) {
                state = press_1; 
                OFF_PB0(); 
                ON_PB1();
            }
            break;
        case press_1:
            OFF_PB0();
            ON_PB1();
            state = A0 ? press_1 : rel_1;
            break;
        case rel_1:
            OFF_PB0();
            ON_PB1();
            state = A0 ? press_2 : rel_1;
            break;
        case press_2:
            ON_PB0();
            OFF_PB1();
            state = A0 ? press_2 : rel_2;
            break;
        case rel_2:
            ON_PB0();
            OFF_PB1();
            state = A0 ? press_1: rel_2;
            break;
        default:
            PORTB = 0xFF;
            break;
    }
}
