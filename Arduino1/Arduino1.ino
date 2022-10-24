// Arduino 1

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t estado_botao = 0;
uint16_t pot = 0;

void interrupcao_botao(){
  EICRA = 0b00000010; // interrupção externa INT0 na borada de descida
  EIMSK = 0b00000001; // habilita interrupção externa INT0
}

ISR(INT0_vect){
  
  estado_botao ^= PIND2;

  if(estado_botao == 2){
    PORTB |= (1<<0);
  }else{
    PORTB &= ~(1<<0);  //desligando o led
  }
}

int main(){
    Serial.begin(9600);
  	DDRC = 0b000000; // Port C como entrada
    DDRD = 0b00000000;
	  DDRB = 0b100001;
  	PORTB = 0b000000;
    PORTD = 0b00000000;
    PORTC = 0b000000;
    interrupcao_botao();
    sei();
    while(1){
        if(Serial.available()){
          pot = Serial.read();
          Serial.println(pot);          
        }
    }
}