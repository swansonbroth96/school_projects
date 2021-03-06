/*
 * 
 * *Program Title: Lab 2, Part 1 - L.E.D. Flash Using Timer2
 * 
 * Program File Name: lab2_part1.c
 * 
 * Microprocessors A 17.383
 * 
 * Spring 2019
 * 
 * Eric Swanson, Danny Richter
 * 
 * 2/27/19
 */


#include "xc.h"

//PIC24HJ.... Configuration Bit Settings
//FBS
#pragma config BWRP = WRPROTECT_OFF  // Boot Segment Write Protect (Boot segment may be written)
#pragma config BSS = NO_FLASH        // Boot Segment Program Flash Code Protection (No Boot program flash segment)

// FGS
#pragma config GWRP = OFF           // General Code Segment Write Protect(User program memory is not write-protected)
#pragma config GSS = OFF            // General Segment Code Protection (User program memory is not code=protected)

//FOSCSEL
#pragma config FNOSC = FRC          // Oscillator Mode (Internal Fast RC(FRC))
#pragma config IESO = OFF           // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

//FOSC
#pragma config POSCMD = NONE        // Primary Oscillator Source (Primary Oscillator)
#pragma config OSCIOFNC = ON        // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF        // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSDCMD       // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS512      // Watchdog Timer Postscaler (1:512)
#pragma config WDTPRE = PR128       // WDT prescaler (1:128)
#pragma config WINDIS = OFF         // Watchdog Timer Window (Watchdog Timer in non-window mode)
#pragma FWDTEN = OFF                // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR16        // POR Timer value (16ms)
#pragma config ALTI2C = OFF         // Alternate I2C pins (I2C mapped to SDA1/SCL1 pins)
#pragma config ICS = PGD1           // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF         // JTAG Port Enable (JTAG is disabled)

//Timer2 Interrupt Service Routine. Toggles the state of the LED once TMR2 == PR2, then clears the Timer2 interrupt flag so Timer2 can reset
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    _LATB14 = _LATB14 ^ 1;
    IFS0bits.T2IF = 0;          //Clear Timer2 interrupt flag
}


/**************************************************/
/**
 * Function: main
 * 
 * Description: Flash RB14 on and off using Timer2 (for approximately one second)
 * 
 * Notes:
 * 
 * RB14 -
 * 
 * @return This routine contains an infinte loop
 */

int main(void) {
    
    /******* PIO Config *******/
    //Push Button
    _ODCB15 = 0;        // Disable open drain
    _TRISB15 = 0;       // Config RB14 as output
    _LATB15 = 0;        // RB14 initially low
    
    _ODCB14 = 0;        // Disable open drain
    _TRISB14 = 0;       // Config RB14 as output
    _LATB14 = 0;        // RB14 initially low
    
    _ODCB13 = 0;        // Disables open drain
    _TRISB13 = 1;       // Config RB13 as input
    _LATB13 = 1;        // RB13 initially low
    _CN13PUE =  1;      //Enable Weak-Pullup 
    
    _PCFG11 = 1;        //Disable A2D
    
    /****** Timer2 16-bit Config******/
    //The Fast RC (FRC) internal oscillator runs at a nominal frequency of 7.37 MHz. So, Fcy = 7.37 MHz  
    T2CONbits.TON = 0;          //clear any previous timer activation
    T2CONbits.T32 = 0;          //timer2 is in 16 bit mode
    T2CONbits.TCS = 0;          //Use internal clock. Synching is ignored
    T2CONbits.TGATE = 0;        //Disable gated timer accumulation
    T2CONbits.TCKPS = 0b11;     //Set prescaler value (PRE) to 256 
    T2CONbits.TON = 1;          //turn on Timer2
    TMR2 = 0;                //Clear 16 bit timer 2

    
    //Need to calculate PR2
    //Fcy = 3.685 MHz, Tinterrupt = 1 s, PRE = 256
    //PR2 = ( Tinterrupt* Fcy / PRE ) - 1 = (1)*(3.685 MHZ)/256 - 1 = 14395
    PR2 = 14395;        //PR2 value loaded, as calculated above 
   
    IEC0bits.T2IE = 1;          //enable interrupt
    IPC1bits.T2IP = 0x01;      //Set Timer2 interrupt priority level
    IFS0bits.T2IF = 0;          // Clear Timer2 Interrupt Flag
    
    
    
    while(1)
    {
        //do the thing
    }
           

    return 0;
}


