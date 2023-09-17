#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h"
#include "esp_log.h"
#include "sdkconfig.h"	

#define BLINK_GPIO 13

static uint8_t s_led_state = 1;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void bin_to_hex(int number)
{
    if (number == 0)
    {
        printf("\nHex: 0");
        return;
    }
    //Finding the length of the array
    float tempFloat = log(number)/(float)log(16);
    int tempInt = log(number)/log(16);
    int size;
    int quotient = number;
    int remainder;
    int index = 0;
    //Seeing if answer is float
    if (tempFloat - tempInt != 0)
    {
        //Is float
        size = ceil(tempFloat);
    }
    else
    {
        //Is integer
        size = tempInt + 1;
    }
    char *answer = malloc(size+1);
    while (quotient != 0)
    {
        remainder = quotient % 16;
        switch(remainder)
        {
            case 10:
                answer[index++] = 'A';
                break;
            case 11:
                answer[index++] = 'B';
                break;
            case 12:
                answer[index++] = 'C';
                break;
            case 13:
                answer[index++] = 'D';
                break;
            case 14:
                answer[index++] = 'E';
                break;
            case 15:
                answer[index++] = 'F';
                break;
            default:
                answer[index++] = remainder + '0';
                break;
        }
        quotient /= 16;
    }

    //Reversing the array
    int front = 0;
    int back = size-1;
    char temp;
    if (size != 2)
    {
        while (front != back)
        {
            temp = answer[front];
            answer[front] = answer[back];
            answer[back] = temp;
            front++;
            back--;
        }
    }
    else
    {
        temp = answer[0];
        answer[0] = answer[1];
        answer[1] = temp;
    }
    answer[size] = '\0';
    printf("\nHex: ");
    for (int i = 0; i < size; i++)
    {
        printf("%c", answer[i]);
    }
    free(answer);
}

void app_main()
{
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0,
      256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);
    
    configure_led();

    //Mode represents the current mode
    int mode = 1;

    printf("\ntoggle mode");

    while(1) {
        char buf = '\0';
        char stringbuf[15];
        char charInput[5];
        stringbuf[0] = '\0';
        charInput[0] = '\0';
        scanf("%c", &buf);
        
        if (buf == 's')
        {
            if (mode == 3)
            {
                mode = 1;
            }
            else
            {
                mode++;
            }
            if (mode == 1)
            {
                printf("\ntoggle mode");
            }
            else if (mode == 2)
            {
                printf("\necho mode");
            }
            else
            {
                printf("\nhex mode");
            }
            fflush(stdin);
            buf = '\0';
        }

        if (mode == 2)
        {
            if (buf != '\n' && buf != '\0' && buf != '\t' && buf != ' ' && buf != 's')
            {
                stringbuf[0] = buf;
            }
            fflush(stdin);
        }

        if (mode == 3)
        {
            if (buf != '\n' && buf != '\0' && buf != '\t' && buf != ' ' && buf != 's')
            {
                charInput[0] = buf;
            }
            fflush(stdin);
        }
        if (mode == 1)
        {
            if (buf == 't')
            {
                printf("\nRead: %c", buf);
                blink_led();
                s_led_state = !s_led_state;
            }

            if (buf == 's')
            {
                printf("\nRead: %c", buf);
                mode++;
            }
            fflush(stdin);
            buf = '\0';
        }

        if (mode == 2)
        {
            char tempbuf[14];
            gets(tempbuf);
            size_t len = strlen(tempbuf);
            if (stringbuf[0] != '\0')
            {
                for (int i = 0; i < len; i++)
                {
                    stringbuf[i+1] = tempbuf[i];
                }
                stringbuf[len + 1] = '\0';
            }
            else
            {
                for (int i = 0; i < len; i++)
                {
                    stringbuf[i] = tempbuf[i];
                }
                stringbuf[len] = '\0';
            }
            if (stringbuf[0] != '\0' && len >= 2)
            {
                printf("\necho: %s", stringbuf);
            }
            if (stringbuf[0] == 's' && len == 1)
            {
                mode++;
            }
            fflush(stdin);
            stringbuf[0] = '\0';
            tempbuf[0] = '\0';
        }

        if (mode == 3)
        {
            char newTemp[4] = {'\0', '\0', '\0', '\0'};
            gets(newTemp);
            size_t len1 = strlen(newTemp);
            int input;

            if (charInput[0] != '\0')
            {
                for (int i = 0; i < len1; i++)
                {
                    charInput[i+1] = newTemp[i];
                }
                charInput[len1+1] = '\0';
            }
            else
            {
                for (int i = 0; i < len1; i++)
                {
                    charInput[i] = newTemp[i];
                }
                charInput[len1] = '\0';
            }

            if (charInput[0] == 's')
            {
                mode++;
            }
            else
            {
                input = atoi(charInput);
                if (input != 0)
                {
                    printf("\nEnter an integer: ");
                    bin_to_hex(input);
                }
                if (charInput[0] == '0')
                {
                    printf("\nEnter an integer: ");
                    bin_to_hex(0);
                }
            }
            fflush(stdin);
            charInput[0] = '\0';
            newTemp[0] = '\0';
        }
    }
}