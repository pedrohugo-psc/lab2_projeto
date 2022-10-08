#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t ldr = 0;

void interrupcao_botao(){
    EICRA = 0b00000010; // interrupção externa INT0 na borada de descida
    EIMSK = 0b00000001; // habilita interrupção externa INT0
}

void interrupcao_ldr(){
    ADMUX = 0b01000000; // habilitando a tensão de referência do AVCC = 5v
    ADCSRA = 0b11100111; // bit 7 - aden = habilita o adc; bit 6 - adsc = inicia a conversão do adc; bit 5 - adate = ativa o auto disparo; bit 3 - adie = habilita a interrupção; bit 2 a 0 - adps =  determina a divisão por clock 
    DIDR0 = 0b00111110; // bit 5 - 1 adc = desabilita o adc nos pinos analogicos
}

ISR(INT0_vect){
  PORTD ^= (1<<3);
  if(PIND == (0<<3)){
    ADCSRA = 0b11101111; // habilita a interrupção
  }else{
    ADCSRA = 0b11100111; // desabilita a interrupção
  }
}

ISR(ADC_vect){
    ldr = ADC;
    if(ldr < 550){
      PORTD |= (1<<3);
    }else{
      PORTD &= ~(1<<3);
    }
}

int main(){
    Serial.begin(9600);
    DDRC = 0b000000;
    DDRD = 0b00001000;
    PORTB = 0b000010;
    PORTD = 0b00000000;
    PORTC = 0b000000;
    interrupcao_ldr();
    interrupcao_botao();
    sei();
  while(1){
    Serial.println(ldr);
  }
}
