/*
 * MyBmp.h
 *
 *  Created on: 5/12/2022
 *      Author: Alexis
 *
 *  Description: Classe pour interagir avec la librairie BMP085 de Adafruit
 */
#ifndef MYBMP_H
#define MYBMP_H

#include <Adafruit_BMP085.h> //la librairie adafruit pour intéragir avec le bmp180

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