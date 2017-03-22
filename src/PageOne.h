#ifndef PAGE_ONE_H
#define PAGE_ONE_H
#include <Adafruit_SSD1306.h>

#include "DataModel.h"
#include "WeatherModel.h"

class PageOne {
  public:
    PageOne(
      Adafruit_SSD1306* display,
      DataModel* dataModel,
      WeatherModel* weatherModel)
      : display(display)
      , dataModel(dataModel)
      , weatherModel(weatherModel)
    {}
    void updateDisplay();

  private:
    Adafruit_SSD1306* display;
    DataModel* dataModel;
    WeatherModel* weatherModel;

    void printWorkStarted();
    void printTime();
    void printWifiState();
    void printWorkDuration();
    void printExitTime();
    void printTemperature();
};
#endif /* PAGE_ONE_H */
