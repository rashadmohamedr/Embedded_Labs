#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON        // Low-Voltage Programming Enable bit (RB3/PGM pin has digital I/O, HV on MCLR must be used for programming)

#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ  4000000UL
#include <xc.h>
#include <stdbool.h>

/////////////////

bool flagBTN = true;
const uint8_t segment_map[10] = {
    0b00111111, // 0
    0b00100001, // 1
    0b01110110, // 2
    0b01110011, // 3
    0b01101001, // 4
    0b01011011, // 5
    0b01011111, // 6
    0b00111001, // 7
    0b01111111, // 8
    0b01111011  // 9
};

void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    
    TRISC = 0b00000000;
    TRISB = 0b10000000;
    
    INTCON |= 1<<7; //Enable the Global interrupt
    
    INTCON |= 1<<3; //Enable the PortB interrupts
    
    IOCB7 = 1;//Enable interrupt on  PortB7
    
    
    while (1) {
//        display_number(25);
        for (uint8_t i = 0; i <= 9; i++) {
            //PORTC = 0b00100000; // Select left digit (C5)
            if(flagBTN){
                PORTC = segment_map[i];
            } else {
                PORTC = 0x00;
            }
            __delay_ms(1000);
        }
    }

    
    return;
}

void __interrupt() ISR(void){
    if (PORTB == 0b10000000) {
        flagBTN = false;
    }else{
        flagBTN = true;
    }
    INTCON &= ~(1 << 0);
}
