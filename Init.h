#ifndef Init
#define	Init

#include <xc.h> 

#define _XTAL_FREQ 8000000
#define BAUDRATE 9600

void UART_Init(void) {
    TRISC6 = 0; // TX output
    TRISC7 = 1; // RX input
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 12; // 9600 baud @ 8MHz
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
}

void init_timer2(void)
{
    T2CONbits.T2CKPS = 0b00;   //Enable Timer2
    PR2 = 9;
    TMR2 = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    INTCONbits.GIEH = 1;    //Enable global interrupts (High Priority)  
    INTCONbits.PEIE = 1;  
    T2CONbits.TMR2ON = 1;
}

void UART_Transmit(unsigned char data) {
    while (!TXSTAbits.TRMT);
    TXREG = data;  // Transmit the combined byte.
}

unsigned char convertPanAngle(int angle) {
    // Clamp the angle to [0, 180]
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    angle /= 18;
	angle = (angle % 2) ? ((angle+1)*18) : (angle*18); //clamp to 0 36 72 108 144 180
    return (unsigned char) angle / 36 + 10; //return corresponding hex value
}

unsigned char convertTiltAngle(int angle) {
    // Clamp the angle to [0, 90]
    if (angle < 0) angle = 0;
    if (angle > 90) angle = 90;
    angle /= 9;
	angle = (angle % 2) ? ((angle+1)*9) : (angle*9); //clamp to 0 36 72 108 144 180
    return (unsigned char) angle / 18; //return corresponding hex value
}

#endif

