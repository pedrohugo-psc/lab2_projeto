#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t ldr = 0;
uint16_t estado = 0;

void interrupcao_botao(){
  EICRA = 0b00000010; // interrupção externa INT0 na borada de descida
  EIMSK = 0b00000001; // habilita interrupção externa INT0
}

void interrupcao_ldr(){
  ADMUX = 0b01000000; // habilitando a tensão de referência do AVCC = 5v
  ADCSRA = 0b11101111; // bit 7 - aden = habilita o adc; bit 6 - adsc = inicia a conversão do adc; bit 5 - adate = ativa o auto disparo; bit 3 - adie = habilita a interrupção; bit 2 a 0 - adps =  determina a divisão por clock 
  DIDR0 = 0b00000001; // bit 5 - 1 adc = habilita o adc nos pinos analogicos como entrada
}

void interrupcao_comparador_analogico(){
   ACSR &=~ (1<<ACD)|(1<<ACBG); 
   ADCSRB |= (1<<ACME);
   ADCSRA &=~ (1<<ADEN);
   ADMUX &=~ 0x0F;
   ADMUX |= 5;
   ACSR &=~ (1<<ACIS0)|(1<<ACIS1);
   ACSR |= (1<<ACIE);
   DIDR1 = 0b10000101;
}

ISR(INT0_vect){
  if(estado == 0){
    PORTB ^= (1<<0);
    if(PINB == (0<<0)){
      ACSR |= (0<<ACIE);
      ADCSRB |= (0<<ACME);
      ACSR |= (1<<ACD);
      interrupcao_ldr();
      sei();
    }else{
      ADCSRA = 0b11100111; // desabilita a interrupção adc
      interrupcao_comparador_analogico();
    }
  }
}

ISR(ADC_vect){
    ldr = ADC;
    if(estado == 0){
      if(ldr < 550){
        PORTB |= (1<<0);
        interrupcao_comparador_analogico();
      }else{
        PORTB &= ~(1<<0);
        ACSR |= (0<<ACIE);
        ADCSRB |= (0<<ACME);
        ACSR |= (1<<ACD);
      }
    } 
}

ISR(ANALOG_COMP_vect){
  if(ACSR & (1<<ACO)){
    estado = 0;
    PORTB &=~ (1<<5);
  }else{
    estado = 1;
    PORTB |= (1<<5);
    PORTB &= ~(1<<0);
  }
}

int main(){
    Serial.begin(9600);
    DDRC = 0b000000;
    DDRD = 0b00001000;
    DDRB = 0b100001;
    PORTB = 0b000000;
    PORTD = 0b00000000;
    PORTC = 0b000000;
    interrupcao_comparador_analogico();
    interrupcao_botao();
    sei();
    while(1){
    Serial.println(ldr);
  }
}
