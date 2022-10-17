#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint16_t ldr = 0;
uint16_t estado = 0;
uint16_t estado_botao = 0;

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
   ACSR = 0b00001000; // bit 7 - ACD = comparador habilitado, bit 6 - ACBG = AIN0 em positivo, bit 4 - ACI = bit sinalizador de interrupção, bit 3 - ACIE = habilitando a interrupção, bit 1 (ACIS1) e 0 (ACIS0) = Modo de interrupção -> Subida ou descida do sinal de comparação 
   ADCSRB = 0b01000000; // bit 6 - ACME = com o AD desligado o multiplexador do AD seleciona a entrada (-) do comparador. 
   ADCSRA = 0b00000000; // desabilitando o ADC
   ADMUX = 0b00000101; // bit 7 (REFS1) e 6 (REFS0) - AREF, tensão interna V REF desligada. bit 5 - ADLAR = alinhado à direita. bit 3 a 0 - ADMUX = habilitando o ADC5 como entrada
   DIDR1 = 0b00000001; // bit 1 - AIN1D = 0 pinos como I/O. bit 0 - AIN0D = uso do comparador analógico
}

ISR(INT0_vect){
  
  estado_botao ^= PIND2;

  if(estado_botao == 2){
    PORTB |= (1<<0);
    interrupcao_ldr();
    sei();
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

ISR(ANALOG_COMP_vect){
  if(ACSR & (1<<ACO)){
    estado = 0;
    PORTB &=~ (1<<5); // desligando o led interno do arduino 
  }else{
    estado = 1;
    PORTB |= (1<<5); // ligando o led interno do arduino
    PORTB &= ~(1<<0); // desligando o led
  }
}

int main(){
    Serial.begin(9600);
    DDRC = 0b000000; // Port C como entrada
    DDRD = 0b00000000; // Port D como entrada
    DDRB = 0b100001; // Port B - bit 0 = saída, bit 5 = saída
    PORTB = 0b000000;
    PORTD = 0b00000000;
    PORTC = 0b000000;
    //interrupcao_comparador_analogico();
    interrupcao_botao();
    sei();
    while(1){
    Serial.println(ldr);
  }
}
