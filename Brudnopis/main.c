#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT11_PIN   GPIO_PIN(PORT_A, 10)

static void delay_us(uint32_t us) {
    xtimer_usleep(us);
}

static void dht11_start_signal(void) {
    gpio_init(DHT11_PIN, GPIO_OUT);
    gpio_clear(DHT11_PIN);
    delay_us(18000);
    gpio_set(DHT11_PIN);
    delay_us(40);
}

static int dht11_read_bit(void) {
    gpio_init(DHT11_PIN, GPIO_IN);

    int timeout = 10000;
    while (!gpio_read(DHT11_PIN) && timeout--);

    if (timeout <= 0) {
        return -1; // Timeout
    }

    delay_us(40);

    return gpio_read(DHT11_PIN);
}

static int dht11_read_byte(void) {
    int byte = 0;

    for (int i = 7; i >= 0; i--) {
        int bit = dht11_read_bit();
        if (bit < 0) {
            return -1; // Timeout
        }
        byte |= (bit << i);
    }

    return byte;
}

int main(void) {
    dht11_start_signal();

    int humidity_byte1 = dht11_read_byte();
    int humidity_byte2 = dht11_read_byte();
    int temperature_byte1 = dht11_read_byte();
    int temperature_byte2 = dht11_read_byte();
    int checksum = dht11_read_byte();

    if ((humidity_byte1 + humidity_byte2 + temperature_byte1 + temperature_byte2) == checksum) {
	printf("Sum correct/n");
}        float humidity = humidity_byte1;
        float temperature = temperature_byte1;

        printf("Humidity: %.1f%%\n", humidity);
        printf("Temperature: %.1f°C\n", temperature);
  //  } else {
    //    printf("Checksum error\n");
    //}

    return 0;
}
