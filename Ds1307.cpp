#include "Ds1307.h"
#include "grovepi.h"
#include <string.h>
#include <iostream>
#include <ctime>
using namespace std;
DS1307::DS1307() {
    fd = wiringPiI2CSetup(RTC_ADDRESS);
    if (fd == -1) {
        printf("RTC not detected\n");
        return;
    }
    wiringPiI2CWriteReg8(fd, 0x00, 0x00); // Activer l'oscillateur de la RTC
}

string DS1307::horodatage() {
    unsigned char buffer[7];
    for (int i = 0; i < 7; i++) {
        buffer[i] = wiringPiI2CReadReg8(fd, i);
    }
    string heure = to_string(buffer[5]) + "-" + to_string(buffer[4]) + "-" + to_string(buffer[6]) + " " + to_string(buffer[2]) + ":" + to_string(buffer[1]);
    
    return heure;
}


void DS1307::Reglagehorloge() {
    
   //int rtc_fd = wiringPiI2CSetup(RTC_ADDRESS);


        // Obtient l'heure actuelle du systeme
        time_t t = time(NULL);
        struct tm* timeinfo = localtime(&t);

        // Definit l'heure actuelle de la RTC
        wiringPiI2CWriteReg8(fd, 0x00, timeinfo->tm_sec);
        wiringPiI2CWriteReg8(fd, 0x01, timeinfo->tm_min);
        wiringPiI2CWriteReg8(fd, 0x02, timeinfo->tm_hour);
        // Definir la date
        int mois = timeinfo->tm_mon + 1;
        wiringPiI2CWriteReg8(fd, 0x04,mois);
        
        wiringPiI2CWriteReg8(fd, 0x05, timeinfo->tm_mday);
        int annee = timeinfo->tm_year -100;
        wiringPiI2CWriteReg8(fd, 0x06, annee);
}
