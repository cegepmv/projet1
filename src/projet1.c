#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "../include/ads1115_rpi.h"

#define LED1 17
#define ADS1115_ADDRESS 0x48 // Adresse I2C du ADS1115

int main() {

    // Initialiser
    if (gpioInitialise() < 0) {
        fprintf(stderr, "Erreur d'initialisation pigpio\n");
        return 1;
    }

    // Test LED
    gpioSetMode(LED1, PI_OUTPUT);
    gpioWrite(LED1, 1);
    usleep(1000000);
    gpioWrite(LED1, 0);
    usleep(1000000);

    // Test PWM LED
    // Envoyer une valeur de "dutycycle" de 0-255
    int cycle;
    for (cycle=0;cycle<256;cycle++) {
        gpioPWM(LED1, cycle); 
        usleep(5000);
    }
    for (;cycle>=0;cycle--) {
        gpioPWM(LED1, cycle); 
        usleep(5000);
    }
    
    // Initialiser ADC
    if(openI2CBus("/dev/i2c-1") == -1) {
		return EXIT_FAILURE;
	}
	setI2CSlave(ADS1115_ADDRESS);

    // Lire les voltages sur A0-A3
	while(1) {
		printf("%.2f | ", readVoltage(0));
        printf("%.2f | ", readVoltage(1));
        printf("%.2f | ", readVoltage(2));
        printf("%.2f\n", readVoltage(3));
	} 

    // Libérer les ressources
    gpioTerminate();

    return 0;
}