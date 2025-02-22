
//{تابع رید تاچ (اصلی)
int read_touch()
{
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

            if (touch_value_x > base_values_x[X] + TOUCH_THRESH && touch_value_y > base_values_y[Y] + TOUCH_THRESH && touch_[X][Y] == 0) //= بررسی لمس شدن تاچ
            {
                touch_[X][Y] = millis();                              //= تنظیم وضعیت لمس شده
                -Serial.printf(" %X  : TOUCH\n", touch_number[X][Y]); //= نمایش پیام لمس شده
            }

            if (touch_value_x < base_values_x[X] + TOUCH_THRESH && touch_value_y < base_values_y[Y] + TOUCH_THRESH && touch_[X][Y] > 0) //= بررسی اتمام تاچ
            {
                touch_long[X][Y] = false;     //=تنظیم وضعیت عدم تاچ طولانی
                touch_big_long[X][Y] = false; //=تنظیم وضعیت عدم ارور تاچ طولانی
                touch_Standard[X][Y] = false; //=تنظیم وضعیت عدم تاچ
                touch_[X][Y] = 0;             //= تنظیم وضعیت عدم لمس شده
                Touch_Two[X] = false;
                //- Serial.printf(" %X  : Touch removed\n", touch_number[X][Y]); //= نمایش پیام لمس برداشته شده
            }
            if (touch_value_x > base_values_x[X] + TOUCH_THRESH && touch_value_y > base_values_y[Y] + TOUCH_THRESH && millis() - touch_[X][Y] >= time_touch_Standard && !touch_Standard[X][Y]) //= برگشت دادن لمس شدن تاچ(برای جلوگیری از نویز اگه بعد تایم تاچ فعال بود برگرداند)
            {
                //- Serial.printf(" %X  : TOUCHED \n", touch_number[X][Y]); //= نمایش پیام طولانی لمس شده
                touch_Standard[X][Y] = true;                       //= تنظیم وضعیت تاچ شدن استاندارد
                return (Touched_Number << 4) + touch_number[X][Y]; //= برگشت مقدار تاچ شده
            }
            if (touch_value_x > base_values_x[X] + TOUCH_THRESH && touch_value_y > base_values_y[Y] + TOUCH_THRESH && millis() - touch_[X][Y] >= time_touch_long && !touch_long[X][Y]) //= بررسی لمس شدن طولانی تاچ
            {
                //- Serial.printf(" %X  : TOUCH LONG\n", touch_number[X][Y]); //= نمایش پیام طولانی لمس شده
                touch_long[X][Y] = true;                         //=تنظیم وضعیت تاچ طولانی
                return (Touched_Long << 4) + touch_number[X][Y]; //= برگشت مقدار تاچ طولانی
            }
            if (touch_value_x > base_values_x[X] + TOUCH_THRESH && touch_value_y > base_values_y[Y] + TOUCH_THRESH && millis() - touch_[X][Y] >= time_touch_big_long && !touch_big_long[X][Y]) //= بررسی خطا لمس شدن تاچ
            {
                //- Serial.printf(" %X  : ERROR!!! TOUCH BIG LONG\n", touch_number[X][Y]); //= نمایش پیام خطا لمس شده
                touch_big_long[X][Y] = true;                             //= تنظیم وضعیت فعال سازی ارور تاچ طولانی
                return (Error_Touch_Big_Long << 4) + touch_number[X][Y]; //= برگشت ارور طولانی شدن تاچ
            }

            if (touch_long[X][0] && touch_long[X][2] && !Touch_Two[X]) //= بررسی دو تاچ شدن
            {
                Touch_Two[X] = true;           //= تنظیم وضعیت دو تاچ شدن
                return (Two_Touched << 4) + X; //= برگشت دادن دو تاچ
            }

            if (touch_value_x > TOUCH_THRESH_FALS || touch_value_y > TOUCH_THRESH_FALS || Equal_touch >= 16) //= بررسی شرایط خاص تاچ
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
    return NO_Number << 4; //= برگشت عدم تاچ
}

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

//{ ستاپ تاچ
void Setup_touch()
{

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

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER); //= فعال‌سازی تایمر داخلی تاچ
    touch_pad_fsm_start();                        //= شروع تایمر داخلی تاچ
    delay(1000);                                  //= تاخیر برای اطمینان از شروع صحیح
    calibrate_touch();                            //= کالیبراسیون تاچ
    // touch_pad_timeout_set(true, 100000);
}
