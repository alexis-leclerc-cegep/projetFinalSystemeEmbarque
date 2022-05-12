/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Stone de base
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe MyStone et MySerial

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1         21/08/15    Alain   Première version du logiciel
        1.2         22/03/02    Alain   Modification de la classe MyStone

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
          Optenir la version du Firmware de l'écran

 * */

#include <Arduino.h>

#define RXD2 18 //16
#define TXD2 19 //17
#define BAUD_RATE 115200

#include <iostream>

#include "MyBmp.h"

MyBmp * Bmp = new MyBmp();

#include "MyButton.h"

MyButton * myButtonT4 = new MyButton();
MyButton * myButtonT5 = new MyButton();

#include "MyStone.h"

MyStone * myStone;

float timer;
float tempsBois = 20;
float tempMin = 25;
bool buttonPressed = false;
bool ovenStarted = false;

std::string intToHexa(int value) {
  char buffer[10];
  sprintf(buffer, "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr) {
  char buffer[10];
  sprintf(buffer, formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr) {
  char buffer[10];
  sprintf(buffer, formatStr.c_str(), value);
  return (buffer);
};

//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone -> getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch (rd.id) {
  case 0x1001: {
    if(rd.type == 1)
    {
      //myStone->beep(1500);
      buttonPressed=true;
      break;
    }
  };

  case 0x0002: { //Version
    std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
    std::string stoneVersion = rd.name;
    std::cout << "Version : " << stoneVersion.c_str() << "\n";

    //std::this_thread::sleep_for(std::chrono::milliseconds(10));

    break;
  };
  case 0x10B0: {
    const char separateur[2] = ":";
    char * token;
    token = strtok(rd.name, separateur);

    token = strtok(NULL, separateur);

    int result = strcmp(rd.name, "\"combo_box_ex2\"");
    if(result == 0)
    {

      char * bois = token;

      if (strcmp(bois, "Erabl") == 0) {
        std::cout << "mmmmmmmmm le bon sirop\n";
        myStone -> setLabel("lblbois", "Érable");
        myStone -> setLabel("lbltype", "Bois de forêt");
        myStone -> setLabel("lblorigine", "Québec");
        myStone -> setLabel("lblsechage", "20");
        myStone -> setLabel("lblsechage2", "/20s");
        tempsBois = 20;
        myStone -> setLabel("lbltemp", "25");
        tempMin = 25;
      }
      if (strcmp(bois, "Melez") == 0) {
        std::cout << "c du bois de pas bonne qualite\n";
        myStone -> setLabel("lblbois", "Mélèze");
        myStone -> setLabel("lbltype", "Bois rigolo");
        myStone -> setLabel("lblorigine", "Arabie saoudite");
        myStone -> setLabel("lblsechage", "25");
        myStone -> setLabel("lblsechage2", "/25s");
        tempsBois = 25;
        myStone -> setLabel("lbltemp", "27");
        tempMin = 27;
        myStone -> setLabel("lbltemp", "27");
      }

      //std::cout << rd.command << " line " << rd.line << "\n";
      std::cout << "Name : " << rd.name << "\n";
      std::cout << "Token : " << token << "\n";
      std::cout << "Bois : " << bois << "\n";

    }
    break;
  };

  }

  if (rd.id < 0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type << ")\n";
}

void setup() {

  Bmp -> init();

  Serial.begin(9600);

  Serial.println("Stone serial Txd is on pin: " + String(TXD2));
  Serial.println("Stone serial Rxd is on pin: " + String(RXD2));

  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);

  myButtonT4 -> init(T4);
  myButtonT4->autoSensibilisation(); //Trouve la sensibilité automatiquement

  myButtonT5 -> init(T5);
  myButtonT5->autoSensibilisation(); //Trouve la sensibilité automatiquement

  cout << std::string("Début de l'exemple Stone de base pour le ESP32") << "\n";
}

void loop() {
  char strTimer[5];

  readStoneData();
  float temp = Bmp -> getTemperature();

  Serial.println(temp);

  char strTemp[64];

  sprintf(strTemp, "%g °C", temp);



  if(buttonPressed){
    if(temp > (tempMin - (tempMin * 0.10)) && temp < (tempMin + (tempMin * 0.10)) )
    {
      if(!ovenStarted)
      {
        std::cout << "Four demarre \n";
        myStone->setLabel("lblfourstatus", "Four demarre");
        ovenStarted = true;

      }
      timer += 0.5;
    }
    else{
      if(ovenStarted)
      {
        ovenStarted = false;
        myStone->setLabel("lblfourstatus", "Trop froid");
        timer = 0;


        //buttonPressed = false;

      }
      else{
        ovenStarted = false;
        timer = 0;
        myStone->setLabel("lblfourstatus", "Trop froid");

      }

   }
  }
  if(timer >= tempsBois)
  {
    ovenStarted = false;
    buttonPressed = false;
    myStone->beep(1000);
    std::cout << "cuisson finie";
    timer = 0;
    myStone->setLabel("lblfourstatus", "Cuisson finie");
  }



  sprintf(strTimer, "%g", timer);
  myStone->setLabel("lblcompteur", strTimer);


  if (myStone) myStone -> setLabel("lbltempactuel", strTemp);

  //Serial.printf("Value = %2.2f\n", temp);  

  int buttonActionT4 = myButtonT4 -> checkMyButton();
  if (buttonActionT4 > 2) { //Si appuyé plus de 0.2 secondes
    Serial.println("Button T4 pressed");
    //if(myStone) myStone->changePage();
    if (myStone) myStone -> setLabel("lbltemp", "bouton");
  }

  int buttonActionT5 = myButtonT5 -> checkMyButton();
  if (buttonActionT5 > 2) { //Si appuyé plus de 0.2 secondes
    Serial.println("Button T5 pressed");

    //Dans la version  1.2, nous allons remplacer ces lignes pour utiliser la
    //méthode getVersion()
    //char cmdFormat2[99];
    //strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
    //std::cout << cmdFormat2 << "\n";
    //myStone->writeIt((char*)cmdFormat2);
    if (myStone) myStone -> getVersion();

  }
  delay(500);
}