#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "PageTwo.h"
#include "Icons.h"

void PageTwo::updateDisplay() {
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setTextWrap(true);
  display->setCursor(0,10);

  display->println(String::format("T: %3.1f%cC", weatherModel->getIndoorTemperature(), char(247)));
  display->print(String::format("H: %3.1f%%", weatherModel->getIndoorHumidity()));
}
