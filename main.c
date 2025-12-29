#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define el pin del LED
#define LED_PIN 16

int main() {
    // Inicializar la biblioteca estándar (para printf, etc.)
    stdio_init_all();
    
    // Inicializar el pin GPIO 16
    gpio_init(LED_PIN);
    
    // Configurar el pin como salida
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    
    // Bucle principal
    while (true) {
        // Encender el LED (nivel alto)
        gpio_put(LED_PIN, 1);
        printf("LED ENCENDIDO\n");
        sleep_ms(5000);  // Esperar 1 segundo
        
        // Apagar el LED (nivel bajo)
        gpio_put(LED_PIN, 0);
        printf("LED APAGADO\n");
        sleep_ms(5000);  // Esperar 1 segundo
    }
    
    return 0;
}
