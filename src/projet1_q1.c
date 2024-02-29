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
    
    // ADC
    if(openI2CBus("/dev/i2c-1") == -1) {
		return EXIT_FAILURE;
	}
	setI2CSlave(ADS1115_ADDRESS);

	while(1) {
        // Convertir valeur 0-3.3v à 0-255
        int dc = readVoltage(0) / 3.3 * 255;
		printf("%.2f\n", readVoltage(0));
        gpioPWM(LED1, dc); 
	} 

    // Libérer les ressources
    gpioTerminate();

    return 0;
}