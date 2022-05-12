#ifndef MYBMP_H
#define MYBMP_H

#include <Adafruit_BMP085.h>

class MyBmp {
    private:
        Adafruit_BMP085 bmp;

    public:
        MyBmp();
        ~MyBmp(){};
        void init();
        float getTemperature();
};

#endif  