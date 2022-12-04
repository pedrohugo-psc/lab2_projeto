#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "deps/io.h"
#include "deps/functions.h"

void initialConfig();
void readInputs();
void lcdPrint();

#define VOLTAGE_PIN PORTC0	//LM35
#define LUMINOSITY_PIN PORTC1	//UVM30A	
#define CURRENT_PIN PORTC2		//HIH-4000
#define TEMPERATURE_PIN PORTC3
#define LED_PIN PORTD0

int main(void){
	initialConfig();	// configura os terminais
	setADC();			// configura o ADC
	setTimer();			// configura o TIMER
	setINT0();			// configura a Interrupção externa (INT0)
	uart_initialization(9600);
	
	while(1){
		if(printFlag) {
			printFlag = 0;
			voltage = convertVoltage(readADC(VOLTAGE_PIN));	// lê o valor de temperatura
			current = convertCurrent(readADC(CURRENT_PIN));			// lê o valor de umidade
			luminosity = convertLuminosity(readADC(LUMINOSITY_PIN));			// lê o valor de índice UV
			temperature = convertTemperature(readADC(TEMPERATURE_PIN));
			control = receive_data();
			if (control && systemUp && luminosity <= 1.0 && current <= 2.0 && voltage <= 2.0 && temperature <= 55.0) { 
				PORTB |= (1<<5);
			} else {
				PORTB &= ~(1<<5);
			}
		}
	}
}

// interrupções
ISR(TIMER1_OVF_vect){
	resetTimer();		// reinicia o TIMER1
	printFlag = 1;
}

ISR(INT0_vect){
	systemOn();
}