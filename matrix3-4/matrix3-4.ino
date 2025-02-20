//!!! مشکل کالیبراسیون
//!!! تاچ آب

//!!! هنگ کردن و از کار افتادن (تقریبا اوکی شده)

#include <Arduino.h>
#include "driver/touch_pad.h"

#define TOUCH_BUTTON_NUM_X 4 //= تعداد تاچ ردیف
#define TOUCH_BUTTON_NUM_Y 3 //= تعداد تاچ ستون
#define TOUCH_THRESH 1500    //= مقدار آستانه برای تشخیص لمس (بسته به طراحی مدار، تنظیم شود)

/* آرایه‌ی پدهای تاچ */
static const touch_pad_t Button_X[TOUCH_BUTTON_NUM_X] = {TOUCH_PAD_NUM8, TOUCH_PAD_NUM14, TOUCH_PAD_NUM11, TOUCH_PAD_NUM9}; //=تاچ نامبر های ردیف
static const touch_pad_t Button_Y[TOUCH_BUTTON_NUM_Y] = {TOUCH_PAD_NUM10, TOUCH_PAD_NUM13, TOUCH_PAD_NUM12};                //= تاچ نامبر های ستون

//{ مقدار پایه تاچ(برای کالیبراسیون)
static uint32_t base_values_x[TOUCH_BUTTON_NUM_X];
static uint32_t base_values_y[TOUCH_BUTTON_NUM_Y];
//}

uint32_t touch_value_x_old[TOUCH_BUTTON_NUM_X]; //= مقدار خوانده شده تاچ ردیف
uint32_t touch_value_y_old[TOUCH_BUTTON_NUM_Y]; //= مقدار خوانده شده تاچ ستون
int Equal_touch = 0;

//{مقدار پیش فرض هر بلوک تاچ
uint8_t touch_number[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'-', '0', '+'}};
//}

bool touch_[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y]; //=وضعیت بلوک تاچ (لمس شده یا نه)

uint32_t touch_value_x; //= مقدار خوانده شده تاچ ردیف
uint32_t touch_value_y; //= مقدار خوانده شده تاچ ستون
uint32_t touch_value;   //= مقدار خوانده شده تاچ

static unsigned long lastReadTime = 0; //= آخرین تایم که تاچ ها خوانده شده
const unsigned long readInterval = 45; //= هر 200 میلی‌ثانیه یکبار خواندن
//{کالیبراسیون تاچ
void calibrate_touch()
{
    Serial.print("benchmark values: ");
    for (int X = 0; X < TOUCH_BUTTON_NUM_X; X++) //{خواندن و ریختن مقدار تاچ های ردیف در متغیر مقدار اولیه تاچ های ردیف
    {
        touch_pad_read_benchmark(Button_X[X], &touch_value);    //= خواندن و ریختن مقدار تاچ
        Serial.printf("T%d: [%4u] ", Button_X[X], touch_value); //= نمایش مقدار تاچ
        base_values_x[X] = touch_value;                         //= ریختن در مقدار اولیه
        // touch_pad_set_thresh(button[i], touch_value + button_threshold[i]); //=تنظیم مقدار فعال شدن تاچ
    }
    for (int Y = 0; Y < TOUCH_BUTTON_NUM_Y; Y++) //{خواندن و ریختن مقدار تاچ های ستون در متغیر مقدار اولیه تاچ های ستون
    {
        touch_pad_read_benchmark(Button_Y[Y], &touch_value);    //= خواندن و ریختن مقدار تاچ
        Serial.printf("T%d: [%4u] ", Button_Y[Y], touch_value); //= نمایش مقدار تاچ
        base_values_y[Y] = touch_value;                         //= ریختن در مقدار اولیه
        // touch_pad_set_thresh(button[i], touch_value + button_threshold[i]); //=تنظیم مقدار فعال شدن تاچ
    }
    Serial.println();
    Serial.println("!! calibrasion !!");
}

void setup()
{
    Serial.begin(115200); //= شروع ارتباط سریال با نرخ 115200
    delay(1000);          //= تاخیر برای اطمینان از شروع صحیح

    Serial.println("Initializing touch pad...");
    touch_pad_init(); //= مقداردهی اولیه‌ی پدهای تاچ

    //{ مقداردهی اولیه‌ی پدهای تاچ
    for (int X = 0; X < TOUCH_BUTTON_NUM_X; X++)
    {
        touch_pad_config(Button_X[X]);                                                     //= تنظیم پد تاچ ردیف
        touch_pad_set_cnt_mode(Button_X[X], TOUCH_PAD_SLOPE_7, TOUCH_PAD_TIE_OPT_DEFAULT); //= تنظیم حساسیت پد تاچ
    }
    for (int Y = 0; Y < TOUCH_BUTTON_NUM_Y; Y++)
    {
        touch_pad_config(Button_Y[Y]);                                                     //= تنظیم پد تاچ ستون
        touch_pad_set_cnt_mode(Button_Y[Y], TOUCH_PAD_SLOPE_7, TOUCH_PAD_TIE_OPT_DEFAULT); //= تنظیم حساسیت پد تاچ
    }
    //}

    //{ تنظیمات کاهش نویز
    touch_pad_denoise_t denoise = {
        .grade = TOUCH_PAD_DENOISE_BIT4,
        .cap_level = TOUCH_PAD_DENOISE_CAP_L7,
    };
    touch_pad_denoise_set_config(&denoise); //= تنظیم کاهش نویز
    touch_filter_config_t filter_config = {
        .mode = TOUCH_PAD_FILTER_IIR_16,
        .debounce_cnt = 7,
        .noise_thr = 2,
        .jitter_step = 4,
        .smh_lvl = TOUCH_PAD_SMOOTH_IIR_2};
    touch_pad_filter_set_config(&filter_config); //= تنظیم فیلتر تاچ

    touch_pad_denoise_enable(); //= فعال‌سازی کاهش نویز
    touch_pad_filter_enable();  //= فعال‌سازی فیلتر
    calibrate_touch();          //= کالیبراسیون تاچ

    Serial.println("Touch pad initialized with denoise.");

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER); //= فعال‌سازی تایمر داخلی تاچ
    touch_pad_fsm_start();                        //= شروع تایمر داخلی تاچ
    delay(1000);                                  //= تاخیر برای اطمینان از شروع صحیح
    calibrate_touch();                            //= کالیبراسیون تاچ
    // touch_pad_timeout_set(true, 100000);
}

void loop()
{

    if (millis() - lastReadTime >= readInterval) //= بررسی زمان خواندن مجدد
    {
        lastReadTime = millis();
        for (uint8_t X = 0; X < TOUCH_BUTTON_NUM_X; X++) //= حلقه برای خواندن مقادیر تاچ ردیف
        {
            touch_pad_filter_read_smooth(Button_X[X], &touch_value_x); //= خواندن مقدار تاچ ردیف
            if (touch_value_x == touch_value_x_old[X])                 //= بررسی مقدار تاچ یکسان
                Equal_touch++;                                         //= افزایش شمارنده تاچ یکسان
            else
            {
                touch_value_x_old[X] = touch_value_x; //= به‌روزرسانی مقدار تاچ قدیمی
                Equal_touch = 0;                      //= تنظیم شمارنده تاچ یکسان به صفر
            }
            for (uint8_t Y = 0; Y < TOUCH_BUTTON_NUM_Y; Y++) //= حلقه برای خواندن مقادیر تاچ ستون
            {
                touch_pad_filter_read_smooth(Button_Y[Y], &touch_value_y); //= خواندن مقدار تاچ ستون
                if (touch_value_y == touch_value_y_old[Y])                 //= بررسی مقدار تاچ یکسان
                    Equal_touch++;                                         //= افزایش شمارنده تاچ یکسان
                else
                {
                    touch_value_y_old[Y] = touch_value_y; //= به‌روزرسانی مقدار تاچ قدیمی
                    Equal_touch = 0;                      //= تنظیم شمارنده تاچ یکسان به صفر
                }

                if (touch_value_x > base_values_x[X] + TOUCH_THRESH && !touch_[X][Y]) //= بررسی لمس شدن تاچ ردیف
                {
                    if (touch_value_y > base_values_y[Y] + TOUCH_THRESH && !touch_[X][Y]) //= بررسی لمس شدن تاچ ستون
                    {
                        touch_[X][Y] = true;                                //= تنظیم وضعیت لمس شده
                        Serial.printf("%c: TOUCHED\n", touch_number[X][Y]); //= نمایش پیام لمس شده
                    }
                }

                if (touch_value_x < base_values_x[X] + TOUCH_THRESH && touch_[X][Y]) //= بررسی عدم لمس شدن تاچ ردیف
                {
                    if (touch_value_y < base_values_y[Y] + TOUCH_THRESH && touch_[X][Y]) //= بررسی عدم لمس شدن تاچ ستون
                    {
                        touch_[X][Y] = false;                                     //= تنظیم وضعیت عدم لمس شده
                        Serial.printf("%c: Touch removed\n", touch_number[X][Y]); //= نمایش پیام لمس برداشته شده
                    }
                }
                if (touch_value_x > 200000 || touch_value_y > 200000 || Equal_touch >= 16) //= بررسی شرایط خاص تاچ
                {
                    // if (Equal_touch >= 16)
                    // Serial.print("Equal_touch : ");
                    // Serial.println(Equal_touch);
                    Equal_touch = 0;            //= تنظیم شمارنده تاچ یکسان به صفر
                    touch_value_x = 0;          //= تنظیم مقدار تاچ ردیف به صفر
                    touch_value_y = 0;          //= تنظیم مقدار تاچ ستون به صفر
                    touch_pad_timeout_resume(); //= از سرگیری تایم اوت تاچ
                    touch_pad_fsm_stop();       //= توقف تایمر داخلی تاچ
                    touch_pad_reset();          //= ریست تاچ پد
                    touch_pad_fsm_start();      //= شروع تایمر داخلی تاچ
                    touch_pad_timeout_resume(); //= از سرگیری تایم اوت تاچ
                }
            }
        }
    }
}
