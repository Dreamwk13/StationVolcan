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
#include "communication.h"
//******************************************************************
//  Pour compiler:  g++ Capteur_Complet.cpp -o Capteur_Complet grovepi.cpp Ds1307.cpp -lwiringPi communication.cpp
//******************************************************************


#define BUZZER_PIN 8
#define GAS_PIN 0
#define TREMB_PIN 2

using namespace std;
using namespace GrovePi;


int main(int argc, char *argv[])
{

//******************************************************************
//                      CREATION VARIABLES
//******************************************************************
    initGrovePi();
    fstream fs;

    DS1307 RTC;
    CapteurTemperature ds18b20;
    Buzzer buzzer(BUZZER_PIN);


    string temperature,gaz,tremblement,donnee,id,horodatage;
    int temp;
    id = "1";

//******************************************************************
//                 AQUISITION DES DONNEES
//******************************************************************

    //temperature
    temp = ds18b20.lire_temperature_ds18b20();
    temperature = to_string(temp);

    //horodatage
    RTC.Reglagehorloge();
    horodatage= RTC.horodatage();

    //Gaz
    //gaz= analogRead(GAS_PIN);

    //Tremblement
    //tremblement = analogRead(TREMB_PIN);


    //Mise en forme des donnees
    donnee= id +"   ;   "+ temperature + "   ;   "+ horodatage + "   ;   " +id;


    if(temp>30)
        {
        buzzer.buzz();
        }
//******************************************************************
//                      CONNEXION AU SERVEUR
//******************************************************************

    
    while(1)
    {
		Communication* communication = new Communication("172.16.8.110", 8888);
		
        if(communication->connectToServer())
        {
            communication->sendData(donnee);
            cout << "connexion reussi" << endl;
            
            if(temp>30)
                {
                buzzer.buzz();
                }
        }
       // else
       // {
			//if(communication->connectToServer())
				//{
				//communication->sendData(donnee);
				//cout << "connexion reussi au deuxieme essaie" << endl;
				//}
			else
				{
				cout << "connexion impossible / enregistrement local" << endl;
				fs.open ("temperature.txt", fstream::out | fstream::app); //trunc permet de supprimer et app d'ecrire a la suite
				fs << donnee << endl;
				fs.close();
				}
		delete communication;	
		sleep(5);	
        }
    
return 0;
    }





