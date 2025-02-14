#include <Arduino.h>          //= کتابخانه آردوینو
#include "driver/touch_pad.h" //= کتابخانه تاچ پد

#define TOUCH_BUTTON_NUM 14          //= تعداد پدهای تاچ
#define RECALIBRATION_INTERVAL 10000 //= هر 10 ثانیه یکبار کالیبراسیون می‌شود

//{ تعریف پدهای تاچ
static const touch_pad_t button[TOUCH_BUTTON_NUM] = {
    TOUCH_PAD_NUM1, TOUCH_PAD_NUM2, TOUCH_PAD_NUM3, TOUCH_PAD_NUM4, TOUCH_PAD_NUM5, TOUCH_PAD_NUM6, TOUCH_PAD_NUM7,
    TOUCH_PAD_NUM8, TOUCH_PAD_NUM9, TOUCH_PAD_NUM10, TOUCH_PAD_NUM11, TOUCH_PAD_NUM12, TOUCH_PAD_NUM13, TOUCH_PAD_NUM14};

//{ تعریف آستانه‌های تاچ
static float button_threshold[TOUCH_BUTTON_NUM] = {
    3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000,
    3000, 3000, 3000, 3000, 3000, 3000};

static uint32_t base_values[TOUCH_BUTTON_NUM]; //= مقادیر پایه‌ای تاچ
unsigned long last_recalibration = 0;          //= زمان آخرین کالیبراسیون

//{ تابع کالیبراسیون تاچ
void calibrate_touch()
{
    Serial.print("benchmark values: "); //= چاپ مقادیر پایه‌ای
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        uint32_t touch_value;
        touch_pad_read_benchmark(button[i], &touch_value);                  //= خواندن مقدار پایه‌ای تاچ
        Serial.printf("T%d: [%4u] ", button[i], touch_value);               //= چاپ مقدار پایه‌ای
        base_values[i] = touch_value;                                       //= ذخیره مقدار پایه‌ای
        touch_pad_set_thresh(button[i], touch_value + button_threshold[i]); //= تنظیم آستانه تاچ
    }
    Serial.println();
    Serial.println("!! calibrasion !!"); //= پیام کالیبراسیون
}

//{ تابع اینتراپت تاچ
void touchsensor_interrupt_cb(void *arg)
{
    uint32_t pad_status = touch_pad_get_status(); //= خواندن وضعیت تاچ
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        if ((pad_status >> button[i]) & 0x01) //= بررسی وضعیت تاچ
        {
            Serial.printf("BUTTON %d TOUCHED \n", button[i]); //= چاپ پیام لمس شدن
        }
    }
    touch_pad_clear_status(); //= پاک کردن وضعیت تاچ
}

void setup()
{
    Serial.begin(115200);        //= شروع ارتباط سریال با نرخ 115200
    Serial.println("SETUP...."); //= پیام شروع تنظیمات

    touch_pad_init(); //= مقداردهی اولیه‌ی پدهای تاچ
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        touch_pad_config(button[i]);                                                     //= تنظیم پد تاچ
        touch_pad_set_cnt_mode(button[i], TOUCH_PAD_SLOPE_7, TOUCH_PAD_TIE_OPT_DEFAULT); //= تنظیم حالت شمارش پد تاچ
    }
    //{ تنظیم فیلتر
    touch_filter_config_t filter_config = {
        .mode = TOUCH_PAD_FILTER_IIR_16,
        .debounce_cnt = 7,
        .noise_thr = 2,
        .jitter_step = 4,
        .smh_lvl = TOUCH_PAD_SMOOTH_IIR_2};
    Serial.println(touch_pad_filter_set_config(&filter_config)); //= تنظیم فیلتر تاچ
    touch_pad_filter_enable();                                   //= فعال‌سازی فیلتر
    calibrate_touch();                                           //= کالیبراسیون تاچ

    touch_pad_isr_register(touchsensor_interrupt_cb, NULL, TOUCH_PAD_INTR_MASK_ALL);                                //= ثبت تابع اینتراپت تاچ
    touch_pad_intr_enable(TOUCH_PAD_INTR_MASK_ACTIVE | TOUCH_PAD_INTR_MASK_INACTIVE | TOUCH_PAD_INTR_MASK_TIMEOUT); //=فعال سازی تاچ
    touch_pad_fsm_start();                                                                                          //= شروع تایمر داخلی تاچ
    calibrate_touch();                                                                                              //= کالیبراسیون تاچ
}

void loop()
{
    //{ بررسی زمان کالیبراسیون مجدد
    if (millis() - last_recalibration > RECALIBRATION_INTERVAL)
    {
        // calibrate_touch(); //= کالیبراسیون مجدد
        last_recalibration = millis(); //= به‌روزرسانی زمان آخرین کالیبراسیون
    }
}
