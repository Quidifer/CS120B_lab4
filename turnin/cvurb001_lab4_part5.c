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

#define LOCKED 0x00
#define UNLOCKED 0x01

#define A7 0x80

#define POUND 0x04
#define Y 0x02
#define X 0x01

void tick();

enum states {locked, sequence_hold, sequence_rel, unlocked};
enum states state = locked;

const unsigned char sequence_array[4] = {(const unsigned char)POUND, (const unsigned char)X, (const unsigned char)Y, (const unsigned char)X};
unsigned i = 0;
unsigned sequence_size = 4;

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //inputs
    DDRB = 0xFF; PORTB = 0x00; //outputs
    DDRC = 0xFF; PORTC = 0x00; //output for debugging

    PORTB = 0x00;
    PORTC = 0x00; //for debuging
    state = locked;
    i = 0;
    while (1) {
        tick();
    }
    return 1;
}

void tick() {
    switch (state) { //transitions
        case locked: 
            if (PINA == sequence_array[i]) state = sequence_hold;
            else state = locked;
            break;

        case sequence_hold: 
            if (PINA == sequence_array[i]) state = sequence_hold;
            else if (PINA == 0x00) {
                if (i == sequence_size - 1) {
                    state = unlocked;
                    i = 0;
                }
                else {
                    state = sequence_rel;
                    ++i;
                }
            }
            else state = locked;
            break;

        case sequence_rel:
            if (PINA == sequence_array[i]) state = sequence_hold; //pressed next button in sequence
            else if (PINA == 0x00) state = sequence_rel;
            else state = locked;
            break;

        case unlocked:
            if (PINA == A7) state = locked;
            else if (PINA == 0x00) state = unlocked;
            else state = unlocked;
            break;
    }

    PORTC = state;

    switch (state) { //actions
        case locked: PORTB = LOCKED; break;
        case sequence_hold: PORTB = LOCKED; break;
        case sequence_rel: PORTB = LOCKED; break;
        case unlocked: PORTB = UNLOCKED; break;
    }
}
