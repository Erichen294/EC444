#include <string.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "sdkconfig.h" // generated by "make menuconfig"
#include "ssd1306.h"
#include "font8x8_basic.h"
​
#define SDA_PIN 23 //CHANGED TO OUR ESP PINOUT 
#define SCL_PIN 22 //CHANGED TO OUR ESP PINOUT
​
#define tag "SSD1306"
​
void task_ssd1306_display_bitmap(void *ignore);
​
void i2c_master_init()
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000
    };
    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}
​
void ssd1306_init() {
    esp_err_t espRc;
​
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
​
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
​
    i2c_master_write_byte(cmd, OLED_CMD_SET_CHARGE_PUMP, true);
    i2c_master_write_byte(cmd, 0x14, true);
​
    i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP, true); // reverse left-right mapping
    i2c_master_write_byte(cmd, OLED_CMD_SET_COM_SCAN_MODE, true); // reverse up-bottom mapping
​
    i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_ON, true);
    i2c_master_stop(cmd);
​
    espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    if (espRc == ESP_OK) {
        ESP_LOGI(tag, "OLED configured successfully");
    } else {
        ESP_LOGE(tag, "OLED configuration failed. code: 0x%.2X", espRc);
    }
    i2c_cmd_link_delete(cmd);
}
​
void task_ssd1306_display_clear(void *ignore) {
    i2c_cmd_handle_t cmd;
​
    uint8_t zero[128];
    memset(zero, 0, sizeof(zero)); 
​
    for (uint8_t i = 0; i < 8; i++) {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_SINGLE, true);
        i2c_master_write_byte(cmd, 0xB0 | i, true);
​
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
        i2c_master_write(cmd, zero, 128, true);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
    }
​
    vTaskDelete(NULL);
}
​
void task_ssd1306_contrast(void *ignore) {
    i2c_cmd_handle_t cmd;
​
    uint8_t contrast = 0;
    uint8_t direction = 1;
    while (true) {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
        i2c_master_write_byte(cmd, OLED_CMD_SET_CONTRAST, true);
        i2c_master_write_byte(cmd, contrast, true);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
        vTaskDelay(1/portTICK_PERIOD_MS);
​
        contrast += direction;
        if (contrast == 0xFF) { direction = -1; }
        if (contrast == 0x0) { direction = 1; }
        // esp_task_wdt_reset();
    }
    vTaskDelete(NULL);
}
​
//
extern const uint8_t bitmap[1024] = 
​
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x1f, 0x0f, 0x0f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f,
0x8f, 0x8f, 0x0f, 0x0f, 0xff, 0xff, 0x0f, 0x0f, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0x7f, 0x7f, 0x8f,
0x8f, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x0f, 0x0f, 0x8f, 0x8f, 0xff, 0xff, 0x0f, 0x0f, 0x8f, 0x8f,
0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x0f, 0x0f, 0x1f, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xe6, 0xe6, 0xe7, 0xe7, 0xe0, 0xe0, 0xe0, 0x18, 0x18, 0x7f,
0x7f, 0x99, 0x99, 0x78, 0x78, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x30, 0x30, 0x30, 0xf3, 0xf3, 0x33, 0x33, 0x33, 0x33, 0xf3, 0xf3, 0x33,
0x33, 0x31, 0x30, 0x30, 0x3f, 0x3f, 0xc3, 0xc3, 0xff, 0xff, 0x03, 0x03, 0x03, 0x0c, 0x0c, 0xc0,
0xc0, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x03, 0x03, 0xff, 0xff, 0x30, 0x30, 0xf1, 0xf3,
0xf3, 0xf3, 0xf3, 0x33, 0x33, 0xf3, 0xf3, 0x33, 0x33, 0x30, 0x30, 0x30, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x78, 0x78, 0xe0, 0xe0, 0x00, 0x00, 0x63, 0x63, 0x63,
0x63, 0x63, 0x63, 0x63, 0xe0, 0xe0, 0x18, 0x18, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x98,
0x98, 0x9f, 0x9f, 0x87, 0x83, 0x87, 0x9f, 0x9f, 0xe3, 0xe3, 0x1f, 0x1f, 0x7f, 0x7f, 0x7c, 0x78,
0x78, 0x63, 0x63, 0x9f, 0x9f, 0x1f, 0x1f, 0x78, 0x78, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x3e, 0x3e, 0x3e, 0xc6, 0xc6, 0x39, 0x39, 0x06, 0x06, 0x38, 0x38, 0xc0,
0xc0, 0xc0, 0xc6, 0xc6, 0x07, 0x07, 0xfe, 0xfe, 0xfe, 0xfe, 0xee, 0xc6, 0xe6, 0xf9, 0xf9, 0x07,
0x07, 0x39, 0x39, 0xc7, 0xc7, 0xc7, 0x39, 0x39, 0xf9, 0xf9, 0xc6, 0xc6, 0x38, 0x38, 0x38, 0x38,
0x18, 0x00, 0x00, 0xff, 0xff, 0xc6, 0xc6, 0xc6, 0xc6, 0x38, 0x38, 0x38, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0xcf, 0xcf, 0xcc, 0xcc, 0xcf, 0xcf, 0xcc, 0xcc, 0xcc,
0xcc, 0x8c, 0x0c, 0x0c, 0xfc, 0xfc, 0x03, 0x03, 0x33, 0x33, 0x03, 0x03, 0x03, 0xcc, 0xcc, 0x03,
0x03, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0x3c, 0x3c, 0x00, 0x00, 0xfc, 0xfc, 0xcc, 0xcc, 0xfc, 0xfc,
0xfc, 0x00, 0x00, 0xfc, 0xfc, 0xc0, 0xc0, 0x0c, 0x0c, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x86, 0x86, 0x86, 0x86, 0x86, 0x9e, 0x9e, 0x99, 0x99, 0x80,
0x80, 0x81, 0x81, 0xe7, 0xe7, 0xe7, 0x86, 0x86, 0x80, 0x80, 0xe1, 0xe1, 0x81, 0x81, 0xf9, 0xf9,
0xf8, 0x00, 0x00, 0xe1, 0xe1, 0x07, 0x07, 0x86, 0x86, 0xe7, 0xe7, 0xe7, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xf8, 0xf0, 0xf0, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1,
0xf1, 0xf1, 0xf0, 0xf0, 0xff, 0xff, 0xf1, 0xf1, 0xff, 0xff, 0xfb, 0xf1, 0xf1, 0xff, 0xff, 0xf1,
0xf1, 0xff, 0xff, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xf1,
0xf1, 0xf0, 0xf0, 0xf1, 0xf1, 0xf0, 0xf0, 0xf1, 0xf1, 0xf1, 0xf1, 0xfb, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
​
// {
// // bit map of your QR code
// 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
// 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 
// 0xff, 0x1f, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
// 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
// 0x03, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x9c, 0x9c, 0xfc, 0xfc, 0xfc, 0x03, 
// 0x03, 0x63, 0x63, 0x63, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x03, 0x03, 
// 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x8f, 0x8f, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 
// 0x8c, 0x8c, 0x8f, 0x8f, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0xf3, 0xf3, 0xf3, 0x8c, 
// 0x8c, 0x70, 0x70, 0x70, 0x03, 0x03, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x8f, 0x8f, 0x8c, 0x8c, 
// 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8f, 0x8f, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x8f, 0x8f, 0xf3, 0xf3, 0xf3, 0x03, 0x03, 0x0f, 
// 0x0f, 0x0f, 0xf3, 0xf3, 0x73, 0x73, 0x73, 0x73, 0x73, 0x73, 0xfc, 0xfc, 0x0f, 0x0f, 0x0f, 0x8f, 
// 0x8f, 0x00, 0x00, 0x00, 0x80, 0x80, 0x8f, 0x8f, 0x8f, 0x83, 0x83, 0x83, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0x03, 0x03, 0x7f, 0x7f, 0x7f, 0xf3, 0xf3, 0x73, 0x73, 0x73, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0xce, 0xce, 0xc1, 0xc1, 0xcf, 0xcf, 0xcf, 0xc0, 0xc0, 0xce, 
// 0xce, 0xce, 0xc1, 0xc1, 0xce, 0xce, 0xce, 0xce, 0xce, 0xce, 0x3f, 0x3f, 0xce, 0xce, 0xce, 0xcf, 
// 0xcf, 0x30, 0x30, 0x30, 0xcf, 0xcf, 0xff, 0xff, 0xff, 0x31, 0x31, 0x31, 0xc1, 0xc1, 0xf1, 0xf1, 
// 0xf1, 0x30, 0x30, 0xc0, 0xc0, 0xc0, 0x01, 0x01, 0x3e, 0x3e, 0x3e, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0xf1, 0xf1, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 
// 0x31, 0x31, 0xf1, 0xf1, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0x01, 0x01, 0x01, 0x31, 
// 0x31, 0xf0, 0xf0, 0xf0, 0x31, 0x31, 0x3f, 0x3f, 0x3f, 0x3e, 0x3e, 0x3e, 0x01, 0x01, 0x0f, 0x0f, 
// 0x0f, 0xf0, 0xf0, 0xcf, 0xcf, 0xcf, 0xf0, 0xf0, 0x30, 0x30, 0x30, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 
// 0xc0, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc1, 0xc1, 0x00, 0x00, 0x00, 0x3e, 
// 0x3e, 0xf9, 0xf9, 0xf9, 0xc6, 0xc6, 0x3e, 0x3e, 0x3e, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0xf8, 0xf8, 
// 0xf8, 0xf9, 0xf9, 0x39, 0x39, 0x39, 0xf9, 0xf9, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 
// 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 
// 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
// };
​
void task_ssd1306_display_bitmap(void *ignore) {
    i2c_cmd_handle_t cmd;
​
    for (uint8_t i = 0; i < 8; i++) {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_SINGLE, true);
        i2c_master_write_byte(cmd, 0xB0 | i, true);
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
        i2c_master_write(cmd, &bitmap[i * 128], 128, true);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
    }
​
    vTaskDelete(NULL);
}
​
void app_main(void)
{
    i2c_master_init();
    ssd1306_init();
    
    xTaskCreate(&task_ssd1306_display_clear, "ssd1306_display_clear",  2048, NULL, 6, NULL);
    vTaskDelay(100/portTICK_PERIOD_MS);
    xTaskCreate(&task_ssd1306_display_bitmap, "ssd1306_display_bitmap", 2048, NULL, 6, NULL);
}
