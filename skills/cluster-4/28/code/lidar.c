#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_PORT I2C_NUM_0
#define LIDAR_DEVICE_ADDRESS 0x62
#define LIDAR_REGISTERS_ACQUISITION 0x00
#define LIDAR_REGISTERS_STATUS 0x01
#define LIDAR_REGISTERS_DISTANCE_LOW 0x10
#define LIDAR_REGISTERS_DISTANCE_HIGH 0x11
#define LIDAR_ACQUISITION_COMMAND 0x04

void initializeI2CPort() {
    i2c_config_t i2cConfig = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 23,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000  // Fast mode
    };
    i2c_param_config(I2C_PORT, &i2cConfig);
    i2c_driver_install(I2C_PORT, i2cConfig.mode, 0, 0, 0);
}

void writeLidarRegister(uint8_t registerAddress, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LIDAR_DEVICE_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, registerAddress, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

void readLidarRegisters(uint8_t registerAddress, uint8_t *data, size_t length) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LIDAR_DEVICE_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, registerAddress, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LIDAR_DEVICE_ADDRESS << 1) | I2C_MASTER_READ, true);
    if (length > 1) {
        i2c_master_read(cmd, data, length - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + length - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

void lidarDistanceTask() {
    uint8_t status;
    uint8_t distanceArray[2];
    int distance;

    while (1) {
        writeLidarRegister(LIDAR_REGISTERS_ACQUISITION, LIDAR_ACQUISITION_COMMAND);

        vTaskDelay(22 / portTICK_PERIOD_MS); // Delay for boot-up

        do {
            readLidarRegisters(LIDAR_REGISTERS_STATUS, &status, 1);
        } while (status & 0x01); // Loop until LSB is low, indicating the device is ready

        readLidarRegisters(LIDAR_REGISTERS_DISTANCE_LOW, distanceArray, 2);
        distance = (distanceArray[1] << 8) | distanceArray[0]; // Combine bytes for distance

        printf("Lidar Distance: %d cm\n", distance);

        vTaskDelay(pdMS_TO_TICKS(100)); // 100 ms delay before the next read
    }
}

void app_main() {
    initializeI2CPort();

    xTaskCreate(&lidarDistanceTask, "lidarDistanceTask", 4096, NULL, 5, NULL);
}
