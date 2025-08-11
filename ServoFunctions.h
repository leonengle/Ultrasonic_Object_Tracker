#ifndef ServoFunctions
#define	ServoFunctions

#include <xc.h> 

#define _XTAL_FREQ 8000000
#define ROWS 32
#define COLS 36
#define RANGETHETA 180
#define RANGEPHI 80

void getServoAngles(int i, int j, int* servoThetaPosition, int* servoPhiPosition, int scanMode) {
    if(i < ROWS/2) { //counting up
		if(i % 2 == 1 && scanMode == 1) { //if row num is odd & scan mode
			*servoThetaPosition = RANGETHETA - j*(RANGETHETA / COLS);
		} else if(i % 2 == 0 && scanMode == 1) { //if row num is even & auto mode
			*servoThetaPosition = j*(RANGETHETA / COLS); //increase degrees
		} else if(scanMode == 0) { //manual mode
			*servoThetaPosition = j*(RANGETHETA / COLS);
		}
		*servoPhiPosition = i * RANGEPHI / (ROWS/2);
	} else if ((i >= ROWS/2) && (i < ROWS)){ //counting down
		if(i % 2 == 1 && scanMode == 1) { //if row num is odd & scan mode
			*servoThetaPosition = RANGETHETA - j*(RANGETHETA / COLS);
		} else if(i % 2 == 0 && scanMode == 1) { //if row num is even & auto mode
			*servoThetaPosition = j*(RANGETHETA / COLS);
		} else if(scanMode == 0) { //manual mode
			*servoThetaPosition = j*(RANGETHETA / COLS);
		}
		*servoPhiPosition = RANGEPHI - (i-ROWS/2+1) * RANGEPHI / (ROWS/2);
	} else {
        i = 0;
	}
}

void setServoHighTime(int i, int j, int scanMode, int* highTimeThetaMicro, int* highTimePhiMicro) { 
    int servoThetaPosition = 0, servoPhiPosition = 0;
    getServoAngles(i, j, &servoThetaPosition, &servoPhiPosition, scanMode);
    
    float dutyCyclePercentTheta = ((180 - servoThetaPosition)/180.0)*(0.08)+0.035; //3% to 11%
    *highTimeThetaMicro = (int)(0.02 * dutyCyclePercentTheta * 1000000/30.0);
    
    float dutyCyclePercentPhi = ((90-servoPhiPosition)/90.0)*(0.05)+0.04; //5% to 10%
    *highTimePhiMicro = (int)(0.02 * dutyCyclePercentPhi * 1000000/30.0);
}

#endif

