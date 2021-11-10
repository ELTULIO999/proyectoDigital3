/*
Programa para proyecto final de curso
Electónica Digital III
Dr. Luis Alberto Rivera

Estudiante: Julio E. Lopez
Carné: 18211



 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/timerfd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>
//*********************************DEFINICIÓN DE CONSTANTES*******************************
#define ADC_CHANNEL      	0

//*****************************DEFINICIÓN DE VARIABLES GLOBALES***************************

//*********************************PROTOTIPOS DE FUNCIONES********************************
uint16_t get_ADC(int channel);
//*****************************DEFINICIÓN DE VARIABLES GLOBALES***************************

int main(void){
	struct sched_param param;
	param.sched_priority = PRIORIDAD_ADCySPI;
	if (sched_setscheduler(0,SCHED_FIFO,&param) == -1){
		perror("sched_setscheduler falló, thread ADC y SPI\n");
		exit(20);}

	
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) {printf("wiringPiSPISetup falló.\n");}
	int timer_fd_ADC = timer_config(PERIODO_ADCySPI, INIT);

	while(1){
		ADCvalue = get_ADC(ADC_CHANNEL);			//Obtención del valor del ADC por medio de SPI
		wait_period(timer_fd_ADC);
	}
	
    return(0);
}


//******************************************FUNCIONES*************************************

uint16_t get_ADC(int ADC_chan)
{									// Función de obtención de valor de ADC por comunicación SPi
	uint8_t spiData[2];	
	uint16_t resultado;

	
	if((ADC_chan < 0) || (ADC_chan > 1))
		ADC_chan = 0;

	
	spiData[0] = 0b01101000 | (ADC_chan << 4);  
												
	spiData[1] = 0;	// "Don't care", este valor no importa.

	wiringPiSPIDataRW(SPI_CHANNEL, spiData, 2);	

	resultado = (spiData[0] << 8) | spiData[1];

	return(resultado);
}


