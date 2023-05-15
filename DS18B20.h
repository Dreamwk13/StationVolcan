#include <fstream>
#include <string>
#include "grovepi.h"

class CapteurTemperature {
public:
  float lire_temperature_ds18b20() {
    std::ifstream file(DS18B20_FILE);
    std::string line;
    float temperature = -1.0;
    // Lecture du fichier ligne par ligne
    while (std::getline(file, line)) {
      // Recherche de la ligne contenant la temperature
      if (line.find("t=") != std::string::npos) {
        // Conversion de la valeur de temperature en flottant
        temperature = std::stof(line.substr(line.find("t=") + 2)) / 1000.0;
        break;
      }
    }
    return temperature;
  }

private:
  // Chemin d'acces au fichier contenant les donnees du capteur DS18B20
  const std::string DS18B20_FILE = "/sys/bus/w1/devices/28-000008f2faca/w1_slave";
};

