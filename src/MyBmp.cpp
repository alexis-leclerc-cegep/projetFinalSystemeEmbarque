#include "MyBmp.h"



MyBmp::MyBmp(){
    //bmp.begin();
};


void MyBmp::init(){
    bmp.begin();
}

/*
float MyBmp::getPressure(){
    return bmp.readPressure();
};
*/

float MyBmp::getTemperature(){
    return(bmp.readTemperature());
};
