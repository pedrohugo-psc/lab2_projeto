// Arduino 1

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t estado_botao = 0;
uint16_t pot = 0;
uint16_t ldr = 0;

void interrupcao_botao(){
  EICRA = 0b00000010; // interrupção externa INT0 na borada de descida
  EIMSK = 0b00000001; // habilita interrupção externa INT0
}

void interrupcao_ldr(){
  ADMUX = 0b01000000; // habilitando a tensão de referência do AVCC = 5v
  ADCSRA = 0b11101111; // bit 7 - aden = habilita o adc; bit 6 - adsc = inicia a conversão do adc; bit 5 - adate = ativa o auto disparo; bit 3 - adie = habilita a interrupção; bit 2 a 0 - adps =  determina a divisão por clock 
  DIDR0 = 0b00000001; // bit 5 - 1 adc = habilita o adc nos pinos analogicos como entrada
}

ISR(INT0_vect){
  
  estado_botao ^= PIND2;

  if(estado_botao == 2){
    interrupcao_ldr();
  }else{
    PORTB &= ~(1<<0);  //desligando o led
    ADCSRA = 0b11100111; // desabilita a interrupção adc
  }
}

ISR(ADC_vect){
    ldr = ADC;
    if(ldr < 550){
      PORTB |= (1<<0); // ligado o led
    }else{
      PORTB &= ~(1<<0); // desligando o led
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
