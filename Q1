#include <msp430.h> 

/**
 * main.c
 */

int light;
const bright = 3000;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //initialize P1.0 as output
    P1DIR |= BIT0;

    //initialize P1.1 as analog input
    P1SEL0 |= BIT1;
    P1SEL1 |= BIT1;
    P1DIR &= ~BIT1;

    PM5CTL0 &= ~LOCKLPM5;

    //configure ADC
    //830uV resolution
    ADCCTL0 &= ~ADCSHT;     //clear ADCSHT
    ADCCTL0 |= ADCSHT_2;    //conversion cycles = 16
    ADCCTL0 |= ADCON;       //ADC on

    ADCCTL1 |= ADCSSEL_2;   //clock source = SMCLK
    ADCCTL1 |= ADCSHP;      //sample signal source = sampling timer

    ADCCTL2 &= ~ADCRES;     //clear ADCRES
    ADCCTL2 |= ADCRES_2;    //12-bit resolution

    ADCMCTL0 |= ADCINCH_1;   //ADC input channel A1 (P1.1)
    ADCMCTL0 |= ADCSREF_0;   //V+ = 3.4V, V- = 0

    while(1){
        ADCCTL0 |= ADCENC | ADCSC;    //start ADC
        while((ADCIFG & ADCIFG0) == 0);  //wait for conversion to complete
        int light = ADCMEM0;   //Read ADC result

        if (light < bright){
            P1OUT |= BIT0; //LED ON (red)
        }
        else{
            P1OUT &= ~BIT0; //LED OFF
        }
    }


    return 0;
}
