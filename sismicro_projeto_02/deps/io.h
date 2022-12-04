#ifndef CUSTOM_IO_H
#define CUSTOM_IO_H

#define INPUT	0
#define OUTPUT	1
#define LOW		0
#define HIGH	1
#define B	1
#define C	2
#define D	3

#define F_CPU 16000000UL

#include <avr/interrupt.h>

uint8_t data = 0;

// configura um PIN como entrada ou saída
void set(uint8_t x, uint8_t number, uint8_t value) {
	switch (x) {
		case (B):
		if (value)	DDRB |=  (1 << number);
		else		DDRB &= ~(1 << number);
		break;
		case (C):
		if (value)	DDRC |=  (1 << number);
		else		DDRC &= ~(1 << number);
		break;
		case (D):
		if (value)	DDRD |=  (1 << number);
		else		DDRD &= ~(1 << number);
		break;
	}

}

// muda o valor do PIN para alto ou baixo
void write(uint8_t x, uint8_t number, uint8_t value) {
	switch (x) {
		case (B):
		if (value)	PORTB |=  (1 << number);
		else		PORTB &= ~(1 << number);
		break;
		case (C):
		if (value)	PORTC |=  (1 << number);
		else		PORTC &= ~(1 << number);
		break;
		case (D):
		if (value)	PORTD |=  (1 << number);
		else		PORTD &= ~(1 << number);
		break;
	}
}

// lê o valor do PIN
uint8_t read(uint8_t pin, uint8_t number) {
	return (pin & (1 << number));
}

// lê o valor do resultado da conversão ADC
uint16_t readADC(uint8_t ADCchannel) {
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);	// seleciona o canal do ADC sem alterar alguns bits
	ADCSRA |= (1<<ADSC);							// modo de conversão simples
	while( ADCSRA & (1<<ADSC) );					// espera a conversão ser concluída
	return ADC;
}

// configura o ADC
void setADC() {
	ADMUX |= (1<<REFS0);									// 10-bits // AVcc com capacitor externo no pin AREF
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);	// pré-escalar de 128 e habilita o ADC
}

// configura o TIMER1
void setTimer(){
	TCCR1A = 0;							// operação normal pinos OC1A e OC1B desconectados
	TCCR1B = 0;							// limpa o registrador
	TCCR1B |= (1<<CS10)|(1 << CS12);	// configura prescaler para 1024: CS12 = 1 e CS10 = 1
	TCNT1 = 0xC2F7;						// incia timer com valor para que estouro ocorra em 1 segundo // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
	TIMSK1 |= (1 << TOIE1);				// habilita a interrupção do TIMER1
	sei();								// habilita interrupções globais
}

// reinicia o contador do TIMER1 para o valor desejado
void resetTimer() {
	TCNT1 = 0xC2F7;
}

// configura a Interrupção Externa (INT0)
void setINT0(){
	EICRA = 0b00000010; // interrupção externa INT0 na borada de descida
	EIMSK = 0b00000001; // habilita interrupção externa INT0
	sei();
}

void uart_initialization(uint32_t baud){
	uint8_t speed = 16;
	
	baud = (F_CPU/(speed * baud)) - 1;
	
	UBRR0H = (baud) >> 8;
	UBRR0L = baud;
	
	UCSR0B |= (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	sei();
}

ISR(USART_RX_vect){
	data = UDR0;
}

#endif