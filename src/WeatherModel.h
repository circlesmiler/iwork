#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

class WeatherModel {
  public:
    WeatherModel(int owmCityIdEEPROMAddress);
    void update(const char *data);

    int getCityId();
    void setCityId(int cityId);

    int getCurrentTemp();

  private:
    int currentTemp_;
    int cityId_;

    int owmCityIdEEPROMAddress_;

    int loadCityIdFromEEPROM();
};
#endif /* WEATHER_MODEL_H */
