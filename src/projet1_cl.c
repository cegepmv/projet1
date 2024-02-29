#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "../include/ads1115_rpi.h"

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

    // Connexion
    int sock = 0;
    struct sockaddr_in dest_addr;
    
    // Créer le socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // Initialiser la struct de l'adresse IP 
    memset(&dest_addr, '0', sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr("10.10.21.9");
    dest_addr.sin_port = htons(9090);

    // Créer la connexion
    connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    
    // Envoyer le message 
    int dc;
	while(1) {
        // Convertir valeur 0-3.3v à 0-255
        //dc = readVoltage(0) / 3.3 * 255;
        dc = 220;
		printf("%d\n", dc);
        send(sock, &dc, sizeof(dc), 0);
	} 

    // Libérer les ressources
    gpioTerminate();

    return 0;
}