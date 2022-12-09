#include <msp430.h> 

/**
 * main.c
 */

unsigned int ADC_Value;
unsigned int Channel_Select;
unsigned int bright_threshold;
unsigned int brightness;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //initialize P6.6 (LED2) as output
    P6DIR |= BIT6;

    //initialize P1.0 and P1.1 as analog inputs
    P1SEL0 |= BIT0 | BIT1;
    P1SEL1 |= BIT0 | BIT1;
    P1DIR &= ~(BIT0 | BIT1);

    PM5CTL0 &= ~LOCKLPM5;

    //configure ADC
    //830uV resolution
    ADCCTL0 &= ~ADCSHT;     //clear ADCSHT
    ADCCTL0 |= ADCSHT_2;    //conversion cycles = 16
    ADCCTL0 |= ADCON;       //ADC on

    ADCCTL1 |= ADCSSEL_2;   //clock source = SMCLK
    ADCCTL1 |= ADCSHP;      //sample signal source = sampling timer
    ADCCTL1 |= ADCCONSEQ_3; //repeat sequence of channels

    ADCCTL2 &= ~ADCRES;     //clear ADCRES
    ADCCTL2 |= ADCRES_2;    //12-bit resolution

    ADCMCTL0 |= ADCINCH_1;   //ADC input channel A1 (P1.1) and A0 (P1.0)
    ADCMCTL0 |= ADCSREF_0;   //V+ = 3.4V, V- = 0

    ADCIE |= ADCIE0;         //Enable ADC conversion complete IRQ
    __enable_interrupt();    //Enable maskable IRQs

    while(1){
        ADCCTL0 |= ADCENC | ADCSC;              //start ADC
        __bis_SR_register(GIE | LPM0_bits);     //CPU is turned off, interrupt is enabled
    }
    return 0;
}

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void){

    ADC_Value = ADCMEM0;                    //read ADC
    __bic_SR_register_on_exit(LPM0_bits);   //Wake up CPU

    Channel_Select = ADCMCTL0 & ADCINCH;
    if (Channel_Select == 1){   //Potentiometer
        bright_threshold = ADC_Value;
    }
    else if(Channel_Select == 0){ //Light Sensor
        brightness = ADC_Value;
        if(ADC_Value < bright_threshold){
            P6OUT |= BIT6;
        }
        else{
            P6OUT &= ~BIT6;
        }
    }
}
