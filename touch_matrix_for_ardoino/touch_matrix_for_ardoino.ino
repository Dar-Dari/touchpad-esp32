#include <Arduino.h>
#include "touch_element/touch_matrix.h"

#define X_AXIS_CHANNEL_NUM 3
#define Y_AXIS_CHANNEL_NUM 4

static touch_matrix_handle_t matrix_handle;

const touch_pad_t x_axis_channel[X_AXIS_CHANNEL_NUM] = {TOUCH_PAD_NUM9, TOUCH_PAD_NUM10, TOUCH_PAD_NUM11};
const touch_pad_t y_axis_channel[Y_AXIS_CHANNEL_NUM] = {TOUCH_PAD_NUM12, TOUCH_PAD_NUM13, TOUCH_PAD_NUM14, TOUCH_PAD_NUM2};
const float x_axis_channel_sens[X_AXIS_CHANNEL_NUM] = {0.01, 0.01, 0.01};
const float y_axis_channel_sens[Y_AXIS_CHANNEL_NUM] = {0.01, 0.01, 0.01, 0.01};

void matrix_handler(touch_matrix_handle_t out_handle, touch_matrix_message_t *out_message, void *arg) {
    if (out_handle != matrix_handle) return;
    Serial.print("Matrix ");
    if (out_message->event == TOUCH_MATRIX_EVT_ON_PRESS) Serial.print("Press, ");
    else if (out_message->event == TOUCH_MATRIX_EVT_ON_RELEASE) Serial.print("Release, ");
    else if (out_message->event == TOUCH_MATRIX_EVT_ON_LONGPRESS) Serial.print("LongPress, ");
    Serial.print("axis: (");
    Serial.print(out_message->position.x_axis);
    Serial.print(", ");
    Serial.print(out_message->position.y_axis);
    Serial.print(") index: ");
    Serial.println(out_message->position.index);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing Touch Matrix...");

    touch_elem_global_config_t global_config = TOUCH_ELEM_GLOBAL_DEFAULT_CONFIG();
    touch_element_install(&global_config);

    touch_matrix_global_config_t matrix_global_config = TOUCH_MATRIX_GLOBAL_DEFAULT_CONFIG();
    touch_matrix_install(&matrix_global_config);

    touch_matrix_config_t matrix_config = {
        .x_channel_array = x_axis_channel,
        .y_channel_array = y_axis_channel,
        .x_sensitivity_array = x_axis_channel_sens,
        .y_sensitivity_array = y_axis_channel_sens,
        .x_channel_num = X_AXIS_CHANNEL_NUM,
        .y_channel_num = Y_AXIS_CHANNEL_NUM
    };
    touch_matrix_create(&matrix_config, &matrix_handle);
    touch_matrix_subscribe_event(matrix_handle, TOUCH_ELEM_EVENT_ON_PRESS | TOUCH_ELEM_EVENT_ON_RELEASE | TOUCH_ELEM_EVENT_ON_LONGPRESS, NULL);
    touch_matrix_set_dispatch_method(matrix_handle, TOUCH_ELEM_DISP_CALLBACK);
    touch_matrix_set_callback(matrix_handle, matrix_handler);
    touch_element_start();
    Serial.println("Touch Matrix Ready.");
}

void loop() {
    delay(100); // جلوگیری از خواندن بیش از حد تاچ سنسورها
}
