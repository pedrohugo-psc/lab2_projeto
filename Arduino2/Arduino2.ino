//Arduino 2

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int regulador_tensao = 0;

void interrupcao_potenciometro(){
  ADMUX = 0b01000000; // habilitando a tensão de referência do AVCC = 5v
  ADCSRA = 0b11101111; // bit 7 - aden = habilita o adc; bit 6 - adsc = inicia a conversão do adc; bit 5 - adate = ativa o auto disparo; bit 3 - adie = habilita a interrupção; bit 2 a 0 - adps =  determina a divisão por clock 
  DIDR0 = 0b00000001; // bit 5 - 1 adc = habilita o adc nos pinos analogicos como entrada
}

ISR(ADC_vect){
    regulador_tensao = ADC;
}


int main(){
  Serial.begin(9600);
  interrupcao_potenciometro();
  sei();
  while(1){
    int val = map(regulador_tensao, 0, 1023, 0, 255);
    Serial.write(val);
  }
  
}
