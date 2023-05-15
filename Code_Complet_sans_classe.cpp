#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <wiringPiI2C.h>
#include "grovepi.h"
#include "DS18B20.h"
#include "Ds1307.h"
#include "buzzer.h"
//******************************************************************
//  Pour compiler:  g++ Capteur_Complet.cpp -o Capteur_Complet grovepi.cpp Ds1307.cpp -lwiringPi
//******************************************************************

//RTC PIN               I2C-1
#define BUZZER_PIN 8   //D8
#define GAS_PIN 0      //A0
#define TREMB_PIN 2     //A2
using namespace std;
using namespace GrovePi;
int main(int argc, char *argv[])
{

//******************************************************************
//                      CREATION VARIABLES
//******************************************************************
   initGrovePi();
    bool connexion = false;
    int sock = 0, floattemp;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    fstream fs;
    unsigned char adresse_capteur = 0x68;

    DS1307 RTC(adresse_capteur);
    CapteurTemperature ds18b20;
    Buzzer buzzer(BUZZER_PIN);


    string temperature,gaz,tremblement,donnee,id,horodatage;
    id = "1";


//******************************************************************
//                      CONNEXION AU SERVEUR
//******************************************************************


    //creation du socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Erreur de creation de socket" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);

    // Convertir l'adresse IP en notation point-virgule
    if(inet_pton(AF_INET, "172.16.8.110", &serv_addr.sin_addr)<=0) //172.16.8.110 est l'adresseIP serveur
    {
        cout << "Adresse IP non valide" << endl;
        return -1;
    }

    //test de connexion
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
    {
        connexion = true;
        cout << "Connexion reussie" << endl;
    }

    else
    {
        cout << "Echec de la connexion" << endl;
    }



//******************************************************************
//                 AQUISITION DES DONNEES
//******************************************************************

        //temperature
    floattemp = ds18b20.lire_temperature_ds18b20();
    temperature = to_string(floattemp);

        //horodatage
        //RTC.Reglagehorloge(10,52,12,04,23);
    horodatage= RTC.horodatage();
    
        //Gaz
    /*gaz= analogRead(GAS_PIN);
        //Tremblement
    tremblement = analogRead(TREMB_PIN);*/


        //Mise en forme des donnees
    donnee= id +"   ;   "+ temperature + "   ;   "+ horodatage;






//******************************************************************
//               ENVOIE/ENREGISTREMENT DES DONNEES
//******************************************************************

    // while(true)
    //{
    if(connexion)   //envoie des donnée
    {
        send(sock , donnee.c_str() , donnee.length() , 0 );
        cout << "temperature envoyé" << endl;
        read( sock , buffer, 1024);
        cout << "Réponse du serveur : " << buffer << endl;
    }
    else   //enregistrement local des données
    {
        fs.open ("temperature.txt", fstream::out | fstream::app); //trunc permet de supprimer le fichier afin d'avoir les nouvelles données
        fs << donnee << endl;
        fs.close();
        //fs.open ("gaz.txt", fstream::out | fstream::trunc); //trunc permet de supprimer le fichier afin d'avoir les nouvelles données
        //fs << temperature << endl;
        //fs.close();
        //fs.open ("treblement.txt", fstream::out | fstream::trunc); //trunc permet de supprimer le fichier afin d'avoir les nouvelles données
        //fs << temperature << endl;
        //fs.close();
    }

    sleep(3);

    //}


    return 0;
}

