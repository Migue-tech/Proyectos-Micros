#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

//Se definen los pines
#define AIN1 16    // GPIO para AIN1
#define AIN2 17    // GPIO para AIN2  
#define PWMA 18    // GPIO para PWMA (PWM)
#define STBY 19    // GPIO para STBY

#define MOTOR_FREQ 25000  // 25kHz (ideal para TB6612)

void setup_motor() {
    // 1. Configurar pines de dirección y standby
    gpio_init(AIN1); gpio_set_dir(AIN1, GPIO_OUT);
    gpio_init(AIN2); gpio_set_dir(AIN2, GPIO_OUT);
    gpio_init(STBY); gpio_set_dir(STBY, GPIO_OUT);
    
    // Activar el driver (quitar de standby)
    gpio_put(STBY, 1);
    
    // 2. Configurar PWM a 25kHz
    gpio_set_function(PWMA, GPIO_FUNC_PWM); //pin modo PWM
    uint slice = pwm_gpio_to_slice_num(PWMA);
    
    // 125MHz / (1 * 25kHZ) = 5000
    pwm_set_clkdiv(slice, 1.0f);      // Divisor: controla la resolucion
    pwm_set_wrap(slice, 5000);        // Wrap value el 5000 equivale al 100%
    pwm_set_enabled(slice, true);     // Activar
}

void motor_forward(uint16_t speed) {
    // Sentido adelante
    gpio_put(AIN1, 1);
    gpio_put(AIN2, 0);
    
    // Velocidad (0-5000)
    pwm_set_gpio_level(PWMA, speed);
}

void motor_backward(uint16_t speed) {
    // Sentido atrás
    gpio_put(AIN1, 0);
    gpio_put(AIN2, 1);
    pwm_set_gpio_level(PWMA, speed);
}

void motor_stop() {
    // Frenado rápido SOLO para el TB6612
    gpio_put(AIN1, 1);
    gpio_put(AIN2, 1);
    pwm_set_gpio_level(PWMA, 0); 
}

void motor_coast() {
    // Desconectar (coast)
    gpio_put(AIN1, 0);
    gpio_put(AIN2, 0);
    pwm_set_gpio_level(PWMA, 0);
}

int main() {
    stdio_init_all();
    setup_motor();
    
    printf("TB6612 con Pico - 25kHz PWM\n");
    
    while (true) {
        // Prueba: adelante 50%
        motor_forward(2500);  // 50% de 5000
        printf("Adelante 50%%\n");
        sleep_ms(2000);
        
        // Adelante 100%
        motor_forward(5000);
        printf("Adelante 100%%\n");
        sleep_ms(5000);
        
        // Parar
        motor_stop();
        printf("Parado\n");
        sleep_ms(1000);
        
        // Atrás 30%
        motor_backward(1500);  // 30% de 5000
        printf("Atrás 30%%\n");
        sleep_ms(3000);
        
        // Coast
        motor_coast();
        printf("Coast\n");
        sleep_ms(1000);
    }
}
