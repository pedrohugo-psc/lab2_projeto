/*
 * sismicro_2.c
 *
 * Created: 24/11/2022 20:36:55
 * Author : pedro
 */ 


#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "library/io.h"
#include "library/functions.h"

uint8_t data;

float datas_recieve;

int main(void)
{
	setINT0();
	uart_initialization(9600);
    
    while (1) 
    {
		
    }
	
}

ISR(INT0_vect){
	systemOn();
	data = estado_botao;
	uart_send_byte(data);
}

ISR(USART_RX_vect){
	datas_recieve = UDR0;
}