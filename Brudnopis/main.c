#include <stdio.h>
#include "periph/gpio.h"
#include "xtimer.h"
#include "shell.h"

int main(void) {
	gpio_t pin1;
	pin1=GPIO_PIN(PORT_A, 3);
	gpio_init(pin1, GPIO_OUT);
    while(1){
    gpio_set(pin1);
    xtimer_sleep(3);
    gpio_clear(pin1);
    xtimer_sleep(3);
    printf("Lap end\n");
    }
    return 0;
}
