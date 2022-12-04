#ifndef CUSTOM_IO_H
#define CUSTOM_IO_H

#define F_CPU 16000000UL



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
	
	UCSR0A = 0;
	UCSR0B |= (1 << TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	sei();
}


#endif