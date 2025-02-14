#include "driver/touch_pad.h"

#define TOUCH_PAD_PIN1 TOUCH_PAD_NUM1   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN2 TOUCH_PAD_NUM2   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN3 TOUCH_PAD_NUM3   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN4 TOUCH_PAD_NUM4   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN5 TOUCH_PAD_NUM5   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN6 TOUCH_PAD_NUM6   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN7 TOUCH_PAD_NUM7   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN8 TOUCH_PAD_NUM8   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN9 TOUCH_PAD_NUM9   // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN10 TOUCH_PAD_NUM10 // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN11 TOUCH_PAD_NUM11 // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN12 TOUCH_PAD_NUM12 // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN13 TOUCH_PAD_NUM13 // شماره‌ی پین تاچ
#define TOUCH_PAD_PIN14 TOUCH_PAD_NUM14 // شماره‌ی پین تاچ

int TT0 = 0;
int TT1 = 0;
int TT2 = 0;
int TT3 = 0;
int TT4 = 0;
int TT5 = 0;
int TT6 = 0;
int TT7 = 0;
int TT8 = 0;
int TT9 = 0;
int TT10 = 0;
int TT11 = 0;
int TT12 = 0;
int TT13 = 0;
int TT14 = 0;

int counter;

void setup()
{
    Serial.begin(115200);

    touch_pad_init();
    touch_pad_config(TOUCH_PAD_PIN1);
    touch_pad_config(TOUCH_PAD_PIN2);
    touch_pad_config(TOUCH_PAD_PIN3);
    touch_pad_config(TOUCH_PAD_PIN4);
    touch_pad_config(TOUCH_PAD_PIN5);
    touch_pad_config(TOUCH_PAD_PIN6);
    touch_pad_config(TOUCH_PAD_PIN7);
    touch_pad_config(TOUCH_PAD_PIN8);
    touch_pad_config(TOUCH_PAD_PIN9);
    touch_pad_config(TOUCH_PAD_PIN10);
    touch_pad_config(TOUCH_PAD_PIN11);
    touch_pad_config(TOUCH_PAD_PIN12);
    touch_pad_config(TOUCH_PAD_PIN13);
    touch_pad_config(TOUCH_PAD_PIN14);

    touch_pad_fsm_start();
}

void loop()
{
    uint32_t value_TOUCH1;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH2;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH3;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH4;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH5;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH6;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH7;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH8;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH9;  // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH10; // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH11; // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH12; // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH13; // نوع متغیر را به uint32_t تغییر دادیم
    uint32_t value_TOUCH14; // نوع متغیر را به uint32_t تغییر دادیم

    touch_pad_read_raw_data(TOUCH_PAD_PIN1, &value_TOUCH1);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN2, &value_TOUCH2);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN3, &value_TOUCH3);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN4, &value_TOUCH4);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN5, &value_TOUCH5);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN6, &value_TOUCH6);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN7, &value_TOUCH7);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN8, &value_TOUCH8);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN9, &value_TOUCH9);   // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN10, &value_TOUCH10); // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN11, &value_TOUCH11); // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN12, &value_TOUCH12); // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN13, &value_TOUCH13); // مقدار خام را بخوان
    touch_pad_read_raw_data(TOUCH_PAD_PIN14, &value_TOUCH14); // مقدار خام را بخوان
    Serial.print(" T1   ");
    Serial.println(value_TOUCH1 - TT1);
    Serial.print(" T2   ");
    Serial.println(value_TOUCH2 - TT2);
    Serial.print(" T3   ");
    Serial.println(value_TOUCH3 - TT3);
    Serial.print(" T4   ");
    Serial.println(value_TOUCH4 - TT4);
    Serial.print(" T5   ");
    Serial.println(value_TOUCH5 - TT5);
    Serial.print(" T6   ");
    Serial.println(value_TOUCH6 - TT6);
    Serial.print(" T7   ");
    Serial.println(value_TOUCH7 - TT7);
    Serial.print(" T8   ");
    Serial.println(value_TOUCH8 - TT8);
    Serial.print(" T9   ");
    Serial.println(value_TOUCH9 - TT9);
    Serial.print(" T10  ");
    Serial.println(value_TOUCH10 - TT10);
    Serial.print(" T11  ");
    Serial.println(value_TOUCH11 - TT11);
    Serial.print(" T12  ");
    Serial.println(value_TOUCH12 - TT12);
    Serial.print(" T13  ");
    Serial.println(value_TOUCH13 - TT13);
    Serial.print(" T14  ");
    Serial.println(value_TOUCH14 - TT14);
    Serial.println(" ");
    delay(100);

    counter++;
    uint32_t m = micros();
    if (counter >= 10)
    {
        // Serial.println("Resetting touch sensors...");
        touch_pad_deinit(); // غیر فعال کردن ماژول تاچ
        // delay(50);           // مکث کوتاه
        touch_pad_init(); // دوباره مقداردهی اولیه
        touch_pad_config(TOUCH_PAD_PIN1);
        touch_pad_config(TOUCH_PAD_PIN2);
        touch_pad_config(TOUCH_PAD_PIN3);
        touch_pad_config(TOUCH_PAD_PIN4);
        touch_pad_config(TOUCH_PAD_PIN5);
        touch_pad_config(TOUCH_PAD_PIN6);
        touch_pad_config(TOUCH_PAD_PIN7);
        touch_pad_config(TOUCH_PAD_PIN8);
        touch_pad_config(TOUCH_PAD_PIN9);
        touch_pad_config(TOUCH_PAD_PIN10);
        touch_pad_config(TOUCH_PAD_PIN11);
        touch_pad_config(TOUCH_PAD_PIN12);
        touch_pad_config(TOUCH_PAD_PIN13);
        touch_pad_config(TOUCH_PAD_PIN14);
        touch_pad_fsm_start();
        counter = 0;
    }
    Serial.println(micros() - m);
}
