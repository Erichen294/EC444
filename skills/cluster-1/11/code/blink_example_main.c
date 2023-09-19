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

int count = 0;
bool up = true;
/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define RED 13
#define GREEN 12
#define YELLOW 27
#define BUTTON 15
#define BLUE 14
#define ON 1
#define OFF 0

static uint8_t s_led_state = 0;
static const char *TAG = "State";

static void init_button(void)
{
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
}

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(RED, s_led_state);
    gpio_set_level(GREEN, s_led_state);
    gpio_set_level(YELLOW, s_led_state);
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

static void count_binary(int count)
{
    // Red is the least significant bit and Yellow is the most significant bit
    if (count == 0)
    {
        gpio_set_level(RED, 0);
        gpio_set_level(GREEN, 0);
        gpio_set_level(YELLOW, 0);
    }

    else if (count == 1)
    {
        gpio_set_level(RED, 1);
        gpio_set_level(GREEN, 0);
        gpio_set_level(YELLOW, 0);
    }

    else if (count == 2)
    {
        gpio_set_level(RED, 0);
        gpio_set_level(GREEN, 1);
        gpio_set_level(YELLOW, 0);
    }

    else if (count == 3)
    {
        gpio_set_level(RED, 1);
        gpio_set_level(GREEN, 1);
        gpio_set_level(YELLOW, 0);
    }

    else if (count == 4)
    {
        gpio_set_level(RED, 0);
        gpio_set_level(GREEN, 0);
        gpio_set_level(YELLOW, 1);
    }

    else if (count == 5)
    {
        gpio_set_level(RED, 1);
        gpio_set_level(GREEN, 0);
        gpio_set_level(YELLOW, 1);
    }

    else if (count == 6)
    {
        gpio_set_level(RED, 0);
        gpio_set_level(GREEN, 1);
        gpio_set_level(YELLOW, 1);
    }

    else if (count == 7)
    {
        gpio_set_level(RED, 1);
        gpio_set_level(GREEN, 1);
        gpio_set_level(YELLOW, 1);
    }
}

static void blink_blue(int state)
{
    if (state == 1)
    {
        //Counting up turn on blue LED
        gpio_set_level(BLUE, ON);
    }
    else
    {
        //Counting down turn off blue LED
        gpio_set_level(BLUE, OFF);
    }
}

static void task_a(void)
{
    configure_led();
    //Counting in binary
    while (1)
    {
        if (up)
        {
            count_binary(count);
            /* Toggle the LED state */
            count++;
            if (count == 8 && !up)
            {
                // Resets count back to 0
                count = 6;
                up = false;
            }
            else if (count == 8 && up)
            {
                count = 7;
            }
        }
        else
        {
            count_binary(count);
            /* Toggle the LED state */
            count--;
            if (count == -1 && up)
            {
                count = 1;
                up = true;
            }
            else if (count == -1 && !up)
            {
                count = 0;
            }
        }
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}

static void task_b(void)
{
    init_button();
    while (1)
    {
        if (gpio_get_level(BUTTON) == 1)
        {
            ESP_LOGI(TAG, "Button Pushed");
            up = !up;
        }
        vTaskDelay((CONFIG_BLINK_PERIOD-850) / portTICK_PERIOD_MS);
    }
}

static void task_c(void)
{
    while (1)
    {
        if (up)
        {
            blink_blue(ON);
            ESP_LOGI(TAG, "Counting up");
        }
        else
        {
            blink_blue(OFF);
            ESP_LOGI(TAG, "Counting down");
        }
        vTaskDelay((CONFIG_BLINK_PERIOD-950) / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    /* Configure the peripheral according to the LED type */
    xTaskCreate(task_a, "task_a", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(task_b, "task_b", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(task_c, "task_c", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}
