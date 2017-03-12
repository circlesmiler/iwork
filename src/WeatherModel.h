//#define Rostock_City_ID 2844588

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
