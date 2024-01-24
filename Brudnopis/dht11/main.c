#include <stdio.h>
#include "fmt.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "xtimer.h"

int main(void) {
    dht_params_t my_params;
    my_params.pin = GPIO_PIN(PORT_A, 10);
    my_params.type=DHT22;
    my_params.in_mode=DHT_PARAM_PULL;
	int16_t temp, hum;

    printf("my params declared\n");
    dht_t dev;
	while(1){
    if(dht_init(&dev, &my_params)==DHT_OK){
       printf("DHT sensor connected\n");
	dht_read(&dev, &temp, &hum);
	break;
    }
    else printf("DHT sensor error\n");
	}

    //int16_t temp, hum;

	for (int i=0; i<15; ++i){
    if(dht_read(&dev, &temp, &hum)!=DHT_OK){
        printf("Error reading values\n");
    } else puts("DHT OK\n");
    printf("temperature: %d, humidity: %d\n", temp, hum);
	xtimer_sleep(2);

}
	return 0;
}
