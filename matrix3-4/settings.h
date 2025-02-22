#include <Arduino.h>
#include "driver/touch_pad.h"

#define NO_Number 0xA
#define Touched_Number 0xB
#define Touched_Long 0xC
#define Error_Touch_Big_Long 0xD

#define TOUCH_BUTTON_NUM_X 4     //= تعداد تاچ ردیف
#define TOUCH_BUTTON_NUM_Y 3     //= تعداد تاچ ستون
#define time_touch_big_long 4000 //= تایم که بعد آن تشخیص طولانی بودن غیر عادی داده میشود
#define time_touch_long 2000     //=تابم تاچ طولانی
#define time_touch_Standard 250  //=تابم تاچ طولانی
#define TOUCH_THRESH 1500        //= مقدار آستانه برای تشخیص لمس (بسته به طراحی مدار، تنظیم شود)
#define TOUCH_THRESH_FALS 200000 //= مقدار آستانه خطا

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

int get_read_touch; //= مقدار دریافتی از تابع
int main_part;      //= پارت تاچ
int Touched_Value;  //= مقدار تاچ شده

//{مقدار پیش فرض هر بلوک تاچ
int touch_number[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9},
    {0x0B, 0, 0x0A}};
//}

static unsigned long touch_[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y]; //=وضعیت بلوک تاچ (لمس شده یا نه)
bool touch_long[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y];             //=وضعیت لانگ تاچ
bool touch_big_long[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y];         //=وضعیت لانگ تاچ
bool touch_Standard[TOUCH_BUTTON_NUM_X][TOUCH_BUTTON_NUM_Y];         //=وضعیت لانگ تاچ

uint32_t touch_value_x; //= مقدار خوانده شده تاچ ردیف
uint32_t touch_value_y; //= مقدار خوانده شده تاچ ستون
uint32_t touch_value;   //= مقدار خوانده شده تاچ

static unsigned long lastReadTime = 0; //= آخرین تایم که تاچ ها خوانده شده
const unsigned long readInterval = 45; //= هر 200 میلی‌ثانیه یکبار خواندن

int read_touch();       //= تابع رید تاچ (اصلی)
void calibrate_touch(); //=کالیبراسیون تاچ
void Setup_touch();     //= ستاپ تاچ
