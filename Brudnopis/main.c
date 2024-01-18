#include "fmt.h"
#include "dht.h"
#include "dht_params.h"

int main(void) {
    dht_params_t my_params;
    my_params.pin = GPIO_PIN(PORT_A, 8);
    my_params.type=DHT11;
    my_params.in_mode=DHT_PARAM_PULL;

    printf("my params declared\n");
    dht_t dev;
    if(dht_init(&dev, &my_params)==DHT_OK){
       printf("DHT sensor connected\n");
    }
    else printf("DHT sensor error\n");

    int16_t temp, hum;
    if(dht_read(&dev, &temp, &hum)!=DHT_OK){
        printf("Error reading values\n");
    }

    printf("temperature: %d, humidity: %d", temp, hum);
}