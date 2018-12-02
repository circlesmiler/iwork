#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "PageTwo.h"
#include "Icons.h"

void PageTwo::updateDisplay() {
  printTime();
  printWifiState();
  printWorkStarted();
  printWorkDuration();
  printExitTime();
  printTemperature();
}

void PageTwo::printWorkStarted() {
  display->drawBitmap(0, 0, loginIcon, 8, 8, WHITE);

  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->setCursor(10,0);

  // Not initialized
  if (dataModel->getLoginTimeSec() == 0
        && dataModel->getLogoutTimeSec() == 0) {
      display->print("--:--");
      return;
  }

  if (!dataModel->atWork()) {
      display->print("--:--");
  } else {
      display->print(Time.format(dataModel->getLoginTimeSec(), "%H:%M"));
  }
}

void PageTwo::printTime() {
  display->drawBitmap(88, 0, clockIcon, 8, 8, WHITE);

  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->setCursor(98,0);

  if (Time.now() % 2 == 0) {
      display->print(Time.format(Time.now(), "%H:%M"));
  } else {
      display->print(Time.format(Time.now(), "%H %M"));
  }
}

void PageTwo::printWifiState() {
  if (!WiFi.ready()) {
      display->drawBitmap(60, 0, noWifiIcon, 8, 8, WHITE);
      return;
  }

  // Signal strength
  int sig = WiFi.RSSI();
  if (sig < -95) {
      // Bad signal
      display->drawBitmap(60, 0, wifiIcon1, 8, 8, WHITE);
  } else if (-95 <= sig && sig < -63) {
      display->drawBitmap(60, 0, wifiIcon2, 8, 8, WHITE);
  } else if (-63 <= sig && sig < -31) {
      display->drawBitmap(60, 0, wifiIcon3, 8, 8, WHITE);
  } else if (-31 <= sig && sig < 0) {
      // Good signal
      display->drawBitmap(60, 0, wifiIcon4, 8, 8, WHITE);
  } else if (sig > 0) {
      // Error
      display->drawBitmap(60, 0, noWifiIcon, 8, 8, WHITE);
  }
}

void PageTwo::printWorkDuration() {
  display->drawBitmap(88, 56, lampIcon, 8, 8, WHITE);

  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->setCursor(98,56);

  // Not initialized
  if (dataModel->getLoginTimeSec() == 0
        && dataModel->getLogoutTimeSec() == 0) {
      display->print("--:--");
      return;
  }

  if (!dataModel->atWork()) {
      display->print("--:--");
  } else {
      int timeDiff = dataModel->getAtWorkTimeSec();
      int minutes = timeDiff / 60;

      // Hours
      display->print(String::format("%02d", minutes / 60));

      if (Time.now() % 2 == 0) {
          display->print(":");
      } else {
          display->print(" ");
      }

      // Minutes
      display->print(String::format("%02d", minutes % 60));
  }
}

void PageTwo::printExitTime() {
  display->drawBitmap(0, 56, logoutIcon, 8, 8, WHITE);

  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->setCursor(10, 56);

  if (dataModel->getLogoutTimeSec() == 0) {
      display->print("--:--");
  } else {
      display->print(Time.format(dataModel->getLogoutTimeSec(), "%H:%M"));
  }
}

void PageTwo::printTemperature() {
  display->setTextSize(2);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->drawBitmap(10, 12, homeIcon2, 16, 16, WHITE);

  display->setCursor(35,13);
  float tempIndoor = weatherModel->getIndoorTemperature();
  if (-10 < tempIndoor && tempIndoor < 50) {
    display->print(String::format("%3.1f%cC", tempIndoor, char(247)));
  } else {
    display->print(String::format("??.?%cC", char(247)));
  }

  display->drawBitmap(10, 32, sunIcon, 16, 16, WHITE);

  display->setCursor(35,33);
  float tempOutdoor = weatherModel->getOutdoorTemp();
  display->print(String::format("%3.1f%cC", tempOutdoor, char(247)));
}
