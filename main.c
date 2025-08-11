#include <xc.h>
#include "CONFIG.h"
#include "Init.h"
#include "ServoFunctions.h"

#define _XTAL_FREQ 8000000  // 8 MHz crystal frequency

int modeButton = 0;
int pwm_count = 0;
int highTimeThetaMicro = 0; 
int highTimePhiMicro = 0; 


void displayOLED(int i, int j) {
    int servoThetaPosition = 0, servoPhiPosition = 0;
    getServoAngles(i, j, &servoThetaPosition, &servoPhiPosition, 0);
    unsigned char panHex = convertPanAngle(servoThetaPosition);
    unsigned char tiltHex  = convertTiltAngle(servoPhiPosition); 
    unsigned char combinedValue = (unsigned char)((panHex & 0x0F) << 4) | (tiltHex & 0x0F);
    UART_Transmit(combinedValue); //combine 4 bit values to 1 byte
}

void __interrupt(high_priority) TMR2_ISR(void)
{
    if(PIR1bits.TMR2IF){
        pwm_count++;
        if(pwm_count < highTimeThetaMicro) LATCbits.LATC2 = 1; else LATCbits.LATC2 = 0; //RC2 pan
        if(pwm_count < highTimePhiMicro) LATCbits.LATC1 = 1; else LATCbits.LATC1 = 0; //RC1 tilt
        
        if(pwm_count >= 670) {
            pwm_count = 0;
        }
        PIR1bits.TMR2IF = 0;
    }
}

void main() {
    OSCCONbits.IRCF = 0b111;  // Set internal oscillator to 500 kHz
    OSCCONbits.SCS = 0b11;
    UART_Init();
    init_timer2();
    TRISCbits.RC5 = 1; //ebutton
    TRISCbits.RC1 = 0; //servo1
    TRISCbits.RC2 = 0; //servo2
    TRISB = 0b00011111; // RB7 = output, RB4-RB0 = input for joystick
    TRISD = 0b11110010; //RD4 - RD7 input for sensors
    int i = 0, j = 0; //servo increment values
    int sensorUp, sensorDown, sensorLeft, sensorRight, sensorTriggered = 0; //ultrasonic sensor vars
    int OLEDcount = 0;
    //int autoCount = 0;
    while (1) {
        sensorUp = PORTDbits.RD6;
        sensorDown = PORTDbits.RD4;
        sensorLeft = PORTDbits.RD5;
        sensorRight = PORTDbits.RD7;
        if(sensorUp && sensorDown && sensorLeft && sensorRight) {sensorTriggered++;}
        /*autoCount++;
        if(autoCount == 10000) {
            if(!sensorUp && i < (ROWS/2 - 1)) { i--; }   //move down to put sensorUp on target
            if(!sensorDown && i > 0) { i++; }            //move up to put sensorDown on target
            if(!sensorLeft && j < (COLS/2 - 1)) { j++; } //move right to put sensorLeft on target
            if(!sensorRight && j > 0) { j--; }           //move left to put sensorRight on target
            setServoHighTime(i, ++j, 1, &highTimeThetaMicro, &highTimePhiMicro);
            autoCount = 0;
        }*/
        //============================test1 code w/o lock mode=====================================
        /*if(PORTDbits.RD1 == 1) { //auto mode
            if(i >= ROWS-1) i = 0;
            if(j >= COLS-1) {
                j = 0;
                i++;
            }
            setServoHighTime(i, ++j, 1, &highTimeThetaMicro, &highTimePhiMicro);
            if(sensorUp && sensorDown && sensorLeft && sensorRight) { //target acquired
                LATDbits.LATD3 = 1; //fire laser
            } else {
                LATDbits.LATD3 = 0;
            }     
        } else { //manual mode
            if(i > ROWS/2) i = ROWS - i;
            if(PORTBbits.RB0 == 1 && i < (ROWS/2 - 1)) { i++; } //joystick command up
            if(PORTBbits.RB1 == 1 && i > 0) { i--; }            //joystick command down
            if(PORTBbits.RB2 == 1 && j > 0) { j--; }            //joystick command left
            if(PORTBbits.RB3 == 1 && i < (COLS/2 - 1)) { j++; } //joystick command right
            setServoHighTime(i, j, 0, &highTimeThetaMicro, &highTimePhiMicro);
            if(PORTBbits.RB4 == 1) { //center joystick pushed
                LATDbits.LATD3 = 1; //fire laser
            } else {
                LATDbits.LATD3 = 0;
            }
        }
        OLEDcount++;
        if (OLEDcount % 10 == 0) {
            displayOLED(i, j);
            OLEDcount = 0;
        }*/
        
        
        
        //============================test 2 sensors driving servos=============================================
        /*if(i > ROWS/2) i = ROWS - i;
        if(sensorUp && i < (ROWS/2 - 1)) { i++; } //joystick command up
        if(sensorDown && i > 0) { i--; }            //joystick command down
        if(sensorLeft && j > 0) { j--; }            //joystick command left
        if(sensorRight && i < (COLS/2 - 1)) { j++; } //joystick command right
        setServoHighTime(i, j, 0, &highTimeThetaMicro, &highTimePhiMicro);*/
        
        //=====================================test 3 no scan mode==============================================
        
        /*if(PORTDbits.RD1 == 1) { //auto mode
            if(!sensorUp && i < (ROWS/2 - 1)) { i--; }   //move down to put sensorUp on target
            if(!sensorDown && i > 0) { i++; }            //move up to put sensorDown on target
            if(!sensorLeft && j < (COLS/2 - 1)) { j++; } //move right to put sensorLeft on target
            if(!sensorRight && j > 0) { j--; }           //move left to put sensorRight on target
            setServoHighTime(i, ++j, 1, &highTimeThetaMicro, &highTimePhiMicro);

            if(sensorUp && sensorDown && sensorLeft && sensorRight) { //target acquired
                LATDbits.LATD3 = 1; //fire laser
            } else {
                LATDbits.LATD3 = 0;
            }
                
            if(PORTCbits.RC5 == 1) { //ebutton
                sensorTriggered = 0; //lock off of object
                //__delay_ms(3000); //wait 3 sec then scan again
            }
            
        } else { //manual mode
            if(i > ROWS/2) i = ROWS - i;
            if(PORTBbits.RB0 == 1 && i < (ROWS/2 - 1)) { i++; } //joystick command up
            if(PORTBbits.RB1 == 1 && i > 0) { i--; }            //joystick command down
            if(PORTBbits.RB2 == 1 && j > 0) { j--; }            //joystick command left
            if(PORTBbits.RB3 == 1 && i < (COLS/2 - 1)) { j++; } //joystick command right
            setServoHighTime(i, j, 0, &highTimeThetaMicro, &highTimePhiMicro);
            if(PORTBbits.RB4 == 1) { //center joystick pushed
                LATDbits.LATD3 = 1; //fire laser
            } else {
                LATDbits.LATD3 = 0;
            }
        }
        OLEDcount++;
        if (OLEDcount % 10 == 0) {
            displayOLED(i, j);
            OLEDcount = 0;
        } */
        
        //=====================================main program================================================
        if(PORTDbits.RD1 == 1) { //auto mode
            if(sensorTriggered == 0) { //no object yet found, scan mode
                if(i >= ROWS-1) i = 0;
                if(j >= COLS-1) {
                    j = 0;
                    i++;
                }
                setServoHighTime(i, ++j, 1, &highTimeThetaMicro, &highTimePhiMicro);
                if(sensorUp && sensorDown && sensorLeft && sensorRight) { //target acquired
                    LATDbits.LATD3 = 1; //fire laser
                } else {
                    LATDbits.LATD3 = 0;
                }
            } else { //sensor has been triggered, lock on mode
                if(i > ROWS/2) i = ROWS - i;
                if(sensorUp && i < (ROWS/2 - 1)) { i++; } //joystick command up
                if(sensorDown && i > 0) { i--; }            //joystick command down
                if(sensorLeft && j > 0) { j--; }            //joystick command left
                if(sensorRight && i < (COLS/2 - 1)) { j++; } //joystick command right
                setServoHighTime(i, j, 0, &highTimeThetaMicro, &highTimePhiMicro);
                
                if(sensorUp && sensorDown && sensorLeft && sensorRight) { //target acquired
                    LATDbits.LATD3 = 1; //fire laser
                } else {
                    LATDbits.LATD3 = 0;
                }
                
                if(PORTCbits.RC5 == 1) { //ebutton
                    sensorTriggered = 0; //lock off of object
                    __delay_ms(3000); //wait 3 sec then scan again
                }
            } //sensor trig
        } else { //manual mode
            if(i > ROWS/2) i = ROWS - i;
            if(PORTBbits.RB0 == 1 && i < (ROWS/2 - 1)) { i++; } //joystick command up
            if(PORTBbits.RB1 == 1 && i > 0) { i--; }            //joystick command down
            if(PORTBbits.RB2 == 1 && j > 0) { j--; }            //joystick command left
            if(PORTBbits.RB3 == 1 && i < (COLS/2 - 1)) { j++; } //joystick command right
            setServoHighTime(i, j, 0, &highTimeThetaMicro, &highTimePhiMicro);
            if(PORTBbits.RB4 == 1) { //center joystick pushed
                LATDbits.LATD3 = 1; //fire laser
            } else {
                LATDbits.LATD3 = 0;
            }
        }
        OLEDcount++;
        if (OLEDcount % 10 == 0) {
            displayOLED(i, j);
            OLEDcount = 0;
        } 
        
        
        //================================= ebutton test ========================================================
        //if(PORTCbits.RC5 == 1) LATDbits.LATD3 = 1; else LATDbits.LATD3 = 0;
        
    } //while loop
} //main
