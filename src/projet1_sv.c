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

#define LED1 17

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

    int socket_local, socket_dist;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Créer le socket et initialiser l'adresse
    socket_local = socket(AF_INET, SOCK_STREAM, 0);
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9090);

    // Associer le socket à l'adresse de l'interface
    bind(socket_local, (struct sockaddr *)&address, sizeof(address));
    
    // Attendre une connexion entrante
    listen(socket_local, 3);
    socket_dist = accept(socket_local, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    // Réception des messages
    int dc;
    while(1) {
        int resultat;
        // Stocker le message
        resultat = recv(socket_dist, &dc, sizeof(dc),0);
      
        if (resultat != -1) {
            // Afficher le message
            printf("Reçu: %d\n", dc);
            gpioPWM(LED1, dc);
            fflush(stdout); 
        }        
    }

    close(socket_dist);
    close(socket_local);
	

    // Libérer les ressources
    gpioTerminate();

    return 0;
}
