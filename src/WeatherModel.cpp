#include <Particle.h>

#include "WeatherModel.h"

WeatherModel::WeatherModel(int owmCityIdEEPROMAddress) {
  owmCityIdEEPROMAddress_ = owmCityIdEEPROMAddress;
  setCityId(loadCityIdFromEEPROM());
  currentTemp_ = 0;
}

void WeatherModel::update(const char *data) {
  currentTemp_ = atof(data);
}

int WeatherModel::getCityId() {
  return cityId_;
}

void WeatherModel::setCityId(int cityId) {
  cityId_ = cityId;
  EEPROM.put(owmCityIdEEPROMAddress_, cityId_);
}

int WeatherModel::getCurrentTemp() {
  return currentTemp_;
}

int WeatherModel::loadCityIdFromEEPROM() {
    int cityId;
    EEPROM.get(owmCityIdEEPROMAddress_, cityId);
    if (cityId > 0)
      return cityId;
    else
      return 2844588;
}
