#include "app.h"
#include <ColorSensor.h>
#include <Clock.h>
#include "util.h"
#include "ports_assignment.hpp"
#include "hsv_converter.hpp"

constexpr float RELOAD_TIME = 0.5; // 画面の更新間隔(秒)

char* numbetToColor(colorid_t colorNum) {
    char* colorStr[8] = {
        (char*)"NONE",
        (char*)"BLACK",
        (char*)"BLUE",
        (char*)"GREEN",
        (char*)"YELLOW",
        (char*)"RED",
        (char*)"WHITE",
        (char*)"BROWN"
    };

    return colorStr[colorNum];
}

void main_task(intptr_t unused) {
    int brightnessLine = 1;
    msg_f("Brightness", brightnessLine);
    // 2行目に反射光
    int rgbLine = brightnessLine + 2;
    msg_f("RGB", rgbLine);
    // 4,5,6行目にRGB
    int hsvLine = rgbLine + 4;
    msg_f("HSV", hsvLine);
    // 8,9,10行目にHSV
    int coloeNameLine = hsvLine + 4;
    msg_f("Color", coloeNameLine);
    //12行目に色判別

    ev3api::ColorSensor colorSensor(COLOR_SENSOR_PORT);
    ev3api::Clock clock;
    clock.reset();
    HsvConverter hsvConverter;
    char str[64];
    rgb_raw_t rgb;
    hsv_t hsv[3] = {0, 0, 0};

    bool doShowBrightness = false;
    while(true){
        if (1000 * RELOAD_TIME * 0.5 < clock.now() && !doShowBrightness) {
            // 反射光を取得して表示
            sprintf(str, "%d", colorSensor.getBrightness());
            msg_f(str, brightnessLine + 1);
            doShowBrightness = true;
        }

        if (1000 * RELOAD_TIME < clock.now() && doShowBrightness) {
            // RGBを取得して表示
            colorSensor.getRawColor(rgb);
            sprintf(str, "%d", rgb.r);
            msg_f(str, rgbLine + 1);
            sprintf(str, "%d", rgb.g);
            msg_f(str, rgbLine + 2);
            sprintf(str, "%d", rgb.b);
            msg_f(str, rgbLine + 3);

            // HSVを取得して表示
            hsvConverter.convertToHsv(rgb.r, rgb.g, rgb.b);
            hsvConverter.getHsv(hsv[0], hsv[1], hsv[2]);
            sprintf(str, "%f", hsv[0]);
            msg_f(str, hsvLine + 1);
            sprintf(str, "%f", hsv[1]);
            msg_f(str, hsvLine + 2);
            sprintf(str, "%f", hsv[2]);
            msg_f(str, hsvLine + 3);

            // 色判定
            msg_f(numbetToColor(colorSensor.getColorNumber()), coloeNameLine + 1);

            doShowBrightness = false;
            clock.reset();
        }
    }
}
