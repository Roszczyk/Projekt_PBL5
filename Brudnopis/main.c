#include <stdio.h>
#include "periph/gpio.h"
#include "xtimer.h"
#include "shell.h"
#include "semtech_loramac.h"

int main(void) {
    while(1){
    gpio_set(PORT_A, 3);
    xtimer_sleep(3);
    gpio_clear(PORT_A, 3);
    xtimer_sleep(3);
    printf("Lap end\n");
    }
    return 0;
}
