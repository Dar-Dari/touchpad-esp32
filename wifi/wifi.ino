//!!! مشکل کالیبراسیون
//!!! تاچ آب

#include "WiFi.h"
#include <Arduino.h>          //= کتابخانه آردوینو
#include "driver/touch_pad.h" //= کتابخانه تاچ پد

#define TOUCH_BUTTON_NUM 14 //= تعداد پدهای تاچ

/* آرایه‌ی پدهای تاچ */
static const touch_pad_t button[TOUCH_BUTTON_NUM] = {
    TOUCH_PAD_NUM1, TOUCH_PAD_NUM2, TOUCH_PAD_NUM3, TOUCH_PAD_NUM4,
    TOUCH_PAD_NUM5, TOUCH_PAD_NUM6, TOUCH_PAD_NUM7, TOUCH_PAD_NUM8,
    TOUCH_PAD_NUM9, TOUCH_PAD_NUM10, TOUCH_PAD_NUM11, TOUCH_PAD_NUM12,
    TOUCH_PAD_NUM13, TOUCH_PAD_NUM14};

static uint32_t base_values[TOUCH_BUTTON_NUM]; //= مقادیر پایه‌ای تاچ

//{ تابع کالیبراسیون تاچ
void calibrate_touch()
{
    Serial.print("benchmark values: "); //= چاپ مقادیر پایه‌ای
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        uint32_t touch_value;
        touch_pad_read_benchmark(button[i], &touch_value);    //= خواندن مقدار پایه‌ای تاچ
        Serial.printf("T%d: [%4u] ", button[i], touch_value); //= چاپ مقدار پایه‌ای
        base_values[i] = touch_value;                         //= ذخیره مقدار پایه‌ای
        // touch_pad_set_thresh(button[i], touch_value + button_threshold[i]);
    }
    Serial.println();
    Serial.println("!! calibrasion !!"); //= پیام کالیبراسیون
}

void setup()
{
    Serial.begin(115200); //= شروع ارتباط سریال با نرخ 115200
    delay(1000);          //= تاخیر برای اطمینان از شروع صحیح

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Initializing touch pad..."); //= پیام شروع تنظیمات
    touch_pad_init();                            //= مقداردهی اولیه‌ی پدهای تاچ

    //= مقداردهی اولیه‌ی پدهای تاچ
    for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
    {
        touch_pad_config(button[i]);                                                     //= تنظیم پد تاچ
        touch_pad_set_cnt_mode(button[i], TOUCH_PAD_SLOPE_7, TOUCH_PAD_TIE_OPT_DEFAULT); //= تنظیم حالت شمارش پد تاچ
    }

    //= تنظیمات کاهش نویز
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

    touch_pad_waterproof_t touch_pad_waterproof = {
        .guard_ring_pad = TOUCH_PAD_NUM1,
        .shield_driver = TOUCH_PAD_SHIELD_DRV_L7,

    };
    touch_pad_waterproof_get_config(&touch_pad_waterproof);
    touch_pad_waterproof_enable();

    touch_pad_denoise_enable(); //= فعال‌سازی کاهش نویز
    touch_pad_filter_enable();  //= فعال‌سازی فیلتر
    calibrate_touch();          //= کالیبراسیون تاچ

    // Serial.println("Touch pad initialized with denoise."); //= پیام اتمام تنظیمات

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER); //= فعال‌سازی تایمر داخلی تاچ
    touch_pad_fsm_start();                        //= شروع تایمر داخلی تاچ
    delay(1000);                                  //= تاخیر برای اطمینان از شروع صحیح
    calibrate_touch();                            //= کالیبراسیون تاچ
}

void loop()
{
    Serial.println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
      Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
      Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.printf("%2d", i + 1);
        Serial.print(" | ");
        Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
        Serial.print(" | ");
        Serial.printf("%4ld", WiFi.RSSI(i));
        Serial.print(" | ");
        Serial.printf("%2ld", WiFi.channel(i));
        Serial.print(" | ");
        switch (WiFi.encryptionType(i)) {
          case WIFI_AUTH_OPEN: Serial.print("open"); break;
          case WIFI_AUTH_WEP: Serial.print("WEP"); break;
          case WIFI_AUTH_WPA_PSK: Serial.print("WPA"); break;
          case WIFI_AUTH_WPA2_PSK: Serial.print("WPA2"); break;
          case WIFI_AUTH_WPA_WPA2_PSK: Serial.print("WPA+WPA2"); break;
          case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
          case WIFI_AUTH_WPA3_PSK: Serial.print("WPA3"); break;
          case WIFI_AUTH_WPA2_WPA3_PSK: Serial.print("WPA2+WPA3"); break;
          case WIFI_AUTH_WAPI_PSK: Serial.print("WAPI"); break;
          default: Serial.print("unknown");
        }
        Serial.println();
        delay(1);
      }
    }
    Serial.println("");
  
    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    //////////////////////////////////////////////////////////

    static unsigned long lastReadTime = 0;  //= زمان آخرین خواندن
    const unsigned long readInterval = 200; //= هر 200 میلی‌ثانیه یکبار خواندن

    // if (touch_pad_meas_is_done()) //= بررسی اتمام اندازه‌گیری تاچ
    {
        if (millis() - lastReadTime >= readInterval) //= بررسی زمان خواندن مجدد
        {
            lastReadTime = millis(); //= به‌روزرسانی زمان آخرین خواندن
            // uint32_t noise_value;
            // touch_pad_denoise_read_data(&noise_value);     //= خواندن مقدار تاچ با کاهش نویز
            // Serial.printf("noise: [%4u] \n", noise_value); //= چاپ مقدار نویز

            Serial.print("Filter values: "); //= چاپ مقادیر فیلتر شده
            for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
            {
                uint32_t touch_value;
                touch_pad_filter_read_smooth(button[i], &touch_value); //= خواندن مقدار فیلتر شده‌ی تاچ
                Serial.printf("T%d: [%4u] ", button[i], touch_value);  //= چاپ مقدار فیلتر شده
            }
            Serial.println();
            // Serial.print("Touchpad values: "); //= چاپ مقادیر تاچ
            // for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
            // {
            //     uint32_t touch_value;
            //     touch_pad_read_raw_data(button[i], &touch_value);     //= خواندن مقدار خام تاچ
            //     Serial.printf("T%d: [%4u] ", button[i], touch_value); //= چاپ مقدار خام
            // }
            // Serial.println();
            // Serial.print("benchmark values: "); //= چاپ مقادیر پایه‌ای
            // for (int i = 0; i < TOUCH_BUTTON_NUM; i++)
            // {
            //     uint32_t touch_value;
            //     touch_pad_read_benchmark(button[i], &touch_value);    //= خواندن مقدار پایه‌ای تاچ
            //     Serial.printf("T%d: [%4u] ", button[i], touch_value); //= چاپ مقدار پایه‌ای
            // }
            // Serial.println();
        }
    }
}

// touch_pad_proximity_set_count()  برای تنظیم تشخیص مجاورت
// touch_pad_set_thresh() برای تنظیم آستانه‌ی تاچ
// touch_pad_read_benchmark() برای خواندن مقدار پایه‌ای تاچ`
// touch_pad_read_raw_data() برای خواندن مقدار خام تاچ
// touch_pad_filter_read_smooth() برای خواندن مقدار فیلتر شده‌ی تاچ
// touch_pad_denoise_read_data() برای خواندن مقدار تاچ با کاهش نویز
// touch_pad_get_status() برای خواندن وضعیت تاچ
// touch_pad_clear_status() برای پاک کردن وضعیت تاچ
// touch_pad_intr_enable() برای فعال‌سازی اینتراپت‌های تاچ
// touch_pad_intr_disable() برای غیرفعال‌سازی اینتراپت‌های تاچ
// touch_pad_isr_register() برای ثبت تابع اینتراپت تاچ
// touch_pad_set_fsm_mode() برای تنظیم حالت تایمر داخلی تاچ
// touch_pad_fsm_start() برای فعال‌سازی تایمر داخلی تاچ
// touch_pad_fsm_stop() برای غیرفعال‌سازی تایمر داخلی تاچ
// touch_pad_set_cnt_mode() برای تنظیم حالت شمارنده‌ی تاچ
// touch_pad_set_meas_time() برای تنظیم زمان اندازه‌گیری تاچ
// touch_pad_set_meas_interval() برای تنظیم فاصله‌ی اندازه‌گیری تاچ
// touch_pad_set_thresh() برای تنظیم آستانه‌ی تاچ
// touch_pad_set_cnt_thres() برای تنظیم آستانه‌ی شمارنده‌ی تاچ
// touch_pad_set_cnt_measure_time() برای تنظیم زمان اندازه‌گیری شمارنده‌ی تاچ
// touch_pad_set_cnt_measure_interval() برای تنظیم فاصله‌ی اندازه‌گیری شمارنده‌ی تاچ
// touch_pad_set_cnt_measure_filter() برای تنظیم فیلتر اندازه‌گیری شمارنده‌ی تاچ
// touch_pad_clear_status() برای پاک کردن وضعیت لمس استفاده می‌شود.
// touch_pad_io_init() برای مقداردهی اولیه‌ی پایه‌های تاچ استفاده می‌شود.
// touch_pad_read() برای خواندن مقدار تاچ استفاده می‌شود.
// touch_pad_read_filtered() برای خواندن مقدار فیلتر شده‌ی تاچ استفاده می‌شود.
