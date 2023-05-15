#ifndef DS1307_H
#define DS1307_H

#include <wiringPiI2C.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "grovepi.h"
#include <string.h>
#include <iostream>
#define RTC_ADDRESS 0x68

using namespace std;
class DS1307 {
    
public:
    DS1307();
    string horodatage();
    void Reglagehorloge();
private:
    int fd;
    
};

#endif
