#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "esp_vfs_dev.h"	// This is associated with VFS -- virtual file system interface and abstraction -- see the docs

void app_main()
{
    /* Install UART driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( uart_driver_install(UART_NUM_0,
      256, 0, 0, NULL, 0) );

    /* Tell VFS to use UART driver */
    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    char str[20];
    int num = 0;

    while(1) {


      // gets()
      char buf[5];
      gets(buf);
      if (buf[0] != '\0') {
       printf("string is: %s\n", buf);

      }

			// or

      // scanf()
      printf("Enter a number: \n");
      scanf("%d", &num);
      printf("Got: %d\n", num);

      vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}