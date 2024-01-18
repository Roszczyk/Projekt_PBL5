#include <stdio.h>
#include "periph/gpio.h"

int main(void) {
    gpio_t pin = GPIO_PIN(PORT_A, 0);
     if (gpio_init(pin, GPIO_IN) == 0) {
        printf("GPIO initialized successfully.\n");
    int value = gpio_read(pin);
    
        printf("Value read from the pin: %d\n", value);
    } else {
        printf("Failed to initialize GPIO.\n");
    }
    return 0;
}
