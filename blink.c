#include <stdio.h>
#include "pico/stdlib.h"    //picoの標準ライブラリ GPIOやUART,sleepmsなどが使える
#include "hardware/adc.h"   //picoのADCライブラリ
#include "hardware/i2c.h"   //picoのI2Cライブラリ
#include "ssd1306.h"        //OLEDディスプレイ用ライブラリ (参照:https://github.com/daschr/pico-ssd1306)

#define BUTTON_LEFT  16     // GP16ボタン
#define BUTTON_RIGHT 17     // GP17ボタン

#define I2C_PORT i2c0       // 使用するI2C通信回路の指定（0番を使用）
#define PIN_SDA 4           // GP4をSDAに使用
#define PIN_SCL 5           // GP5をSCLに使用

#define SOIL_ADC_PIN 26      // GP26 = ADC0
#define RELAY_PIN    15      // GP15　トランジスタにつながっているが、役割を明確にするためにリレーと表記
    
#define DRY_THRESHOLD  3500
#define WET_THRESHOLD  2500

#define PUMP_TIME_MS 2000   // ポンプ動作時間 2秒

// 測定間隔 6時間 = 6 × 60 × 60秒
#define CHECK_INTERVAL_MS (6ULL * 60ULL * 60ULL)

char buf[32];   //soil_valueのint型を文字列に変換するためのバッファ

uint32_t seconds_counter = CHECK_INTERVAL_MS;  // 経過秒数をカウントする変数、初期値は6時間に設定

//関数を定義、短いためプロトタイプ宣言ではなくそのまま定義する
void pump_on(void){
    gpio_put(RELAY_PIN, true);
}

void pump_off(void){
    gpio_put(RELAY_PIN, false);
}

int main()
{
    stdio_init_all();

    //左ボタン (GP16) の初期化
    gpio_init(BUTTON_LEFT);
    gpio_set_dir(BUTTON_LEFT, GPIO_IN);
    gpio_pull_up(BUTTON_LEFT);

    //右ボタン (GP17) の初期化
    gpio_init(BUTTON_RIGHT);
    gpio_set_dir(BUTTON_RIGHT, GPIO_IN);
    gpio_pull_up(BUTTON_RIGHT);

    i2c_init(i2c0, 400 * 1000);     // I2C通信の初期化、400kHzで通信 
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);      //I2Cを使えるようにするための初期化
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);      //I2Cを使えるようにするための初期化
    gpio_pull_up(PIN_SDA);      //内部抵抗を使ってプルアップする
    gpio_pull_up(PIN_SCL);      //内部抵抗を使ってプルアップする

    ssd1306_t disp;             //githubのexampleを参考
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&disp);   //画面クリア
    ssd1306_show(&disp);    //画面に反映


    // リレー制御用GPIO
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);

    // 起動時は必ずポンプOFF
    pump_off();

    // ADC初期化
    adc_init();
    adc_gpio_init(SOIL_ADC_PIN);    //ADCを使えるようにするための初期化
    adc_select_input(0);

    while (true)
    {
        uint16_t soil_value = adc_read();   // ADCの値を読み取る、0～4095の範囲で返ってくる

        ssd1306_clear(&disp); // 前の表示を一度消去
        sprintf(buf, "Soil: %d", soil_value);
        ssd1306_draw_string(&disp, 0, 0, 2, buf);
        ssd1306_show(&disp);

        if (seconds_counter >= CHECK_INTERVAL_MS){

            if (soil_value >= DRY_THRESHOLD){
                pump_on();
                sleep_ms(PUMP_TIME_MS);     // ポンプを動作させる時間だけ待機
                pump_off();
            }
            else{
                pump_off();
            }

            seconds_counter = 0;
        }

    sleep_ms(1000); 
    seconds_counter++;

    }

    return 0;
}