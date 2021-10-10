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

#define A7 0x80

void tick();

enum states { start, pound, pound_rel, y, complete, pound_, pound_rel_, y_};
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
        case pound: // hold #
            if (PINA == POUND) state = pound;
            else if (PINA == 0x00) state = pound_rel;
            else state = start;
            break;
        case pound_rel: // release #
            if (PINA == 0x00) state = pound_rel;
            else if (PINA == Y) state = y;
            else state = start;
            break;
        case y: // hold Y
            if (PINA == Y) state = y;
            else state = complete;
            break;
        case complete: //release Y
            if (PINA == 0x00) state = complete;
            else if (PINA == POUND) state = pound_;
            else if (PINA == A7) state = start;
            break;
        case pound_: //hold #
            if (PINA == POUND) state = pound_;
            else if (PINA == 0x00) state = pound_rel_;
            else if (PINA == A7) state = start;
            else state = complete;
            break;
        case pound_rel_: //release #
            if (PINA == 0x00) state = pound_rel_;
            else if (PINA == Y) state = y_;
            else if (PINA == A7) state = start;
            else state = complete;
            break;
        case y_: //hold Y
            if (PINA == Y) state = y_;
            else if (PINA == 0x00) state = start;
            else if (PINA == A7) state = start;
            break;

    }

    PORTC = state;

    switch (state) { //actions
        case start: PORTB = LOCKED; break;
        case pound: PORTB = LOCKED; break;
        case pound_rel: PORTB = LOCKED; break;
        case y: PORTB = UNLOCKED; break;
        case complete: PORTB = UNLOCKED; break;
        case pound_: PORTB = UNLOCKED; break;
        case pound_rel_: PORTB = UNLOCKED; break;
        case y_: PORTB = LOCKED; break;
    }
}
