// C++ code
//
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

ISR(ANALOG_COMP_vect){
  if(ACSR & (1<<ACO)){
    //PORTB |= (1<<PORTB5);
    PORTB &=~ (1<<PORTB5);
  }else{
    //PORTB &=~ (1<<PORTB5);
    PORTB |= (1<<PORTB5);
  }
}

int main(){
    cli();
    DDRB |= (1<<DDB5); // setando o pino 13 digital como saída
    ACSR &=~ (1<<ACD)|(1<<ACBG); 
    ADCSRB |= (1<<ACME);
    ACSR &=~ (1<<ADEN);
    ADMUX &=~ 0x0F;
    ADMUX |= 5;
    ACSR &=~ (1<<ACIS0)|(1<<ACIS1);
    ACSR |= (1<<ACIE);
    sei();
  
  
  while(1){}

}
