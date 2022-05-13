/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
* Cours Systèmes embarqués (c)2022
 *  
    @file     MyStone.h
    @author   Alain Dubé
    @author   Alexis Leclerc
    @version  1.3 22/05/12 
    @description
      Démonstration comment utiliser l'écran STONE

    platform = ESP32
    OS = Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1         21/08/15  Alain       Première version du logiciel
        1.2         22/02/02  Alain       Ajout des méthodes changePage, setLabel, getVersion
        1.3         22/05/12  Alexis      Ajout de la méthode beep

**/
#ifndef MYSTONE_H
#define MYSTONE_H

#include <string>
#include "MySerial.h"

//Structure local utilisés pour garder les informations lues de l'écran
struct datasRead {
    int id; //Si 0 alors il n'y a pas de données, <0 Numero commande non traitée, >1 Numero de la commande traité
    char command[80];
    char name[80];
    int type;
    char line[2048];
};

class MyStone {
    private:
        MySerial *mySerial;

        //Transfert de ces deux méthodes en privées depuis la version 1.2
        int writeIt(std::string cmdFormat2);
        int readIt(char *data, int len);

    public:
        MyStone(int speed, uint32_t config, int rxd, int txd);
        ~MyStone() { };

        datasRead getValidsDatasIfExists(); 
        
        //Nouvelles méthodes ajoutées dans la version 1.2
        void changePage(const char *pageName = "");
        void setLabel(const char *labelName, const char *value);
        void beep(int millisecs);
        void getVersion();

};
#endif
