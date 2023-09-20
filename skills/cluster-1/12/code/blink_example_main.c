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

int ledArray[4] = {RED, GREEN, YELLOW, BLUE};
static const char *TAG = "example";
int arrayIndex = -1;

static void init_button(void)
{
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
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
    while (gpio_get_level(BUTTON) == 1 || gpio_get_level(BUTTON) == 0)
    {
        if (gpio_get_level(BUTTON) == 1)
        {
            if (arrayIndex + 1 > 3)
            {
                arrayIndex = 0;
            }
            else
            {
                arrayIndex++;
            }
            ESP_LOGI(TAG, "Button Pushed");
            blink_led(ledArray[arrayIndex]);
        }
        vTaskDelay((CONFIG_BLINK_PERIOD-850) / portTICK_PERIOD_MS);
    }
}
