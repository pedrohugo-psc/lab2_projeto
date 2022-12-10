#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

#include "io.h"

uint8_t systemUp = 0;
uint8_t buttonStillPressed = 0;
uint8_t estado_botao = 0;

void uart_send_byte(uint8_t data){
	while(!( UCSR0A & (1<<UDRE0)));
	UDR0 = data;	
}

void systemOn(){
	estado_botao ^= PIND2;

	if (estado_botao){
		PORTB |= (1<<5);
	}else{
		PORTB &= ~(1<<5);
	}
}

#endif