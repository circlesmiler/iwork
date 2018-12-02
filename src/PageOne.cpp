#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "PageOne.h"
#include "Icons.h"

void PageOne::updateDisplay() {
  printTime();
  printWifiState();
  printWorkStarted();
  printWorkDuration();
  printExitTime();
  printTemperature();
}

void PageOne::printWorkStarted() {
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

void PageOne::printTime() {
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

void PageOne::printWifiState() {
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

void PageOne::printWorkDuration() {
  display->drawBitmap(0, 20, lampIcon24, 24, 24, WHITE);

  display->setTextSize(3);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->setCursor(30,23);

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

void PageOne::printExitTime() {
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

void PageOne::printTemperature() {
  display->drawBitmap(88, 56, tempIcon, 8, 8, WHITE);

  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setTextWrap(false);

  display->setCursor(98, 56);

  display->print(String::format("%+3d%cC", weatherModel->getOutdoorTemp(), char(247)));
}
