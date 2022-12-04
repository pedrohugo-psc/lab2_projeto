#ifndef CUSTOM_FUNCTIONS_H
#define CUSTOM_FUNCTIONS_H

#include "io.h"

#define CACHE_SIZE 10

uint8_t printFlag = 0;
float voltage = 0;
float current = 0;
float temperature = 0;
float luminosity = 0;
uint8_t control = 0;

uint8_t systemUp = 0;
uint8_t buttonStillPressed = 0;
uint16_t estado_botao = 0;

// configura os terminais na inicializacao
void initialConfig() {
	//	DDR		PORT
	//	 0		 0		input => 5V: PIN = 1; 0V: PIN = 0; (pull-up desligado)
	//	 0		 1		input => 5V: PIN = 0; 0V: PIN = 1; (pull-up ligado)
	//	 1		 0		output => PIN = 0 (0V);
	//	 1		 1		output => PIN = 1 (5V);
	
	set(B,5,OUTPUT);
	write(B,5,LOW);
	
	set(D,2,INPUT);
	write(D,2,LOW);
}

void systemOn(){
	estado_botao ^= PIND2;
	if (estado_botao) {
		if (buttonStillPressed == 0) {
			systemUp = (1-systemUp);
		}
		buttonStillPressed = 1;
	}
	else {
		buttonStillPressed = 0;
	}
}

// realiza a proporcao entre duas escalas
int32_t scale(uint16_t value, float scaleStart, float scaleEnd) {
	return (((float)value * (scaleEnd - scaleStart))/1023.0 + scaleStart);
}

float getVoltage(uint16_t readValue) {
	return 5.0 * (float) readValue / 1023.0;
}

// converte o sinal de tensao para o valor de volts
float convertVoltage(uint16_t readValue) {
	return getVoltage(readValue);
}

// converte o sinal de tensao para o valor de corrente
float convertCurrent(uint16_t readValue) {
	return getVoltage(readValue);
}

float convertTemperature(uint16_t readValue) { // temperatura vai de -55 a 150
	return scale(readValue, -55, 150);
}

// converte o sinal de tensao para o valor de indice UV
float convertLuminosity(uint16_t readValue) {
	return getVoltage(readValue);
}

uint8_t receive_data(){
	return data;
}

#endif