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
    rgb_raw_t rgb;
    hsv_t hsv[3] = {0, 0, 0};

    // see https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki/Spec
    const float redCorrection   = 255.0 / 329.0;
    const float greenCorrection = 255.0 / 389.0;
    const float blueCorrection  = 255.0 / 238.0;

    bool doShowBrightness = false;
    while(true){
        if (1000 * RELOAD_TIME * 0.5 < clock.now() && !doShowBrightness) {
            // 反射光を取得して表示
            msg_f(colorSensor.getBrightness(), brightnessLine + 1);
            doShowBrightness = true;
        }

        if (1000 * RELOAD_TIME < clock.now() && doShowBrightness) {
            // RGBを取得して表示
            colorSensor.getRawColor(rgb);
            msg_f(rgb.r * redCorrection, rgbLine + 1);
            msg_f(rgb.g * greenCorrection, rgbLine + 2);
            msg_f(rgb.b * blueCorrection, rgbLine + 3);

            // HSVを取得して表示
            hsvConverter.convertToHsv(rgb.r * redCorrection, rgb.g * greenCorrection, rgb.b * blueCorrection);
            hsvConverter.getHsv(hsv[0], hsv[1], hsv[2]);
            msg_f(hsv[0], hsvLine + 1);
            msg_f(hsv[1], hsvLine + 2);
            msg_f(hsv[2], hsvLine + 3);

            // 色判定
            msg_f(numbetToColor(colorSensor.getColorNumber()), coloeNameLine + 1);

            doShowBrightness = false;
            clock.reset();
        }
    }
}
