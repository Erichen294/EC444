/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define RED 13
#define GREEN 12
#define YELLOW 27
#define BLUE 14
#define BUTTON 15
#define ON 1
#define OFF 0

#define GPIO_INPUT_IO_1       15
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_INPUT_PIN_SEL    1ULL<<GPIO_INPUT_IO_1

int ledArray[4] = {RED, GREEN, YELLOW, BLUE};
static const char *TAG = "example";
int arrayIndex = -1;
int flag = 0;
int lastFlag = 0;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    lastFlag = flag;
    flag ^= 1;
}

static void init_button(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    gpio_intr_enable(GPIO_INPUT_IO_1 );
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);
}

static void blink_led(int led)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(led, ON);
    if (arrayIndex - 1 < 0)
    {
        gpio_set_level(BLUE, OFF);
    }
    else
    {
        gpio_set_level(ledArray[arrayIndex-1], OFF);
    }
}

static void configure_led(void)
{
    gpio_reset_pin(RED);
    gpio_reset_pin(GREEN);
    gpio_reset_pin(YELLOW);
    gpio_reset_pin(BLUE);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_direction(BLUE, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();
    init_button();

    //Logic to cycle through LEDS with push button
    while (1)
    {
        if (lastFlag != flag)
        {
            if (arrayIndex + 1 > 3)
            {
                arrayIndex = 0;
            }
            else
            {
                arrayIndex++;
            }
            blink_led(ledArray[arrayIndex]);
            lastFlag = flag;
        }
        vTaskDelay((CONFIG_BLINK_PERIOD-850) / portTICK_PERIOD_MS);
    }
}
