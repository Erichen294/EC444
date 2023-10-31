/*
 * ANNOTATED 2023-10-19
 *
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

static const char *TAG = "example";

// Consult the ESC before you change these parameters for the buggy speed control

// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        -90   // Minimum angle
#define SERVO_MAX_DEGREE        90    // Maximum angle

#define SERVO_PULSE_GPIO             26        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US)
      / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

void app_main(void)
{
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;             // Create PWM timer
    mcpwm_timer_config_t timer_config = {          // Configure PWM timer 
      .group_id = 0,                               // Pick PWM group 0
      .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,      // Default clock source 
      .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ, // Hz
      .period_ticks = SERVO_TIMEBASE_PERIOD,       // Set servo period (20ms -- 50 Hz)
      .count_mode = MCPWM_TIMER_COUNT_MODE_UP,     // Count up
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;               // Create PWM operator 
    mcpwm_operator_config_t operator_config = {    // Configure PWM operator
      .group_id = 0,                               // operator same group and PWM timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");   // Connect PWM timer and PWM operator
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    mcpwm_cmpr_handle_t comparator = NULL;         // Create PWM comparator
    mcpwm_comparator_config_t comparator_config = {// Updates when timer = zero
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    mcpwm_gen_handle_t generator = NULL;            // Create generator
    mcpwm_generator_config_t generator_config = {   // Output to GPIO pin 
        .gen_gpio_num = SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));       // Enable
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP)); // Run continuously

    // This code drives the servo over a range
    // The PWM is changed slowly allow the (mechanical) servo to keep up 

    int angle = 0;
    while (1) {// This code changes the PWM slowly to allow the (mechanical) servo to keep up 
        printf("Enter an angle: ");
        scanf("%d",&angle);
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "Angle of rotation: %d", angle);
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
        // Add delay, since it takes time for servo to rotate, usually 200ms/60degree rotation under 5V power supply
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}