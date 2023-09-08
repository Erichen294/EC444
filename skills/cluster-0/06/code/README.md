# Code Readme

Name: Eric Chen
Date: 9/8/23

Here is my code that I used for blinking LEDs using GPIO pins. Although I ended up hardcoding each number, I think that it's okay because I'm not counting up that high. If the max number is very high, then I would create a better algorithm.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";
int count = 0;
bool up = true;
/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO
#define RED 13
#define GREEN 12
#define YELLOW 27
static uint8_t s_led_state = 0;

#ifdef CONFIG_BLINK_LED_RMT

static led_strip_handle_t led_strip;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        led_strip_set_pixel(led_strip, 0, 16, 16, 16);
        /* Refresh the strip to send data */
        led_strip_refresh(led_strip);
    } else {
        /* Set all LED off to clear all pixels */
        led_strip_clear(led_strip);
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

#elif CONFIG_BLINK_LED_GPIO

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
    gpio_set_level(RED, s_led_state);
    gpio_set_level(GREEN, s_led_state);
    gpio_set_level(YELLOW, s_led_state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    gpio_reset_pin(RED);
    gpio_reset_pin(GREEN);
    gpio_reset_pin(YELLOW);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
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
#endif

void app_main(void)
{
    /* Configure the peripheral according to the LED type */
    configure_led();

    while (1) {
        if (up)
        {
            ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
            count_binary(count);
            /* Toggle the LED state */
            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            count++;
            if (count == 8)
            {
                // Resets count back to 0
                count = 6;
                up = false;
            }
        }

        else
        {
            ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
            count_binary(count);
            /* Toggle the LED state */
            vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
            count--;
            if (count == -1)
            {
                count = 1;
                up = true;
            }
        }

        }
}

