#include "settings.h"
#include "Function_matrix3-4.h"

void setup()
{
    Serial.begin(115200); //= شروع ارتباط سریال با نرخ 115200
    delay(1000);          //= تاخیر برای اطمینان از شروع صحیح

    Serial.println("Setup touch pad...");
    Setup_touch(); //= ستاپ تاچ بد
}

void loop()
{

    if (millis() - lastReadTime >= readInterval) //= بررسی زمان خواندن مجدد
    {
        lastReadTime = millis();
        get_read_touch = read_touch();           //= مقدار دریافتی از تابع
        main_part = (get_read_touch >> 4) & 0xF; //= استخراج پارت
        Touched_Value = get_read_touch & 0xF;    //= استخراج مقدار تاچ

        switch (main_part)
        {
        case Touched_Number:
            Serial.printf(" %X  : TOUCHED \n", Touched_Value); //= نمایش پیام لمس شده
            break;

        case Touched_Long:
            Serial.printf(" %X  : TOUCH LONG \n", Touched_Value); //= نمایش پیام لمس طولانی شده
            break;

        case Error_Touch_Big_Long:
            Serial.printf(" %X  : ERROR!!! TOUCH BIG LONG \n", Touched_Value); //= نمایش پیام خطا لمس شده
            break;

        case Two_Touched:
            Serial.printf(" %X  : TOUCHED TWO \n", Touched_Value); //= نمایش پیام دوتاچ لمس شده(دوتاچ یا 1و3 یا 4و6 یا 7و9 یا AوB )
            break;

        case NO_Number:
            // Serial.printf("NO TOUCH\n"); //= نمایش پیام عدم لمس
            break;

        default:
            break;
        }
    }
}
