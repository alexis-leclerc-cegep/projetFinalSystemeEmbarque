/*
 * MyBmp.cpp
 * 
 *  Created on: 5/12/2022
 *      Author: Alexis
 */

#include "MyBmp.h"


//constructor
MyBmp::MyBmp(){ };


//initialisation
void MyBmp::init(){
    bmp.begin();
}

/* commentee car non-nécessaire
float MyBmp::getPressure(){
    return bmp.readPressure();
};
*/

//obtient la température
float MyBmp::getTemperature(){
    return(bmp.readTemperature());
};
