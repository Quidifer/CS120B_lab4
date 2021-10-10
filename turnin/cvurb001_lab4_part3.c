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

#define POUND 0x04
#define Y 0x02
#define X 0x01

#define LOCKED 0x00
#define UNLOCKED 0x01

void tick();

enum states { start, pound, pound_rel, pound_y, complete};
enum states state = start;

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //inputs
    DDRB = 0xFF; PORTB = 0x00; //outputs
    DDRC = 0xFF; PORTC = 0x00; //output for debugging

    PORTB = 0x00;
    PORTC = 0x00; //for debuging
    state = start;
    while (1) {
        tick();
    }
    return 1;
}

void tick() {
    switch (state) { //transitions
        case start:
            if (PINA == POUND) state = pound;
            else state = start;
            break;
        case pound:
            if (PINA == POUND) state = pound;
            else if (PINA == 0x00) state = pound_rel;
            else state = start;
            break;
        case pound_rel:
            if (PINA == 0x00) state = pound_rel;
            else if (PINA == Y) state = pound_y;
            else state = start;
            break;
        case pound_y:
            if (PINA == Y) state = pound_y;
            else state = complete;
            break;
        case complete:
            if (PINA == 0x00) state = complete;
            else state = start;
            break;
    }

    PORTC = state;

    switch (state) { //actions
        case start: PORTB = LOCKED; break;
        case pound: PORTB = LOCKED; break;
        case pound_rel: PORTB = LOCKED; break;
        case pound_y: PORTB = UNLOCKED; break;
        case complete: PORTB = UNLOCKED; break;
    }
}
