#include "pico/stdlib.h"

#define RELAY_PIN 15

int main()
{
    gpio_init(RELAY_PIN);   // GP15を初期化
    gpio_set_dir(RELAY_PIN, GPIO_OUT);  // GP15を出力設定にする

    while (true)
    {
        // リレー ON
        gpio_put(RELAY_PIN, 0);
        sleep_ms(3000);

        // リレー OFF
        gpio_put(RELAY_PIN, 1);
        sleep_ms(3000);
    }
}