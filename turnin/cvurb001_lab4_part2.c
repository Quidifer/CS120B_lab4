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
#define A1 PINA & 0x02
#define A1A0 PINA & 0x03

void tick();

enum states { reset, increase, increase_hold, decrease, decrease_hold, rest};
enum states state = reset;

int main(void) {
    DDRA = 0x00; PORTA = 0xFF; //inputs
    DDRC = 0xFF; PORTC = 0x00; //outputs

    PORTC = 0x07;
    state = rest;
    while (1) {
        tick();
    }
    return 1;
}

void tick() {
    switch (state) { //transitions
        case reset:
            switch (A1A0) {
                case 0: state = rest; break;
                case 1: state = increase; break;
                case 2: state = decrease; break;
                case 3: state = reset; break;
            }

            break;
        case increase:
            switch (A1A0) {
                case 0: state = rest; break;
                case 1: state = increase_hold; break;
                case 2: state = decrease; break;
                case 3: state = reset; break;
            }

            break;
        case increase_hold:
            switch (A1A0) {
                case 0: state = rest; break;
                case 1: state = increase_hold; break;
                case 2: state = decrease; break;
                case 3: state = reset; break;
            }

            break;
        case decrease:
            switch (A1A0) {
                case 0: state = rest; break;
                case 1: state = increase; break;
                case 2: state = decrease_hold; break;
                case 3: state = reset; break;
            }

            break;
        case decrease_hold:
            switch (A1A0) {
                case 0: state = rest; break;
                case 1: state = increase; break;
                case 2: state = decrease_hold; break;
                case 3: state = reset; break;
            } 
            break;
        case rest: 
            switch (A1A0) {
                case 0: state = rest; break;
                case 1: state = increase; break;
                case 2: state = decrease; break;
                case 3: state = reset; break;
            }
    }

    switch (state) { //actions
        case rest: break;
        case increase: if (PORTC < 9) ++PORTC; break;
        case increase_hold: break;
        case decrease: if (PORTC > 0) --PORTC; break;
        case decrease_hold: break;
        case reset: PORTC = 0x00; break;
    }
}
