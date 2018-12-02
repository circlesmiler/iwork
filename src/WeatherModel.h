#ifndef WEATHER_MODEL_H
#define WEATHER_MODEL_H

class WeatherModel {
  public:
    WeatherModel(int owmCityIdEEPROMAddress);
    void setOutdoorData(const char *data);

    int getCityId();
    void setCityId(int cityId);

    int getOutdoorTemp();

    void setIndoorValues(float temperature, float humidity);
    float getIndoorTemperature();
    float getIndoorHumidity();
  private:
    int outdoorTemp_;
    int cityId_;

    float indoorTemperature_;
    float indoorHumidity_;

    int owmCityIdEEPROMAddress_;

    int loadCityIdFromEEPROM();
};
#endif /* WEATHER_MODEL_H */
