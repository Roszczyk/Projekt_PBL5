#include <stdio.h>
#include "periph/gpio.h"
#include "xtimer.h"

int main(void) {
    gpio_t pin = GPIO_PIN(PORT_B, 12);
     if (gpio_init(pin, GPIO_IN) == 0) {
        printf("GPIO initialized successfully.\n");
    	for(int i=0;i<100;++i){
		int value = gpio_read(pin);
        	printf("Value read from the pin: %d\n", value);
		xtimer_sleep(1);
	}
    } else {
        printf("Failed to initialize GPIO.\n");
    }
    return 0;
}
